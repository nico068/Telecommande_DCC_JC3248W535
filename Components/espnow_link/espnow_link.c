#include "espnow_link.h"

#include <string.h>
#include "esp_check.h"
#include "esp_event.h"
#include "esp_log.h"
#include "esp_now.h"
#include "esp_wifi.h"
#include "esp_netif.h"
#include "nvs_flash.h"

#define ESPNOW_LINK_TAG "espnow_link"
#define ESPNOW_LINK_NAMESPACE "espnow"
#define ESPNOW_LINK_KEY_MAC "peer_mac"

// PMK fixe (16 octets) partagée avec la centrale DCC
#define ESPNOW_LINK_PMK "DCC_PAIR_PMK_1234"
// Canal à utiliser (doit correspondre à la centrale)
#define ESPNOW_LINK_CHANNEL 1

#define DCC_HELLO_MAGIC 0x44434330u /* "DCC0" */

typedef struct __attribute__((packed)) {
    uint32_t magic;
    uint8_t version;
    uint8_t role;     // 0 = centrale, 1 = telecommande
    uint8_t reserved[2];
} dcc_hello_packet_t;

typedef struct {
    bool initialized;
    bool pairing;
    bool paired;
    uint8_t peer_mac[ESP_NOW_ETH_ALEN];
    int8_t last_rssi;
    uint8_t channel;
    bool netif_ready;
} espnow_link_ctx_t;

static espnow_link_ctx_t s_ctx = {
    .initialized = false,
    .pairing = false,
    .paired = false,
    .last_rssi = -120,
    .channel = ESPNOW_LINK_CHANNEL,
    .netif_ready = false,
};

static esp_err_t espnow_link_ensure_wifi(void);
static esp_err_t espnow_link_add_peer(const uint8_t *mac);
static esp_err_t espnow_link_store_peer(const uint8_t *mac);
static esp_err_t espnow_link_load_peer(uint8_t *mac_out);
static esp_err_t espnow_link_erase_peer(void);
static bool espnow_link_is_hello(const uint8_t *data, int len);
static void espnow_link_recv_cb(const esp_now_recv_info_t *info, const uint8_t *data, int len);
static void espnow_link_send_cb(const wifi_tx_info_t *info, esp_now_send_status_t status);
static int8_t rssi_to_level(int8_t rssi);

esp_err_t espnow_link_init(void)
{
    if (s_ctx.initialized) {
        return ESP_OK;
    }

    ESP_RETURN_ON_ERROR(espnow_link_ensure_wifi(), ESPNOW_LINK_TAG, "wifi setup failed");

    ESP_RETURN_ON_ERROR(esp_now_init(), ESPNOW_LINK_TAG, "esp_now_init failed");
    ESP_RETURN_ON_ERROR(esp_now_set_pmk((const uint8_t *)ESPNOW_LINK_PMK), ESPNOW_LINK_TAG, "pmk set failed");

    ESP_ERROR_CHECK(esp_now_register_recv_cb(espnow_link_recv_cb));
    ESP_ERROR_CHECK(esp_now_register_send_cb(espnow_link_send_cb));

    uint8_t saved_mac[ESP_NOW_ETH_ALEN];
    if (espnow_link_load_peer(saved_mac) == ESP_OK) {
        if (espnow_link_add_peer(saved_mac) == ESP_OK) {
            memcpy(s_ctx.peer_mac, saved_mac, ESP_NOW_ETH_ALEN);
            s_ctx.paired = true;
            s_ctx.pairing = false;
            ESP_LOGI(ESPNOW_LINK_TAG, "Pair restauré: %02X:%02X:%02X:%02X:%02X:%02X",
                     saved_mac[0], saved_mac[1], saved_mac[2], saved_mac[3], saved_mac[4], saved_mac[5]);
        }
    }

    s_ctx.initialized = true;
    return ESP_OK;
}

esp_err_t espnow_link_start_pairing(void)
{
    ESP_RETURN_ON_FALSE(s_ctx.initialized, ESP_ERR_INVALID_STATE, ESPNOW_LINK_TAG, "init first");

    // On repart de zéro (sans effacer l'ancien pair en NVS pour l'instant)
    s_ctx.pairing = true;
    s_ctx.paired = false;
    s_ctx.last_rssi = -120;

    // Supprimer le peer enregistré côté esp-now pour éviter les collisions
    if (!esp_now_is_peer_exist(s_ctx.peer_mac)) {
        memset(s_ctx.peer_mac, 0, ESP_NOW_ETH_ALEN);
    } else {
        esp_now_del_peer(s_ctx.peer_mac);
        memset(s_ctx.peer_mac, 0, ESP_NOW_ETH_ALEN);
    }

    ESP_LOGI(ESPNOW_LINK_TAG, "Pairing lancé - attente HELLO de la centrale");
    return ESP_OK;
}

esp_err_t espnow_link_forget_peer(void)
{
    if (s_ctx.paired && esp_now_is_peer_exist(s_ctx.peer_mac)) {
        esp_now_del_peer(s_ctx.peer_mac);
    }
    s_ctx.paired = false;
    s_ctx.pairing = false;
    s_ctx.last_rssi = -120;
    memset(s_ctx.peer_mac, 0, ESP_NOW_ETH_ALEN);
    ESP_RETURN_ON_ERROR(espnow_link_erase_peer(), ESPNOW_LINK_TAG, "erase peer failed");
    ESP_LOGI(ESPNOW_LINK_TAG, "Pair oublié (NVS + esp-now)");
    return ESP_OK;
}

esp_err_t espnow_link_send(const void *data, size_t len)
{
    ESP_RETURN_ON_FALSE(s_ctx.paired, ESP_ERR_INVALID_STATE, ESPNOW_LINK_TAG, "not paired");
    return esp_now_send(s_ctx.peer_mac, data, len);
}

espnow_link_state_t espnow_link_get_state(void)
{
    if (s_ctx.paired) return ESPNOW_LINK_STATE_PAIRED;
    if (s_ctx.pairing) return ESPNOW_LINK_STATE_PAIRING;
    return ESPNOW_LINK_STATE_IDLE;
}

bool espnow_link_is_paired(void)
{
    return s_ctx.paired;
}

const uint8_t * espnow_link_get_peer_mac(void)
{
    return s_ctx.paired ? s_ctx.peer_mac : NULL;
}

int8_t espnow_link_get_last_rssi(void)
{
    return s_ctx.last_rssi;
}

uint8_t espnow_link_get_channel(void)
{
    return s_ctx.channel;
}

// ====================== internes ==========================

static esp_err_t espnow_link_ensure_wifi(void)
{
    if (!s_ctx.netif_ready) {
        ESP_RETURN_ON_ERROR(esp_netif_init(), ESPNOW_LINK_TAG, "netif init failed");
        esp_err_t loop_err = esp_event_loop_create_default();
        if (loop_err != ESP_OK && loop_err != ESP_ERR_INVALID_STATE) {
            ESP_RETURN_ON_ERROR(loop_err, ESPNOW_LINK_TAG, "event loop failed");
        }
        esp_netif_create_default_wifi_sta();
        s_ctx.netif_ready = true;
    }

    wifi_init_config_t cfg = WIFI_INIT_CONFIG_DEFAULT();
    esp_err_t err = esp_wifi_init(&cfg);
    if (err == ESP_ERR_WIFI_INIT_STATE) {
        // déjà initialisé
        err = ESP_OK;
    }
    ESP_RETURN_ON_ERROR(err, ESPNOW_LINK_TAG, "wifi init failed");
    ESP_RETURN_ON_ERROR(esp_wifi_set_storage(WIFI_STORAGE_RAM), ESPNOW_LINK_TAG, "storage");
    ESP_RETURN_ON_ERROR(esp_wifi_set_mode(WIFI_MODE_STA), ESPNOW_LINK_TAG, "mode sta");
    ESP_RETURN_ON_ERROR(esp_wifi_start(), ESPNOW_LINK_TAG, "wifi start");
    ESP_RETURN_ON_ERROR(esp_wifi_set_channel(s_ctx.channel, WIFI_SECOND_CHAN_NONE), ESPNOW_LINK_TAG, "set channel");
    return ESP_OK;
}

static esp_err_t espnow_link_add_peer(const uint8_t *mac)
{
    if (esp_now_is_peer_exist(mac)) {
        esp_now_del_peer(mac);
    }
    esp_now_peer_info_t peer = {
        .channel = s_ctx.channel,
        .ifidx = WIFI_IF_STA,
        .encrypt = false,
    };
    memcpy(peer.peer_addr, mac, ESP_NOW_ETH_ALEN);
    return esp_now_add_peer(&peer);
}

static esp_err_t espnow_link_store_peer(const uint8_t *mac)
{
    nvs_handle_t nvs;
    ESP_RETURN_ON_ERROR(nvs_open(ESPNOW_LINK_NAMESPACE, NVS_READWRITE, &nvs), ESPNOW_LINK_TAG, "nvs open");
    esp_err_t err = nvs_set_blob(nvs, ESPNOW_LINK_KEY_MAC, mac, ESP_NOW_ETH_ALEN);
    if (err == ESP_OK) err = nvs_commit(nvs);
    nvs_close(nvs);
    return err;
}

static esp_err_t espnow_link_load_peer(uint8_t *mac_out)
{
    size_t size = ESP_NOW_ETH_ALEN;
    nvs_handle_t nvs;
    ESP_RETURN_ON_ERROR(nvs_open(ESPNOW_LINK_NAMESPACE, NVS_READONLY, &nvs), ESPNOW_LINK_TAG, "nvs open");
    esp_err_t err = nvs_get_blob(nvs, ESPNOW_LINK_KEY_MAC, mac_out, &size);
    nvs_close(nvs);
    return err;
}

static esp_err_t espnow_link_erase_peer(void)
{
    nvs_handle_t nvs;
    ESP_RETURN_ON_ERROR(nvs_open(ESPNOW_LINK_NAMESPACE, NVS_READWRITE, &nvs), ESPNOW_LINK_TAG, "nvs open");
    esp_err_t err = nvs_erase_key(nvs, ESPNOW_LINK_KEY_MAC);
    if (err == ESP_ERR_NVS_NOT_FOUND) {
        nvs_close(nvs);
        return ESP_OK;
    }
    if (err == ESP_OK) err = nvs_commit(nvs);
    nvs_close(nvs);
    return err;
}

static bool espnow_link_is_hello(const uint8_t *data, int len)
{
    if (len < (int)sizeof(dcc_hello_packet_t)) return false;
    const dcc_hello_packet_t *pkt = (const dcc_hello_packet_t *)data;
    return pkt->magic == DCC_HELLO_MAGIC && pkt->role == 0; // 0 = centrale
}

static void espnow_link_recv_cb(const esp_now_recv_info_t *info, const uint8_t *data, int len)
{
    if (!info) return;
    const uint8_t *src = info->src_addr;
    int8_t rssi = info->rx_ctrl ? info->rx_ctrl->rssi : -120;

    // Déjà appairé avec cette source: juste mettre à jour RSSI
    if (s_ctx.paired && memcmp(src, s_ctx.peer_mac, ESP_NOW_ETH_ALEN) == 0) {
        s_ctx.last_rssi = rssi;
        return;
    }

    // En phase d'appairage, accepter le premier HELLO valide
    if (s_ctx.pairing && espnow_link_is_hello(data, len)) {
        ESP_LOGI(ESPNOW_LINK_TAG, "HELLO reçu de %02X:%02X:%02X:%02X:%02X:%02X (RSSI=%d)",
                 src[0], src[1], src[2], src[3], src[4], src[5], rssi);
        if (espnow_link_add_peer(src) == ESP_OK) {
            memcpy(s_ctx.peer_mac, src, ESP_NOW_ETH_ALEN);
            s_ctx.paired = true;
            s_ctx.pairing = false;
            s_ctx.last_rssi = rssi;
            espnow_link_store_peer(src);
            // Envoyer un ACK court pour confirmer
            const char ack[] = "ACK";
            esp_now_send(src, (const uint8_t *)ack, sizeof(ack));
        } else {
            ESP_LOGW(ESPNOW_LINK_TAG, "Impossible d'ajouter le peer esp-now");
        }
        return;
    }
}

static void espnow_link_send_cb(const wifi_tx_info_t *info, esp_now_send_status_t status)
{
    (void)info;
    if (status != ESP_NOW_SEND_SUCCESS) {
        ESP_LOGW(ESPNOW_LINK_TAG, "Envoi ESP-NOW status=%d", status);
    }
}

static int8_t rssi_to_level(int8_t rssi)
{
    if (rssi > -50) return 4;
    if (rssi > -60) return 3;
    if (rssi > -70) return 2;
    if (rssi > -80) return 1;
    return 0;
}

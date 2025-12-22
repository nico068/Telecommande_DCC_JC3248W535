#ifndef ESPNOW_LINK_H
#define ESPNOW_LINK_H

#include <stdbool.h>
#include <stdint.h>
#include "esp_err.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef enum {
    ESPNOW_LINK_STATE_IDLE = 0,
    ESPNOW_LINK_STATE_PAIRING,
    ESPNOW_LINK_STATE_PAIRED,
} espnow_link_state_t;

/**
 * @brief Initialiser ESP-NOW côté télécommande.
 * Doit être appelé après nvs_flash_init et esp_event_loop_create_default.
 */
esp_err_t espnow_link_init(void);

/**
 * @brief Lancer un appairage: écoute des trames HELLO et enregistrement du premier pair valide.
 */
esp_err_t espnow_link_start_pairing(void);

/**
 * @brief Oublier le pair actuel (NVS + esp-now peer).
 */
esp_err_t espnow_link_forget_peer(void);

/**
 * @brief Envoyer une charge utile au pair enregistré.
 */
esp_err_t espnow_link_send(const void *data, size_t len);

/**
 * @brief Obtenir l'état courant.
 */
espnow_link_state_t espnow_link_get_state(void);

/**
 * @brief Indiquer si un pair est enregistré.
 */
bool espnow_link_is_paired(void);

/**
 * @brief MAC du pair courant (6 octets) ou NULL si non appairé.
 */
const uint8_t * espnow_link_get_peer_mac(void);

/**
 * @brief Dernier RSSI reçu (en dBm) ou -120 si inconnu.
 */
int8_t espnow_link_get_last_rssi(void);

/**
 * @brief Canal ESP-NOW utilisé.
 */
uint8_t espnow_link_get_channel(void);

#ifdef __cplusplus
}
#endif

#endif // ESPNOW_LINK_H

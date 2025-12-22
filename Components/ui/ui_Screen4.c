// LVGL version: 9.4
// Écran d'appairage ESP-NOW avec la centrale DCC

#include "ui.h"
#include "espnow_link.h"
#include "esp_lvgl_port.h"
#include <esp_log.h>
#include <stdio.h>

#include "wifi_signal.h"

static const char *TAG = "UI_SCREEN4";

lv_obj_t * ui_Screen4 = NULL;
lv_obj_t * ui_Panel4 = NULL;
lv_obj_t * ui_headerP4 = NULL;
lv_obj_t * ui_LabelPageP4 = NULL;
lv_obj_t * ui_icnbleP4 = NULL;
lv_obj_t * ui_icnwifiP4 = NULL;
lv_obj_t * ui_icnbattP4 = NULL;

lv_obj_t * ui_PanelWiFiStatus = NULL;
lv_obj_t * ui_LabelScanStatus = NULL;
lv_obj_t * ui_SpinnerScan = NULL;

lv_obj_t * ui_PanelWiFiList = NULL;
lv_obj_t * ui_LabelPeerMac = NULL;
lv_obj_t * ui_LabelLinkRssi = NULL;

lv_obj_t * ui_BtnRetourP4 = NULL;
lv_obj_t * ui_LabelBtnRetourP4 = NULL;
lv_obj_t * ui_BtnScanP4 = NULL;
lv_obj_t * ui_LabelBtnScanP4 = NULL;
lv_obj_t * ui_BtnForgetP4 = NULL;
lv_obj_t * ui_LabelBtnForgetP4 = NULL;

// Timer pour rafraîchir l'état de lien
static lv_timer_t * link_status_timer = NULL;

static void update_pairing_ui(void);
static void wifi_icon_update_cb(lv_timer_t * timer);

// Event handler pour le bouton retour
void ui_event_BtnRetourP4(lv_event_t * e)
{
    if (lv_event_get_code(e) == LV_EVENT_CLICKED) {
        _ui_screen_change(&ui_Screen0, LV_SCR_LOAD_ANIM_NONE, 0, 0, &ui_Screen0_screen_init);
    }
}

// Event handler pour lancer l'appairage
void ui_event_BtnScanP4(lv_event_t * e)
{
    if (lv_event_get_code(e) == LV_EVENT_CLICKED) {
        espnow_link_init();
        espnow_link_start_pairing();
        if (ui_LabelScanStatus) {
            lv_label_set_text(ui_LabelScanStatus, "Recherche centrale...");
        }
        if (ui_SpinnerScan) {
            lv_obj_clear_flag(ui_SpinnerScan, LV_OBJ_FLAG_HIDDEN);
        }
        update_pairing_ui();
    }
}

// Event handler pour oublier le pair mémorisé
void ui_event_BtnForgetP4(lv_event_t * e)
{
    if (lv_event_get_code(e) == LV_EVENT_CLICKED) {
        espnow_link_forget_peer();
        update_pairing_ui();
    }
}

// Mise à jour des icônes et du statut de lien
static void update_pairing_ui(void)
{
    // Mettre à jour toutes les icônes WiFi (utilise wifi_signal_update qui s'appuie sur ESP-NOW)
    wifi_update_all_icons();

    lvgl_port_lock(0);

    espnow_link_state_t state = espnow_link_get_state();
    const uint8_t *mac = espnow_link_get_peer_mac();
    int8_t rssi = espnow_link_get_last_rssi();

    // Statut principal + spinner
    if (ui_LabelScanStatus) {
        switch (state) {
            case ESPNOW_LINK_STATE_PAIRED:
                lv_label_set_text(ui_LabelScanStatus, "Centrale appairée");
                break;
            case ESPNOW_LINK_STATE_PAIRING:
                lv_label_set_text(ui_LabelScanStatus, "Recherche centrale...");
                break;
            default:
                lv_label_set_text(ui_LabelScanStatus, "Non appairé");
                break;
        }
    }
    if (ui_SpinnerScan) {
        if (state == ESPNOW_LINK_STATE_PAIRING) {
            lv_obj_clear_flag(ui_SpinnerScan, LV_OBJ_FLAG_HIDDEN);
        } else {
            lv_obj_add_flag(ui_SpinnerScan, LV_OBJ_FLAG_HIDDEN);
        }
    }

    // MAC centrale
    if (ui_LabelPeerMac) {
        if (mac) {
            char buf[64];
            snprintf(buf, sizeof(buf), "MAC centrale: %02X:%02X:%02X:%02X:%02X:%02X",
                     mac[0], mac[1], mac[2], mac[3], mac[4], mac[5]);
            lv_label_set_text(ui_LabelPeerMac, buf);
        } else {
            lv_label_set_text(ui_LabelPeerMac, "MAC centrale: --");
        }
    }

    // RSSI
    if (ui_LabelLinkRssi) {
        if (state == ESPNOW_LINK_STATE_PAIRED && rssi > -120) {
            char buf[32];
            snprintf(buf, sizeof(buf), "RSSI: %ddBm", rssi);
            lv_label_set_text(ui_LabelLinkRssi, buf);
        } else {
            lv_label_set_text(ui_LabelLinkRssi, "RSSI: --");
        }
    }

    // Boutons actifs/inactifs
    if (ui_BtnScanP4) {
        if (state == ESPNOW_LINK_STATE_PAIRING) {
            lv_obj_add_state(ui_BtnScanP4, LV_STATE_DISABLED);
        } else {
            lv_obj_clear_state(ui_BtnScanP4, LV_STATE_DISABLED);
        }
    }
    if (ui_BtnForgetP4) {
        if (state == ESPNOW_LINK_STATE_PAIRED) {
            lv_obj_clear_state(ui_BtnForgetP4, LV_STATE_DISABLED);
        } else {
            lv_obj_add_state(ui_BtnForgetP4, LV_STATE_DISABLED);
        }
    }

    lvgl_port_unlock();
}

// Timer LVGL pour rafraîchir l'état (icône + RSSI)
static void wifi_icon_update_cb(lv_timer_t * timer)
{
    (void)timer;
    update_pairing_ui();
}

void ui_Screen4_screen_init(void)
{
    ESP_LOGI(TAG, "Initialisation de l'écran ESP-NOW");

    // S'assurer que le module ESP-NOW est prêt
    espnow_link_init();

    ui_Screen4 = lv_obj_create(NULL);
    ui_Panel4 = ui_Screen4;
    lv_obj_clear_flag(ui_Screen4, LV_OBJ_FLAG_SCROLLABLE);

    // En-tête
    ui_headerP4 = lv_obj_create(ui_Screen4);
    lv_obj_remove_style_all(ui_headerP4);
    lv_obj_set_width(ui_headerP4, 320);
    lv_obj_set_height(ui_headerP4, 40);
    lv_obj_set_x(ui_headerP4, 0);
    lv_obj_set_y(ui_headerP4, -220);
    lv_obj_set_align(ui_headerP4, LV_ALIGN_CENTER);
    lv_obj_remove_flag(ui_headerP4, LV_OBJ_FLAG_CLICKABLE | LV_OBJ_FLAG_SCROLLABLE);
    lv_obj_set_style_radius(ui_headerP4, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_color(ui_headerP4, lv_color_hex(0x292831), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(ui_headerP4, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_border_color(ui_headerP4, lv_color_hex(0x000000), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_border_opa(ui_headerP4, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_border_width(ui_headerP4, 0, LV_PART_MAIN | LV_STATE_DEFAULT);

    // Label page
    ui_LabelPageP4 = lv_label_create(ui_headerP4);
    lv_obj_set_align(ui_LabelPageP4, LV_ALIGN_CENTER);
    lv_label_set_text(ui_LabelPageP4, "ESP-NOW");
    lv_obj_set_style_text_color(ui_LabelPageP4, lv_color_hex(0xFFFFFF), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_font(ui_LabelPageP4, &lv_font_montserrat_14, LV_PART_MAIN | LV_STATE_DEFAULT);

    // Icône Batterie
    ui_icnbattP4 = lv_image_create(ui_headerP4);
    lv_image_set_src(ui_icnbattP4, &bat_pleine); // Image par défaut
    lv_obj_set_x(ui_icnbattP4, -135);
    lv_obj_set_align(ui_icnbattP4, LV_ALIGN_CENTER);
    lv_obj_add_flag(ui_icnbattP4, LV_OBJ_FLAG_CLICKABLE);
    lv_obj_remove_flag(ui_icnbattP4, LV_OBJ_FLAG_SCROLLABLE);

    // Icône WiFi / lien radio
    ui_icnwifiP4 = lv_image_create(ui_headerP4);
    lv_image_set_src(ui_icnwifiP4, &wifi0);
    lv_obj_set_x(ui_icnwifiP4, 135);
    lv_obj_set_align(ui_icnwifiP4, LV_ALIGN_CENTER);
    lv_obj_add_flag(ui_icnwifiP4, LV_OBJ_FLAG_CLICKABLE);
    lv_obj_remove_flag(ui_icnwifiP4, LV_OBJ_FLAG_SCROLLABLE);

    // Panel statut
    ui_PanelWiFiStatus = lv_obj_create(ui_Screen4);
    lv_obj_set_width(ui_PanelWiFiStatus, 310);
    lv_obj_set_height(ui_PanelWiFiStatus, 40);
    lv_obj_set_y(ui_PanelWiFiStatus, -160);
    lv_obj_set_align(ui_PanelWiFiStatus, LV_ALIGN_CENTER);
    lv_obj_set_flex_flow(ui_PanelWiFiStatus, LV_FLEX_FLOW_ROW);
    lv_obj_set_flex_align(ui_PanelWiFiStatus, LV_FLEX_ALIGN_SPACE_AROUND, LV_FLEX_ALIGN_CENTER, LV_FLEX_ALIGN_CENTER);
    lv_obj_set_style_bg_opa(ui_PanelWiFiStatus, 0, LV_PART_MAIN | LV_STATE_DEFAULT);

    ui_LabelScanStatus = lv_label_create(ui_PanelWiFiStatus);
    lv_obj_set_align(ui_LabelScanStatus, LV_ALIGN_CENTER);
    lv_label_set_text(ui_LabelScanStatus, "Non appairé");
    lv_obj_set_style_text_color(ui_LabelScanStatus, lv_color_hex(0xFFFFFF), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_font(ui_LabelScanStatus, &lv_font_montserrat_14, LV_PART_MAIN | LV_STATE_DEFAULT);

    ui_SpinnerScan = lv_spinner_create(ui_PanelWiFiStatus);
    lv_spinner_set_anim_params(ui_SpinnerScan, 1000, 60);
    lv_obj_set_width(ui_SpinnerScan, 30);
    lv_obj_set_height(ui_SpinnerScan, 30);
    lv_obj_set_align(ui_SpinnerScan, LV_ALIGN_CENTER);
    lv_obj_add_flag(ui_SpinnerScan, LV_OBJ_FLAG_HIDDEN);

    // Panel info lien
    ui_PanelWiFiList = lv_obj_create(ui_Screen4);
    lv_obj_set_width(ui_PanelWiFiList, 310);
    lv_obj_set_height(ui_PanelWiFiList, 220);
    lv_obj_set_y(ui_PanelWiFiList, 20);
    lv_obj_set_align(ui_PanelWiFiList, LV_ALIGN_CENTER);
    lv_obj_set_style_bg_color(ui_PanelWiFiList, lv_color_hex(0x1E1E1E), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_border_color(ui_PanelWiFiList, lv_color_hex(0x404040), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_border_width(ui_PanelWiFiList, 2, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_flex_flow(ui_PanelWiFiList, LV_FLEX_FLOW_COLUMN);
    lv_obj_set_flex_align(ui_PanelWiFiList, LV_FLEX_ALIGN_SPACE_EVENLY, LV_FLEX_ALIGN_CENTER, LV_FLEX_ALIGN_CENTER);

    ui_LabelPeerMac = lv_label_create(ui_PanelWiFiList);
    lv_label_set_text(ui_LabelPeerMac, "MAC centrale: --");
    lv_obj_set_style_text_font(ui_LabelPeerMac, &lv_font_montserrat_18, LV_PART_MAIN | LV_STATE_DEFAULT);

    ui_LabelLinkRssi = lv_label_create(ui_PanelWiFiList);
    lv_label_set_text(ui_LabelLinkRssi, "RSSI: --");
    lv_obj_set_style_text_font(ui_LabelLinkRssi, &lv_font_montserrat_18, LV_PART_MAIN | LV_STATE_DEFAULT);

    // Bouton Retour
    ui_BtnRetourP4 = lv_btn_create(ui_Screen4);
    lv_obj_set_width(ui_BtnRetourP4, 95);
    lv_obj_set_height(ui_BtnRetourP4, 40);
    lv_obj_set_x(ui_BtnRetourP4, -100);
    lv_obj_set_y(ui_BtnRetourP4, 195);
    lv_obj_set_align(ui_BtnRetourP4, LV_ALIGN_CENTER);
    lv_obj_set_style_bg_color(ui_BtnRetourP4, lv_color_hex(0x7F7F7F), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(ui_BtnRetourP4, 255, LV_PART_MAIN | LV_STATE_DEFAULT);

    ui_LabelBtnRetourP4 = lv_label_create(ui_BtnRetourP4);
    lv_obj_center(ui_LabelBtnRetourP4);
    lv_label_set_text(ui_LabelBtnRetourP4, "Retour");
    lv_obj_set_style_text_font(ui_LabelBtnRetourP4, &lv_font_montserrat_18, LV_PART_MAIN | LV_STATE_DEFAULT);

    // Bouton Appairer
    ui_BtnScanP4 = lv_btn_create(ui_Screen4);
    lv_obj_set_width(ui_BtnScanP4, 95);
    lv_obj_set_height(ui_BtnScanP4, 40);
    lv_obj_set_x(ui_BtnScanP4, 0);
    lv_obj_set_y(ui_BtnScanP4, 195);
    lv_obj_set_align(ui_BtnScanP4, LV_ALIGN_CENTER);
    lv_obj_set_style_bg_color(ui_BtnScanP4, lv_color_hex(0x0078D7), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(ui_BtnScanP4, 255, LV_PART_MAIN | LV_STATE_DEFAULT);

    ui_LabelBtnScanP4 = lv_label_create(ui_BtnScanP4);
    lv_obj_center(ui_LabelBtnScanP4);
    lv_label_set_text(ui_LabelBtnScanP4, "Appairer");
    lv_obj_set_style_text_font(ui_LabelBtnScanP4, &lv_font_montserrat_18, LV_PART_MAIN | LV_STATE_DEFAULT);

    // Bouton Oublier
    ui_BtnForgetP4 = lv_btn_create(ui_Screen4);
    lv_obj_set_width(ui_BtnForgetP4, 95);
    lv_obj_set_height(ui_BtnForgetP4, 40);
    lv_obj_set_x(ui_BtnForgetP4, 100);
    lv_obj_set_y(ui_BtnForgetP4, 195);
    lv_obj_set_align(ui_BtnForgetP4, LV_ALIGN_CENTER);
    lv_obj_set_style_bg_color(ui_BtnForgetP4, lv_color_hex(0x505050), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(ui_BtnForgetP4, 255, LV_PART_MAIN | LV_STATE_DEFAULT);

    ui_LabelBtnForgetP4 = lv_label_create(ui_BtnForgetP4);
    lv_obj_center(ui_LabelBtnForgetP4);
    lv_label_set_text(ui_LabelBtnForgetP4, "Oublier");
    lv_obj_set_style_text_font(ui_LabelBtnForgetP4, &lv_font_montserrat_18, LV_PART_MAIN | LV_STATE_DEFAULT);

    // Event handler
    lv_obj_add_event_cb(ui_BtnRetourP4, ui_event_BtnRetourP4, LV_EVENT_ALL, NULL);
    lv_obj_add_event_cb(ui_BtnScanP4, ui_event_BtnScanP4, LV_EVENT_ALL, NULL);
    lv_obj_add_event_cb(ui_BtnForgetP4, ui_event_BtnForgetP4, LV_EVENT_ALL, NULL);

    // Timer pour l'état de lien
    link_status_timer = lv_timer_create(wifi_icon_update_cb, 1000, NULL);
    update_pairing_ui();

    ESP_LOGI(TAG, "Écran ESP-NOW initialisé");
}

void ui_Screen4_screen_destroy(void)
{
    ESP_LOGI(TAG, "Destruction de l'écran ESP-NOW");

    if (link_status_timer) {
        lv_timer_del(link_status_timer);
        link_status_timer = NULL;
    }

    if (ui_Screen4 != NULL) {
        lv_obj_del(ui_Screen4);
        ui_Screen4 = NULL;
        ui_Panel4 = NULL;
    }
}

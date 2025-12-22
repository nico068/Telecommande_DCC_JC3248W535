#include "wifi_signal.h"
#include "esp_wifi.h"
#include "ui.h" // For wifi0..wifi4 declarations
#include "esp_lvgl_port.h"
#include "espnow_link.h"

static int8_t rssi_to_level(int8_t rssi)
{
    if (rssi > -50) return 4;
    if (rssi > -60) return 3;
    if (rssi > -70) return 2;
    if (rssi > -80) return 1;
    return 0;
}

wifi_signal_data_t wifi_signal_update(void)
{
    wifi_signal_data_t data = { .rssi = -120, .signal_level = 0 };

    // Priorité à ESP-NOW si un pair est présent
    if (espnow_link_is_paired()) {
        data.rssi = espnow_link_get_last_rssi();
        data.signal_level = rssi_to_level(data.rssi);
        return data;
    }

    // Fallback: RSSI du WiFi STA connecté (si encore utilisé)
    wifi_ap_record_t info;
    if (esp_wifi_sta_get_ap_info(&info) == ESP_OK) {
        data.rssi = info.rssi;
        data.signal_level = rssi_to_level(info.rssi);
    }

    return data;
}

const lv_image_dsc_t * wifi_get_image(int8_t level)
{
    switch (level) {
        case 0: return &wifi0;
        case 1: return &wifi1;
        case 2: return &wifi2;
        case 3: return &wifi3;
        case 4: return &wifi4;
        default: return &wifi0;
    }
}

void wifi_update_all_icons(void)
{
    wifi_signal_data_t data = wifi_signal_update();
    const lv_image_dsc_t * wifi_img = wifi_get_image(data.signal_level);

    // Cette fonction est appelée depuis le timer LVGL de Screen4: pas de lock pour éviter un deadlock.
    if (ui_icnwifiP0) lv_image_set_src(ui_icnwifiP0, wifi_img);
    if (ui_icnwifiP1) lv_image_set_src(ui_icnwifiP1, wifi_img);
    if (ui_icnwifiP2) lv_image_set_src(ui_icnwifiP2, wifi_img);
    if (ui_icnwifiP3) lv_image_set_src(ui_icnwifiP3, wifi_img);
    if (ui_icnwifiP4) lv_image_set_src(ui_icnwifiP4, wifi_img);
}

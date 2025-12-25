#include "battery_ui.h"
#include "battery_monitor.h"
#include "ui.h"
#include "esp_log.h"
#include "esp_lvgl_port.h"

static const char *TAG = "BATTERY_UI";

/**
 * @brief Obtenir l'image LVGL correspondant au niveau de batterie
 */
const lv_image_dsc_t* battery_ui_get_image(battery_level_t level)
{
    switch (level) {
        case BATTERY_LEVEL_EMPTY:
            return &bat_vide;
        case BATTERY_LEVEL_LOW:
            return &bat_faible;
        case BATTERY_LEVEL_MEDIUM:
            return &bat_moitie;
        case BATTERY_LEVEL_HIGH:
            return &bat_3_4;
        case BATTERY_LEVEL_FULL:
            return &bat_pleine;
        case BATTERY_LEVEL_CHARGING:
            return &bat_en_charge;
        default:
            return &bat_vide;
    }
}

/**
 * @brief Mettre à jour toutes les icônes de batterie sur tous les écrans
 */
esp_err_t battery_ui_update_all_icons(void)
{
    battery_level_t level = battery_monitor_get_level();
    const lv_image_dsc_t* bat_img = battery_ui_get_image(level);
    uint8_t percentage = battery_monitor_get_percentage();
    uint16_t voltage = battery_monitor_get_voltage_mv();

    ESP_LOGD(TAG, "Updating battery icons: %d%% (%d mV) - Level %d", 
             percentage, voltage, level);

    // Verrouiller LVGL avant de toucher l'interface
    lvgl_port_lock(0);

    // Optimisation : tableau de pointeurs et boucle pour icônes et labels
    lv_obj_t* icons[] = {ui_icnbattP0, ui_icnbattP1, ui_icnbattP2, ui_icnbattP3, ui_icnbattP4};
    lv_obj_t* labels[] = {ui_LabelBattPctP0, ui_LabelBattPctP1, ui_LabelBattPctP2, ui_LabelBattPctP3, ui_LabelBattPctP4};
    char pct_str[8];
    snprintf(pct_str, sizeof(pct_str), "%d%%", percentage);
    for (size_t i = 0; i < sizeof(icons)/sizeof(icons[0]); ++i) {
        if (icons[i] != NULL) {
            lv_image_set_src(icons[i], bat_img);
        }
        if (labels[i] != NULL) {
            lv_label_set_text(labels[i], pct_str);
        }
    }

    lvgl_port_unlock();

    return ESP_OK;
}

/**
 * @brief Callback appelé lors d'un changement de niveau de batterie
 */
static void battery_level_changed_callback(battery_level_t level, uint16_t voltage_mv, uint8_t percentage)
{
    ESP_LOGI(TAG, "Battery level changed: %d%% (%d mV)", percentage, voltage_mv);
    
    // Mettre à jour toutes les icônes
    battery_ui_update_all_icons();
}

/**
 * @brief Initialiser l'intégration UI de la batterie
 */
esp_err_t battery_ui_init(void)
{
    ESP_LOGI(TAG, "Initializing battery UI integration");

    // Initialiser le moniteur de batterie avec notre callback
    esp_err_t ret = battery_monitor_init(battery_level_changed_callback);
    if (ret != ESP_OK) {
        ESP_LOGE(TAG, "Failed to initialize battery monitor");
        return ret;
    }

    // Mise à jour initiale de toutes les icônes
    battery_ui_update_all_icons();

    ESP_LOGI(TAG, "Battery UI integration initialized");
    return ESP_OK;
}

/**
 * @file battery_integration_example.c
 * @brief Exemple d'intégration du moniteur de batterie avec l'UI LVGL
 * 
 * Ce fichier montre comment initialiser et utiliser le moniteur de batterie
 * avec mise à jour automatique des icônes sur tous les écrans.
 */

#include "battery_ui.h"
#include "ui.h"
#include "esp_log.h"

static const char *TAG = "BATTERY_EXAMPLE";

/**
 * @brief Fonction d'initialisation à appeler dans main.c
 * 
 * À appeler APRÈS ui_init() pour que tous les écrans soient créés
 */
void init_battery_system(void)
{
    ESP_LOGI(TAG, "Initializing battery monitoring system...");
    
    // Initialiser l'UI LVGL d'abord
    ui_init();
    
    // Puis initialiser le moniteur de batterie avec intégration UI
    // Cela va automatiquement :
    // 1. Initialiser l'ADC sur GPIO5
    // 2. Démarrer les mesures périodiques
    // 3. Mettre à jour toutes les icônes de batterie automatiquement
    esp_err_t ret = battery_ui_init();
    if (ret != ESP_OK) {
        ESP_LOGE(TAG, "Failed to initialize battery system");
        return;
    }
    
    ESP_LOGI(TAG, "Battery system initialized successfully");
    
    // Les icônes de batterie sur tous les écrans (Screen0-4)
    // seront maintenant mises à jour automatiquement toutes les 5 secondes
    // ou lorsque le niveau de batterie change
}

/**
 * @brief Obtenir manuellement les informations de batterie
 */
void print_battery_info(void)
{
    uint16_t voltage = battery_monitor_get_voltage_mv();
    uint8_t percentage = battery_monitor_get_percentage();
    battery_level_t level = battery_monitor_get_level();
    
    ESP_LOGI(TAG, "Battery Status:");
    ESP_LOGI(TAG, "  Voltage: %d mV", voltage);
    ESP_LOGI(TAG, "  Percentage: %d%%", percentage);
    ESP_LOGI(TAG, "  Level: %d", level);
    ESP_LOGI(TAG, "  Charging: %s", battery_monitor_is_charging() ? "Yes" : "No");
}

/**
 * @brief Forcer une mise à jour manuelle des icônes
 */
void force_battery_update(void)
{
    ESP_LOGI(TAG, "Forcing battery update...");
    battery_monitor_force_update();
    battery_ui_update_all_icons();
}

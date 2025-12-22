#ifndef BATTERY_MONITOR_H
#define BATTERY_MONITOR_H

#include "esp_err.h"
#include "esp_adc/adc_oneshot.h"

#ifdef __cplusplus
extern "C" {
#endif

// GPIO pour la lecture de la tension de la batterie
#define BATTERY_ADC_GPIO     5
#define BATTERY_ADC_CHANNEL  ADC_CHANNEL_4  // GPIO5 = ADC1_CH4

// Seuils de tension batterie (en millivolts)
#define BATTERY_VOLTAGE_FULL        4200    // 100%
#define BATTERY_VOLTAGE_75          3900    // 75%
#define BATTERY_VOLTAGE_50          3700    // 50%
#define BATTERY_VOLTAGE_25          3500    // 25%
#define BATTERY_VOLTAGE_EMPTY       3200    // 0%
#define BATTERY_VOLTAGE_CHARGING    4300    // Seuil de détection charge

// Énumération des niveaux de batterie
typedef enum {
    BATTERY_LEVEL_EMPTY = 0,
    BATTERY_LEVEL_LOW,          // < 25%
    BATTERY_LEVEL_MEDIUM,       // 25-50%
    BATTERY_LEVEL_HIGH,         // 50-75%
    BATTERY_LEVEL_FULL,         // > 75%
    BATTERY_LEVEL_CHARGING
} battery_level_t;

// Callback pour les changements de niveau de batterie
typedef void (*battery_level_callback_t)(battery_level_t level, uint16_t voltage_mv, uint8_t percentage);

// Configuration du moniteur de batterie
typedef struct {
    adc_channel_t adc_channel;
    uint32_t update_interval_ms;    // Intervalle de mise à jour en ms
    battery_level_callback_t callback;
} battery_monitor_config_t;

/**
 * @brief Initialiser le moniteur de batterie avec configuration par défaut
 * @param callback Fonction callback appelée lors des changements (peut être NULL)
 * @return ESP_OK si succès, ESP_FAIL sinon
 */
esp_err_t battery_monitor_init(battery_level_callback_t callback);

/**
 * @brief Initialiser le moniteur avec une configuration personnalisée
 * @param config Pointeur vers la configuration
 * @return ESP_OK si succès, ESP_FAIL sinon
 */
esp_err_t battery_monitor_init_with_config(const battery_monitor_config_t *config);

/**
 * @brief Désactiver le moniteur de batterie
 * @return ESP_OK si succès, ESP_FAIL sinon
 */
esp_err_t battery_monitor_deinit(void);

/**
 * @brief Lire la tension actuelle de la batterie en millivolts
 * @return Tension en millivolts (0 en cas d'erreur)
 */
uint16_t battery_monitor_get_voltage_mv(void);

/**
 * @brief Obtenir le pourcentage de charge de la batterie
 * @return Pourcentage (0-100)
 */
uint8_t battery_monitor_get_percentage(void);

/**
 * @brief Obtenir le niveau actuel de la batterie
 * @return Niveau de batterie
 */
battery_level_t battery_monitor_get_level(void);

/**
 * @brief Obtenir le nom de l'image correspondant au niveau de batterie
 * @param level Niveau de batterie
 * @return Nom de l'image (bat_vide, bat_faible, etc.)
 */
const char* battery_monitor_get_image_name(battery_level_t level);

/**
 * @brief Vérifier si la batterie est en charge
 * @return true si en charge, false sinon
 */
bool battery_monitor_is_charging(void);

/**
 * @brief Enregistrer un callback pour les changements de niveau
 * @param callback Fonction à appeler
 * @return ESP_OK si succès, ESP_FAIL sinon
 */
esp_err_t battery_monitor_register_callback(battery_level_callback_t callback);

/**
 * @brief Forcer une mise à jour de la lecture de batterie
 * @return ESP_OK si succès, ESP_FAIL sinon
 */
esp_err_t battery_monitor_force_update(void);

#ifdef __cplusplus
}
#endif

#endif // BATTERY_MONITOR_H

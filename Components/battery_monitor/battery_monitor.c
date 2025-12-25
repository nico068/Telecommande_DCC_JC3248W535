#include "battery_monitor.h"
#include "esp_log.h"
#include "esp_adc/adc_oneshot.h"
#include "esp_adc/adc_cali.h"
#include "esp_adc/adc_cali_scheme.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/timers.h"
#include <string.h>

static const char *TAG = "BATTERY_MONITOR";

// Variables statiques
static adc_oneshot_unit_handle_t adc1_handle = NULL;
static adc_cali_handle_t adc_cali_handle = NULL;
static battery_level_callback_t user_callback = NULL;
static TimerHandle_t battery_timer = NULL;
static battery_level_t current_level = BATTERY_LEVEL_EMPTY;
static uint16_t current_voltage_mv = 0;
static uint8_t current_percentage = 0;
static adc_channel_t configured_channel = ADC_CHANNEL_4;

/**
 * @brief Calculer le pourcentage de batterie à partir de la tension
 */
static uint8_t calculate_percentage(uint16_t voltage_mv)
{
    if (voltage_mv >= BATTERY_VOLTAGE_FULL) {
        return 100;
    } else if (voltage_mv <= BATTERY_VOLTAGE_EMPTY) {
        return 0;
    }
    
    // Interpolation linéaire
    uint32_t range = BATTERY_VOLTAGE_FULL - BATTERY_VOLTAGE_EMPTY;
    uint32_t value = voltage_mv - BATTERY_VOLTAGE_EMPTY;
    return (value * 100) / range;
}

/**
 * @brief Déterminer le niveau de batterie à partir de la tension
 */
static battery_level_t calculate_level(uint16_t voltage_mv)
{
    if (voltage_mv >= BATTERY_VOLTAGE_CHARGING) {
        return BATTERY_LEVEL_CHARGING;
    } else if (voltage_mv >= BATTERY_VOLTAGE_FULL) {
        return BATTERY_LEVEL_FULL;
    } else if (voltage_mv >= BATTERY_VOLTAGE_75) {
        return BATTERY_LEVEL_HIGH;
    } else if (voltage_mv >= BATTERY_VOLTAGE_50) {
        return BATTERY_LEVEL_MEDIUM;
    } else if (voltage_mv >= BATTERY_VOLTAGE_25) {
        return BATTERY_LEVEL_LOW;
    } else {
        return BATTERY_LEVEL_EMPTY;
    }
}

/**
 * @brief Lire la valeur ADC et convertir en tension
 */
static uint16_t read_battery_voltage(void)
{
    if (adc1_handle == NULL) {
        ESP_LOGE(TAG, "ADC not initialized");
        return 0;
    }

    int adc_raw = 0;
    int voltage = 0;

    // Lire la valeur ADC brute
    esp_err_t ret = adc_oneshot_read(adc1_handle, configured_channel, &adc_raw);
    if (ret != ESP_OK) {
        ESP_LOGE(TAG, "ADC read failed: %s", esp_err_to_name(ret));
        return 0;
    }

    // Convertir en tension si calibration disponible
    if (adc_cali_handle != NULL) {
        ret = adc_cali_raw_to_voltage(adc_cali_handle, adc_raw, &voltage);
        if (ret != ESP_OK) {
            ESP_LOGW(TAG, "ADC calibration failed: %s", esp_err_to_name(ret));
        }
    }

    // Si pas de calibration, estimation approximative
    if (voltage == 0) {
        // Pour ESP32, ADC 12-bit, Vref ~1.1V, attenuation 11dB -> 0-3.3V
        // Avec diviseur de tension externe, ajuster selon votre circuit
        voltage = (adc_raw * 3300) / 4095;
    }

    // Appliquer le facteur de division de tension si nécessaire
    // Si vous avez un diviseur de tension (par ex. R1=10k, R2=10k), voltage_battery = voltage * 2
    uint16_t battery_voltage = voltage * 2; // Ajuster selon votre circuit

    ESP_LOGD(TAG, "ADC Raw: %d, Voltage: %d mV, Battery: %d mV", adc_raw, voltage, battery_voltage);

    return battery_voltage;
}

/**
 * @brief Timer callback pour la mise à jour périodique
 */
static void battery_timer_callback(TimerHandle_t xTimer)
{
    battery_monitor_force_update();
}

/**
 * @brief Initialiser la calibration ADC
 */
static bool init_adc_calibration(void)
{
    esp_err_t ret;
    bool calibrated = false;

    // Essayer la calibration par courbe (méthode préférée)
#if ADC_CALI_SCHEME_CURVE_FITTING_SUPPORTED
    adc_cali_curve_fitting_config_t cali_config = {
        .unit_id = ADC_UNIT_1,
        .atten = ADC_ATTEN_DB_12,
        .bitwidth = ADC_BITWIDTH_DEFAULT,
    };
    ret = adc_cali_create_scheme_curve_fitting(&cali_config, &adc_cali_handle);
    if (ret == ESP_OK) {
        calibrated = true;
        ESP_LOGI(TAG, "ADC calibration: Curve Fitting");
    }
#endif

    // Sinon, essayer la calibration par ligne
#if ADC_CALI_SCHEME_LINE_FITTING_SUPPORTED
    if (!calibrated) {
        adc_cali_line_fitting_config_t cali_config = {
            .unit_id = ADC_UNIT_1,
            .atten = ADC_ATTEN_DB_12,
            .bitwidth = ADC_BITWIDTH_DEFAULT,
        };
        ret = adc_cali_create_scheme_line_fitting(&cali_config, &adc_cali_handle);
        if (ret == ESP_OK) {
            calibrated = true;
            ESP_LOGI(TAG, "ADC calibration: Line Fitting");
        }
    }
#endif

    if (!calibrated) {
        ESP_LOGW(TAG, "ADC calibration not available, using raw values");
    }

    return calibrated;
}

/**
 * @brief Initialiser le moniteur de batterie avec configuration par défaut
 */
esp_err_t battery_monitor_init(battery_level_callback_t callback)
{
    battery_monitor_config_t config = {
        .adc_channel = BATTERY_ADC_CHANNEL,
        .update_interval_ms = 5000,  // Mise à jour toutes les 5 secondes
        .callback = callback
    };

    return battery_monitor_init_with_config(&config);
}

/**
 * @brief Initialiser le moniteur avec une configuration personnalisée
 */
esp_err_t battery_monitor_init_with_config(const battery_monitor_config_t *config)
{
    if (config == NULL) {
        ESP_LOGE(TAG, "Config is NULL");
        return ESP_FAIL;
    }

    // Sauvegarder le callback
    user_callback = config->callback;
    configured_channel = config->adc_channel;

    // Configurer ADC
    adc_oneshot_unit_init_cfg_t init_config = {
        .unit_id = ADC_UNIT_1,
        .ulp_mode = ADC_ULP_MODE_DISABLE,
    };

    esp_err_t ret = adc_oneshot_new_unit(&init_config, &adc1_handle);
    if (ret != ESP_OK) {
        ESP_LOGE(TAG, "Failed to initialize ADC unit: %s", esp_err_to_name(ret));
        return ESP_FAIL;
    }

    // Configurer le canal ADC
    adc_oneshot_chan_cfg_t chan_config = {
        .bitwidth = ADC_BITWIDTH_DEFAULT,
        .atten = ADC_ATTEN_DB_12,  // 0-3.3V
    };

    ret = adc_oneshot_config_channel(adc1_handle, config->adc_channel, &chan_config);
    if (ret != ESP_OK) {
        ESP_LOGE(TAG, "Failed to configure ADC channel: %s", esp_err_to_name(ret));
        adc_oneshot_del_unit(adc1_handle);
        adc1_handle = NULL;
        return ESP_FAIL;
    }

    // Initialiser la calibration
    init_adc_calibration();

    // Créer le timer pour les mises à jour périodiques
    battery_timer = xTimerCreate(
        "battery_timer",
        pdMS_TO_TICKS(config->update_interval_ms),
        pdTRUE,  // Auto-reload
        NULL,
        battery_timer_callback
    );

    if (battery_timer == NULL) {
        ESP_LOGE(TAG, "Failed to create battery timer");
        return ESP_FAIL;
    }

    // Démarrer le timer
    xTimerStart(battery_timer, 0);

    // Lecture initiale
    battery_monitor_force_update();

    ESP_LOGI(TAG, "Battery monitor initialized on GPIO %d (ADC1_CH%d)", 
             BATTERY_ADC_GPIO, config->adc_channel);

    return ESP_OK;
}

/**
 * @brief Désactiver le moniteur de batterie
 */
esp_err_t battery_monitor_deinit(void)
{
    // Arrêter et supprimer le timer
    if (battery_timer != NULL) {
        xTimerStop(battery_timer, 0);
        xTimerDelete(battery_timer, 0);
        battery_timer = NULL;
    }

    // Supprimer la calibration
    if (adc_cali_handle != NULL) {
#if ADC_CALI_SCHEME_CURVE_FITTING_SUPPORTED
        adc_cali_delete_scheme_curve_fitting(adc_cali_handle);
#elif ADC_CALI_SCHEME_LINE_FITTING_SUPPORTED
        adc_cali_delete_scheme_line_fitting(adc_cali_handle);
#endif
        adc_cali_handle = NULL;
    }

    // Supprimer l'unité ADC
    if (adc1_handle != NULL) {
        adc_oneshot_del_unit(adc1_handle);
        adc1_handle = NULL;
    }

    user_callback = NULL;

    ESP_LOGI(TAG, "Battery monitor deinitialized");

    return ESP_OK;
}

/**
 * @brief Lire la tension actuelle de la batterie
 */
uint16_t battery_monitor_get_voltage_mv(void)
{
    return current_voltage_mv;
}

/**
 * @brief Obtenir le pourcentage de charge
 */
uint8_t battery_monitor_get_percentage(void)
{
    return current_percentage;
}

/**
 * @brief Obtenir le niveau actuel
 */
battery_level_t battery_monitor_get_level(void)
{
    return current_level;
}

/**
 * @brief Obtenir le nom de l'image correspondant au niveau
 */
const char* battery_monitor_get_image_name(battery_level_t level)
{
    switch (level) {
        case BATTERY_LEVEL_EMPTY:
            return "bat_vide";
        case BATTERY_LEVEL_LOW:
            return "bat_faible";
        case BATTERY_LEVEL_MEDIUM:
            return "bat_moitie";
        case BATTERY_LEVEL_HIGH:
            return "bat_3_4";
        case BATTERY_LEVEL_FULL:
            return "bat_pleine";
        case BATTERY_LEVEL_CHARGING:
            return "bat_en_charge";
        default:
            return "bat_vide";
    }
}

/**
 * @brief Vérifier si la batterie est en charge
 */
bool battery_monitor_is_charging(void)
{
    return current_level == BATTERY_LEVEL_CHARGING;
}

/**
 * @brief Enregistrer un callback
 */
esp_err_t battery_monitor_register_callback(battery_level_callback_t callback)
{
    if (adc1_handle == NULL) {
        ESP_LOGE(TAG, "Battery monitor not initialized");
        return ESP_FAIL;
    }
    user_callback = callback;
    return ESP_OK;
}

/**
 * @brief Forcer une mise à jour de la lecture
 */
esp_err_t battery_monitor_force_update(void)
{
    if (adc1_handle == NULL) {
        ESP_LOGE(TAG, "Battery monitor not initialized");
        return ESP_FAIL;
    }

    // Lire la tension
    uint16_t new_voltage = read_battery_voltage();
    if (new_voltage == 0) {
        return ESP_FAIL;
    }

    // Calculer le nouveau niveau et pourcentage
    battery_level_t new_level = calculate_level(new_voltage);
    uint8_t new_percentage = calculate_percentage(new_voltage);

    // Toujours appeler le callback si défini (garantit la synchro UI)
    current_voltage_mv = new_voltage;
    current_level = new_level;
    current_percentage = new_percentage;

    ESP_LOGD(TAG, "Battery: %d mV (%d%%) - Level: %d", 
             current_voltage_mv, current_percentage, current_level);

    if (user_callback != NULL) {
        user_callback(current_level, current_voltage_mv, current_percentage);
    }

    return ESP_OK;
}

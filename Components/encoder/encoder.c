#include "encoder.h"
#include "esp_log.h"
#include "driver/gpio.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/queue.h"
#include <string.h>
#include <portmacro.h>

static const char *TAG = "ENCODER";

// Structure pour les événements
typedef struct {
    encoder_direction_t direction;
    uint32_t button_duration_ms;
    bool is_button_event;
} encoder_event_t;

// Variables statiques
static QueueHandle_t encoder_queue = NULL;
static encoder_rotation_callback_t user_rotation_callback = NULL;
static encoder_button_callback_t user_button_callback = NULL;
static encoder_direction_t last_direction = ENCODER_DIR_IDLE;

// Variables pour le suivi des états GPIO
static volatile uint32_t last_a_state = 0;
static volatile uint32_t last_b_state = 0;
static volatile uint32_t button_press_time = 0;
static volatile bool button_pressed = false;

// Handle de la tâche de traitement
static TaskHandle_t encoder_task_handle = NULL;

/**
 * @brief Interrupt handler pour l'encodeur rotatif (GPIO A et B)
 */
static void IRAM_ATTR encoder_isr_handler(void *arg)
{
    uint32_t a_state = gpio_get_level(ENCODER_A_GPIO);
    uint32_t b_state = gpio_get_level(ENCODER_B_GPIO);

    // Déterminer la direction de rotation
    encoder_direction_t direction = ENCODER_DIR_IDLE;

    if (a_state != last_a_state) {
        if (a_state && b_state) {
            direction = ENCODER_DIR_CCW;  // Sens antihoraire
        } else if (a_state && !b_state) {
            direction = ENCODER_DIR_CW;   // Sens horaire
        }
        last_a_state = a_state;
    } else if (b_state != last_b_state) {
        if (!a_state && b_state) {
            direction = ENCODER_DIR_CCW;  // Sens antihoraire
        } else if (a_state && b_state) {
            direction = ENCODER_DIR_CW;   // Sens horaire
        }
        last_b_state = b_state;
    }

    if (direction != ENCODER_DIR_IDLE) {
        encoder_event_t event = {
            .direction = direction,
            .is_button_event = false
        };
        xQueueSendFromISR(encoder_queue, &event, NULL);
    }
}

/**
 * @brief Interrupt handler pour le bouton de l'encodeur
 */
static void IRAM_ATTR encoder_button_isr_handler(void *arg)
{
    uint32_t button_state = gpio_get_level(ENCODER_BUTTON_GPIO);

    if (button_state == 0) {
        // Bouton appuyé
        button_press_time = xTaskGetTickCountFromISR();
        button_pressed = true;
    } else {
        // Bouton relâché
        if (button_pressed) {
            uint32_t press_duration = (xTaskGetTickCountFromISR() - button_press_time) * portTICK_PERIOD_MS;
            encoder_event_t event = {
                .button_duration_ms = press_duration,
                .is_button_event = true
            };
            xQueueSendFromISR(encoder_queue, &event, NULL);
            button_pressed = false;
        }
    }
}

/**
 * @brief Tâche de traitement des événements de l'encodeur
 */
static void encoder_task(void *pvParameter)
{
    encoder_event_t event;

    while (1) {
        if (xQueueReceive(encoder_queue, &event, portMAX_DELAY)) {
            if (event.is_button_event) {
                // Événement bouton
                ESP_LOGD(TAG, "Button pressed for %lu ms", event.button_duration_ms);
                if (user_button_callback) {
                    user_button_callback(event.button_duration_ms);
                }
            } else {
                // Événement rotation
                const char *dir_str = (event.direction == ENCODER_DIR_CW) ? "CW" : "CCW";
                ESP_LOGD(TAG, "Encoder rotated: %s", dir_str);
                last_direction = event.direction;
                if (user_rotation_callback) {
                    user_rotation_callback(event.direction);
                }
            }
        }
    }
}

/**
 * @brief Initialiser l'encodeur avec la configuration par défaut
 */
esp_err_t encoder_init(encoder_rotation_callback_t rotation_cb,
                       encoder_button_callback_t button_cb)
{
    encoder_config_t config = {
        .gpio_a = ENCODER_A_GPIO,
        .gpio_b = ENCODER_B_GPIO,
        .gpio_button = ENCODER_BUTTON_GPIO,
        .rotation_cb = rotation_cb,
        .button_cb = button_cb
    };

    return encoder_init_with_config(&config);
}

/**
 * @brief Initialiser l'encodeur avec une configuration personnalisée
 */
esp_err_t encoder_init_with_config(const encoder_config_t *config)
{
    if (config == NULL) {
        ESP_LOGE(TAG, "Config is NULL");
        return ESP_FAIL;
    }

    // Créer la queue pour les événements
    encoder_queue = xQueueCreate(10, sizeof(encoder_event_t));
    if (encoder_queue == NULL) {
        ESP_LOGE(TAG, "Failed to create event queue");
        return ESP_FAIL;
    }

    // Enregistrer les callbacks
    user_rotation_callback = config->rotation_cb;
    user_button_callback = config->button_cb;

    // Configurer les GPIO pour l'encodeur (A et B)
    gpio_config_t io_conf = {
        .pin_bit_mask = (1ULL << config->gpio_a) | (1ULL << config->gpio_b),
        .mode = GPIO_MODE_INPUT,
        .pull_up_en = GPIO_PULLUP_ENABLE,
        .pull_down_en = GPIO_PULLDOWN_DISABLE,
        .intr_type = GPIO_INTR_ANYEDGE
    };

    if (gpio_config(&io_conf) != ESP_OK) {
        ESP_LOGE(TAG, "Failed to configure encoder A/B GPIO");
        return ESP_FAIL;
    }

    // Configurer le GPIO pour le bouton
    gpio_config_t button_conf = {
        .pin_bit_mask = 1ULL << config->gpio_button,
        .mode = GPIO_MODE_INPUT,
        .pull_up_en = GPIO_PULLUP_ENABLE,
        .pull_down_en = GPIO_PULLDOWN_DISABLE,
        .intr_type = GPIO_INTR_ANYEDGE
    };

    if (gpio_config(&button_conf) != ESP_OK) {
        ESP_LOGE(TAG, "Failed to configure button GPIO");
        return ESP_FAIL;
    }

    // Attacher les handlers d'interruption
    esp_err_t err = gpio_isr_handler_add(config->gpio_a, encoder_isr_handler, NULL);
    if (err == ESP_ERR_INVALID_STATE) {
        // Service ISR non installé: l'installer une seule fois, puis réessayer
        err = gpio_install_isr_service(0);
        if (err != ESP_OK && err != ESP_ERR_INVALID_STATE) {
            ESP_LOGE(TAG, "Failed to install GPIO ISR service: %s", esp_err_to_name(err));
            return ESP_FAIL;
        }
        // Réessayer l'ajout du handler
        err = gpio_isr_handler_add(config->gpio_a, encoder_isr_handler, NULL);
    }
    if (err != ESP_OK) {
        ESP_LOGE(TAG, "Failed to add ISR handler for GPIO A: %s", esp_err_to_name(err));
        return ESP_FAIL;
    }

    err = gpio_isr_handler_add(config->gpio_b, encoder_isr_handler, NULL);
    if (err != ESP_OK) {
        ESP_LOGE(TAG, "Failed to add ISR handler for GPIO B: %s", esp_err_to_name(err));
        return ESP_FAIL;
    }

    err = gpio_isr_handler_add(config->gpio_button, encoder_button_isr_handler, NULL);
    if (err != ESP_OK) {
        ESP_LOGE(TAG, "Failed to add ISR handler for button GPIO: %s", esp_err_to_name(err));
        return ESP_FAIL;
    }

    // Créer la tâche de traitement des événements
    // Augmenté à 4096 bytes pour supporter les appels LVGL dans les callbacks
    if (xTaskCreate(encoder_task, "encoder_task", 4096, NULL, 10, &encoder_task_handle) != pdPASS) {
        ESP_LOGE(TAG, "Failed to create encoder task");
        return ESP_FAIL;
    }

    // Lire l'état initial des GPIO
    last_a_state = gpio_get_level(config->gpio_a);
    last_b_state = gpio_get_level(config->gpio_b);

    ESP_LOGI(TAG, "Encoder initialized on GPIO A=%d, B=%d, Button=%d",
             config->gpio_a, config->gpio_b, config->gpio_button);

    return ESP_OK;
}

/**
 * @brief Désactiver l'encodeur
 */
esp_err_t encoder_deinit(void)
{
    // Supprimer les handlers d'interruption
    gpio_isr_handler_remove(ENCODER_A_GPIO);
    gpio_isr_handler_remove(ENCODER_B_GPIO);
    gpio_isr_handler_remove(ENCODER_BUTTON_GPIO);

    // Supprimer la tâche
    if (encoder_task_handle != NULL) {
        vTaskDelete(encoder_task_handle);
        encoder_task_handle = NULL;
    }

    // Supprimer la queue
    if (encoder_queue != NULL) {
        vQueueDelete(encoder_queue);
        encoder_queue = NULL;
    }

    // Réinitialiser les callbacks
    user_rotation_callback = NULL;
    user_button_callback = NULL;
    last_direction = ENCODER_DIR_IDLE;

    ESP_LOGI(TAG, "Encoder deinitialized");

    return ESP_OK;
}

/**
 * @brief Enregistrer un callback pour les rotations
 */
esp_err_t encoder_register_rotation_callback(encoder_rotation_callback_t callback)
{
    if (encoder_queue == NULL) {
        ESP_LOGE(TAG, "Encoder not initialized");
        return ESP_FAIL;
    }
    user_rotation_callback = callback;
    return ESP_OK;
}

/**
 * @brief Enregistrer un callback pour le bouton
 */
esp_err_t encoder_register_button_callback(encoder_button_callback_t callback)
{
    if (encoder_queue == NULL) {
        ESP_LOGE(TAG, "Encoder not initialized");
        return ESP_FAIL;
    }
    user_button_callback = callback;
    return ESP_OK;
}

/**
 * @brief Obtenir l'état actuel de l'encodeur
 */
encoder_direction_t encoder_get_direction(void)
{
    return last_direction;
}

/**
 * @brief Réinitialiser l'état de l'encodeur
 */
void encoder_reset_state(void)
{
    last_direction = ENCODER_DIR_IDLE;
}

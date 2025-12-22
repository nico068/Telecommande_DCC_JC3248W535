#ifndef ENCODER_H
#define ENCODER_H

#include "driver/gpio.h"

#ifdef __cplusplus
extern "C" {
#endif

// Définition des GPIO
#define ENCODER_A_GPIO      17
#define ENCODER_B_GPIO      18
#define ENCODER_BUTTON_GPIO 7

// Enum pour la direction de rotation
typedef enum {
    ENCODER_DIR_IDLE = 0,
    ENCODER_DIR_CW,      // Clockwise (sens horaire)
    ENCODER_DIR_CCW      // Counter-clockwise (sens antihoraire)
} encoder_direction_t;

// Callbacks pour les événements de l'encodeur
typedef void (*encoder_rotation_callback_t)(encoder_direction_t direction);
typedef void (*encoder_button_callback_t)(uint32_t press_duration_ms);

// Structure de configuration
typedef struct {
    gpio_num_t gpio_a;
    gpio_num_t gpio_b;
    gpio_num_t gpio_button;
    encoder_rotation_callback_t rotation_cb;
    encoder_button_callback_t button_cb;
} encoder_config_t;

/**
 * @brief Initialiser l'encodeur avec la configuration par défaut
 * @param rotation_cb Callback pour les rotations (peut être NULL)
 * @param button_cb Callback pour le bouton (peut être NULL)
 * @return ESP_OK si succès, ESP_FAIL sinon
 */
esp_err_t encoder_init(encoder_rotation_callback_t rotation_cb,
                       encoder_button_callback_t button_cb);

/**
 * @brief Initialiser l'encodeur avec une configuration personnalisée
 * @param config Pointeur vers la configuration
 * @return ESP_OK si succès, ESP_FAIL sinon
 */
esp_err_t encoder_init_with_config(const encoder_config_t *config);

/**
 * @brief Désactiver l'encodeur
 * @return ESP_OK si succès, ESP_FAIL sinon
 */
esp_err_t encoder_deinit(void);

/**
 * @brief Enregistrer un callback pour les rotations
 * @param callback Fonction à appeler lors d'une rotation
 * @return ESP_OK si succès, ESP_FAIL sinon
 */
esp_err_t encoder_register_rotation_callback(encoder_rotation_callback_t callback);

/**
 * @brief Enregistrer un callback pour le bouton
 * @param callback Fonction à appeler lors d'un appui sur le bouton
 * @return ESP_OK si succès, ESP_FAIL sinon
 */
esp_err_t encoder_register_button_callback(encoder_button_callback_t callback);

/**
 * @brief Obtenir l'état actuel de l'encodeur
 * @return La direction de rotation détectée
 */
encoder_direction_t encoder_get_direction(void);

/**
 * @brief Réinitialiser l'état de l'encodeur
 */
void encoder_reset_state(void);

#ifdef __cplusplus
}
#endif

#endif // ENCODER_H

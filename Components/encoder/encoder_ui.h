#ifndef ENCODER_UI_H
#define ENCODER_UI_H

#include "encoder.h"
#include "lvgl.h"

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @brief Initialiser l'intégration UI de l'encodeur
 * 
 * Cette fonction initialise l'encodeur et configure automatiquement :
 * - La rotation de l'encodeur pour contrôler la jauge de vitesse (0-127, cran de 1)
 * - L'appui sur le bouton pour changer la direction (ui_imgDirectionP1)
 * 
 * @return ESP_OK si succès, ESP_FAIL sinon
 */
esp_err_t encoder_ui_init(void);

/**
 * @brief Obtenir la valeur actuelle de la vitesse (0-127)
 * @return Valeur de vitesse
 */
uint8_t encoder_ui_get_speed(void);

/**
 * @brief Définir la valeur de vitesse manuellement (0-127)
 * @param speed Nouvelle valeur
 */
void encoder_ui_set_speed(uint8_t speed);

/**
 * @brief Obtenir la direction actuelle
 * @return 0 = Avant (flèche droite), 1 = Arrière (flèche gauche)
 */
uint8_t encoder_ui_get_direction(void);

/**
 * @brief Basculer la direction manuellement
 */
void encoder_ui_toggle_direction(void);

#ifdef __cplusplus
}
#endif

#endif // ENCODER_UI_H

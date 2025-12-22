#ifndef BATTERY_UI_H
#define BATTERY_UI_H

#include "battery_monitor.h"
#include "lvgl.h"

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @brief Initialiser l'intégration UI de la batterie
 * 
 * Cette fonction initialise le moniteur de batterie et configure
 * automatiquement la mise à jour des icônes de batterie sur tous les écrans
 * 
 * @return ESP_OK si succès, ESP_FAIL sinon
 */
esp_err_t battery_ui_init(void);

/**
 * @brief Mettre à jour manuellement toutes les icônes de batterie
 * 
 * @return ESP_OK si succès, ESP_FAIL sinon
 */
esp_err_t battery_ui_update_all_icons(void);

/**
 * @brief Obtenir l'image LVGL correspondant au niveau de batterie
 * 
 * @param level Niveau de batterie
 * @return Pointeur vers la structure d'image LVGL
 */
const lv_image_dsc_t* battery_ui_get_image(battery_level_t level);

#ifdef __cplusplus
}
#endif

#endif // BATTERY_UI_H

#ifndef WIFI_SIGNAL_H
#define WIFI_SIGNAL_H

#include "lvgl.h"
#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

typedef struct {
    int8_t rssi;
    int8_t signal_level; // 0 (none) to 4 (excellent)
} wifi_signal_data_t;

/**
 * @brief Update and return current WiFi signal data
 */
wifi_signal_data_t wifi_signal_update(void);

/**
 * @brief Get the appropriate LVGL image descriptor for a signal level
 */
const lv_image_dsc_t * wifi_get_image(int8_t level);

/**
 * @brief Mettre à jour les icônes WiFi sur tous les écrans
 */
void wifi_update_all_icons(void);

#ifdef __cplusplus
}
#endif

#endif // WIFI_SIGNAL_H

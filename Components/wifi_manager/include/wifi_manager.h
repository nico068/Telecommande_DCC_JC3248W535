#ifndef WIFI_MANAGER_H
#define WIFI_MANAGER_H

#include "esp_err.h"
#include "esp_wifi.h"
#include <stdbool.h>

#ifdef __cplusplus
extern "C" {
#endif

typedef void (*wifi_scan_done_cb_t)(void);

/**
 * @brief Initialize the WiFi manager
 */
esp_err_t wifi_manager_init(void);

/**
 * @brief Check if WiFi is disabled
 */
bool wifi_manager_is_disabled(void);

/**
 * @brief Start a WiFi scan
 */
esp_err_t wifi_manager_start_scan(void);

/**
 * @brief Set the callback for when a scan is done
 */
void wifi_manager_set_scan_done_callback(wifi_scan_done_cb_t cb);

/**
 * @brief Connect to a WiFi network
 * @param ssid      Target SSID (null-terminated)
 * @param password  Password (can be empty for open networks)
 * @param authmode  Auth mode threshold (use WIFI_AUTH_OPEN for open networks)
 */
esp_err_t wifi_manager_connect(const char *ssid, const char *password, wifi_auth_mode_t authmode);

#ifdef __cplusplus
}
#endif

#endif // WIFI_MANAGER_H

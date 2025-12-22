#include "wifi_manager.h"
#include "esp_wifi.h"
#include "esp_log.h"
#include "esp_event.h"
#include "esp_check.h"
#include <string.h>

static const char *TAG = "WIFI_MGR";
static wifi_scan_done_cb_t scan_done_cb = NULL;
static bool wifi_init_done = false;

static void wifi_event_handler(void* arg, esp_event_base_t event_base,
                                int32_t event_id, void* event_data)
{
    if (event_base == WIFI_EVENT && event_id == WIFI_EVENT_SCAN_DONE) {
        ESP_LOGI(TAG, "WiFi scan done event received");
        if (scan_done_cb) {
            scan_done_cb();
        }
    }
}

esp_err_t wifi_manager_init(void)
{
    if (wifi_init_done) return ESP_OK;

    ESP_LOGI(TAG, "Initializing WiFi...");

    // esp_event_loop_create_default() should be called in main.c or here if not already done
    esp_netif_create_default_wifi_sta();

    wifi_init_config_t cfg = WIFI_INIT_CONFIG_DEFAULT();
    ESP_ERROR_CHECK(esp_wifi_init(&cfg));

    ESP_ERROR_CHECK(esp_event_handler_instance_register(WIFI_EVENT,
                                                        ESP_EVENT_ANY_ID,
                                                        &wifi_event_handler,
                                                        NULL,
                                                        NULL));

    ESP_ERROR_CHECK(esp_wifi_set_mode(WIFI_MODE_STA));
    ESP_ERROR_CHECK(esp_wifi_start());

    wifi_init_done = true;
    return ESP_OK;
}

bool wifi_manager_is_disabled(void)
{
    return false; // Assuming enabled by default for now
}

esp_err_t wifi_manager_connect(const char *ssid, const char *password, wifi_auth_mode_t authmode)
{
    if (!ssid || ssid[0] == '\0') {
        return ESP_ERR_INVALID_ARG;
    }

    // Ensure WiFi is initialized
    if (!wifi_init_done) {
        ESP_RETURN_ON_ERROR(wifi_manager_init(), TAG, "wifi init failed");
    }

    wifi_config_t wifi_cfg = {0};
    strncpy((char *)wifi_cfg.sta.ssid, ssid, sizeof(wifi_cfg.sta.ssid) - 1);
    if (password) {
        strncpy((char *)wifi_cfg.sta.password, password, sizeof(wifi_cfg.sta.password) - 1);
    }

    // Set minimum authmode required; open networks use WIFI_AUTH_OPEN
    wifi_cfg.sta.threshold.authmode = authmode;

    ESP_LOGI(TAG, "Connecting to SSID='%s' (auth=%d)", ssid, authmode);
    ESP_ERROR_CHECK(esp_wifi_set_mode(WIFI_MODE_STA));
    ESP_ERROR_CHECK(esp_wifi_set_config(WIFI_IF_STA, &wifi_cfg));
    return esp_wifi_connect();
}

esp_err_t wifi_manager_start_scan(void)
{
    wifi_scan_config_t scan_config = {
        .ssid = NULL,
        .bssid = NULL,
        .channel = 0,
        .show_hidden = false
    };
    
    ESP_LOGI(TAG, "Starting WiFi scan...");
    return esp_wifi_scan_start(&scan_config, false); // non-blocking
}

void wifi_manager_set_scan_done_callback(wifi_scan_done_cb_t cb)
{
    scan_done_cb = cb;
}

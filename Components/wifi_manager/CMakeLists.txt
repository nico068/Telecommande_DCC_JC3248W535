idf_component_register(SRCS "wifi_manager.c" "wifi_signal.c"
                    INCLUDE_DIRS "include"
                    REQUIRES esp_wifi esp_event log lvgl ui espnow_link)

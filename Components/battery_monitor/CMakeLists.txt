idf_component_register(
    SRCS "battery_integration_example.c" "battery_monitor.c" "battery_ui.c"
    INCLUDE_DIRS "."
    REQUIRES driver esp_adc ui lvgl__lvgl espressif__esp_lvgl_port
)

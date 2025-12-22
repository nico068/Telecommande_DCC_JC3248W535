idf_component_register(
    SRCS "encoder_integration_example.c" "encoder.c" "encoder_ui.c"
    INCLUDE_DIRS "."
    REQUIRES driver freertos ui lvgl__lvgl espressif__esp_lvgl_port
)

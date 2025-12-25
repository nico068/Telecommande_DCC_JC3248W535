#include "driver/gpio.h"
#include "driver/i2c_master.h"
#include "driver/ledc.h"
#include "driver/spi_master.h"
#include "esp_check.h"
#include "esp_err.h"
#include "esp_event.h"
#include "esp_lcd_axs15231b.h"
#include "esp_lcd_panel_io.h"
#include "esp_lcd_panel_io_interface.h"
#include "esp_lcd_panel_ops.h"
#include "esp_lcd_panel_rgb.h"
#include "esp_lcd_panel_vendor.h"
#include "esp_log.h"
#include "esp_lvgl_port.h"
#include "esp_random.h"
#include "esp_task_wdt.h"
#include "esp_timer.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "lv_demos.h"
#include "lvgl.h"
#include "ui.h"
#include "encoder.h"
#include "encoder_ui.h"
#include "battery_monitor.h"
#include "battery_ui.h"
#include "nvs_flash.h"
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <string.h>
#include <sys/lock.h>
#include <sys/param.h>
#include <unistd.h>

/**************************************************************************************************
 *  Brochage GPIO
 **************************************************************************************************/
#define BSP_I2C_NUM (I2C_NUM_0)
#define BSP_I2C_CLK_SPEED_HZ (400000)

#define LCD_QSPI_HOST (SPI2_HOST)
#define LCD_QSPI_H_RES (320)
#define LCD_QSPI_V_RES (480)

/**
 * @brief Structure de configuration de synchronisation verticale (Tear Effect)
 *
 */

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////// Spécifications LCD de l'interface QSPI /////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
#define LCD_LEDC_CH 1

#define LCD_PIN_NUM_QSPI_CS (GPIO_NUM_45)
#define LCD_PIN_NUM_QSPI_PCLK (GPIO_NUM_47)
#define LCD_PIN_NUM_QSPI_DATA0 (GPIO_NUM_21)
#define LCD_PIN_NUM_QSPI_DATA1 (GPIO_NUM_48)
#define LCD_PIN_NUM_QSPI_DATA2 (GPIO_NUM_40)
#define LCD_PIN_NUM_QSPI_DATA3 (GPIO_NUM_39)
#define LCD_PIN_NUM_QSPI_RST (GPIO_NUM_NC)
#define LCD_PIN_NUM_QSPI_DC (GPIO_NUM_8)
#define LCD_PIN_NUM_QSPI_TE (GPIO_NUM_38)
#define LCD_PIN_NUM_QSPI_BL (GPIO_NUM_1)

#define LCD_PIN_NUM_QSPI_TOUCH_SCL (GPIO_NUM_8)
#define LCD_PIN_NUM_QSPI_TOUCH_SDA (GPIO_NUM_4)
#define LCD_PIN_NUM_QSPI_TOUCH_RST (-1)
#define LCD_PIN_NUM_QSPI_TOUCH_INT (-1)
///////////////////*************//////////////////////////////////// */

/* Étiquette pour les logs */
static const char *TAG = "lcd_touch_example";

static i2c_master_bus_handle_t i2c_bus_handle = NULL;

/* Configuration matérielle */
#define LCD_HOST  SPI2_HOST  // Hôte SPI utilisé
#define EXAMPLE_LCD_PIXEL_CLOCK_HZ     (20 * 1000 * 1000)  // Fréquence d'horloge SPI
#define EXAMPLE_LCD_BK_LIGHT_ON_LEVEL  1  // Niveau actif du rétroéclairage
#define EXAMPLE_LCD_BK_LIGHT_OFF_LEVEL !EXAMPLE_LCD_BK_LIGHT_ON_LEVEL

////////////////////////////
/* Paramètres LCD */
//#define LCD_DRAW_BUFF_HEIGHT (60)

#define DISPLAY_MIRROR_X        false
#define DISPLAY_MIRROR_Y        false
#define DISPLAY_SWAP_XY         false
#define DISPLAY_INVERT_COLOR    false

// Écran LCD
    esp_lcd_panel_io_handle_t io_handle_lcd = NULL;
// Initialisation de l'interface d'entrée/sortie pour le contrôle de l'écran LCD
    static esp_lcd_panel_handle_t panel_handle = NULL;


/* Interface LCD et panneau */

static esp_lcd_touch_handle_t touch_handle = NULL;

/* Affichage LVGL et tactile */
static lv_display_t *lvgl_disp = NULL;
static lv_indev_t *lvgl_touch_indev = NULL;

static axs15231b_lcd_init_cmd_t lcd_init_cmds[] = {
    {0xBB, (uint8_t[]){0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x5A, 0xA5}, 8, 0},
    {0xA0, (uint8_t[]){0xC0, 0x10, 0x00, 0x02, 0x00, 0x00, 0x04, 0x3F, 0x20, 0x05, 0x3F, 0x3F, 0x00, 0x00, 0x00, 0x00, 0x00}, 17, 0},
    {0xA2, (uint8_t[]){0x30, 0x3C, 0x24, 0x14, 0xD0, 0x20, 0xFF, 0xE0, 0x40, 0x19, 0x80, 0x80, 0x80, 0x20, 0xf9, 0x10, 0x02, 0xff, 0xff, 0xF0, 0x90, 0x01, 0x32, 0xA0, 0x91, 0xE0, 0x20, 0x7F, 0xFF, 0x00, 0x5A}, 31, 0},
    {0xD0, (uint8_t[]){0xE0, 0x40, 0x51, 0x24, 0x08, 0x05, 0x10, 0x01, 0x20, 0x15, 0x42, 0xC2, 0x22, 0x22, 0xAA, 0x03, 0x10, 0x12, 0x60, 0x14, 0x1E, 0x51, 0x15, 0x00, 0x8A, 0x20, 0x00, 0x03, 0x3A, 0x12}, 30, 0},
    {0xA3, (uint8_t[]){0xA0, 0x06, 0xAa, 0x00, 0x08, 0x02, 0x0A, 0x04, 0x04, 0x04, 0x04, 0x04, 0x04, 0x04, 0x04, 0x04, 0x04, 0x04, 0x04, 0x00, 0x55, 0x55}, 22, 0},
    {0xC1, (uint8_t[]){0x31, 0x04, 0x02, 0x02, 0x71, 0x05, 0x24, 0x55, 0x02, 0x00, 0x41, 0x00, 0x53, 0xFF, 0xFF, 0xFF, 0x4F, 0x52, 0x00, 0x4F, 0x52, 0x00, 0x45, 0x3B, 0x0B, 0x02, 0x0d, 0x00, 0xFF, 0x40}, 30, 0},
    {0xC3, (uint8_t[]){0x00, 0x00, 0x00, 0x50, 0x03, 0x00, 0x00, 0x00, 0x01, 0x80, 0x01}, 11, 0},
    {0xC4, (uint8_t[]){0x00, 0x24, 0x33, 0x80, 0x00, 0xea, 0x64, 0x32, 0xC8, 0x64, 0xC8, 0x32, 0x90, 0x90, 0x11, 0x06, 0xDC, 0xFA, 0x00, 0x00, 0x80, 0xFE, 0x10, 0x10, 0x00, 0x0A, 0x0A, 0x44, 0x50}, 29, 0},
    {0xC5, (uint8_t[]){0x18, 0x00, 0x00, 0x03, 0xFE, 0x3A, 0x4A, 0x20, 0x30, 0x10, 0x88, 0xDE, 0x0D, 0x08, 0x0F, 0x0F, 0x01, 0x3A, 0x4A, 0x20, 0x10, 0x10, 0x00}, 23, 0},
    {0xC6, (uint8_t[]){0x05, 0x0A, 0x05, 0x0A, 0x00, 0xE0, 0x2E, 0x0B, 0x12, 0x22, 0x12, 0x22, 0x01, 0x03, 0x00, 0x3F, 0x6A, 0x18, 0xC8, 0x22}, 20, 0},
    {0xC7, (uint8_t[]){0x50, 0x32, 0x28, 0x00, 0xa2, 0x80, 0x8f, 0x00, 0x80, 0xff, 0x07, 0x11, 0x9c, 0x67, 0xff, 0x24, 0x0c, 0x0d, 0x0e, 0x0f}, 20, 0},
    {0xC9, (uint8_t[]){0x33, 0x44, 0x44, 0x01}, 4, 0},
    {0xCF, (uint8_t[]){0x2C, 0x1E, 0x88, 0x58, 0x13, 0x18, 0x56, 0x18, 0x1E, 0x68, 0x88, 0x00, 0x65, 0x09, 0x22, 0xC4, 0x0C, 0x77, 0x22, 0x44, 0xAA, 0x55, 0x08, 0x08, 0x12, 0xA0, 0x08}, 27, 0},
    {0xD5, (uint8_t[]){0x40, 0x8E, 0x8D, 0x01, 0x35, 0x04, 0x92, 0x74, 0x04, 0x92, 0x74, 0x04, 0x08, 0x6A, 0x04, 0x46, 0x03, 0x03, 0x03, 0x03, 0x82, 0x01, 0x03, 0x00, 0xE0, 0x51, 0xA1, 0x00, 0x00, 0x00}, 30, 0},
    {0xD6, (uint8_t[]){0x10, 0x32, 0x54, 0x76, 0x98, 0xBA, 0xDC, 0xFE, 0x93, 0x00, 0x01, 0x83, 0x07, 0x07, 0x00, 0x07, 0x07, 0x00, 0x03, 0x03, 0x03, 0x03, 0x03, 0x03, 0x00, 0x84, 0x00, 0x20, 0x01, 0x00}, 30, 0},
    {0xD7, (uint8_t[]){0x03, 0x01, 0x0b, 0x09, 0x0f, 0x0d, 0x1E, 0x1F, 0x18, 0x1d, 0x1f, 0x19, 0x40, 0x8E, 0x04, 0x00, 0x20, 0xA0, 0x1F}, 19, 0},
    {0xD8, (uint8_t[]){0x02, 0x00, 0x0a, 0x08, 0x0e, 0x0c, 0x1E, 0x1F, 0x18, 0x1d, 0x1f, 0x19}, 12, 0},
    {0xD9, (uint8_t[]){0x1F, 0x1F, 0x1F, 0x1F, 0x1F, 0x1F, 0x1F, 0x1F, 0x1F, 0x1F, 0x1F, 0x1F}, 12, 0},
    {0xDD, (uint8_t[]){0x1F, 0x1F, 0x1F, 0x1F, 0x1F, 0x1F, 0x1F, 0x1F, 0x1F, 0x1F, 0x1F, 0x1F}, 12, 0},
    {0xDF, (uint8_t[]){0x44, 0x73, 0x4B, 0x69, 0x00, 0x0A, 0x02, 0x90}, 8, 0},
    {0xE0, (uint8_t[]){0x3B, 0x28, 0x10, 0x16, 0x0c, 0x06, 0x11, 0x28, 0x5c, 0x21, 0x0D, 0x35, 0x13, 0x2C, 0x33, 0x28, 0x0D}, 17, 0},
    {0xE1, (uint8_t[]){0x37, 0x28, 0x10, 0x16, 0x0b, 0x06, 0x11, 0x28, 0x5C, 0x21, 0x0D, 0x35, 0x14, 0x2C, 0x33, 0x28, 0x0F}, 17, 0},
    {0xE2, (uint8_t[]){0x3B, 0x07, 0x12, 0x18, 0x0E, 0x0D, 0x17, 0x35, 0x44, 0x32, 0x0C, 0x14, 0x14, 0x36, 0x3A, 0x2F, 0x0D}, 17, 0},
    {0xE3, (uint8_t[]){0x37, 0x07, 0x12, 0x18, 0x0E, 0x0D, 0x17, 0x35, 0x44, 0x32, 0x0C, 0x14, 0x14, 0x36, 0x32, 0x2F, 0x0F}, 17, 0},
    {0xE4, (uint8_t[]){0x3B, 0x07, 0x12, 0x18, 0x0E, 0x0D, 0x17, 0x39, 0x44, 0x2E, 0x0C, 0x14, 0x14, 0x36, 0x3A, 0x2F, 0x0D}, 17, 0},
    {0xE5, (uint8_t[]){0x37, 0x07, 0x12, 0x18, 0x0E, 0x0D, 0x17, 0x39, 0x44, 0x2E, 0x0C, 0x14, 0x14, 0x36, 0x3A, 0x2F, 0x0F}, 17, 0},
    {0xA4, (uint8_t[]){0x85, 0x85, 0x95, 0x82, 0xAF, 0xAA, 0xAA, 0x80, 0x10, 0x30, 0x40, 0x40, 0x20, 0xFF, 0x60, 0x30}, 16, 0},
    {0xA4, (uint8_t[]){0x85, 0x85, 0x95, 0x85}, 4, 0},
    {0xBB, (uint8_t[]){0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00}, 8, 0},
    {0x13, (uint8_t[]){0x00}, 0, 0},
    {0x11, (uint8_t[]){0x00}, 0, 120},
    {0x2C, (uint8_t[]){0x00, 0x00, 0x00, 0x00}, 4, 0},
    {0x2a, (uint8_t[]){0x00, 0x00, 0x01, 0x3f}, 4, 0},
    {0x2b, (uint8_t[]){0x00, 0x00, 0x01, 0xdf}, 4, 0}
};

static void touch_i2c_init(void)
{
    const i2c_master_bus_config_t i2c_bus_cfg = {
        .clk_source = I2C_CLK_SRC_DEFAULT,
        .i2c_port = BSP_I2C_NUM,
        .sda_io_num = LCD_PIN_NUM_QSPI_TOUCH_SDA,
        .scl_io_num = LCD_PIN_NUM_QSPI_TOUCH_SCL,
        .glitch_ignore_cnt = 7,
        .flags.enable_internal_pullup = true,
    };
    ESP_ERROR_CHECK(i2c_new_master_bus(&i2c_bus_cfg, &i2c_bus_handle));
}

static esp_err_t bsp_display_brightness_init(void)
{
    // Configuration du périphérique LEDC pour le contrôle PWM du rétroéclairage
    const ledc_channel_config_t LCD_backlight_channel = {
        .gpio_num = LCD_PIN_NUM_QSPI_BL,
        .speed_mode = LEDC_LOW_SPEED_MODE,
        .channel = LCD_LEDC_CH,
        .intr_type = LEDC_INTR_DISABLE,
        .timer_sel = 1,
        .duty = 0,
        .hpoint = 0};
    const ledc_timer_config_t LCD_backlight_timer = {
        .speed_mode = LEDC_LOW_SPEED_MODE,
        .duty_resolution = LEDC_TIMER_10_BIT,
        .timer_num = 1,
        .freq_hz = 5000,
        .clk_cfg = LEDC_AUTO_CLK};

    ESP_ERROR_CHECK(ledc_timer_config(&LCD_backlight_timer));
    ESP_ERROR_CHECK(ledc_channel_config(&LCD_backlight_channel));

    return ESP_OK;
}

esp_err_t bsp_display_brightness_set(int brightness_percent)
{
    if (brightness_percent > 100)
    {
        brightness_percent = 100;
    }
    if (brightness_percent < 0)
    {
        brightness_percent = 0;
    }

    ESP_LOGI(TAG, "Setting LCD backlight: %d%%", brightness_percent);
    uint32_t duty_cycle = (1023 * brightness_percent) / 100; // Résolution LEDC fixée à 10 bits, donc: 100% = 1023
    ESP_ERROR_CHECK(ledc_set_duty(LEDC_LOW_SPEED_MODE, LCD_LEDC_CH, duty_cycle));
    ESP_ERROR_CHECK(ledc_update_duty(LEDC_LOW_SPEED_MODE, LCD_LEDC_CH));

    return ESP_OK;
}

void Initialize_AXS15231B_Display() {

ESP_LOGI(TAG, "Initialize QSPI bus");
    const spi_bus_config_t buscfg = AXS15231B_PANEL_BUS_QSPI_CONFIG(LCD_PIN_NUM_QSPI_PCLK,
                                                                    LCD_PIN_NUM_QSPI_DATA0,
                                                                    LCD_PIN_NUM_QSPI_DATA1,
                                                                    LCD_PIN_NUM_QSPI_DATA2,
                                                                    LCD_PIN_NUM_QSPI_DATA3,
                                                                    LCD_QSPI_V_RES * 10 * sizeof(uint16_t));
    ESP_ERROR_CHECK(spi_bus_initialize(LCD_QSPI_HOST, &buscfg, SPI_DMA_CH_AUTO));

    ESP_LOGI(TAG, "Install panel IO");


    esp_lcd_panel_io_spi_config_t io_config = AXS15231B_PANEL_IO_QSPI_CONFIG(LCD_PIN_NUM_QSPI_CS, 
        NULL, 
        NULL);

    ESP_ERROR_CHECK(esp_lcd_new_panel_io_spi((esp_lcd_spi_bus_handle_t)LCD_QSPI_HOST, &io_config, &io_handle_lcd));

    
    // Initialiser la puce du pilote LCD
ESP_LOGI(TAG, "Install LCD driver");
  const  axs15231b_vendor_config_t vendor_config = {
        .init_cmds = lcd_init_cmds, // Décommenter cette ligne si vous utilisez des commandes d'initialisation personnalisées
        .init_cmds_size = sizeof(lcd_init_cmds) / sizeof(lcd_init_cmds[0]),
        .flags = {
            .use_qspi_interface = 1,
        },
    };
    const esp_lcd_panel_dev_config_t panel_config = {
        .reset_gpio_num = LCD_PIN_NUM_QSPI_RST,
        .rgb_ele_order = LCD_RGB_ELEMENT_ORDER_RGB, // Implémenté par la commande LCD `36h`
        .bits_per_pixel = 16,                       // Implémenté par la commande LCD `3Ah` (16/18)
        .vendor_config = (void *)&vendor_config,
    };
    ESP_ERROR_CHECK(esp_lcd_new_panel_axs15231b(io_handle_lcd, &panel_config, &panel_handle));

    esp_lcd_panel_reset(panel_handle);
    esp_lcd_panel_init(panel_handle);
    esp_lcd_panel_disp_on_off(panel_handle, DISPLAY_INVERT_COLOR); // false);
      // esp_lcd_panel_disp_on_off(panneau, false);
        esp_lcd_panel_swap_xy(panel_handle, DISPLAY_SWAP_XY);
        esp_lcd_panel_mirror(panel_handle, DISPLAY_MIRROR_X, DISPLAY_MIRROR_Y);

 }

/**
 * @brief Callback pour le moniteur de batterie qui met à jour les icônes UI
 */
static void battery_update_callback(battery_level_t level, uint16_t voltage_mv, uint8_t percentage)
{
    (void)level;        // Paramètres non utilisés mais requis par la signature
    (void)voltage_mv;
    (void)percentage;

    // Verrouiller LVGL avant de modifier l'UI
    lvgl_port_lock(0);
    battery_ui_update_all_icons();
    lvgl_port_unlock();
}

void app_main(void)
{
    /* Initialiser la NVS (Non-Volatile Storage) - nécessaire pour le WiFi */
    esp_err_t ret = nvs_flash_init();
    if (ret == ESP_ERR_NVS_NO_FREE_PAGES || ret == ESP_ERR_NVS_NEW_VERSION_FOUND) {
        /* La NVS doit être effacée si elle est pleine ou si une nouvelle version est détectée */
        ESP_ERROR_CHECK(nvs_flash_erase());
        ret = nvs_flash_init();
    }
    ESP_ERROR_CHECK(ret);

    /* Créer la boucle d'événements par défaut (requise pour WiFi) */
    ESP_ERROR_CHECK(esp_event_loop_create_default());

    bsp_display_brightness_init();
    bsp_display_brightness_set(0);
    touch_i2c_init();
	
    // Initialisation de l'écran
    Initialize_AXS15231B_Display();
    
    // Initialisation de l'écran tactile
    esp_lcd_touch_config_t tp_cfg = {
        .x_max = LCD_QSPI_H_RES,
        .y_max = LCD_QSPI_V_RES,
        .rst_gpio_num = LCD_PIN_NUM_QSPI_TOUCH_RST,
        .int_gpio_num = LCD_PIN_NUM_QSPI_TOUCH_INT,
        .levels = {
            .reset = 0,
            .interrupt = 0,
        },
        .flags = {
            .swap_xy = 0,
            .mirror_x = 0,
            .mirror_y = 0,
        },
    };
    const esp_lcd_panel_io_i2c_config_t tp_io_config = {
        .dev_addr = ESP_LCD_TOUCH_IO_I2C_AXS15231B_ADDRESS,
        .control_phase_bytes = 1,
        .dc_bit_offset = 0,
        .lcd_cmd_bits = 8,
        .scl_speed_hz = BSP_I2C_CLK_SPEED_HZ,
        .flags = {
            .disable_control_phase = 1,
        },
    };
    esp_lcd_panel_io_handle_t tp_io_handle = NULL;
    ESP_ERROR_CHECK(esp_lcd_new_panel_io_i2c(i2c_bus_handle, &tp_io_config, &tp_io_handle));
    ESP_ERROR_CHECK(esp_lcd_touch_new_i2c_axs15231b(tp_io_handle, &tp_cfg, &touch_handle));

    // Configuration LVGL 
    lvgl_port_cfg_t lvgl_cfg = ESP_LVGL_PORT_INIT_CONFIG();
    lvgl_cfg.task_priority = 2;
    lvgl_cfg.timer_period_ms = 40;
    ESP_ERROR_CHECK(lvgl_port_init(&lvgl_cfg));

    /* Ajouter l'écran LCD */
    ESP_LOGI(TAG, "Adding LCD display");

    const lvgl_port_display_cfg_t disp_cfg = {
        .io_handle = io_handle_lcd,
        .panel_handle = panel_handle,
        .buffer_size = LCD_QSPI_H_RES * LCD_QSPI_V_RES, //LCD_DRAW_BUFF_HEIGHT, // LCD_DRAW_BUFF_HEIGHT,
        .double_buffer = true,
        .hres = LCD_QSPI_H_RES,
        .vres = LCD_QSPI_V_RES,
        .monochrome = false,
        //.mipi_dsi = false,
        .color_format = LV_COLOR_FORMAT_RGB565,
        .rotation = {
            .swap_xy = 0, // ne fonctionne pas
            .mirror_x = 0, // fonctionne
            .mirror_y = 0, // fonctionne
        },
        .flags = {
            .buff_spiram = 1, // Utilisation du buffer PSRAM
            .buff_dma = 1,
            .swap_bytes = 1,
            .full_refresh = 1,
            .sw_rotate = 1, // true: logiciel; false: matériel
        }};

    ESP_LOGI(TAG, "[APP] Free memory: %" PRIu32 " bytes", esp_get_free_heap_size());
    lvgl_disp = lvgl_port_add_disp(&disp_cfg);

    /* Ajouter l'entrée tactile (pour l'écran sélectionné) */
    const lvgl_port_touch_cfg_t touch_cfg = {
        .disp = lvgl_disp,
        .handle = touch_handle,
    };

    lvgl_touch_indev = lvgl_port_add_touch(&touch_cfg);
    ESP_LOGI(TAG, "Touch panel initialized successfully");


    lv_display_set_rotation(lvgl_disp, LV_DISP_ROTATION_180); // Rotation 180°


    bsp_display_brightness_set(100);

    /* Installer le service ISR GPIO (requis pour l'encodeur) */
    ESP_ERROR_CHECK(gpio_install_isr_service(0));

    /* Initialiser le moniteur de batterie */
    ESP_LOGI(TAG, "Initializing battery monitor");
    ESP_ERROR_CHECK(battery_monitor_init(battery_update_callback));

    /* Création et affichage de l'interface utilisateur */
    lvgl_port_lock(0);

    /* Désactiver le watchdog pour la tâche principale pendant l'initialisation UI */
    esp_task_wdt_delete(xTaskGetIdleTaskHandleForCore(0));
    
    /* Initialiser l'interface utilisateur */
    ui_init();

    /* Réactiver le watchdog */
    esp_task_wdt_add(xTaskGetIdleTaskHandleForCore(0));

    lvgl_port_unlock();

    /* Rafraîchir les icônes batterie maintenant que l'UI est créée */
    battery_ui_update_all_icons();

    /* Initialiser l'encodeur avec intégration UI (doit être après ui_init) */
    ESP_LOGI(TAG, "Initializing encoder with UI integration");
    ESP_ERROR_CHECK(encoder_ui_init());
}


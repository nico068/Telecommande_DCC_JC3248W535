// LVGL version: 9.4

#include "ui.h"
#include "ui_helpers.h"
#include "esp_log.h"

///////////////////// VARIABLES ////////////////////

// EVENTS
lv_obj_t * ui____initial_actions0;

// IMAGES AND IMAGE SETS

///////////////////// TEST LVGL SETTINGS ////////////////////
#if LV_COLOR_DEPTH != 16
    #error "LV_COLOR_DEPTH should be 16bit to match SquareLine Studio's settings"
#endif

///////////////////// ANIMATIONS ////////////////////

///////////////////// FUNCTIONS ////////////////////

void ui_update_loco_info(const char *address, const lv_image_dsc_t *image)
{
    if (ui_LabelAdrLocoP1 != NULL) {
        lv_label_set_text(ui_LabelAdrLocoP1, address);
    }
    if (ui_imglocoP1 != NULL) {
        if (image != NULL) {
            lv_image_set_src(ui_imglocoP1, image);
            lv_obj_remove_flag(ui_imglocoP1, LV_OBJ_FLAG_HIDDEN);
        } else {
            lv_obj_add_flag(ui_imglocoP1, LV_OBJ_FLAG_HIDDEN);
        }
    }
}

void ui_update_direction(ui_loco_direction_t direction)
{
    if (ui_imgDirectionP1 != NULL) {
        if (direction == UI_LOC_DIR_REVERSE) {
            lv_image_set_src(ui_imgDirectionP1, &arrow_left_15597);
        } else {
            lv_image_set_src(ui_imgDirectionP1, &arrow_right_15604);
        }
    }
}

///////////////////// SCREENS ////////////////////

void ui_init(void)
{
    lv_display_t * dispp = lv_display_get_default();
    
    if (dispp == NULL) {
        ESP_LOGE("UI", "No display found!");
        return;
    }
    
    // LVGL 9.4: Utiliser le thème par défaut
    lv_theme_t * theme = lv_theme_default_init(
        dispp,
        lv_palette_main(LV_PALETTE_BLUE),      // Couleur primaire
        lv_palette_main(LV_PALETTE_RED),       // Couleur secondaire
        true,                                  // Mode sombre: true
        LV_FONT_DEFAULT                        // Police par défaut
    );
    
    lv_display_set_theme(dispp, theme);
    
    // Initialiser l'écran d'accueil au démarrage
    ui_Screen0_screen_init();
    
    // Pré-initialiser Screen1 pour les dépendances (ex: ui_Gaugevitesse)
    // sans le charger, afin que les composants (encodeur) puissent référencer
    // ses widgets.
    ui_Screen1_screen_init();
    
    ui____initial_actions0 = lv_obj_create(NULL);
    
    // LVGL 9.4: charger l'écran d'accueil uniquement
    lv_screen_load(ui_Screen0);
    
    // Forcer un refresh complet de l'écran pour s'assurer que tout est dessiné
    lv_obj_invalidate(ui_Screen0);
    ESP_LOGI("UI", "Screen0 loaded and invalidated for full redraw");
    
}

void ui_destroy(void)
{
    ui_Screen0_screen_destroy();
    ui_Screen1_screen_destroy();
    ui_Screen2_screen_destroy();
    ui_Screen3_screen_destroy();
    ui_Screen4_screen_destroy();
}

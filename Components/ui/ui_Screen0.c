// LVGL version: 9.4

#include "ui.h"
#include "esp_log.h"

lv_obj_t * ui_Screen0 = NULL;
lv_obj_t * ui_headerP0 = NULL;
lv_obj_t * ui_LabelPageP0 = NULL;
lv_obj_t * ui_icnwifiP0 = NULL;
lv_obj_t * ui_icnbattP0 = NULL;
lv_obj_t * ui_LabelBattPctP0 = NULL;

// Menu principal
lv_obj_t * ui_PanelMenuP0 = NULL;
lv_obj_t * ui_LabelTitleP0 = NULL;
lv_obj_t * ui_BtnCabineP0 = NULL;
lv_obj_t * ui_LabelBtnCabineP0 = NULL;
lv_obj_t * ui_BtnLocosP0 = NULL;
lv_obj_t * ui_LabelBtnLocosP0 = NULL;
lv_obj_t * ui_BtnFonctionsP0 = NULL;
lv_obj_t * ui_LabelBtnFonctionsP0 = NULL;
lv_obj_t * ui_BtnSettingP0 = NULL;
lv_obj_t * ui_LabelBtnSettingP0 = NULL;

// Event handlers
void ui_event_BtnCabineP0(lv_event_t * e)
{
    lv_event_code_t event_code = lv_event_get_code(e);
    if(event_code == LV_EVENT_PRESSED) {
        _ui_screen_change(&ui_Screen1, LV_SCR_LOAD_ANIM_FADE_ON, 200, 0, &ui_Screen1_screen_init);
    }
}

void ui_event_BtnLocosP0(lv_event_t * e)
{
    lv_event_code_t event_code = lv_event_get_code(e);
    if(event_code == LV_EVENT_PRESSED) {
        _ui_screen_change(&ui_Screen2, LV_SCR_LOAD_ANIM_FADE_ON, 200, 0, &ui_Screen2_screen_init);
    }
}

void ui_event_BtnSettingP0(lv_event_t * e)
{
    lv_event_code_t event_code = lv_event_get_code(e);
    if(event_code == LV_EVENT_PRESSED) {
        _ui_screen_change(&ui_Screen4, LV_SCR_LOAD_ANIM_FADE_ON, 200, 0, &ui_Screen4_screen_init);
    }
}

// Initialisation de l'écran
void ui_Screen0_screen_init(void)
{
    ESP_LOGI("UI_SCREEN0", "=== Début initialisation Screen0 ===");
    ui_Screen0 = lv_obj_create(NULL);
    lv_obj_remove_flag(ui_Screen0, LV_OBJ_FLAG_SCROLLABLE);      /// Flags
    lv_obj_set_style_bg_color(ui_Screen0, lv_color_hex(0x000000), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(ui_Screen0, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
    ESP_LOGI("UI_SCREEN0", "Screen0 créé (fond noir)");

    // En-tête
    ui_headerP0 = lv_obj_create(ui_Screen0);
    lv_obj_remove_style_all(ui_headerP0);
    lv_obj_set_width(ui_headerP0, 320);
    lv_obj_set_height(ui_headerP0, 40);
    lv_obj_set_x(ui_headerP0, 0);
    lv_obj_set_y(ui_headerP0, -220);
    lv_obj_set_align(ui_headerP0, LV_ALIGN_CENTER);
    lv_obj_remove_flag(ui_headerP0, LV_OBJ_FLAG_CLICKABLE | LV_OBJ_FLAG_SCROLLABLE);      /// Flags
    lv_obj_set_style_radius(ui_headerP0, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_color(ui_headerP0, lv_color_hex(0x292831), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(ui_headerP0, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_border_color(ui_headerP0, lv_color_hex(0x000000), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_border_opa(ui_headerP0, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_border_width(ui_headerP0, 0, LV_PART_MAIN | LV_STATE_DEFAULT);

    // Label page
    ui_LabelPageP0 = lv_label_create(ui_headerP0);
    lv_obj_set_width(ui_LabelPageP0, LV_SIZE_CONTENT);   /// 1
    lv_obj_set_height(ui_LabelPageP0, LV_SIZE_CONTENT);    /// 1
    lv_obj_set_align(ui_LabelPageP0, LV_ALIGN_CENTER);
    lv_label_set_text(ui_LabelPageP0, "Menu");
    lv_obj_set_style_text_color(ui_LabelPageP0, lv_color_hex(0xFFFFFF), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_opa(ui_LabelPageP0, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_font(ui_LabelPageP0, &lv_font_montserrat_28, LV_PART_MAIN | LV_STATE_DEFAULT);    
    
    // Icône Batterie
    ui_icnbattP0 = lv_image_create(ui_headerP0);
    lv_image_set_src(ui_icnbattP0, &bat_pleine); // Image par défaut
    lv_obj_set_width(ui_icnbattP0, LV_SIZE_CONTENT);   /// 30
    lv_obj_set_height(ui_icnbattP0, LV_SIZE_CONTENT);    /// 30
    lv_obj_set_x(ui_icnbattP0, -135);
    lv_obj_set_y(ui_icnbattP0, 0);
    lv_obj_set_align(ui_icnbattP0, LV_ALIGN_CENTER);
    lv_obj_add_flag(ui_icnbattP0, LV_OBJ_FLAG_CLICKABLE);     /// Flags
    lv_obj_remove_flag(ui_icnbattP0, LV_OBJ_FLAG_SCROLLABLE);      /// Flags

    // Label pourcentage batterie
    ui_LabelBattPctP0 = lv_label_create(ui_headerP0);
    lv_obj_set_width(ui_LabelBattPctP0, LV_SIZE_CONTENT);
    lv_obj_set_height(ui_LabelBattPctP0, LV_SIZE_CONTENT);
    lv_obj_set_align(ui_LabelBattPctP0, LV_ALIGN_CENTER);
    lv_obj_set_x(ui_LabelBattPctP0, -90); // à droite de l’icône
    lv_obj_set_y(ui_LabelBattPctP0, 0);
    lv_label_set_text(ui_LabelBattPctP0, "100%"); // Valeur par défaut, à mettre à jour dynamiquement
    lv_obj_set_style_text_color(ui_LabelBattPctP0, lv_color_hex(0xFFFFFF), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_font(ui_LabelBattPctP0, &lv_font_montserrat_18, LV_PART_MAIN | LV_STATE_DEFAULT);

    // Icône WiFi
    ui_icnwifiP0 = lv_image_create(ui_headerP0);
    lv_image_set_src(ui_icnwifiP0, &wifi0); // Image par défaut (déconnecté)
    lv_obj_set_width(ui_icnwifiP0, LV_SIZE_CONTENT);   /// 32
    lv_obj_set_height(ui_icnwifiP0, LV_SIZE_CONTENT);    /// 32
    lv_obj_set_x(ui_icnwifiP0, 135);
    lv_obj_set_y(ui_icnwifiP0, 0);
    lv_obj_set_align(ui_icnwifiP0, LV_ALIGN_CENTER);
    lv_obj_add_flag(ui_icnwifiP0, LV_OBJ_FLAG_CLICKABLE);     /// Flags
    lv_obj_remove_flag(ui_icnwifiP0, LV_OBJ_FLAG_SCROLLABLE);      /// Flags
    
    // Panel menu principal
    ui_PanelMenuP0 = lv_obj_create(ui_Screen0);
    lv_obj_remove_style_all(ui_PanelMenuP0);
    lv_obj_set_width(ui_PanelMenuP0, 280);
    lv_obj_set_height(ui_PanelMenuP0, 379);
    lv_obj_set_x(ui_PanelMenuP0, 0);
    lv_obj_set_y(ui_PanelMenuP0, 6);
    lv_obj_set_align(ui_PanelMenuP0, LV_ALIGN_CENTER);
    lv_obj_set_flex_flow(ui_PanelMenuP0, LV_FLEX_FLOW_COLUMN);
    lv_obj_set_flex_align(ui_PanelMenuP0, LV_FLEX_ALIGN_SPACE_AROUND, LV_FLEX_ALIGN_CENTER, LV_FLEX_ALIGN_CENTER);
    lv_obj_remove_flag(ui_PanelMenuP0, LV_OBJ_FLAG_CLICKABLE | LV_OBJ_FLAG_SCROLLABLE);      /// Flags

    // Bouton Cabine (Screen1)
    ui_BtnCabineP0 = lv_button_create(ui_PanelMenuP0);
    lv_obj_set_width(ui_BtnCabineP0, 160);
    lv_obj_set_height(ui_BtnCabineP0, 60);
    lv_obj_set_align(ui_BtnCabineP0, LV_ALIGN_CENTER);
    lv_obj_add_flag(ui_BtnCabineP0, LV_OBJ_FLAG_SCROLL_ON_FOCUS);     /// Flags
    lv_obj_remove_flag(ui_BtnCabineP0, LV_OBJ_FLAG_SCROLLABLE);      /// Flags
    lv_obj_set_style_bg_color(ui_BtnCabineP0, lv_color_hex(0x7F7F7F), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(ui_BtnCabineP0, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_grad_color(ui_BtnCabineP0, lv_color_hex(0xB5B5B5), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_grad_dir(ui_BtnCabineP0, LV_GRAD_DIR_VER, LV_PART_MAIN | LV_STATE_DEFAULT);

    ui_LabelBtnCabineP0 = lv_label_create(ui_BtnCabineP0);
    lv_obj_set_width(ui_LabelBtnCabineP0, LV_SIZE_CONTENT);
    lv_obj_set_height(ui_LabelBtnCabineP0, LV_SIZE_CONTENT);
    lv_obj_set_align(ui_LabelBtnCabineP0, LV_ALIGN_CENTER);
    lv_label_set_text(ui_LabelBtnCabineP0, "Cabine");
    lv_obj_set_style_text_color(ui_LabelBtnCabineP0, lv_color_hex(0xFFFFFF), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_font(ui_LabelBtnCabineP0, &lv_font_montserrat_20, LV_PART_MAIN | LV_STATE_DEFAULT);

    // Bouton Locomotives (Screen2)
    ui_BtnLocosP0 = lv_button_create(ui_PanelMenuP0);
    lv_obj_set_width(ui_BtnLocosP0, 160);
    lv_obj_set_height(ui_BtnLocosP0, 60);
    lv_obj_set_align(ui_BtnLocosP0, LV_ALIGN_CENTER);
    lv_obj_add_flag(ui_BtnLocosP0, LV_OBJ_FLAG_SCROLL_ON_FOCUS);     /// Flags
    lv_obj_remove_flag(ui_BtnLocosP0, LV_OBJ_FLAG_SCROLLABLE);      /// Flags
    lv_obj_set_style_bg_color(ui_BtnLocosP0, lv_color_hex(0x7F7F7F), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(ui_BtnLocosP0, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_grad_color(ui_BtnLocosP0, lv_color_hex(0xB5B5B5), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_grad_dir(ui_BtnLocosP0, LV_GRAD_DIR_VER, LV_PART_MAIN | LV_STATE_DEFAULT);

    ui_LabelBtnLocosP0 = lv_label_create(ui_BtnLocosP0);
    lv_obj_set_width(ui_LabelBtnLocosP0, LV_SIZE_CONTENT);   /// 1
    lv_obj_set_height(ui_LabelBtnLocosP0, LV_SIZE_CONTENT);    /// 1
    lv_obj_set_align(ui_LabelBtnLocosP0, LV_ALIGN_CENTER);
    lv_label_set_text(ui_LabelBtnLocosP0, "Loco");
    lv_obj_set_style_text_font(ui_LabelBtnLocosP0, &lv_font_montserrat_24, LV_PART_MAIN | LV_STATE_DEFAULT);

    // Bouton Paramètres (Screen4)
    ui_BtnSettingP0 = lv_button_create(ui_PanelMenuP0);
    lv_obj_set_width(ui_BtnSettingP0, 160);
    lv_obj_set_height(ui_BtnSettingP0, 60);
    lv_obj_set_align(ui_BtnSettingP0, LV_ALIGN_CENTER);
    lv_obj_add_flag(ui_BtnSettingP0, LV_OBJ_FLAG_SCROLL_ON_FOCUS);     /// Flags
    lv_obj_remove_flag(ui_BtnSettingP0, LV_OBJ_FLAG_SCROLLABLE);      /// Flags
    lv_obj_set_style_bg_color(ui_BtnSettingP0, lv_color_hex(0x7F7F7F), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(ui_BtnSettingP0, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_grad_color(ui_BtnSettingP0, lv_color_hex(0xB5B5B5), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_grad_dir(ui_BtnSettingP0, LV_GRAD_DIR_VER, LV_PART_MAIN | LV_STATE_DEFAULT);
    ui_LabelBtnSettingP0 = lv_label_create(ui_BtnSettingP0);
    lv_obj_set_width(ui_LabelBtnSettingP0, LV_SIZE_CONTENT);   /// 1
    lv_obj_set_height(ui_LabelBtnSettingP0, LV_SIZE_CONTENT);    /// 1
    lv_obj_set_align(ui_LabelBtnSettingP0, LV_ALIGN_CENTER);
    lv_label_set_text(ui_LabelBtnSettingP0, "Parametre");
    lv_obj_set_style_text_font(ui_LabelBtnSettingP0, &lv_font_montserrat_24, LV_PART_MAIN | LV_STATE_DEFAULT);

    // Attache l'événement au bouton
    lv_obj_add_event_cb(ui_BtnCabineP0, ui_event_BtnCabineP0, LV_EVENT_ALL, NULL);
    lv_obj_add_event_cb(ui_BtnLocosP0, ui_event_BtnLocosP0, LV_EVENT_ALL, NULL);
    lv_obj_add_event_cb(ui_BtnSettingP0, ui_event_BtnSettingP0, LV_EVENT_ALL, NULL);

    ESP_LOGI("UI_SCREEN0", "=== Screen0 initialisé: 4 boutons + header ===");
}

void ui_Screen0_screen_destroy(void)
{
    if(ui_Screen0 != NULL) {
        lv_obj_del(ui_Screen0);
        ui_Screen0 = NULL;
    }
}

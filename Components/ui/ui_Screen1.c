// LVGL version: 9.4

#include "ui.h"

lv_obj_t * ui_Screen1 = NULL;
lv_obj_t * ui_Panel1 = NULL;
lv_obj_t * ui_headerP1 = NULL;
lv_obj_t * ui_LabelPageP1 = NULL;
lv_obj_t * ui_icnbleP1 = NULL;
lv_obj_t * ui_icnwifiP1 = NULL;
lv_obj_t * ui_icnbattP1 = NULL;
lv_obj_t * ui_PanelBtnP1 = NULL;
lv_obj_t * ui_BtnMenuP1 = NULL;
lv_obj_t * ui_LabelBtnMenuP1 = NULL;
lv_obj_t * ui_BtnAdrLocoP1 = NULL;
lv_obj_t * ui_LabelBtnAdrLocoP1 = NULL;
lv_obj_t * ui_BtnFonctionP1 = NULL;
lv_obj_t * ui_LabelBtnFonctionP1 = NULL;
lv_obj_t * ui_infolocoP1 = NULL;
lv_obj_t * ui_LabelAdrLocoP1 = NULL;
lv_obj_t * ui_imglocoP1 = NULL;
lv_obj_t * ui_imgDirectionP1 = NULL;  // Image de direction (arrow)
lv_obj_t * ui_PanelGauge = NULL;
lv_obj_t * ui_Gaugevitesse = NULL;
lv_obj_t * ui_PanelVitesse = NULL;
lv_obj_t * ui_LabelVitesse = NULL;
lv_obj_t * ui_panelfonction = NULL;
lv_obj_t * ui_BtnF0 = NULL;
lv_obj_t * ui_LabelF0 = NULL;
lv_obj_t * ui_LedF0 = NULL;
lv_obj_t * ui_PanelbtnF0 = NULL;
lv_obj_t * ui_BtnF1 = NULL;
lv_obj_t * ui_LabelF1 = NULL;
lv_obj_t * ui_LedF1 = NULL;
lv_obj_t * ui_PanelbtnF1 = NULL;
lv_obj_t * ui_BtnF2 = NULL;
lv_obj_t * ui_LabelF2 = NULL;
lv_obj_t * ui_LedF2 = NULL;
lv_obj_t * ui_PanelbtnF2 = NULL;
lv_obj_t * ui_BtnF3 = NULL;
lv_obj_t * ui_LabelF3 = NULL;
lv_obj_t * ui_LedF3 = NULL;
lv_obj_t * ui_PanelbtnF3 = NULL;

// États des fonctions (pour tracking)
static bool function_states[4] = {false, false, false, false};

// Rappel : F0 à F3 sont gérés ici (écran cabine), F4 à F12 sont sur l'écran des fonctions (Screen3)

// Event handlers
void ui_event_BtnMenuP1(lv_event_t * e)
{
    lv_event_code_t event_code = lv_event_get_code(e);

    if(event_code == LV_EVENT_CLICKED) {
        _ui_screen_change(&ui_Screen0, LV_SCR_LOAD_ANIM_NONE, 0, 0, &ui_Screen0_screen_init);
    }
}

void ui_event_BtnAdrLocoP1(lv_event_t * e)
{
    lv_event_code_t event_code = lv_event_get_code(e);

    if(event_code == LV_EVENT_CLICKED) {
        _ui_screen_change(&ui_Screen2, LV_SCR_LOAD_ANIM_NONE, 0, 0, &ui_Screen2_screen_init);
    }
}

void ui_event_BtnFonctionP1(lv_event_t * e)
{
    lv_event_code_t event_code = lv_event_get_code(e);

    if(event_code == LV_EVENT_CLICKED) {
        _ui_screen_change(&ui_Screen3, LV_SCR_LOAD_ANIM_NONE, 0, 0, &ui_Screen3_screen_init);
    }
}

void ui_event_Gaugevitesse(lv_event_t * e)
{
    lv_event_code_t event_code = lv_event_get_code(e);
    lv_obj_t * target = lv_event_get_target(e);

    if(event_code == LV_EVENT_VALUE_CHANGED) {
        _ui_arc_set_text_value(ui_LabelVitesse, target, "", "");
    }
}

void ui_event_BtnF0(lv_event_t * e)
{
    lv_event_code_t event_code = lv_event_get_code(e);
    if(event_code == LV_EVENT_CLICKED) {
        function_states[0] = !function_states[0];
        if(function_states[0]) {
            lv_obj_add_state(ui_BtnF0, LV_STATE_CHECKED);
            lv_led_on(ui_LedF0);
        } else {
            lv_obj_clear_state(ui_BtnF0, LV_STATE_CHECKED);
            lv_led_off(ui_LedF0);
        }
    }
}

void ui_event_BtnF1(lv_event_t * e)
{
    lv_event_code_t event_code = lv_event_get_code(e);
    if(event_code == LV_EVENT_CLICKED) {
        function_states[1] = !function_states[1];
        if(function_states[1]) {
            lv_obj_add_state(ui_BtnF1, LV_STATE_CHECKED);
            lv_led_on(ui_LedF1);
        } else {
            lv_obj_clear_state(ui_BtnF1, LV_STATE_CHECKED);
            lv_led_off(ui_LedF1);
        }
    }
}

void ui_event_BtnF2(lv_event_t * e)
{
    lv_event_code_t event_code = lv_event_get_code(e);
    if(event_code == LV_EVENT_CLICKED) {
        function_states[2] = !function_states[2];
        if(function_states[2]) {
            lv_obj_add_state(ui_BtnF2, LV_STATE_CHECKED);
            lv_led_on(ui_LedF2);
        } else {
            lv_obj_clear_state(ui_BtnF2, LV_STATE_CHECKED);
            lv_led_off(ui_LedF2);
        }
    }
}

void ui_event_BtnF3(lv_event_t * e)
{
    lv_event_code_t event_code = lv_event_get_code(e);
    if(event_code == LV_EVENT_CLICKED) {
        function_states[3] = !function_states[3];
        if(function_states[3]) {
            lv_obj_add_state(ui_BtnF3, LV_STATE_CHECKED);
            lv_led_on(ui_LedF3);
        } else {
            lv_obj_clear_state(ui_BtnF3, LV_STATE_CHECKED);
            lv_led_off(ui_LedF3);
        }
    }
}

// Build functions
void ui_Screen1_screen_init(void)
{
    ui_Screen1 = lv_obj_create(NULL);
    lv_obj_remove_flag(ui_Screen1, LV_OBJ_FLAG_SCROLLABLE);      /// Flags
    
    // En-tête
    ui_headerP1 = lv_obj_create(ui_Screen1);
    lv_obj_remove_style_all(ui_headerP1);
    lv_obj_set_width(ui_headerP1, 320);
    lv_obj_set_height(ui_headerP1, 40);
    lv_obj_set_x(ui_headerP1, 0);
    lv_obj_set_y(ui_headerP1, -220);
    lv_obj_set_align(ui_headerP1, LV_ALIGN_CENTER);
    lv_obj_remove_flag(ui_headerP1, LV_OBJ_FLAG_CLICKABLE | LV_OBJ_FLAG_SCROLLABLE);      /// Flags
    lv_obj_set_style_radius(ui_headerP1, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_color(ui_headerP1, lv_color_hex(0x292831), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(ui_headerP1, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_border_color(ui_headerP1, lv_color_hex(0x000000), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_border_opa(ui_headerP1, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_border_width(ui_headerP1, 0, LV_PART_MAIN | LV_STATE_DEFAULT);

    ui_LabelPageP1 = lv_label_create(ui_headerP1);
    lv_obj_set_width(ui_LabelPageP1, LV_SIZE_CONTENT);   /// 1
    lv_obj_set_height(ui_LabelPageP1, LV_SIZE_CONTENT);    /// 1
    lv_obj_set_align(ui_LabelPageP1, LV_ALIGN_CENTER);
    lv_label_set_text(ui_LabelPageP1, "Cabine");
    lv_obj_set_style_text_color(ui_LabelPageP1, lv_color_hex(0xFFFFFF), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_opa(ui_LabelPageP1, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_font(ui_LabelPageP1, &lv_font_montserrat_28, LV_PART_MAIN | LV_STATE_DEFAULT); 

    // Icône Batterie
    ui_icnbattP1 = lv_image_create(ui_headerP1);
    lv_image_set_src(ui_icnbattP1, &bat_pleine); // image par défaut
    lv_obj_set_width(ui_icnbattP1, LV_SIZE_CONTENT);
    lv_obj_set_height(ui_icnbattP1, LV_SIZE_CONTENT);
    lv_obj_set_x(ui_icnbattP1, -135);
    lv_obj_set_y(ui_icnbattP1, 0);
    lv_obj_set_align(ui_icnbattP1, LV_ALIGN_CENTER);
    lv_obj_add_flag(ui_icnbattP1, LV_OBJ_FLAG_CLICKABLE);     /// Flags
    lv_obj_remove_flag(ui_icnbattP1, LV_OBJ_FLAG_SCROLLABLE);      /// Flags

    // Icône WiFi
    ui_icnwifiP1 = lv_image_create(ui_headerP1);
    lv_image_set_src(ui_icnwifiP1, &wifi0); // image par défaut (déconnecté)
    lv_obj_set_width(ui_icnwifiP1, LV_SIZE_CONTENT);
    lv_obj_set_height(ui_icnwifiP1, LV_SIZE_CONTENT);
    lv_obj_set_x(ui_icnwifiP1, 135);
    lv_obj_set_y(ui_icnwifiP1, 0);
    lv_obj_set_align(ui_icnwifiP1, LV_ALIGN_CENTER);
    lv_obj_add_flag(ui_icnwifiP1, LV_OBJ_FLAG_CLICKABLE);     /// Flags
    lv_obj_remove_flag(ui_icnwifiP1, LV_OBJ_FLAG_SCROLLABLE);      /// Flags

    // Panel boutons de navigation
    ui_PanelBtnP1 = lv_obj_create(ui_Screen1);
    lv_obj_remove_style_all(ui_PanelBtnP1);
    lv_obj_set_width(ui_PanelBtnP1, 320);
    lv_obj_set_height(ui_PanelBtnP1, 60);
    lv_obj_set_x(ui_PanelBtnP1, 0);
    lv_obj_set_y(ui_PanelBtnP1, -165);
    lv_obj_set_align(ui_PanelBtnP1, LV_ALIGN_CENTER);
    lv_obj_set_flex_flow(ui_PanelBtnP1, LV_FLEX_FLOW_ROW);
    lv_obj_set_flex_align(ui_PanelBtnP1, LV_FLEX_ALIGN_SPACE_AROUND, LV_FLEX_ALIGN_CENTER, LV_FLEX_ALIGN_CENTER);
    lv_obj_remove_flag(ui_PanelBtnP1, LV_OBJ_FLAG_CLICKABLE | LV_OBJ_FLAG_SCROLLABLE);      /// Flags

    // Bouton Menu
    ui_BtnMenuP1 = lv_btn_create(ui_PanelBtnP1);
    lv_obj_set_width(ui_BtnMenuP1, 95);
    lv_obj_set_height(ui_BtnMenuP1, 50);
    lv_obj_set_align(ui_BtnMenuP1, LV_ALIGN_CENTER);
    lv_obj_add_flag(ui_BtnMenuP1, LV_OBJ_FLAG_SCROLL_ON_FOCUS);     /// Flags
    lv_obj_remove_flag(ui_BtnMenuP1, LV_OBJ_FLAG_SCROLLABLE);      /// Flags
    lv_obj_set_style_bg_color(ui_BtnMenuP1, lv_color_hex(0x7F7F7F), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(ui_BtnMenuP1, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_grad_color(ui_BtnMenuP1, lv_color_hex(0xB5B5B5), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_grad_dir(ui_BtnMenuP1, LV_GRAD_DIR_VER, LV_PART_MAIN | LV_STATE_DEFAULT);

    ui_LabelBtnMenuP1 = lv_label_create(ui_BtnMenuP1);
    lv_obj_set_width(ui_LabelBtnMenuP1, LV_SIZE_CONTENT);
    lv_obj_set_height(ui_LabelBtnMenuP1, LV_SIZE_CONTENT);
    lv_obj_set_align(ui_LabelBtnMenuP1, LV_ALIGN_CENTER);
    lv_label_set_text(ui_LabelBtnMenuP1, "Menu");
    lv_obj_set_style_text_font(ui_LabelBtnMenuP1, &lv_font_montserrat_18, LV_PART_MAIN | LV_STATE_DEFAULT);

    // Bouton Adresse Loco
    ui_BtnAdrLocoP1 = lv_btn_create(ui_PanelBtnP1);
    lv_obj_set_width(ui_BtnAdrLocoP1, 95);
    lv_obj_set_height(ui_BtnAdrLocoP1, 50);
    lv_obj_set_align(ui_BtnAdrLocoP1, LV_ALIGN_CENTER);
    lv_obj_add_flag(ui_BtnAdrLocoP1, LV_OBJ_FLAG_SCROLL_ON_FOCUS);     /// Flags
    lv_obj_remove_flag(ui_BtnAdrLocoP1, LV_OBJ_FLAG_SCROLLABLE);      /// Flags
    lv_obj_set_style_bg_color(ui_BtnAdrLocoP1, lv_color_hex(0x7F7F7F), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(ui_BtnAdrLocoP1, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_grad_color(ui_BtnAdrLocoP1, lv_color_hex(0xB5B5B5), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_grad_dir(ui_BtnAdrLocoP1, LV_GRAD_DIR_VER, LV_PART_MAIN | LV_STATE_DEFAULT);

    ui_LabelBtnAdrLocoP1 = lv_label_create(ui_BtnAdrLocoP1);
    lv_obj_set_width(ui_LabelBtnAdrLocoP1, LV_SIZE_CONTENT);
    lv_obj_set_height(ui_LabelBtnAdrLocoP1, LV_SIZE_CONTENT);
    lv_obj_set_align(ui_LabelBtnAdrLocoP1, LV_ALIGN_CENTER);
    lv_label_set_text(ui_LabelBtnAdrLocoP1, "Loco");
    lv_obj_set_style_text_font(ui_LabelBtnAdrLocoP1, &lv_font_montserrat_18, LV_PART_MAIN | LV_STATE_DEFAULT);

    // Bouton Fonctions
    ui_BtnFonctionP1 = lv_btn_create(ui_PanelBtnP1);
    lv_obj_set_width(ui_BtnFonctionP1, 95);
    lv_obj_set_height(ui_BtnFonctionP1, 50);
    lv_obj_set_align(ui_BtnFonctionP1, LV_ALIGN_CENTER);
    lv_obj_add_flag(ui_BtnFonctionP1, LV_OBJ_FLAG_SCROLL_ON_FOCUS);     /// Flags
    lv_obj_remove_flag(ui_BtnFonctionP1, LV_OBJ_FLAG_SCROLLABLE);      /// Flags
    lv_obj_set_style_bg_color(ui_BtnFonctionP1, lv_color_hex(0x7F7F7F), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(ui_BtnFonctionP1, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_grad_color(ui_BtnFonctionP1, lv_color_hex(0xB5B5B5), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_grad_dir(ui_BtnFonctionP1, LV_GRAD_DIR_VER, LV_PART_MAIN | LV_STATE_DEFAULT);

    ui_LabelBtnFonctionP1 = lv_label_create(ui_BtnFonctionP1);
    lv_obj_set_width(ui_LabelBtnFonctionP1, LV_SIZE_CONTENT);
    lv_obj_set_height(ui_LabelBtnFonctionP1, LV_SIZE_CONTENT);
    lv_obj_set_align(ui_LabelBtnFonctionP1, LV_ALIGN_CENTER);
    lv_label_set_text(ui_LabelBtnFonctionP1, "Fonctions");
    lv_obj_set_style_text_font(ui_LabelBtnFonctionP1, &lv_font_montserrat_18, LV_PART_MAIN | LV_STATE_DEFAULT);
    
    // Info Loco
    ui_infolocoP1 = lv_obj_create(ui_Screen1);
    lv_obj_set_width(ui_infolocoP1, 200);
    lv_obj_set_height(ui_infolocoP1, 80);
    lv_obj_set_x(ui_infolocoP1, 0);
    lv_obj_set_y(ui_infolocoP1, -95);
    lv_obj_set_align(ui_infolocoP1, LV_ALIGN_CENTER);
    lv_obj_clear_flag(ui_infolocoP1, LV_OBJ_FLAG_SCROLLABLE);

    ui_imglocoP1 = lv_image_create(ui_infolocoP1);
    // Pas d'image par défaut : sera renseignée après validation dans Screen2
    lv_image_set_src(ui_imglocoP1, NULL);
    lv_obj_set_width(ui_imglocoP1, LV_SIZE_CONTENT);
    lv_obj_set_height(ui_imglocoP1, LV_SIZE_CONTENT);
    lv_obj_set_x(ui_imglocoP1, 0);
    lv_obj_set_y(ui_imglocoP1, -11);
    lv_obj_set_align(ui_imglocoP1, LV_ALIGN_CENTER);
    lv_obj_add_flag(ui_imglocoP1, LV_OBJ_FLAG_CLICKABLE);
    lv_obj_add_flag(ui_imglocoP1, LV_OBJ_FLAG_HIDDEN);  // Masquée au départ
    lv_obj_clear_flag(ui_imglocoP1, LV_OBJ_FLAG_SCROLLABLE);

    ui_LabelAdrLocoP1 = lv_label_create(ui_infolocoP1);
    lv_obj_set_width(ui_LabelAdrLocoP1, LV_SIZE_CONTENT);
    lv_obj_set_height(ui_LabelAdrLocoP1, LV_SIZE_CONTENT);
    lv_obj_set_x(ui_LabelAdrLocoP1, 0);
    lv_obj_set_y(ui_LabelAdrLocoP1, 24);
    lv_obj_set_align(ui_LabelAdrLocoP1, LV_ALIGN_CENTER);
    lv_label_set_text(ui_LabelAdrLocoP1, "...");
    lv_obj_set_style_text_color(ui_LabelAdrLocoP1, lv_color_hex(0xDA9902), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_opa(ui_LabelAdrLocoP1, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_font(ui_LabelAdrLocoP1, &lv_font_montserrat_20, LV_PART_MAIN | LV_STATE_DEFAULT);

    // ========== SPEEDO-STYLE GAUGE ==========
    
    // Constantes et palette (définies une seule fois)
    static const int rpm_max = 127;
    static const int rpm_redline = 110;
    static const int rpm_arc_width = 16;
    static const int rpm_line_width = 3;
    static const int dimension = 200;
    static const int rpm_arc_size = dimension - (rpm_line_width * 10);
    static const int arc_start = 120;
    static const int arc_end = 344;
    static const float marker_gap_val = ((float)arc_end - arc_start) / (rpm_max / 10);
    
    static const lv_color_t palette_amber = LV_COLOR_MAKE(250, 140, 0);
    static const lv_color_t palette_black = LV_COLOR_MAKE(0, 0, 0);
    static const lv_color_t palette_red = LV_COLOR_MAKE(255, 0, 0);
    static const lv_color_t palette_white = LV_COLOR_MAKE(255, 255, 255);
    
    // Paramètres calculés
    const float marker_gap = marker_gap_val;

    // Panel Gauge container
    ui_PanelGauge = lv_obj_create(ui_Screen1);
    lv_obj_set_width(ui_PanelGauge, dimension);
    lv_obj_set_height(ui_PanelGauge, dimension);
    lv_obj_set_x(ui_PanelGauge, 0);
    lv_obj_set_y(ui_PanelGauge, 50);
    lv_obj_set_align(ui_PanelGauge, LV_ALIGN_CENTER);
    lv_obj_clear_flag(ui_PanelGauge, LV_OBJ_FLAG_SCROLLABLE);
    lv_obj_set_style_bg_color(ui_PanelGauge, palette_black, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_border_width(ui_PanelGauge, 0, LV_PART_MAIN | LV_STATE_DEFAULT);

    // Arc principal (rpm_arc équivalent)
    ui_Gaugevitesse = lv_arc_create(ui_PanelGauge);
    lv_obj_set_size(ui_Gaugevitesse, rpm_arc_size, rpm_arc_size);
    lv_obj_align(ui_Gaugevitesse, LV_ALIGN_CENTER, 0, 0);
    lv_arc_set_bg_angles(ui_Gaugevitesse, arc_start + 2, arc_end - 1);
    lv_obj_set_style_arc_color(ui_Gaugevitesse, palette_black, LV_PART_MAIN);
    lv_obj_set_style_arc_color(ui_Gaugevitesse, palette_amber, LV_PART_INDICATOR);
    lv_obj_set_style_arc_rounded(ui_Gaugevitesse, false, LV_PART_INDICATOR);
    lv_obj_set_style_arc_width(ui_Gaugevitesse, rpm_arc_width, LV_PART_INDICATOR);
    lv_obj_remove_style(ui_Gaugevitesse, NULL, LV_PART_KNOB);
    lv_arc_set_range(ui_Gaugevitesse, 0, rpm_max);
    lv_arc_set_value(ui_Gaugevitesse, 0);

    // Bordure intérieure (rpm_arc_border)
    lv_obj_t *rpm_arc_border = lv_arc_create(ui_PanelGauge);
    lv_obj_set_size(rpm_arc_border, rpm_arc_size - (rpm_arc_width * 2) - (rpm_line_width * 2), 
                                     rpm_arc_size - (rpm_arc_width * 2) - (rpm_line_width * 2));
    lv_obj_align(rpm_arc_border, LV_ALIGN_CENTER, 0, 0);
    lv_arc_set_bg_angles(rpm_arc_border, arc_start, arc_end);
    lv_obj_set_style_arc_width(rpm_arc_border, rpm_line_width, LV_PART_MAIN);
    lv_obj_set_style_arc_rounded(rpm_arc_border, false, LV_PART_MAIN);
    lv_obj_set_style_arc_color(rpm_arc_border, palette_white, LV_PART_MAIN);
    lv_obj_remove_style(rpm_arc_border, NULL, LV_PART_KNOB);
    lv_obj_remove_style(rpm_arc_border, NULL, LV_PART_INDICATOR);

    // Marqueurs (rpm_markers)
    int rpm_marker_count = (rpm_max / 10) + 1;
    for (int i = 0; i < rpm_marker_count; i++) {
        lv_obj_t *rpm_marker = lv_arc_create(ui_PanelGauge);
        lv_obj_set_size(rpm_marker, rpm_arc_size + (rpm_line_width * 2), rpm_arc_size + (rpm_line_width * 2));
        lv_obj_align(rpm_marker, LV_ALIGN_CENTER, 0, 0);
        lv_obj_set_style_arc_width(rpm_marker, rpm_arc_width + (rpm_line_width * 2), LV_PART_MAIN);
        lv_obj_set_style_arc_rounded(rpm_marker, false, LV_PART_MAIN);
        lv_obj_set_style_arc_color(rpm_marker, palette_white, LV_PART_MAIN);
        lv_arc_set_bg_angles(rpm_marker, arc_start + (i * marker_gap), arc_start + (i * marker_gap) + 1);
        lv_obj_remove_style(rpm_marker, NULL, LV_PART_KNOB);
        lv_obj_remove_style(rpm_marker, NULL, LV_PART_INDICATOR);
    }

    // Zone redline (rpm_redline_marker)
    lv_obj_t *rpm_redline_marker = lv_arc_create(ui_PanelGauge);
    lv_obj_set_size(rpm_redline_marker, dimension, dimension);
    lv_obj_align(rpm_redline_marker, LV_ALIGN_CENTER, 0, 0);
    lv_arc_set_bg_angles(rpm_redline_marker, 
                         arc_start + ((arc_end - arc_start) * ((float)rpm_redline / rpm_max)), 
                         arc_end);
    lv_obj_set_style_arc_color(rpm_redline_marker, palette_red, LV_PART_MAIN);
    lv_obj_set_style_arc_rounded(rpm_redline_marker, false, LV_PART_MAIN);
    lv_obj_set_style_arc_width(rpm_redline_marker, rpm_line_width * 1, LV_PART_MAIN);
    lv_obj_remove_style(rpm_redline_marker, NULL, LV_PART_KNOB);
    lv_obj_remove_style(rpm_redline_marker, NULL, LV_PART_INDICATOR);

    // Flèche direction (au-dessus de la vitesse)
    ui_imgDirectionP1 = lv_image_create(ui_PanelGauge);
    lv_image_set_src(ui_imgDirectionP1, &arrow_right_15604);
    lv_obj_align(ui_imgDirectionP1, LV_ALIGN_CENTER, 70, 15);    

    // Panel vitesse invisible (pour structurer)
    ui_PanelVitesse = lv_obj_create(ui_PanelGauge);
    lv_obj_set_size(ui_PanelVitesse, 100, 80);
    lv_obj_align(ui_PanelVitesse, LV_ALIGN_CENTER, 0, 0);
    lv_obj_clear_flag(ui_PanelVitesse, LV_OBJ_FLAG_SCROLLABLE);
    lv_obj_set_style_bg_opa(ui_PanelVitesse, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_border_width(ui_PanelVitesse, 0, LV_PART_MAIN | LV_STATE_DEFAULT);

    // Label vitesse (style mph_label du speedo)
    ui_LabelVitesse = lv_label_create(ui_PanelGauge);
    lv_label_set_text(ui_LabelVitesse, "0");
    lv_obj_set_style_text_font(ui_LabelVitesse, &lv_font_montserrat_24, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_color(ui_LabelVitesse, palette_white, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_align(ui_LabelVitesse, LV_ALIGN_CENTER, 0, 0);

    // Panel fonctions
    ui_panelfonction = lv_obj_create(ui_Screen1);
    lv_obj_remove_style_all(ui_panelfonction);
    lv_obj_set_width(ui_panelfonction, 320);
    lv_obj_set_height(ui_panelfonction, 106);
    lv_obj_set_x(ui_panelfonction, 0);
    lv_obj_set_y(ui_panelfonction, 180);
    lv_obj_set_align(ui_panelfonction, LV_ALIGN_CENTER);
    lv_obj_set_flex_flow(ui_panelfonction, LV_FLEX_FLOW_ROW);
    lv_obj_set_flex_align(ui_panelfonction, LV_FLEX_ALIGN_SPACE_AROUND, LV_FLEX_ALIGN_CENTER, LV_FLEX_ALIGN_CENTER);
    lv_obj_remove_flag(ui_panelfonction, LV_OBJ_FLAG_CLICKABLE | LV_OBJ_FLAG_SCROLLABLE);      /// Flags

    // Bouton F0
    ui_BtnF0 = lv_button_create(ui_panelfonction);
    lv_obj_set_width(ui_BtnF0, 70);
    lv_obj_set_height(ui_BtnF0, 80);
    lv_obj_set_align(ui_BtnF0, LV_ALIGN_CENTER);
    lv_obj_add_flag(ui_BtnF0, LV_OBJ_FLAG_SCROLL_ON_FOCUS);     /// Flags
    lv_obj_remove_flag(ui_BtnF0, LV_OBJ_FLAG_SCROLLABLE);      /// Flags
    lv_obj_set_style_radius(ui_BtnF0, 10, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_color(ui_BtnF0, lv_color_hex(0x1E1D1D), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(ui_BtnF0, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_grad_color(ui_BtnF0, lv_color_hex(0x646464), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_grad_dir(ui_BtnF0, LV_GRAD_DIR_VER, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_radius(ui_BtnF0, 10, LV_PART_MAIN | LV_STATE_CHECKED);
    lv_obj_set_style_bg_color(ui_BtnF0, lv_color_hex(0xD8AF3E), LV_PART_MAIN | LV_STATE_CHECKED);
    lv_obj_set_style_bg_opa(ui_BtnF0, 255, LV_PART_MAIN | LV_STATE_CHECKED);
    lv_obj_set_style_bg_grad_color(ui_BtnF0, lv_color_hex(0x646464), LV_PART_MAIN | LV_STATE_CHECKED);
    lv_obj_set_style_bg_grad_dir(ui_BtnF0, LV_GRAD_DIR_VER, LV_PART_MAIN | LV_STATE_CHECKED);
    
    ui_LabelF0 = lv_label_create(ui_BtnF0);
    lv_obj_set_width(ui_LabelF0, LV_SIZE_CONTENT);
    lv_obj_set_height(ui_LabelF0, LV_SIZE_CONTENT);    
    lv_obj_set_align(ui_LabelF0, LV_ALIGN_CENTER);
    lv_label_set_text(ui_LabelF0, "F0");
    lv_obj_set_style_text_font(ui_LabelF0, &lv_font_montserrat_20, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_color(ui_LabelF0, lv_color_hex(0xFFFFFF), LV_PART_MAIN | LV_STATE_DEFAULT);
    
    ui_LedF0 = lv_led_create(ui_BtnF0);
    lv_obj_set_width(ui_LedF0, 12);
    lv_obj_set_height(ui_LedF0, 12);
    lv_obj_set_align(ui_LedF0, LV_ALIGN_TOP_RIGHT);
    lv_obj_set_pos(ui_LedF0, 0, 0);
    lv_led_set_color(ui_LedF0, lv_color_hex(0xf30345));
    lv_led_off(ui_LedF0);

    // Bouton F1
    ui_BtnF1 = lv_button_create(ui_panelfonction);
    lv_obj_set_width(ui_BtnF1, 70);
    lv_obj_set_height(ui_BtnF1, 80);
    lv_obj_set_align(ui_BtnF1, LV_ALIGN_CENTER);
    lv_obj_add_flag(ui_BtnF1, LV_OBJ_FLAG_SCROLL_ON_FOCUS);     /// Flags
    lv_obj_remove_flag(ui_BtnF1, LV_OBJ_FLAG_SCROLLABLE);      /// Flags
    lv_obj_set_style_radius(ui_BtnF1, 10, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_color(ui_BtnF1, lv_color_hex(0x1E1D1D), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(ui_BtnF1, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_grad_color(ui_BtnF1, lv_color_hex(0x646464), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_grad_dir(ui_BtnF1, LV_GRAD_DIR_VER, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_radius(ui_BtnF1, 10, LV_PART_MAIN | LV_STATE_CHECKED);
    lv_obj_set_style_bg_color(ui_BtnF1, lv_color_hex(0xD8AF3E), LV_PART_MAIN | LV_STATE_CHECKED);
    lv_obj_set_style_bg_opa(ui_BtnF1, 255, LV_PART_MAIN | LV_STATE_CHECKED);
    lv_obj_set_style_bg_grad_color(ui_BtnF1, lv_color_hex(0x646464), LV_PART_MAIN | LV_STATE_CHECKED);
    lv_obj_set_style_bg_grad_dir(ui_BtnF1, LV_GRAD_DIR_VER, LV_PART_MAIN | LV_STATE_CHECKED);
    
    ui_LabelF1 = lv_label_create(ui_BtnF1);
    lv_obj_set_width(ui_LabelF1, LV_SIZE_CONTENT);
    lv_obj_set_height(ui_LabelF1, LV_SIZE_CONTENT);
    lv_obj_set_align(ui_LabelF1, LV_ALIGN_CENTER);
    lv_label_set_text(ui_LabelF1, "F1");
    lv_obj_set_style_text_font(ui_LabelF1, &lv_font_montserrat_20, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_color(ui_LabelF1, lv_color_hex(0xFFFFFF), LV_PART_MAIN | LV_STATE_DEFAULT);
    
    ui_LedF1 = lv_led_create(ui_BtnF1);
    lv_obj_set_width(ui_LedF1, 12);
    lv_obj_set_height(ui_LedF1, 12);
    lv_obj_set_align(ui_LedF1, LV_ALIGN_TOP_RIGHT);
    lv_obj_set_pos(ui_LedF1, 0, 0);
    lv_led_set_color(ui_LedF1, lv_color_hex(0xf30345));
    lv_led_off(ui_LedF1);

    // Bouton F2
    ui_BtnF2 = lv_button_create(ui_panelfonction);
    lv_obj_set_width(ui_BtnF2, 70);
    lv_obj_set_height(ui_BtnF2, 80);
    lv_obj_set_align(ui_BtnF2, LV_ALIGN_CENTER);
    lv_obj_add_flag(ui_BtnF2, LV_OBJ_FLAG_SCROLL_ON_FOCUS);     /// Flags
    lv_obj_remove_flag(ui_BtnF2, LV_OBJ_FLAG_SCROLLABLE);      /// Flags
    lv_obj_set_style_radius(ui_BtnF2, 10, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_color(ui_BtnF2, lv_color_hex(0x1E1D1D), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(ui_BtnF2, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_grad_color(ui_BtnF2, lv_color_hex(0x646464), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_grad_dir(ui_BtnF2, LV_GRAD_DIR_VER, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_radius(ui_BtnF2, 10, LV_PART_MAIN | LV_STATE_CHECKED);
    lv_obj_set_style_bg_color(ui_BtnF2, lv_color_hex(0xD8AF3E), LV_PART_MAIN | LV_STATE_CHECKED);
    lv_obj_set_style_bg_opa(ui_BtnF2, 255, LV_PART_MAIN | LV_STATE_CHECKED);
    lv_obj_set_style_bg_grad_color(ui_BtnF2, lv_color_hex(0x646464), LV_PART_MAIN | LV_STATE_CHECKED);
    lv_obj_set_style_bg_grad_dir(ui_BtnF2, LV_GRAD_DIR_VER, LV_PART_MAIN | LV_STATE_CHECKED);
    
    ui_LabelF2 = lv_label_create(ui_BtnF2);
    lv_obj_set_width(ui_LabelF2, LV_SIZE_CONTENT);
    lv_obj_set_height(ui_LabelF2, LV_SIZE_CONTENT);
    lv_obj_set_align(ui_LabelF2, LV_ALIGN_CENTER);
    lv_label_set_text(ui_LabelF2, "F2");
    lv_obj_set_style_text_font(ui_LabelF2, &lv_font_montserrat_20, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_color(ui_LabelF2, lv_color_hex(0xFFFFFF), LV_PART_MAIN | LV_STATE_DEFAULT);
    
    ui_LedF2 = lv_led_create(ui_BtnF2);
    lv_obj_set_width(ui_LedF2, 12);
    lv_obj_set_height(ui_LedF2, 12);
    lv_obj_set_align(ui_LedF2, LV_ALIGN_TOP_RIGHT);
    lv_obj_set_pos(ui_LedF2, 0, 0);
    lv_led_set_color(ui_LedF2, lv_color_hex(0xf30345));
    lv_led_off(ui_LedF2);

    // Bouton F3
    ui_BtnF3 = lv_button_create(ui_panelfonction);
    lv_obj_set_width(ui_BtnF3, 70);
    lv_obj_set_height(ui_BtnF3, 80);
    lv_obj_set_align(ui_BtnF3, LV_ALIGN_CENTER);
    lv_obj_add_flag(ui_BtnF3, LV_OBJ_FLAG_SCROLL_ON_FOCUS);     /// Flags
    lv_obj_remove_flag(ui_BtnF3, LV_OBJ_FLAG_SCROLLABLE);      /// Flags
    lv_obj_set_style_radius(ui_BtnF3, 10, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_color(ui_BtnF3, lv_color_hex(0x1E1D1D), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(ui_BtnF3, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_grad_color(ui_BtnF3, lv_color_hex(0x646464), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_grad_dir(ui_BtnF3, LV_GRAD_DIR_VER, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_radius(ui_BtnF3, 10, LV_PART_MAIN | LV_STATE_CHECKED);
    lv_obj_set_style_bg_color(ui_BtnF3, lv_color_hex(0xD8AF3E), LV_PART_MAIN | LV_STATE_CHECKED);
    lv_obj_set_style_bg_opa(ui_BtnF3, 255, LV_PART_MAIN | LV_STATE_CHECKED);
    lv_obj_set_style_bg_grad_color(ui_BtnF3, lv_color_hex(0x646464), LV_PART_MAIN | LV_STATE_CHECKED);
    lv_obj_set_style_bg_grad_dir(ui_BtnF3, LV_GRAD_DIR_VER, LV_PART_MAIN | LV_STATE_CHECKED);
    
    ui_LabelF3 = lv_label_create(ui_BtnF3);
    lv_obj_set_width(ui_LabelF3, LV_SIZE_CONTENT);
    lv_obj_set_height(ui_LabelF3, LV_SIZE_CONTENT);
    lv_obj_set_align(ui_LabelF3, LV_ALIGN_CENTER);
    lv_label_set_text(ui_LabelF3, "F3");
    lv_obj_set_style_text_font(ui_LabelF3, &lv_font_montserrat_20, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_color(ui_LabelF3, lv_color_hex(0xFFFFFF), LV_PART_MAIN | LV_STATE_DEFAULT);
    
    ui_LedF3 = lv_led_create(ui_BtnF3);
    lv_obj_set_width(ui_LedF3, 12);
    lv_obj_set_height(ui_LedF3, 12);
    lv_obj_set_align(ui_LedF3, LV_ALIGN_TOP_RIGHT);
    lv_obj_set_pos(ui_LedF3, 0, 0);
    lv_led_set_color(ui_LedF3, lv_color_hex(0xf30345));
    lv_led_off(ui_LedF3);
    
    // Event handlers
    lv_obj_add_event_cb(ui_BtnMenuP1, ui_event_BtnMenuP1, LV_EVENT_ALL, NULL);
    lv_obj_add_event_cb(ui_BtnAdrLocoP1, ui_event_BtnAdrLocoP1, LV_EVENT_ALL, NULL);
    lv_obj_add_event_cb(ui_BtnFonctionP1, ui_event_BtnFonctionP1, LV_EVENT_ALL, NULL);
    lv_obj_add_event_cb(ui_Gaugevitesse, ui_event_Gaugevitesse, LV_EVENT_ALL, NULL);
    lv_obj_add_event_cb(ui_BtnF0, ui_event_BtnF0, LV_EVENT_ALL, NULL);
    lv_obj_add_event_cb(ui_BtnF1, ui_event_BtnF1, LV_EVENT_ALL, NULL);
    lv_obj_add_event_cb(ui_BtnF2, ui_event_BtnF2, LV_EVENT_ALL, NULL);
    lv_obj_add_event_cb(ui_BtnF3, ui_event_BtnF3, LV_EVENT_ALL, NULL);
}

void ui_Screen1_screen_destroy(void)
{
    if(ui_Screen1 != NULL) {
        lv_obj_del(ui_Screen1);
        ui_Screen1 = NULL;
    }
}
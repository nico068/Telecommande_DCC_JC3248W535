// LVGL version: 9.4

#include "ui.h"

lv_obj_t * ui_Screen3 = NULL;
lv_obj_t * ui_Panel3 = NULL;
lv_obj_t * ui_headerP3 = NULL;
lv_obj_t * ui_LabelPageP3 = NULL;
lv_obj_t * ui_icnwifiP3 = NULL;
lv_obj_t * ui_icnbattP3 = NULL;
lv_obj_t * ui_PanelBtnP3 = NULL;
lv_obj_t * ui_BtnCabineP3 = NULL;
lv_obj_t * ui_LabelBtnCabineP3 = NULL;
lv_obj_t * ui_panelfonctionP3 = NULL;

// Boutons fonctions F4-F15
lv_obj_t * ui_LabelF4P3 = NULL;
lv_obj_t * ui_BtnF4P3 = NULL;
lv_obj_t * ui_LedF4P3 = NULL;
lv_obj_t * ui_LabelF5P3 = NULL;
lv_obj_t * ui_BtnF5P3 = NULL;
lv_obj_t * ui_LedF5P3 = NULL;
lv_obj_t * ui_LabelF6P3 = NULL;
lv_obj_t * ui_BtnF6P3 = NULL;
lv_obj_t * ui_LedF6P3 = NULL;
lv_obj_t * ui_LabelF7P3 = NULL;
lv_obj_t * ui_BtnF7P3 = NULL;
lv_obj_t * ui_LedF7P3 = NULL;
lv_obj_t * ui_LabelF8P3 = NULL;
lv_obj_t * ui_BtnF8P3 = NULL;
lv_obj_t * ui_LedF8P3 = NULL;
lv_obj_t * ui_LabelF9P3 = NULL;
lv_obj_t * ui_BtnF9P3 = NULL;
lv_obj_t * ui_LedF9P3 = NULL;
lv_obj_t * ui_LabelF10P3 = NULL;
lv_obj_t * ui_BtnF10P3 = NULL;
lv_obj_t * ui_LedF10P3 = NULL;
lv_obj_t * ui_LabelF11P3 = NULL;
lv_obj_t * ui_BtnF11P3 = NULL;
lv_obj_t * ui_LedF11P3 = NULL;
lv_obj_t * ui_LabelF12P3 = NULL;
lv_obj_t * ui_BtnF12P3 = NULL;
lv_obj_t * ui_LedF12P3 = NULL;
lv_obj_t * ui_LabelF13P3 = NULL;
lv_obj_t * ui_BtnF13P3 = NULL;
lv_obj_t * ui_LedF13P3 = NULL;
lv_obj_t * ui_LabelF14P3 = NULL;
lv_obj_t * ui_BtnF14P3 = NULL;
lv_obj_t * ui_LedF14P3 = NULL;
lv_obj_t * ui_LabelF15P3 = NULL;
lv_obj_t * ui_BtnF15P3 = NULL;
lv_obj_t * ui_LedF15P3 = NULL;

// États des fonctions F4-F15
static bool function_states_p3[12] = {false, false, false, false, false, false, false, false, false, false, false, false};

// event funtions
void ui_event_BtnCabineP3(lv_event_t * e)
{
    lv_event_code_t event_code = lv_event_get_code(e);

    if(event_code == LV_EVENT_CLICKED) {
        _ui_screen_change(&ui_Screen1, LV_SCR_LOAD_ANIM_FADE_ON, 500, 0, &ui_Screen1_screen_init);
    }
}

// Fonctions événements F4-F12
void ui_event_BtnF4P3(lv_event_t * e)
{
    lv_event_code_t event_code = lv_event_get_code(e);
    if(event_code == LV_EVENT_CLICKED) {
        function_states_p3[0] = !function_states_p3[0];
        if(function_states_p3[0]) {
            lv_obj_add_state(ui_BtnF4P3, LV_STATE_CHECKED);
            lv_led_on(ui_LedF4P3);
        } else {
            lv_obj_clear_state(ui_BtnF4P3, LV_STATE_CHECKED);
            lv_led_off(ui_LedF4P3);
        }
        // TODO: Envoyer commande DCC F4
    }
}

void ui_event_BtnF5P3(lv_event_t * e)
{
    lv_event_code_t event_code = lv_event_get_code(e);
    if(event_code == LV_EVENT_CLICKED) {
        function_states_p3[1] = !function_states_p3[1];
        if(function_states_p3[1]) {
            lv_obj_add_state(ui_BtnF5P3, LV_STATE_CHECKED);
            lv_led_on(ui_LedF5P3);
        } else {
            lv_obj_clear_state(ui_BtnF5P3, LV_STATE_CHECKED);
            lv_led_off(ui_LedF5P3);
        }
        // TODO: Envoyer commande DCC F5
    }
}

void ui_event_BtnF6P3(lv_event_t * e)
{
    lv_event_code_t event_code = lv_event_get_code(e);
    if(event_code == LV_EVENT_CLICKED) {
        function_states_p3[2] = !function_states_p3[2];
        if(function_states_p3[2]) {
            lv_obj_add_state(ui_BtnF6P3, LV_STATE_CHECKED);
            lv_led_on(ui_LedF6P3);
        } else {
            lv_obj_clear_state(ui_BtnF6P3, LV_STATE_CHECKED);
            lv_led_off(ui_LedF6P3);
        }
        // TODO: Envoyer commande DCC F6
    }
}

void ui_event_BtnF7P3(lv_event_t * e)
{
    lv_event_code_t event_code = lv_event_get_code(e);
    if(event_code == LV_EVENT_CLICKED) {
        function_states_p3[3] = !function_states_p3[3];
        if(function_states_p3[3]) {
            lv_obj_add_state(ui_BtnF7P3, LV_STATE_CHECKED);
            lv_led_on(ui_LedF7P3);
        } else {
            lv_obj_clear_state(ui_BtnF7P3, LV_STATE_CHECKED);
            lv_led_off(ui_LedF7P3);
        }
        // TODO: Envoyer commande DCC F7
    }
}

void ui_event_BtnF8P3(lv_event_t * e)
{
    lv_event_code_t event_code = lv_event_get_code(e);
    if(event_code == LV_EVENT_CLICKED) {
        function_states_p3[4] = !function_states_p3[4];
        if(function_states_p3[4]) {
            lv_obj_add_state(ui_BtnF8P3, LV_STATE_CHECKED);
            lv_led_on(ui_LedF8P3);
        } else {
            lv_obj_clear_state(ui_BtnF8P3, LV_STATE_CHECKED);
            lv_led_off(ui_LedF8P3);
        }
        // TODO: Envoyer commande DCC F8
    }
}

void ui_event_BtnF9P3(lv_event_t * e)
{
    lv_event_code_t event_code = lv_event_get_code(e);
    if(event_code == LV_EVENT_CLICKED) {
        function_states_p3[5] = !function_states_p3[5];
        if(function_states_p3[5]) {
            lv_obj_add_state(ui_BtnF9P3, LV_STATE_CHECKED);
            lv_led_on(ui_LedF9P3);
        } else {
            lv_obj_clear_state(ui_BtnF9P3, LV_STATE_CHECKED);
            lv_led_off(ui_LedF9P3);
        }
        // TODO: Envoyer commande DCC F9
    }
}

void ui_event_BtnF10P3(lv_event_t * e)
{
    lv_event_code_t event_code = lv_event_get_code(e);
    if(event_code == LV_EVENT_CLICKED) {
        function_states_p3[6] = !function_states_p3[6];
        if(function_states_p3[6]) {
            lv_obj_add_state(ui_BtnF10P3, LV_STATE_CHECKED);
            lv_led_on(ui_LedF10P3);
        } else {
            lv_obj_clear_state(ui_BtnF10P3, LV_STATE_CHECKED);
            lv_led_off(ui_LedF10P3);
        }
        // TODO: Envoyer commande DCC F10
    }
}

void ui_event_BtnF11P3(lv_event_t * e)
{
    lv_event_code_t event_code = lv_event_get_code(e);
    if(event_code == LV_EVENT_CLICKED) {
        function_states_p3[7] = !function_states_p3[7];
        if(function_states_p3[7]) {
            lv_obj_add_state(ui_BtnF11P3, LV_STATE_CHECKED);
            lv_led_on(ui_LedF11P3);
        } else {
            lv_obj_clear_state(ui_BtnF11P3, LV_STATE_CHECKED);
            lv_led_off(ui_LedF11P3);
        }
        // TODO: Envoyer commande DCC F11
    }
}

void ui_event_BtnF12P3(lv_event_t * e)
{
    lv_event_code_t event_code = lv_event_get_code(e);
    if(event_code == LV_EVENT_CLICKED) {
        function_states_p3[8] = !function_states_p3[8];
        if(function_states_p3[8]) {
            lv_obj_add_state(ui_BtnF12P3, LV_STATE_CHECKED);
            lv_led_on(ui_LedF12P3);
        } else {
            lv_obj_clear_state(ui_BtnF12P3, LV_STATE_CHECKED);
            lv_led_off(ui_LedF12P3);
        }
        // TODO: Envoyer commande DCC F12
    }
}

void ui_event_BtnF13P3(lv_event_t * e)
{
    lv_event_code_t event_code = lv_event_get_code(e);
    if(event_code == LV_EVENT_CLICKED) {
        function_states_p3[9] = !function_states_p3[9];
        if(function_states_p3[9]) {
            lv_obj_add_state(ui_BtnF13P3, LV_STATE_CHECKED);
            lv_led_on(ui_LedF13P3);
        } else {
            lv_obj_clear_state(ui_BtnF13P3, LV_STATE_CHECKED);
            lv_led_off(ui_LedF13P3);
        }
        // TODO: Envoyer commande DCC F13
    }
}

void ui_event_BtnF14P3(lv_event_t * e)
{
    lv_event_code_t event_code = lv_event_get_code(e);
    if(event_code == LV_EVENT_CLICKED) {
        function_states_p3[10] = !function_states_p3[10];
        if(function_states_p3[10]) {
            lv_obj_add_state(ui_BtnF14P3, LV_STATE_CHECKED);
            lv_led_on(ui_LedF14P3);
        } else {
            lv_obj_clear_state(ui_BtnF14P3, LV_STATE_CHECKED);
            lv_led_off(ui_LedF14P3);
        }
        // TODO: Envoyer commande DCC F14
    }
}

void ui_event_BtnF15P3(lv_event_t * e)
{
    lv_event_code_t event_code = lv_event_get_code(e);
    if(event_code == LV_EVENT_CLICKED) {
        function_states_p3[11] = !function_states_p3[11];
        if(function_states_p3[11]) {
            lv_obj_add_state(ui_BtnF15P3, LV_STATE_CHECKED);
            lv_led_on(ui_LedF15P3);
        } else {
            lv_obj_clear_state(ui_BtnF15P3, LV_STATE_CHECKED);
            lv_led_off(ui_LedF15P3);
        }
        // TODO: Envoyer commande DCC F15
    }
}

// Build functions
void ui_Screen3_screen_init(void)
{
    ui_Screen3 = lv_obj_create(NULL);
    lv_obj_clear_flag(ui_Screen3, LV_OBJ_FLAG_SCROLLABLE);

    // En-tête
    ui_headerP3 = lv_obj_create(ui_Screen3);
    lv_obj_remove_style_all(ui_headerP3);
    lv_obj_set_width(ui_headerP3, 320);
    lv_obj_set_height(ui_headerP3, 40);
    lv_obj_set_x(ui_headerP3, 0);
    lv_obj_set_y(ui_headerP3, -220);
    lv_obj_set_align(ui_headerP3, LV_ALIGN_CENTER);
    lv_obj_remove_flag(ui_headerP3, LV_OBJ_FLAG_CLICKABLE | LV_OBJ_FLAG_SCROLLABLE);      /// Flags
    lv_obj_set_style_radius(ui_headerP3, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_color(ui_headerP3, lv_color_hex(0x292831), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(ui_headerP3, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_border_color(ui_headerP3, lv_color_hex(0x000000), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_border_opa(ui_headerP3, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_border_width(ui_headerP3, 0, LV_PART_MAIN | LV_STATE_DEFAULT);

    ui_LabelPageP3 = lv_label_create(ui_headerP3);
    lv_obj_set_width(ui_LabelPageP3, LV_SIZE_CONTENT);
    lv_obj_set_height(ui_LabelPageP3, LV_SIZE_CONTENT);
    lv_obj_set_align(ui_LabelPageP3, LV_ALIGN_CENTER);
    lv_label_set_text(ui_LabelPageP3, "Loco");
    lv_obj_set_style_text_color(ui_LabelPageP3, lv_color_hex(0xFFFFFF), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_opa(ui_LabelPageP3, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_font(ui_LabelPageP3, &lv_font_montserrat_28, LV_PART_MAIN | LV_STATE_DEFAULT); 

    // Icône Batterie
    ui_icnbattP3 = lv_image_create(ui_headerP3);
    lv_image_set_src(ui_icnbattP3, &bat_pleine); // image par défaut
    lv_obj_set_width(ui_icnbattP3, LV_SIZE_CONTENT);
    lv_obj_set_height(ui_icnbattP3, LV_SIZE_CONTENT);
    lv_obj_set_x(ui_icnbattP3, -135);
    lv_obj_set_y(ui_icnbattP3, 0);
    lv_obj_set_align(ui_icnbattP3, LV_ALIGN_CENTER);
    lv_obj_add_flag(ui_icnbattP3, LV_OBJ_FLAG_CLICKABLE);     /// Flags
    lv_obj_remove_flag(ui_icnbattP3, LV_OBJ_FLAG_SCROLLABLE);      /// Flags
    
    // Icône WiFi
    ui_icnwifiP3 = lv_image_create(ui_headerP3);
    lv_image_set_src(ui_icnwifiP3, &wifi0); // image par défaut (déconnecté)
    lv_obj_set_width(ui_icnwifiP3, LV_SIZE_CONTENT);
    lv_obj_set_height(ui_icnwifiP3, LV_SIZE_CONTENT);
    lv_obj_set_x(ui_icnwifiP3, 135);
    lv_obj_set_y(ui_icnwifiP3, 0);
    lv_obj_set_align(ui_icnwifiP3, LV_ALIGN_CENTER);
    lv_obj_add_flag(ui_icnwifiP3, LV_OBJ_FLAG_CLICKABLE);     /// Flags
    lv_obj_remove_flag(ui_icnwifiP3, LV_OBJ_FLAG_SCROLLABLE);      /// Flags

    // Panel boutons de navigation
    ui_PanelBtnP3 = lv_obj_create(ui_Screen3);
    lv_obj_remove_style_all(ui_PanelBtnP3);
    lv_obj_set_width(ui_PanelBtnP3, 320);
    lv_obj_set_height(ui_PanelBtnP3, 60);
    lv_obj_set_x(ui_PanelBtnP3, 0);
    lv_obj_set_y(ui_PanelBtnP3, -165);
    lv_obj_set_align(ui_PanelBtnP3, LV_ALIGN_CENTER);
    lv_obj_set_flex_flow(ui_PanelBtnP3, LV_FLEX_FLOW_ROW);
    lv_obj_set_flex_align(ui_PanelBtnP3, LV_FLEX_ALIGN_SPACE_AROUND, LV_FLEX_ALIGN_CENTER, LV_FLEX_ALIGN_CENTER);
    lv_obj_remove_flag(ui_PanelBtnP3, LV_OBJ_FLAG_CLICKABLE | LV_OBJ_FLAG_SCROLLABLE);      /// Flags

    // Bouton Retour Cabine
    ui_BtnCabineP3 = lv_btn_create(ui_PanelBtnP3);
    lv_obj_set_width(ui_BtnCabineP3, 95);
    lv_obj_set_height(ui_BtnCabineP3, 50);
    lv_obj_set_align(ui_BtnCabineP3, LV_ALIGN_CENTER);
    lv_obj_add_flag(ui_BtnCabineP3, LV_OBJ_FLAG_SCROLL_ON_FOCUS);     /// Flags
    lv_obj_remove_flag(ui_BtnCabineP3, LV_OBJ_FLAG_SCROLLABLE);      /// Flags
    lv_obj_set_style_bg_color(ui_BtnCabineP3, lv_color_hex(0x7F7F7F), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(ui_BtnCabineP3, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_grad_color(ui_BtnCabineP3, lv_color_hex(0xB5B5B5), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_grad_dir(ui_BtnCabineP3, LV_GRAD_DIR_VER, LV_PART_MAIN | LV_STATE_DEFAULT);

    ui_LabelBtnCabineP3 = lv_label_create(ui_BtnCabineP3);
    lv_obj_set_width(ui_LabelBtnCabineP3, LV_SIZE_CONTENT);
    lv_obj_set_height(ui_LabelBtnCabineP3, LV_SIZE_CONTENT);
    lv_obj_set_align(ui_LabelBtnCabineP3, LV_ALIGN_CENTER);
    lv_label_set_text(ui_LabelBtnCabineP3, "Cabine");
    lv_obj_set_style_text_font(ui_LabelBtnCabineP3, &lv_font_montserrat_18, LV_PART_MAIN | LV_STATE_DEFAULT);

    // Grille fonctions 3x4 (F4-F15) - sous le bouton Cabine
    ui_panelfonctionP3 = lv_obj_create(ui_Screen3);
    lv_obj_set_width(ui_panelfonctionP3, 320);
    lv_obj_set_height(ui_panelfonctionP3, 370);
    lv_obj_set_x(ui_panelfonctionP3, 0);
    lv_obj_set_y(ui_panelfonctionP3, 50);
    lv_obj_set_align(ui_panelfonctionP3, LV_ALIGN_CENTER);
    lv_obj_set_flex_flow(ui_panelfonctionP3, LV_FLEX_FLOW_ROW_WRAP);
    lv_obj_set_flex_align(ui_panelfonctionP3, LV_FLEX_ALIGN_SPACE_EVENLY, LV_FLEX_ALIGN_SPACE_EVENLY, LV_FLEX_ALIGN_CENTER);
    lv_obj_clear_flag(ui_panelfonctionP3, LV_OBJ_FLAG_SCROLLABLE);
    lv_obj_set_style_bg_color(ui_panelfonctionP3, lv_color_hex(0x000000), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(ui_panelfonctionP3, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_border_color(ui_panelfonctionP3, lv_color_hex(0x000000), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_border_opa(ui_panelfonctionP3, 0, LV_PART_MAIN | LV_STATE_DEFAULT);

    // Macro pour créer un bouton de fonction (panel + label + bouton) - style Screen1
    #define CREATE_FUNCTION_BTN(f_num) \
    ui_BtnF##f_num##P3 = lv_button_create(ui_panelfonctionP3); \
    lv_obj_set_width(ui_BtnF##f_num##P3, 70); \
    lv_obj_set_height(ui_BtnF##f_num##P3, 80); \
    lv_obj_set_align(ui_BtnF##f_num##P3, LV_ALIGN_CENTER); \
    lv_obj_add_flag(ui_BtnF##f_num##P3, LV_OBJ_FLAG_SCROLL_ON_FOCUS); \
    lv_obj_remove_flag(ui_BtnF##f_num##P3, LV_OBJ_FLAG_SCROLLABLE); \
    lv_obj_set_style_radius(ui_BtnF##f_num##P3, 10, LV_PART_MAIN | LV_STATE_DEFAULT); \
    lv_obj_set_style_bg_color(ui_BtnF##f_num##P3, lv_color_hex(0x1E1D1D), LV_PART_MAIN | LV_STATE_DEFAULT); \
    lv_obj_set_style_bg_opa(ui_BtnF##f_num##P3, 255, LV_PART_MAIN | LV_STATE_DEFAULT); \
    lv_obj_set_style_bg_grad_color(ui_BtnF##f_num##P3, lv_color_hex(0x646464), LV_PART_MAIN | LV_STATE_DEFAULT); \
    lv_obj_set_style_bg_grad_dir(ui_BtnF##f_num##P3, LV_GRAD_DIR_VER, LV_PART_MAIN | LV_STATE_DEFAULT); \
    lv_obj_set_style_radius(ui_BtnF##f_num##P3, 10, LV_PART_MAIN | LV_STATE_CHECKED); \
    lv_obj_set_style_bg_color(ui_BtnF##f_num##P3, lv_color_hex(0xD8AF3E), LV_PART_MAIN | LV_STATE_CHECKED); \
    lv_obj_set_style_bg_opa(ui_BtnF##f_num##P3, 255, LV_PART_MAIN | LV_STATE_CHECKED); \
    lv_obj_set_style_bg_grad_color(ui_BtnF##f_num##P3, lv_color_hex(0x646464), LV_PART_MAIN | LV_STATE_CHECKED); \
    lv_obj_set_style_bg_grad_dir(ui_BtnF##f_num##P3, LV_GRAD_DIR_VER, LV_PART_MAIN | LV_STATE_CHECKED); \
    ui_LabelF##f_num##P3 = lv_label_create(ui_BtnF##f_num##P3); \
    lv_obj_set_width(ui_LabelF##f_num##P3, LV_SIZE_CONTENT); \
    lv_obj_set_height(ui_LabelF##f_num##P3, LV_SIZE_CONTENT); \
    lv_obj_set_align(ui_LabelF##f_num##P3, LV_ALIGN_CENTER); \
    lv_label_set_text(ui_LabelF##f_num##P3, "F"#f_num); \
    lv_obj_set_style_text_font(ui_LabelF##f_num##P3, &lv_font_montserrat_20, LV_PART_MAIN | LV_STATE_DEFAULT); \
    lv_obj_set_style_text_color(ui_LabelF##f_num##P3, lv_color_hex(0xFFFFFF), LV_PART_MAIN | LV_STATE_DEFAULT); \
    ui_LedF##f_num##P3 = lv_led_create(ui_BtnF##f_num##P3); \
    lv_obj_set_width(ui_LedF##f_num##P3, 12); \
    lv_obj_set_height(ui_LedF##f_num##P3, 12); \
    lv_obj_set_align(ui_LedF##f_num##P3, LV_ALIGN_TOP_RIGHT); \
    lv_obj_set_pos(ui_LedF##f_num##P3, 0, 0); \
    lv_led_set_color(ui_LedF##f_num##P3, lv_color_hex(0xf30345)); \
    lv_led_off(ui_LedF##f_num##P3); \
    lv_obj_add_event_cb(ui_BtnF##f_num##P3, ui_event_BtnF##f_num##P3, LV_EVENT_ALL, NULL);    
        
    // Créer les boutons F4 à F15 en grille 3x4
    CREATE_FUNCTION_BTN(4)
    CREATE_FUNCTION_BTN(5)
    CREATE_FUNCTION_BTN(6)
    CREATE_FUNCTION_BTN(7)
    CREATE_FUNCTION_BTN(8)
    CREATE_FUNCTION_BTN(9)
    CREATE_FUNCTION_BTN(10)
    CREATE_FUNCTION_BTN(11)
    CREATE_FUNCTION_BTN(12)
    CREATE_FUNCTION_BTN(13)
    CREATE_FUNCTION_BTN(14)
    CREATE_FUNCTION_BTN(15)

    // Event handlers
    lv_obj_add_event_cb(ui_BtnCabineP3, ui_event_BtnCabineP3, LV_EVENT_ALL, NULL);
}

void ui_Screen3_screen_destroy(void)
{
    if(ui_Screen3 != NULL) {
        lv_obj_del(ui_Screen3);
        ui_Screen3 = NULL;
    }
}
// LVGL version: 9.4

#ifndef _UI_SCREEN2_H
#define _UI_SCREEN2_H

#ifdef __cplusplus
extern "C" {
#endif



// SCREEN OBJECTS
extern lv_obj_t * ui_Screen2;
extern lv_obj_t * ui_PanelP2;
extern lv_obj_t * ui_headerP2;
extern lv_obj_t * ui_LabelPageP2;

extern lv_obj_t * ui_icnbattP2;
extern lv_obj_t * ui_infolocoP2;
extern lv_obj_t * ui_LabelAdrLocoP2;
extern lv_obj_t * ui_imglocoP2;
extern lv_obj_t * ui_BtnValidationP2;
extern lv_obj_t * ui_LabelBtnValidationP2;
extern lv_obj_t * ui_Screen2_Panel6;
extern lv_obj_t * ui_Screen2_Label5;
extern lv_obj_t * ui_TextAreaAdrLocoP2;
extern lv_obj_t * ui_PanelKeypadP2;

// EVENT HANDLERS
void ui_event_BtnValidationP2(lv_event_t * e);

// SCREEN INIT/DESTROY
void ui_Screen2_screen_init(void);
void ui_Screen2_screen_destroy(void);

#ifdef __cplusplus
} /*extern "C"*/
#endif

#endif

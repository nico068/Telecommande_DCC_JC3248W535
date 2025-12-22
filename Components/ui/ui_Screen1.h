// LVGL version: 9.4

#ifndef UI_SCREEN1_H
#define UI_SCREEN1_H

#ifdef __cplusplus
extern "C" {
#endif

// SCREEN: ui_Screen1
extern lv_obj_t *ui_Screen1;
extern lv_obj_t *ui_Panel1;
extern lv_obj_t *ui_headerP1;
extern lv_obj_t *ui_LabelPageP1;
extern lv_obj_t *ui_icnwifiP1;
extern lv_obj_t *ui_icnbattP1;
extern lv_obj_t *ui_PanelBtnP1;
extern lv_obj_t *ui_BtnAdrLocoP1;
extern lv_obj_t *ui_LabelBtnAdrLocoP1;
extern lv_obj_t *ui_BtnFonctionP1;
extern lv_obj_t *ui_LabelBtnFonctionP1;
extern lv_obj_t *ui_infolocoP1;
extern lv_obj_t *ui_LabelAdrLocoP1;
extern lv_obj_t *ui_imglocoP1;
extern lv_obj_t *ui_PanelGauge;
extern lv_obj_t *ui_Gaugevitesse;
extern lv_obj_t *ui_PanelVitesse;
extern lv_obj_t *ui_LabelVitesse;
extern lv_obj_t *ui_imgDirectionP1;
extern lv_obj_t *ui_panelfonction;
extern lv_obj_t *ui_PanelbtnF0;
extern lv_obj_t *ui_LabelF0;
extern lv_obj_t *ui_BtnF0;
extern lv_obj_t *ui_PanelbtnF1;
extern lv_obj_t *ui_LabelF1;
extern lv_obj_t *ui_BtnF1;
extern lv_obj_t *ui_PanelbtnF2;
extern lv_obj_t *ui_LabelF2;
extern lv_obj_t *ui_BtnF2;
extern lv_obj_t *ui_PanelbtnF3;
extern lv_obj_t *ui_LabelF3;
extern lv_obj_t *ui_BtnF3;

// EVENT HANDLERS
void ui_event_BtnMenuP1(lv_event_t * e);
void ui_event_BtnAdrLocoP1(lv_event_t * e);
void ui_event_BtnFonctionP1(lv_event_t * e);
void ui_event_Gaugevitesse(lv_event_t * e);
void ui_event_BtnF0(lv_event_t * e);
void ui_event_BtnF1(lv_event_t * e);
void ui_event_BtnF2(lv_event_t * e);
void ui_event_BtnF3(lv_event_t * e);

// SCREEN INIT/DESTROY
void ui_Screen1_screen_init(void);
void ui_Screen1_screen_destroy(void);

#ifdef __cplusplus
} /*extern "C"*/
#endif

#endif

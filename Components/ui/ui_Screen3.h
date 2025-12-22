// LVGL version: 9.4

#ifndef UI_SCREEN3_H
#define UI_SCREEN3_H

#ifdef __cplusplus
extern "C" {
#endif

// SCREEN: ui_Screen3
extern lv_obj_t * ui_Screen3;
extern lv_obj_t * ui_Panel3;
extern lv_obj_t * ui_headerP3;
extern lv_obj_t * ui_LabelPageP3;
extern lv_obj_t * ui_icnwifiP3;
extern lv_obj_t * ui_icnbattP3;
extern lv_obj_t * ui_PanelPageBtn2;
extern lv_obj_t * ui_BtnCabineP3;
extern lv_obj_t * ui_LabelBtnCabineP3;
extern lv_obj_t * ui_panelfonctionP3;

// EVENT HANDLERS
void ui_event_BtnCabineP3(lv_event_t * e);

// SCREEN INIT/DESTROY
void ui_Screen3_screen_init(void);
void ui_Screen3_screen_destroy(void);

#ifdef __cplusplus
} /*extern "C"*/
#endif

#endif

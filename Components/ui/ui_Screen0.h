// LVGL version: 9.4

#ifndef _UI_SCREEN0_H
#define _UI_SCREEN0_H

#ifdef __cplusplus
extern "C" {
#endif

#include "lvgl.h"

// SCREEN OBJECTS
extern lv_obj_t * ui_Screen0;
extern lv_obj_t * ui_Panel0;
extern lv_obj_t * ui_headerP0;
extern lv_obj_t * ui_LabelPageP0;
extern lv_obj_t * ui_icnbattP0;

extern lv_obj_t * ui_PanelMenuP0;
extern lv_obj_t * ui_LabelTitleP0;
extern lv_obj_t * ui_BtnCabineP0;
extern lv_obj_t * ui_LabelBtnCabineP0;
extern lv_obj_t * ui_BtnLocosP0;
extern lv_obj_t * ui_LabelBtnLocosP0;
extern lv_obj_t * ui_BtnFonctionsP0;
extern lv_obj_t * ui_LabelBtnFonctionsP0;

// EVENTS
void ui_event_BtnCabineP0(lv_event_t * e);
void ui_event_BtnLocosP0(lv_event_t * e);
void ui_event_BtnFonctionsP0(lv_event_t * e);

// INIT SCREEN
void ui_Screen0_screen_init(void);
void ui_Screen0_screen_destroy(void);

#ifdef __cplusplus
} /*extern "C"*/
#endif

#endif

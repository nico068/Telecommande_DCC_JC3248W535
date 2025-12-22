// Télécommande DCC - Écran appairage ESP-NOW
// Adapté pour LVGL 9.4

#ifndef _UI_SCREEN4_H
#define _UI_SCREEN4_H

#ifdef __cplusplus
extern "C" {
#endif

#include "lvgl.h"

// SCREEN OBJECTS
extern lv_obj_t * ui_Screen4;
extern lv_obj_t * ui_Panel4;
extern lv_obj_t * ui_headerP4;
extern lv_obj_t * ui_LabelPageP4;

extern lv_obj_t * ui_icnbleP4;
extern lv_obj_t * ui_icnwifiP4;
extern lv_obj_t * ui_icnbattP4;
extern lv_obj_t * ui_BtnRetourP4;
extern lv_obj_t * ui_LabelBtnRetourP4;
extern lv_obj_t * ui_PanelWiFiList;
extern lv_obj_t * ui_LabelScanStatus;
extern lv_obj_t * ui_SpinnerScan;
extern lv_obj_t * ui_LabelPeerMac;
extern lv_obj_t * ui_LabelLinkRssi;
extern lv_obj_t * ui_BtnScanP4;
extern lv_obj_t * ui_LabelBtnScanP4;
extern lv_obj_t * ui_BtnForgetP4;
extern lv_obj_t * ui_LabelBtnForgetP4;

// EVENTS
void ui_event_BtnRetourP4(lv_event_t * e);
void ui_event_BtnScanP4(lv_event_t * e);
void ui_event_BtnForgetP4(lv_event_t * e);

// INIT SCREEN
void ui_Screen4_screen_init(void);
void ui_Screen4_screen_destroy(void);

#ifdef __cplusplus
} /*extern "C"*/
#endif

#endif

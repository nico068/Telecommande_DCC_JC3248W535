// LVGL version: 9.4
#ifndef PROJECT_UI_H
#define PROJECT_UI_H

#ifdef __cplusplus
extern "C" {
#endif

#include "lvgl.h"

#include "ui_events.h"
#include "ui_helpers.h"

///////////////////// SCREENS ////////////////////

#include "ui_Screen0.h"
#include "ui_Screen1.h"
#include "ui_Screen2.h"
#include "ui_Screen3.h"
#include "ui_Screen4.h"

///////////////////// VARIABLES ////////////////////

typedef enum {
  UI_LOC_DIR_FORWARD = 0,
  UI_LOC_DIR_REVERSE = 1
} ui_loco_direction_t;

// EVENTS

extern lv_obj_t *ui____initial_actions0;

// BATTERY ICONS (one per screen)
extern lv_obj_t *ui_icnbattP0;
extern lv_obj_t *ui_icnbattP1;
extern lv_obj_t *ui_icnbattP2;
extern lv_obj_t *ui_icnbattP3;
extern lv_obj_t *ui_icnbattP4;

// WIFI ICONS (one per screen)
extern lv_obj_t *ui_icnwifiP0;
extern lv_obj_t *ui_icnwifiP1;
extern lv_obj_t *ui_icnwifiP2;
extern lv_obj_t *ui_icnwifiP3;
extern lv_obj_t *ui_icnwifiP4;

// ENCODER UI ELEMENTS (Screen1)
extern lv_obj_t *ui_Gaugevitesse;
extern lv_obj_t *ui_imgDirectionP1;
extern lv_obj_t *ui_LabelVitesse;

// IMAGES AND IMAGE SETS
LV_IMAGE_DECLARE(arrow_left_15597);       // assets/arrow_left_15597.png
LV_IMAGE_DECLARE(arrow_right_15604);      // assets/arrow_right_15604.png
LV_IMAGE_DECLARE(bat_3_4);                // assets/bat_3_4.png
LV_IMAGE_DECLARE(bat_en_charge);          // assets/bat_en_charge.png
LV_IMAGE_DECLARE(bat_faible);             // assets/bat_faible.png
LV_IMAGE_DECLARE(bat_moitie);             // assets/bat_moitie.png
LV_IMAGE_DECLARE(bat_pleine);             // assets/bat_pleine.png
LV_IMAGE_DECLARE(bat_vide);               // assets/bat_vide.png

LV_IMAGE_DECLARE(wifi0);                  // assets/wifi0.png
LV_IMAGE_DECLARE(wifi1);                  // assets/wifi1.png
LV_IMAGE_DECLARE(wifi2);                  // assets/wifi2.png
LV_IMAGE_DECLARE(wifi3);                  // assets/wifi3.png
LV_IMAGE_DECLARE(wifi4);                  // assets/wifi4.png

LV_IMAGE_DECLARE(Elok_ABe812);            // assets/Elok_ABe812.png
LV_IMAGE_DECLARE(Elok_Ge44_II);           // assets/Elok_Ge44_II.png
LV_IMAGE_DECLARE(Elok_GE44_III);          // assets/ELok_GE44_III.png
LV_IMAGE_DECLARE(ELok_Ge44_III_RhB);      // assets/ELok_Ge44_III_RhB.png
LV_IMAGE_DECLARE(ELok_RE44_II_R4);        // assets/ELok_RE44_II_R4.png

// FONTS

///////////////////// FUNCTIONS ////////////////////

// Fonction pour mettre à jour les infos de la loco sur Screen1
void ui_update_loco_info(const char *address, const lv_image_dsc_t *image);

// Fonction pour mettre à jour la direction sur Screen1
void ui_update_direction(ui_loco_direction_t direction);

// Fonctions pour Screen2 (sélection de locomotive)
void ui_screen2_validate_loco(void);     // Valider la locomotive sélectionnée
bool ui_screen2_is_loco_ready(void);    // Vérifier si une loco est prête

// UI INIT
void ui_init(void);                     // Initialize UI
void ui_destroy(void);                  // Destroy UI

#ifdef __cplusplus                      // For C++ compatibility
} /*extern "C"*/
#endif

#endif // PROJECT_UI_H



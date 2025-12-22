// LVGL version: 9.4

#include "ui.h"
#include <string.h>
#include <stdlib.h>

lv_obj_t * ui_Screen2 = NULL;
lv_obj_t * ui_PanelP2 = NULL;
lv_obj_t * ui_headerP2 = NULL;
lv_obj_t * ui_LabelPageP2 = NULL;
lv_obj_t * ui_icnbleP2 = NULL;
lv_obj_t * ui_icnwifiP2 = NULL;
lv_obj_t * ui_icnbattP2 = NULL;
lv_obj_t * ui_infolocoP2 = NULL;
lv_obj_t * ui_LabelAdrLocoP2 = NULL;
lv_obj_t * ui_imglocoP2 = NULL;
lv_obj_t * ui_BtnValidationP2 = NULL;
lv_obj_t * ui_LabelBtnValidationP2 = NULL;
lv_obj_t * ui_Screen2_Panel6 = NULL;
lv_obj_t * ui_Screen2_Label5 = NULL;
lv_obj_t * ui_TextAreaAdrLocoP2 = NULL;
lv_obj_t * ui_PanelKeypadP2 = NULL;

// Gestion du pavé numérique : chiffres, retour (suppression) et validation (Entrer)

// Variable globale pour stocker l'adresse saisie et l'image de la loco sélectionnée
static char current_address[4] = "";  // Buffer pour stocker l'adresse en cours de saisie (max 3 chiffres + null)
static char validated_address[4] = "";  // Adresse validée à afficher dans le label
static const lv_image_dsc_t * selected_loco_image = NULL;

// Fonction pour sélectionner l'image de la loco selon l'adresse
static const lv_image_dsc_t * select_loco_image(const char * adr)
{
    if(!adr || strlen(adr) == 0) return NULL;
    
    int addr_num = atoi(adr);
    const lv_image_dsc_t * img = NULL;
    
    // Mapper les adresses DCC aux images de locos disponibles
    switch(addr_num) {
        case 650:
            img = &Elok_GE44_III;
            break;
        case 620:
            img = &Elok_Ge44_II;
            break;
        case 644:
            img = &ELok_Ge44_III_RhB;
            break;
        case 812:
            img = &Elok_ABe812;
            break;
        case 228:
            img = &ELok_RE44_II_R4;
            break;
        default:
            // Adresse non reconnue - pas d'image
            return NULL;
    }
    
    // Afficher l'image sur Screen2
    if(img) {
        lv_image_set_src(ui_imglocoP2, img);
        lv_obj_clear_flag(ui_imglocoP2, LV_OBJ_FLAG_HIDDEN);
    }
    
    return img;
}

// Gestion des boutons du clavier
static void keypad_event_handler(lv_event_t * e)
{
    lv_event_code_t code = lv_event_get_code(e);
    lv_obj_t * btn = lv_event_get_target(e);
    
    if(code == LV_EVENT_CLICKED) {
        const char * txt = lv_btnmatrix_get_btn_text(btn, lv_btnmatrix_get_selected_btn(btn));
        
        if(txt == NULL) return;
        
        if(strcmp(txt, "Retour") == 0) {
            // Supprimer le dernier caractère
            int len = strlen(current_address);
            if(len > 0) {
                current_address[len - 1] = '\0';
                lv_label_set_text(ui_LabelAdrLocoP2, current_address[0] == '\0' ? "Adr Loco" : current_address);
            }
        }
        else if(strcmp(txt, "Ok") == 0) {
            // Valider l'adresse saisie
            if(strlen(current_address) > 0) {
                selected_loco_image = select_loco_image(current_address);  // Sélectionner l'image appropriée et la stocker
                
                // Stocker l'adresse validée pour le label et Screen1
                strcpy(validated_address, current_address);
                lv_label_set_text(ui_LabelAdrLocoP2, validated_address);
                
                if(selected_loco_image == NULL) {
                    lv_obj_add_flag(ui_imglocoP2, LV_OBJ_FLAG_HIDDEN);
                }
                // Réinitialiser le buffer pour pouvoir entrer une nouvelle adresse
                current_address[0] = '\0';
            }
        }
        else {
            // Ajouter le chiffre à l'adresse (max 3 caractères)
            if(strlen(current_address) < 3) {
                int len = strlen(current_address);
                current_address[len] = txt[0];
                current_address[len + 1] = '\0';
                lv_label_set_text(ui_LabelAdrLocoP2, current_address);
            }
        }
    }
}

/**
 * @brief Fonction publique pour valider la locomotive (appelée par l'encoder ou le clic)
 */
void ui_screen2_validate_loco(void)
{
    // Récupérer l'adresse affichée et la passer à Screen1
    const char * adr = lv_label_get_text(ui_LabelAdrLocoP2);
    
    // Vérifier que ce n'est pas le texte par défaut et qu'une loco est sélectionnée
    if(strcmp(adr, "Adresse Loco") != 0 && selected_loco_image != NULL) {
        // Mettre à jour les infos de la loco sur Screen1
        ui_update_loco_info(adr, selected_loco_image);
        
        // Réinitialiser le buffer d'adresse
        current_address[0] = '\0';
        
        // Retourner à la cabine
        _ui_screen_change(&ui_Screen1, LV_SCR_LOAD_ANIM_NONE, 0, 0, &ui_Screen1_screen_init);
    }
}

/**
 * @brief Vérifier si une locomotive est prête à être validée
 */
bool ui_screen2_is_loco_ready(void)
{
    const char * adr = lv_label_get_text(ui_LabelAdrLocoP2);
    return (strcmp(adr, "Adresse Loco") != 0 && selected_loco_image != NULL);
}

// Event handlers
void ui_event_infolocoP2(lv_event_t * e)
{
    lv_event_code_t event_code = lv_event_get_code(e);

    if(event_code == LV_EVENT_CLICKED) {
        // Utiliser la fonction publique pour valider
        ui_screen2_validate_loco();
    }
}

// Build functions
void ui_Screen2_screen_init(void)
{
    ui_Screen2 = lv_obj_create(NULL);
    lv_obj_clear_flag(ui_Screen2, LV_OBJ_FLAG_SCROLLABLE);

    // En-tête
    ui_headerP2 = lv_obj_create(ui_Screen2);
    lv_obj_remove_style_all(ui_headerP2);
    lv_obj_set_width(ui_headerP2, 320);
    lv_obj_set_height(ui_headerP2, 40);
    lv_obj_set_x(ui_headerP2, 0);
    lv_obj_set_y(ui_headerP2, -220);
    lv_obj_set_align(ui_headerP2, LV_ALIGN_CENTER);
    lv_obj_remove_flag(ui_headerP2, LV_OBJ_FLAG_CLICKABLE | LV_OBJ_FLAG_SCROLLABLE);      /// Flags
    lv_obj_set_style_radius(ui_headerP2, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_color(ui_headerP2, lv_color_hex(0x292831), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(ui_headerP2, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_border_color(ui_headerP2, lv_color_hex(0x000000), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_border_opa(ui_headerP2, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_border_width(ui_headerP2, 0, LV_PART_MAIN | LV_STATE_DEFAULT);

    ui_LabelPageP2 = lv_label_create(ui_headerP2);
    lv_obj_set_width(ui_LabelPageP2, LV_SIZE_CONTENT);
    lv_obj_set_height(ui_LabelPageP2, LV_SIZE_CONTENT);
    lv_obj_set_align(ui_LabelPageP2, LV_ALIGN_CENTER);
    lv_label_set_text(ui_LabelPageP2, "Loco");
    lv_obj_set_style_text_color(ui_LabelPageP2, lv_color_hex(0xFFFFFF), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_opa(ui_LabelPageP2, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_font(ui_LabelPageP2, &lv_font_montserrat_28, LV_PART_MAIN | LV_STATE_DEFAULT); 

    // Icône Batterie
    ui_icnbattP2 = lv_image_create(ui_headerP2);
    lv_image_set_src(ui_icnbattP2, &bat_pleine); // image par défaut
    lv_obj_set_width(ui_icnbattP2, LV_SIZE_CONTENT);
    lv_obj_set_height(ui_icnbattP2, LV_SIZE_CONTENT);
    lv_obj_set_x(ui_icnbattP2, -135);
    lv_obj_set_y(ui_icnbattP2, 0);
    lv_obj_set_align(ui_icnbattP2, LV_ALIGN_CENTER);
    lv_obj_add_flag(ui_icnbattP2, LV_OBJ_FLAG_CLICKABLE);     /// Flags
    lv_obj_remove_flag(ui_icnbattP2, LV_OBJ_FLAG_SCROLLABLE);      /// Flags
    
    // Icône WiFi
    ui_icnwifiP2 = lv_image_create(ui_headerP2);
    lv_image_set_src(ui_icnwifiP2, &wifi0); // image par défaut (déconnecté)
    lv_obj_set_width(ui_icnwifiP2, LV_SIZE_CONTENT);
    lv_obj_set_height(ui_icnwifiP2, LV_SIZE_CONTENT);
    lv_obj_set_x(ui_icnwifiP2, 135);
    lv_obj_set_y(ui_icnwifiP2, 0);
    lv_obj_set_align(ui_icnwifiP2, LV_ALIGN_CENTER);
    lv_obj_add_flag(ui_icnwifiP2, LV_OBJ_FLAG_CLICKABLE);     /// Flags
    lv_obj_remove_flag(ui_icnwifiP2, LV_OBJ_FLAG_SCROLLABLE);      /// Flags

    // Info Loco
    ui_infolocoP2 = lv_obj_create(ui_Screen2);
    lv_obj_set_width(ui_infolocoP2, 200);
    lv_obj_set_height(ui_infolocoP2, 90);
    lv_obj_set_x(ui_infolocoP2, 0);
    lv_obj_set_y(ui_infolocoP2, -153);
    lv_obj_set_align(ui_infolocoP2, LV_ALIGN_CENTER);
    lv_obj_clear_flag(ui_infolocoP2, LV_OBJ_FLAG_SCROLLABLE);

    ui_imglocoP2 = lv_image_create(ui_infolocoP2);
    lv_image_set_src(ui_imglocoP2, NULL);
    lv_obj_set_width(ui_imglocoP2, LV_SIZE_CONTENT);
    lv_obj_set_height(ui_imglocoP2, LV_SIZE_CONTENT);
    lv_obj_set_x(ui_imglocoP2, 0);
    lv_obj_set_y(ui_imglocoP2, -11);
    lv_obj_set_align(ui_imglocoP2, LV_ALIGN_CENTER);
    lv_obj_add_flag(ui_imglocoP2, LV_OBJ_FLAG_HIDDEN); // Cachée au départ
    lv_obj_clear_flag(ui_imglocoP2, LV_OBJ_FLAG_SCROLLABLE);

    ui_LabelAdrLocoP2 = lv_label_create(ui_infolocoP2);
    lv_obj_set_width(ui_LabelAdrLocoP2, LV_SIZE_CONTENT);
    lv_obj_set_height(ui_LabelAdrLocoP2, LV_SIZE_CONTENT);
    lv_obj_set_x(ui_LabelAdrLocoP2, 0);
    lv_obj_set_y(ui_LabelAdrLocoP2, 28);
    lv_obj_set_align(ui_LabelAdrLocoP2, LV_ALIGN_CENTER);
    lv_label_set_text(ui_LabelAdrLocoP2, "Adresse Loco");
    lv_obj_set_style_text_color(ui_LabelAdrLocoP2, lv_color_hex(0xDA9902), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_opa(ui_LabelAdrLocoP2, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_font(ui_LabelAdrLocoP2, &lv_font_montserrat_24, LV_PART_MAIN | LV_STATE_DEFAULT);
    
    // Bouton invisible pour valider la loco (recouvre tout le panel info)
    lv_obj_t * ui_BtnInvisibleValidation = lv_btn_create(ui_infolocoP2);
    lv_obj_set_width(ui_BtnInvisibleValidation, 200);
    lv_obj_set_height(ui_BtnInvisibleValidation, 90);
    lv_obj_set_align(ui_BtnInvisibleValidation, LV_ALIGN_CENTER);
    lv_obj_set_style_bg_opa(ui_BtnInvisibleValidation, 0, LV_PART_MAIN | LV_STATE_DEFAULT);  // Transparent
    lv_obj_set_style_border_opa(ui_BtnInvisibleValidation, 0, LV_PART_MAIN | LV_STATE_DEFAULT);  // Pas de bordure
    lv_obj_set_style_shadow_opa(ui_BtnInvisibleValidation, 0, LV_PART_MAIN | LV_STATE_DEFAULT);  // Pas d'ombre
    lv_obj_add_event_cb(ui_BtnInvisibleValidation, ui_event_infolocoP2, LV_EVENT_ALL, NULL);
    
    // Clavier numérique personnalisé (grille 3x4)
    // Créer une matrice de boutons 3x4
    static const char * btnm_map[] = {"1", "2", "3", "\n",
                                       "4", "5", "6", "\n",
                                       "7", "8", "9", "\n",
                                       "Retour", "0", "Ok", ""};
    
    lv_obj_t * btnmatrix = lv_btnmatrix_create(ui_Screen2);
    lv_btnmatrix_set_map(btnmatrix, btnm_map);
    lv_obj_set_size(btnmatrix, 320, 330);
    lv_obj_set_x(btnmatrix, 0);
    lv_obj_set_y(btnmatrix, 70);
    lv_obj_set_align(btnmatrix, LV_ALIGN_CENTER);
    lv_obj_add_event_cb(btnmatrix, keypad_event_handler, LV_EVENT_ALL, NULL);
    
    // Style pour le background du clavier (invisible)
    lv_obj_set_style_bg_opa(btnmatrix, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_border_opa(btnmatrix, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_all(btnmatrix, 5, LV_PART_MAIN | LV_STATE_DEFAULT);
    
    // Style pour les boutons
    lv_obj_set_style_bg_color(btnmatrix, lv_color_hex(0x404040), LV_PART_ITEMS | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_color(btnmatrix, lv_color_hex(0x606060), LV_PART_ITEMS | LV_STATE_PRESSED);
    lv_obj_set_style_text_font(btnmatrix, &lv_font_montserrat_24, LV_PART_ITEMS | LV_STATE_DEFAULT);
}

void ui_Screen2_screen_destroy(void)
{
    if(ui_Screen2 != NULL) {
        lv_obj_del(ui_Screen2);
        ui_Screen2 = NULL;
    }
}
#include "encoder_ui.h"
#include "encoder.h"
#include "ui.h"
#include "esp_log.h"
#include "esp_lvgl_port.h"

static const char *TAG = "ENCODER_UI";

// Variables d'état
static uint8_t current_speed = 0;      // Vitesse actuelle (0-100)
static uint8_t current_direction = 0;  // 0 = Avant, 1 = Arrière

// Déclaration externe de l'objet image de direction
extern lv_obj_t *ui_imgDirectionP1;
extern lv_obj_t *ui_LabelVitesse;

/**
 * @brief Mettre à jour la jauge de vitesse
 */
static void update_speed_gauge(void)
{
    // Verrouiller LVGL
    lvgl_port_lock(0);

    if (ui_Gaugevitesse != NULL) {
        // La jauge LVGL arc va de 0 à 127
        lv_arc_set_value(ui_Gaugevitesse, current_speed);
        ESP_LOGD(TAG, "Speed gauge updated: %d", current_speed);
    }
    
    // Mettre à jour le label de vitesse si disponible
    if (ui_LabelVitesse != NULL) {
        char speed_text[8];
        snprintf(speed_text, sizeof(speed_text), "%d", current_speed);
        lv_label_set_text(ui_LabelVitesse, speed_text);
    }

    lvgl_port_unlock();
}

/**
 * @brief Mettre à jour l'image de direction
 */
static void update_direction_image(void)
{
    // Verrouiller LVGL
    lvgl_port_lock(0);

    if (ui_imgDirectionP1 != NULL) {
        if (current_direction == 0) {
            // Avant - flèche droite
            lv_image_set_src(ui_imgDirectionP1, &arrow_right_15604);
            ESP_LOGD(TAG, "Direction: FORWARD (right arrow)");
        } else {
            // Arrière - flèche gauche
            lv_image_set_src(ui_imgDirectionP1, &arrow_left_15597);
            ESP_LOGD(TAG, "Direction: REVERSE (left arrow)");
        }
    }

    lvgl_port_unlock();
}

/**
 * @brief Callback pour la rotation de l'encodeur
 */
static void encoder_rotation_callback(encoder_direction_t direction)
{
    if (direction == ENCODER_DIR_CW) {
        // Sens horaire : augmenter la vitesse
        if (current_speed < 127) {
            current_speed += 1;  // Incrément de 1
            if (current_speed > 127) {
                current_speed = 127;
            }
        }
        ESP_LOGI(TAG, "Speed increased to %d", current_speed);
    } else if (direction == ENCODER_DIR_CCW) {
        // Sens antihoraire : diminuer la vitesse
        if (current_speed > 0) {
            if (current_speed < 1) {
                current_speed = 0;
            } else {
                current_speed -= 1;  // Décrément de 1
            }
        }
        ESP_LOGI(TAG, "Speed decreased to %d", current_speed);
    }
    
    // Mettre à jour la jauge
    update_speed_gauge();
}

/**
 * @brief Callback pour l'appui sur le bouton de l'encodeur
 */
static void encoder_button_callback(uint32_t press_duration_ms)
{
    // Vérifier si on est sur Screen2 (sélection de loco)
    extern lv_obj_t * ui_Screen2;
    lv_obj_t * active_screen = lv_screen_active();
    
    if (active_screen == ui_Screen2) {
        // Sur Screen2 : valider la locomotive si elle est prête
        if (ui_screen2_is_loco_ready()) {
            ESP_LOGI(TAG, "Encoder button: validating locomotive on Screen2");
            lvgl_port_lock(0);
            ui_screen2_validate_loco();
            lvgl_port_unlock();
        } else {
            ESP_LOGW(TAG, "Encoder button: no locomotive ready to validate on Screen2");
        }
    } else {
        // Sur les autres écrans : basculer la direction
        current_direction = (current_direction == 0) ? 1 : 0;
        
        ESP_LOGI(TAG, "Direction toggled to %s (button pressed for %lu ms)", 
                 current_direction == 0 ? "FORWARD" : "REVERSE", 
                 press_duration_ms);
        
        // Mettre à jour l'image de direction
        update_direction_image();
    }
}

/**
 * @brief Initialiser l'intégration UI de l'encodeur
 */
esp_err_t encoder_ui_init(void)
{
    ESP_LOGI(TAG, "Initializing encoder UI integration");
    
    // Vérifier que les objets UI existent
    if (ui_Gaugevitesse == NULL || ui_imgDirectionP1 == NULL) {
        ESP_LOGW(TAG, "UI widgets missing (Gauge/Direction). Pre-initializing Screen1...");
        // Appeler directement l'init de Screen1; l'appelant peut gérer le lock
        ui_Screen1_screen_init();
        if (ui_Gaugevitesse == NULL) {
            ESP_LOGE(TAG, "ui_Gaugevitesse is NULL after pre-init - Screen1 not ready");
            return ESP_FAIL;
        }
        if (ui_imgDirectionP1 == NULL) {
            ESP_LOGE(TAG, "ui_imgDirectionP1 is NULL after pre-init - Screen1 not ready");
            return ESP_FAIL;
        }
    }
    
    // Initialiser l'encodeur avec les callbacks
    esp_err_t ret = encoder_init(encoder_rotation_callback, encoder_button_callback);
    if (ret != ESP_OK) {
        ESP_LOGE(TAG, "Failed to initialize encoder");
        return ret;
    }
    
    // Initialiser l'affichage
    current_speed = 0;
    current_direction = 0;
    update_speed_gauge();
    update_direction_image();
    
    ESP_LOGI(TAG, "Encoder UI integration initialized successfully");
    ESP_LOGI(TAG, "  - Rotate encoder to control speed (0-127)");
    ESP_LOGI(TAG, "  - Press encoder button to toggle direction");
    
    return ESP_OK;
}

/**
 * @brief Obtenir la valeur actuelle de la vitesse
 */
uint8_t encoder_ui_get_speed(void)
{
    return current_speed;
}

/**
 * @brief Définir la valeur de vitesse manuellement
 */
void encoder_ui_set_speed(uint8_t speed)
{
    if (speed > 127) {
        speed = 127;
    }
    
    current_speed = speed;
    update_speed_gauge();
    
    ESP_LOGI(TAG, "Speed set to %d", current_speed);
}

/**
 * @brief Obtenir la direction actuelle
 */
uint8_t encoder_ui_get_direction(void)
{
    return current_direction;
}

/**
 * @brief Basculer la direction manuellement
 */
void encoder_ui_toggle_direction(void)
{
    current_direction = (current_direction == 0) ? 1 : 0;
    update_direction_image();
    
    ESP_LOGI(TAG, "Direction toggled to %s", 
             current_direction == 0 ? "FORWARD" : "REVERSE");
}

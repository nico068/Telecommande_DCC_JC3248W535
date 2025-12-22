/**
 * @file encoder_integration_example.c
 * @brief Exemple d'intégration de l'encodeur avec l'UI LVGL
 * 
 * Ce fichier montre comment initialiser et utiliser l'encodeur
 * pour contrôler la jauge de vitesse et la direction sur Screen1.
 */

#include "encoder_ui.h"
#include "ui.h"
#include "esp_log.h"

static const char *TAG = "ENCODER_EXAMPLE";

/**
 * @brief Fonction d'initialisation à appeler dans main.c
 * 
 * À appeler APRÈS ui_init() et après que Screen1 soit créé
 */
void init_encoder_system(void)
{
    ESP_LOGI(TAG, "Initializing encoder control system...");
    
    // Initialiser l'UI LVGL d'abord
    ui_init();
    
    // S'assurer que Screen1 est initialisé
    ui_Screen1_screen_init();
    
    // Puis initialiser l'encodeur avec intégration UI
    // Cela va automatiquement :
    // 1. Initialiser l'encodeur sur GPIO17/18 (rotation) et GPIO6 (bouton)
    // 2. Lier la rotation à la jauge de vitesse (ui_Gaugevitesse)
    // 3. Lier l'appui bouton au changement de direction (ui_imgDirectionP1)
    esp_err_t ret = encoder_ui_init();
    if (ret != ESP_OK) {
        ESP_LOGE(TAG, "Failed to initialize encoder system");
        return;
    }
    
    ESP_LOGI(TAG, "Encoder system initialized successfully");
    ESP_LOGI(TAG, "Usage:");
    ESP_LOGI(TAG, "  - Rotate encoder CW  : Increase speed (0-127)");
    ESP_LOGI(TAG, "  - Rotate encoder CCW : Decrease speed (0-127)");
    ESP_LOGI(TAG, "  - Press encoder button: Toggle direction (forward/reverse)");
}

/**
 * @brief Contrôler la vitesse par programme
 */
void set_train_speed(uint8_t speed)
{
    ESP_LOGI(TAG, "Setting train speed to %d", speed);
    encoder_ui_set_speed(speed);
}

/**
 * @brief Obtenir l'état actuel du contrôle
 */
void print_control_status(void)
{
    uint8_t speed = encoder_ui_get_speed();
    uint8_t direction = encoder_ui_get_direction();
    
    ESP_LOGI(TAG, "Train Control Status:");
    ESP_LOGI(TAG, "  Speed: %d", speed);
    ESP_LOGI(TAG, "  Direction: %s", direction == 0 ? "FORWARD" : "REVERSE");
}

/**
 * @brief Arrêt d'urgence
 */
void emergency_stop(void)
{
    ESP_LOGW(TAG, "EMERGENCY STOP!");
    encoder_ui_set_speed(0);
}

/**
 * @brief Marche arrière
 */
void reverse_train(void)
{
    ESP_LOGI(TAG, "Setting reverse direction");
    
    // S'assurer que la vitesse est à 0 avant de changer de direction
    encoder_ui_set_speed(0);
    
    // Changer la direction si nécessaire
    if (encoder_ui_get_direction() == 0) {
        encoder_ui_toggle_direction();
    }
}

/**
 * @brief Marche avant
 */
void forward_train(void)
{
    ESP_LOGI(TAG, "Setting forward direction");
    
    // S'assurer que la vitesse est à 0 avant de changer de direction
    encoder_ui_set_speed(0);
    
    // Changer la direction si nécessaire
    if (encoder_ui_get_direction() == 1) {
        encoder_ui_toggle_direction();
    }
}

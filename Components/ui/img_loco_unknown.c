#include "ui.h"

// Image générique pour loco inconnue
// À remplacer par une vraie image LVGL si besoin
const lv_image_dsc_t img_loco_unknown = {
    .header.w = 32,
    .header.h = 32,
    .data_size = 4,
    .header.cf = LV_COLOR_FORMAT_ARGB8888,
    .data = (const uint8_t[]){0xFF,0x00,0x00,0xFF} // Un pixel rouge, à remplacer par une vraie image
};

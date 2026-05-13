#include <zephyr/kernel.h>

#include "battery_peripheral.h"
#include "../assets/custom_fonts.h"

LV_IMG_DECLARE(bolt);
LV_IMG_DECLARE(r_battery_100);
LV_IMG_DECLARE(r_battery_90);
LV_IMG_DECLARE(r_battery_75);
LV_IMG_DECLARE(r_battery_50);
LV_IMG_DECLARE(r_battery_25);
LV_IMG_DECLARE(r_battery_10);

static void canvas_draw_image_lvgl9(
    lv_obj_t *canvas,
    lv_coord_t x,
    lv_coord_t y,
    const lv_image_dsc_t *src,
    const lv_draw_image_dsc_t *image_dsc
) {
    lv_layer_t layer;
    lv_canvas_init_layer(canvas, &layer);

    lv_area_t coords = {
        .x1 = x,
        .y1 = y,
        .x2 = x + src->header.w - 1,
        .y2 = y + src->header.h - 1,
    };

    lv_draw_image_dsc_t dsc = *image_dsc;
    dsc.src = src;

    lv_draw_image(&layer, &dsc, &coords);

    lv_canvas_finish_layer(canvas, &layer);
}

static const lv_image_dsc_t *battery_peripheral_image_for_level(uint8_t level) {
    if (level > 90) {
        return &r_battery_100;
    } else if (level > 75) {
        return &r_battery_90;
    } else if (level > 50) {
        return &r_battery_75;
    } else if (level > 25) {
        return &r_battery_50;
    } else if (level > 10) {
        return &r_battery_25;
    } else if (level > 1) {
        return &r_battery_10;
    }

    return NULL;
}

static void draw_level_peripheral(lv_obj_t *canvas, const struct status_state *state) {
    lv_draw_image_dsc_t img_dsc_r;
    lv_draw_image_dsc_init(&img_dsc_r);

    const lv_image_dsc_t *image = battery_peripheral_image_for_level(state->battery_p);

    if (image != NULL) {
        canvas_draw_image_lvgl9(canvas, 80, 10, image, &img_dsc_r);
    }
}

void draw_battery_peripheral_status(lv_obj_t *canvas, const struct status_state *state) {
    draw_level_peripheral(canvas, state);
}

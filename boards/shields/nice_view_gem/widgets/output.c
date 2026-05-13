#include <zephyr/kernel.h>

#include "output.h"
#include "../assets/custom_fonts.h"

LV_IMG_DECLARE(bt_no_signal);
LV_IMG_DECLARE(bt_unbonded);
LV_IMG_DECLARE(bt);
LV_IMG_DECLARE(usb);

static void canvas_draw_text_lvgl9(
    lv_obj_t *canvas,
    lv_coord_t x,
    lv_coord_t y,
    lv_coord_t w,
    const lv_draw_label_dsc_t *label_dsc,
    const char *text
) {
    lv_layer_t layer;
    lv_canvas_init_layer(canvas, &layer);

    lv_area_t area = {
        .x1 = x,
        .y1 = y,
        .x2 = x + w - 1,
        .y2 = y + 20,
    };

    lv_draw_label_dsc_t dsc = *label_dsc;
    dsc.text = text;

    lv_draw_label(&layer, &dsc, &area);

    lv_canvas_finish_layer(canvas, &layer);
}

static void draw_endpoint_text(lv_obj_t *canvas, const char *text) {
    lv_draw_label_dsc_t label_dsc;

    init_label_dsc(
        &label_dsc,
        LVGL_FOREGROUND,
        &quinquefive_8,
        LV_TEXT_ALIGN_LEFT
    );

    canvas_draw_text_lvgl9(
        canvas,
        12,
        140,
        SCREEN_WIDTH - 8,
        &label_dsc,
        text
    );
}

#if !IS_ENABLED(CONFIG_ZMK_SPLIT) || IS_ENABLED(CONFIG_ZMK_SPLIT_ROLE_CENTRAL)
static void draw_usb_connected(lv_obj_t *canvas) {
    draw_endpoint_text(canvas, "USB");
}
#endif

static void draw_ble_disconnected(lv_obj_t *canvas) {
    draw_endpoint_text(canvas, "NULL");
}

static void draw_ble_connected(lv_obj_t *canvas) {
    draw_endpoint_text(canvas, "BLE");
}

void draw_output_status(lv_obj_t *canvas, const struct status_state *state) {
    switch (state->selected_endpoint.transport) {
#if !IS_ENABLED(CONFIG_ZMK_SPLIT) || IS_ENABLED(CONFIG_ZMK_SPLIT_ROLE_CENTRAL)
    case ZMK_TRANSPORT_USB:
        draw_usb_connected(canvas);
        break;
#endif

    case ZMK_TRANSPORT_BLE:
        draw_ble_connected(canvas);
        break;

    default:
        draw_ble_disconnected(canvas);
        break;
    }
}

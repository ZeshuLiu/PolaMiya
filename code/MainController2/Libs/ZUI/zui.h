#ifndef __ZUI_H__
#define __ZUI_H__

#include "main.h"
#include "lcd.h"
#include "pic.h"
#include "spi.h"
#include "zui_usr.h"
#include "lcd_init.h"

#define DISP_HOR 280
#define DISP_VER 240
#define ZUI_BUF_LFRAC 20
#define ZUI_BUF_LINES (DISP_VER / ZUI_BUF_LFRAC)
#define ZUI_BUF_SIZE (ZUI_BUF_LINES * DISP_HOR * 2)

extern uint16_t zui_pixel_buffer0[ZUI_BUF_LINES * DISP_HOR];

typedef struct UI_Element {
    uint16_t x, y, w, h;
    void (*render)(struct UI_Element *el, uint16_t *buf);
    uint8_t dirty;
    struct UI_Element *next;
    void *user_data;  // 可选：绑定显示值或状态
} UI_Element;

typedef struct {
    UI_Element *head;
    void (*on_key)(uint8_t key);
} UI_Layer;

void zui_init(void);
void zui_elm_set_dirty(UI_Element * el);
void zui_set_current_layer(UI_Layer * tgt_layer);
void zui_fill_screen_color(uint16_t color, uint16_t *buf);
void zui_layer_add_element(UI_Layer *layer, UI_Element *el);
void zui_render_current_layer(void);

#endif
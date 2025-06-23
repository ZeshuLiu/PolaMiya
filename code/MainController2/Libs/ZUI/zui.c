#include "zui.h"
#include "string.h"
#include <string.h>


uint16_t zui_pixel_buffer0[ZUI_BUF_LINES * DISP_HOR];

UI_Layer *current_layer = NULL;

/**
 * @brief 初始化zui
 * 
 */
void zui_init()
{
    LCD_Init(hspi1);//LCD初始化
    zui_layer_init();
    zui_elm_init();
}

/**
 * @brief 切换显示层
 * 
 * @param tgt_layer 要切换的显示层
 */
void zui_set_current_layer(UI_Layer * tgt_layer)
{
    current_layer = tgt_layer;
}

void zui_elm_set_dirty(struct UI_Element *el)
{
    el->dirty = 1;
}

/**
 * @brief 填充整个屏幕颜色，整屏刷新
 * 
 * @param color 要填充的颜色
 */
void zui_fill_screen_color(uint16_t color, uint16_t *buf)
{
    // memset(zui_pixel_buffer0, WHITE, ZUI_BUF_SIZE);
    memcpy(buf, gImage_DOG, ZUI_BUF_SIZE);
    for (uint8_t i = 0; i < ZUI_BUF_LFRAC; i++)
    {
        LCD_DrawArea(0,i*ZUI_BUF_LINES, DISP_HOR, (i+1)*ZUI_BUF_LINES, buf);
    }
}

/**
 * @brief 向显示层中添加元素
 * 
 * @param layer 显示层
 * @param el 元素
 */
void zui_layer_add_element(UI_Layer *layer, UI_Element *el)
{
    el->next = layer->head;
    layer->head = el;
}

/**
 * @brief 刷新当前层
 * 
 */
void zui_render_current_layer(void)
{
    if (current_layer) return;

    UI_Element *el = current_layer->head;
    while (el) {
        if (el->dirty) {

            el->render(el, zui_pixel_buffer0);
            el->dirty = 0;
        }
        el = el->next;
    }
}

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
    memset(buf, color, ZUI_BUF_SIZE);
    //memcpy(buf, gImage_DOG, ZUI_BUF_SIZE);
    for (uint8_t i = 0; i < ZUI_BUF_LFRAC; i++)
    {
        LCD_DrawArea(0,i*ZUI_BUF_LINES, DISP_HOR-1, (i+1)*ZUI_BUF_LINES-1, buf);
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
    if (!current_layer) return;

    UI_Element *el = current_layer->head;
    while (el) {
        if (el->dirty) {
            el->render(el, zui_pixel_buffer0);
            el->dirty = 0;
        }
        el = el->next;
    }
}

/**
 * @brief 将给定层所有对象设为脏
 *
 */
void zui_dirty_layer(UI_Layer * tgt_layer)
{
    if (!tgt_layer) return;

    UI_Element *el = tgt_layer->head;
    while (el) {
        el->dirty = 1;
        el = el->next;
    }
}

/**
 * @brief 将层所有对象设为脏
 *
 */
void zui_dirty_current_layer(void)
{
    UI_Layer * tgt_layer = current_layer;
    if (!tgt_layer) return;

    UI_Element *el = tgt_layer->head;
    while (el) {
        el->dirty = 1;
        el = el->next;
    }
}

/**
 * @brief 响应按键
 * 
 */
void zui_key_respond(USR_KEY * key)
{
    if (!current_layer) return;

    current_layer->on_key(key);
}

void char_12_elm_render(struct UI_Element *el, uint16_t *buf)
{
    uint32_t bufi = 0;
    uint16_t sizex = (el->usr_data_len-1) * 7;
    uint16_t num_char = (el->usr_data_len-1);
    uint8_t tmp;
    char *str = (char *)(el->user_data);  // 强制转换

    for (int row = 0; row < 12; row++) {
        for (int chari = 0; chari<num_char; chari++){
            tmp = ascii_1206[str[chari] - ' '] [row];
            for(uint8_t t=0;t<6;t++){
                if(tmp&(0x01<<t)) buf[bufi++] = NORMAL_LAYER_FORE;
                else buf[bufi++] = NORMAL_LAYER_BACK;
            }
            buf[bufi++] = NORMAL_LAYER_BACK;
        }
    }
    LCD_DrawArea(el->x,el->y, el->x + sizex - 1, el->y + 11, buf);

	// static uint16_t tmp=BLACK;
	// clr++;
    // zui_fill_screen_color(clr, buf);
}

void char_16_elm_render(struct UI_Element *el, uint16_t *buf)
{
    uint32_t bufi = 0;
    uint16_t sizex = (el->usr_data_len-1) * 9;
    uint16_t num_char = (el->usr_data_len-1);
    uint16_t tmp;
    char *str = (char *)(el->user_data);  // 强制转换

    for (int row = 0; row < 16; row++) {
        for (int chari = 0; chari<num_char; chari++){
            // tmp = ascii_2412[str[chari] - ' '];
            tmp = ascii_1608[str[chari] - ' '][row ];
            for(uint8_t t=0;t<8;t++){
                if(tmp&(0x01<<t)) buf[bufi++] = NORMAL_LAYER_FORE;
                else buf[bufi++] = NORMAL_LAYER_BACK;
            }
            buf[bufi++] = NORMAL_LAYER_BACK;
        }
    }
    LCD_DrawArea(el->x,el->y, el->x + sizex - 1, el->y + 15, buf);

	// static uint16_t tmp=BLACK;
	// clr++;
    // zui_fill_screen_color(clr, buf);
}

void char_24_elm_render(struct UI_Element *el, uint16_t *buf)
{
    uint32_t bufi = 0;
    uint16_t sizex = (el->usr_data_len-1) * 13;
    uint16_t num_char = (el->usr_data_len-1);
    uint16_t tmp;
    char *str = (char *)(el->user_data);  // 强制转换

    for (int row = 0; row < 24; row++) {
        for (int chari = 0; chari<num_char; chari++){
            // tmp = ascii_2412[str[chari] - ' '];
            tmp = (ascii_2412[str[chari] - ' '][row * 2 + 1] << 8) | ascii_2412[str[chari] - ' '][row * 2];  // 每行2字节，共12bit有效
            for(uint8_t t=0;t<12;t++){
                if(tmp&(0x01<<t)) buf[bufi++] = NORMAL_LAYER_FORE;
                else buf[bufi++] = NORMAL_LAYER_BACK;
            }
            buf[bufi++] = NORMAL_LAYER_BACK;
        }
    }
    LCD_DrawArea(el->x,el->y, el->x + sizex - 1, el->y + 23, buf);

	// static uint16_t tmp=BLACK;
	// clr++;
    // zui_fill_screen_color(clr, buf);
}

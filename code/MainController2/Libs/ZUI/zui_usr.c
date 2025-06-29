#include "zui_usr.h"
#include <stdint.h>

/* PURE LAYER*/
UI_Layer pure_layer;
UI_Element pure_element;
void zui_pure_layer_on_key(uint8_t key)
{
    return;
}

void pure_elm_render(struct UI_Element *el, uint16_t *buf)
{
	static uint16_t clr=BLACK;
	clr++;
    zui_fill_screen_color(clr, buf);
}

void pure_layer_elm_init(void)
{
    pure_element.x = 0;
    pure_element.y = 0;
    pure_element.w = DISP_HOR;
    pure_element.h = DISP_VER;
    pure_element.render = pure_elm_render;
    pure_element.dirty = 0;
    pure_element.next = NULL;
    pure_element.user_data = NULL;
    zui_layer_add_element(&pure_layer, &pure_element);
}

/* Noraml Layer*/
UI_Layer normal_layer;
UI_Element bat_elm_char16;
UI_Element batnum_elm_char16;
UI_Element zui_elm_char12;

void zui_normal_layer_on_key(uint8_t key)
{
    return;
}

void normal_layer_elm_init(void)
{
    bat_elm_char16.x = BAT_TITLE_X;
    bat_elm_char16.y = BAT_TITLE_Y;
    bat_elm_char16.w = 50;
    bat_elm_char16.h = 20;
    bat_elm_char16.render = char_16_elm_render;
    bat_elm_char16.dirty = 0;
    bat_elm_char16.next = NULL;
    bat_elm_char16.usr_data_len = 5;
    bat_elm_char16.user_data = (uint8_t *) "BAT:";
    zui_layer_add_element(&normal_layer, &bat_elm_char16);

    batnum_elm_char16.x = BAT_NUM_X;
    batnum_elm_char16.y = BAT_NUM_Y;
    batnum_elm_char16.w = 50;
    batnum_elm_char16.h = 20;
    batnum_elm_char16.render = char_16_elm_render;
    batnum_elm_char16.dirty = 0;
    batnum_elm_char16.next = NULL;
    batnum_elm_char16.usr_data_len = 6;
    batnum_elm_char16.user_data = (uint8_t *) "-.--V";
    zui_layer_add_element(&normal_layer, &batnum_elm_char16);

    zui_elm_char12.x = 20;
    zui_elm_char12.y = 220;
    zui_elm_char12.w = 50;
    zui_elm_char12.h = 20;
    zui_elm_char12.render = char_12_elm_render;
    zui_elm_char12.dirty = 0;
    zui_elm_char12.next = NULL;
    zui_elm_char12.usr_data_len = 28;
    zui_elm_char12.user_data = (uint8_t *) "Powered By Z-UI (Ver 0.0.1)";
    zui_layer_add_element(&normal_layer, &zui_elm_char12);

}


/* Global Calls */
void zui_layer_init(void)
{
    pure_layer.head = NULL;
    pure_layer.on_key = zui_pure_layer_on_key;
    zui_set_current_layer(&pure_layer);

    normal_layer.head = NULL;
    normal_layer.on_key = zui_normal_layer_on_key;
    zui_set_current_layer(&normal_layer);
}



void zui_elm_init(void)
{
    pure_layer_elm_init();
    normal_layer_elm_init();

}

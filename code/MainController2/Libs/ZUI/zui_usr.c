#include "zui_usr.h"

UI_Layer pure_layer;
UI_Element pure_element;

void zui_pure_layer_on_key(uint8_t key)
{
    return;
}
void pure_elm_render(struct UI_Element *el, uint16_t *buf)
{
    zui_fill_screen_color(BLACK, buf);
}


void zui_layer_init(void)
{
    pure_layer.head = NULL;
    pure_layer.on_key = zui_pure_layer_on_key;
}



void zui_elm_init(void)
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

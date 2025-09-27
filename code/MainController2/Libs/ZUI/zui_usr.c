#include "zui_usr.h"
#include "motor.h"

/* PURE LAYER*/
UI_Layer pure_layer;
UI_Element pure_element;
void zui_pure_layer_on_key(USR_KEY * key)
{
    return;
}

void pure_elm_render(struct UI_Element *el, uint16_t *buf)
{
	static uint16_t clr=WHITE;
	// clr++;
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
char keyval_vals[4] = "x-x";
char distanceval_vals[6] = "xx.xx";
char Coretemp_vals[3] = "xx";
char Boardtemp_vals[3] = "xx";
char RBC_vals[6] = "00000";
char MTL_vals[6] = "00000";
char STC_vals[6] = "00000";
UI_Layer normal_layer;
UI_Element bat_elm_char16;
UI_Element batnum_elm_char16;
UI_Element key_elm_char16;
UI_Element keyval_elm_char16;
UI_Element distance_elm_char16;
UI_Element distanceval_elm_char16;
UI_Element CoreTemp_elm_char16;
UI_Element CoreTempVal_elm_char16;
UI_Element BoardTemp_elm_char16;
UI_Element BoardTempVal_elm_char16;
UI_Element RBC_elm_char16;
UI_Element RBCVal_elm_char16;
UI_Element MTL_elm_char16;
UI_Element MTLVal_elm_char16;
UI_Element STC_elm_char16;
UI_Element STCVal_elm_char16;
UI_Element zui_elm_char12;
UI_Element Cal_elm_char12;

void zui_normal_layer_on_key(USR_KEY * key)
{
    extern uint8_t motor_state;

    keyval_vals[0] = key->key_index + '0' ;
    keyval_vals[2] = key->key_action + '0' ;
    keyval_elm_char16.dirty = 1;

    // 按键响应0
    if (motor_state == 0 && key->key_index == 0 && key->key_action == 2)
    {
        motor_start();
    }
    else if(motor_state == 1)
    {
        // motor_end();
    }


    key->key_action = 0;
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

    key_elm_char16.x = KEY_TITLE_X;
    key_elm_char16.y = KEY_TITLE_Y;
    key_elm_char16.w = 50;
    key_elm_char16.h = 20;
    key_elm_char16.render = char_16_elm_render;
    key_elm_char16.dirty = 0;
    key_elm_char16.next = NULL;
    key_elm_char16.usr_data_len = 5;
    key_elm_char16.user_data = (uint8_t *) "KEY:";
    zui_layer_add_element(&normal_layer, &key_elm_char16);

    keyval_elm_char16.x = KEY_NUM_X;
    keyval_elm_char16.y = KEY_NUM_Y;
    keyval_elm_char16.w = 50;
    keyval_elm_char16.h = 20;
    keyval_elm_char16.render = char_16_elm_render;
    keyval_elm_char16.dirty = 0;
    keyval_elm_char16.next = NULL;
    keyval_elm_char16.usr_data_len = 4;
    keyval_elm_char16.user_data = (uint8_t *) keyval_vals;
    zui_layer_add_element(&normal_layer, &keyval_elm_char16);

    distance_elm_char16.x = DISTANCE_TITLE_X;
    distance_elm_char16.y = DISTANCE_TITLE_Y;
    distance_elm_char16.w = 50;
    distance_elm_char16.h = 20;
    distance_elm_char16.render = char_16_elm_render;
    distance_elm_char16.dirty = 0;
    distance_elm_char16.next = NULL;
    distance_elm_char16.usr_data_len = 5;
    distance_elm_char16.user_data = (uint8_t *) "DIS:";
    zui_layer_add_element(&normal_layer, &distance_elm_char16);

    distanceval_elm_char16.x = DISTANCE_NUM_X;
    distanceval_elm_char16.y = DISTANCE_NUM_Y;
    distanceval_elm_char16.w = 50;
    distanceval_elm_char16.h = 20;
    distanceval_elm_char16.render = char_16_elm_render;
    distanceval_elm_char16.dirty = 0;
    distanceval_elm_char16.next = NULL;
    distanceval_elm_char16.usr_data_len = 6;
    distanceval_elm_char16.user_data = (uint8_t *) distanceval_vals;
    zui_layer_add_element(&normal_layer, &distanceval_elm_char16);

    CoreTemp_elm_char16.x = CORETEMP_TITLE_X;
    CoreTemp_elm_char16.y = CORETEMP_TITLE_Y;
    CoreTemp_elm_char16.w = 50;
    CoreTemp_elm_char16.h = 20;
    CoreTemp_elm_char16.render = char_16_elm_render;
    CoreTemp_elm_char16.dirty = 0;
    CoreTemp_elm_char16.next = NULL;
    CoreTemp_elm_char16.usr_data_len = 5;
    CoreTemp_elm_char16.user_data = (uint8_t *) "CTP:";
    zui_layer_add_element(&normal_layer, &CoreTemp_elm_char16);

    CoreTempVal_elm_char16.x = CORETEMP_NUM_X;
    CoreTempVal_elm_char16.y = CORETEMP_NUM_Y;
    CoreTempVal_elm_char16.w = 50;
    CoreTempVal_elm_char16.h = 20;
    CoreTempVal_elm_char16.render = char_16_elm_render;
    CoreTempVal_elm_char16.dirty = 0;
    CoreTempVal_elm_char16.next = NULL;
    CoreTempVal_elm_char16.usr_data_len = 3;
    CoreTempVal_elm_char16.user_data = (uint8_t *) Coretemp_vals;
    zui_layer_add_element(&normal_layer, &CoreTempVal_elm_char16);

    BoardTemp_elm_char16.x = BOARDTEMP_TITLE_X;
    BoardTemp_elm_char16.y = BOARDTEMP_TITLE_Y;
    BoardTemp_elm_char16.w = 50;
    BoardTemp_elm_char16.h = 20;
    BoardTemp_elm_char16.render = char_16_elm_render;
    BoardTemp_elm_char16.dirty = 0;
    BoardTemp_elm_char16.next = NULL;
    BoardTemp_elm_char16.usr_data_len = 5;
    BoardTemp_elm_char16.user_data = (uint8_t *) "BTP:";
    zui_layer_add_element(&normal_layer, &BoardTemp_elm_char16);

    BoardTempVal_elm_char16.x = BOARDTEMP_NUM_X;
    BoardTempVal_elm_char16.y = BOARDTEMP_NUM_Y;
    BoardTempVal_elm_char16.w = 50;
    BoardTempVal_elm_char16.h = 20;
    BoardTempVal_elm_char16.render = char_16_elm_render;
    BoardTempVal_elm_char16.dirty = 0;
    BoardTempVal_elm_char16.next = NULL;
    BoardTempVal_elm_char16.usr_data_len = 3;
    BoardTempVal_elm_char16.user_data = (uint8_t *) Boardtemp_vals;
    zui_layer_add_element(&normal_layer, &BoardTempVal_elm_char16);

    RBC_elm_char16.x = RBC_TITLE_X;
    RBC_elm_char16.y = RBC_TITLE_Y;
    RBC_elm_char16.w = 50;
    RBC_elm_char16.h = 20;
    RBC_elm_char16.render = char_16_elm_render;
    RBC_elm_char16.dirty = 0;
    RBC_elm_char16.next = NULL;
    RBC_elm_char16.usr_data_len = 5;
    RBC_elm_char16.user_data = (uint8_t *) "RBC:";
    zui_layer_add_element(&normal_layer, &RBC_elm_char16);

    RBCVal_elm_char16.x = RBC_NUM_X;
    RBCVal_elm_char16.y = RBC_NUM_Y;
    RBCVal_elm_char16.w = 50;
    RBCVal_elm_char16.h = 20;
    RBCVal_elm_char16.render = char_16_elm_render;
    RBCVal_elm_char16.dirty = 0;
    RBCVal_elm_char16.next = NULL;
    RBCVal_elm_char16.usr_data_len = 6;
    RBCVal_elm_char16.user_data = (uint8_t *) RBC_vals;
    zui_layer_add_element(&normal_layer, &RBCVal_elm_char16);

    MTL_elm_char16.x = MTL_TITLE_X;
    MTL_elm_char16.y = MTL_TITLE_Y;
    MTL_elm_char16.w = 50;
    MTL_elm_char16.h = 20;
    MTL_elm_char16.render = char_16_elm_render;
    MTL_elm_char16.dirty = 0;
    MTL_elm_char16.next = NULL;
    MTL_elm_char16.usr_data_len = 5;
    MTL_elm_char16.user_data = (uint8_t *) "MTL:";
    zui_layer_add_element(&normal_layer, &MTL_elm_char16);

    MTLVal_elm_char16.x = MTL_NUM_X;
    MTLVal_elm_char16.y = MTL_NUM_Y;
    MTLVal_elm_char16.w = 50;
    MTLVal_elm_char16.h = 20;
    MTLVal_elm_char16.render = char_16_elm_render;
    MTLVal_elm_char16.dirty = 0;
    MTLVal_elm_char16.next = NULL;
    MTLVal_elm_char16.usr_data_len = 6;
    MTLVal_elm_char16.user_data = (uint8_t *) MTL_vals;
    zui_layer_add_element(&normal_layer, &MTLVal_elm_char16);

    STC_elm_char16.x = STC_TITLE_X;
    STC_elm_char16.y = STC_TITLE_Y;
    STC_elm_char16.w = 50;
    STC_elm_char16.h = 20;
    STC_elm_char16.render = char_16_elm_render;
    STC_elm_char16.dirty = 0;
    STC_elm_char16.next = NULL;
    STC_elm_char16.usr_data_len = 5;
    STC_elm_char16.user_data = (uint8_t *) "STC:";
    zui_layer_add_element(&normal_layer, &STC_elm_char16);

    STCVal_elm_char16.x = STC_NUM_X;
    STCVal_elm_char16.y = STC_NUM_Y;
    STCVal_elm_char16.w = 50;
    STCVal_elm_char16.h = 20;
    STCVal_elm_char16.render = char_16_elm_render;
    STCVal_elm_char16.dirty = 0;
    STCVal_elm_char16.next = NULL;
    STCVal_elm_char16.usr_data_len = 6;
    STCVal_elm_char16.user_data = (uint8_t *) STC_vals;
    zui_layer_add_element(&normal_layer, &STCVal_elm_char16);

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

    Cal_elm_char12.x = 20;
    Cal_elm_char12.y = 200;
    Cal_elm_char12.w = 50;
    Cal_elm_char12.h = 20;
    Cal_elm_char12.render = char_12_elm_render;
    Cal_elm_char12.dirty = 0;
    Cal_elm_char12.next = NULL;
    Cal_elm_char12.usr_data_len = 31;
    Cal_elm_char12.user_data = (uint8_t *) "inf-7m-4m-2.5m-2m-1.5m-1.2m-1m";
    zui_layer_add_element(&normal_layer, &Cal_elm_char12);

}


/* Global Calls */
void zui_layer_init(void)
{
    pure_layer.head = NULL;
    pure_layer.on_key = zui_pure_layer_on_key;
    zui_set_current_layer(&pure_layer);

    normal_layer.head = NULL;
    normal_layer.on_key = zui_normal_layer_on_key;
    // zui_set_current_layer(&normal_layer);
}



void zui_elm_init(void)
{
    pure_layer_elm_init();
    normal_layer_elm_init();

}

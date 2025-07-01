#ifndef __ZUI_USR_H__
#define __ZUI_USR_H__

#include "main.h"
#include "zui.h"
#include "lcd.h"
#include "pic.h"

#include "string.h"
#include <string.h>

#define NORMAL_LAYER_BACK MAGENTA
#define NORMAL_LAYER_FORE BLACK

#define BAT_TITLE_X 30
#define BAT_TITLE_Y 30

#define KEY_TITLE_X 30
#define KEY_TITLE_Y 50


#define BAT_NUM_X 30 + 37
#define BAT_NUM_Y 30

#define KEY_NUM_X 30 + 37
#define KEY_NUM_Y 50

void zui_layer_init(void);
void zui_elm_init(void);
#endif
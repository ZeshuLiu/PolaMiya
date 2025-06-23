#include "lcd_usr.h"
#include <stdint.h>

#define MODE_0_START_X 175
#define DOG_END 101
#define FRAME_COLOR_0 GRAYBLUE
#define CMD_IN_COLOR (HEX_DISP) ? BLUE:GREEN
#define CMD_OUT_COLOR (HEX_DISP) ? MAGENTA:RED

void DRAW_FRAME(uint8_t mode)
{
  // uint8_t pos;

  switch (mode)
  {
    case 0:
      /* 进出指令框 */
      LCD_DrawLine(0,MODE_0_START_X, 320,MODE_0_START_X, FRAME_COLOR_0);      // 命令行下方横线
      LCD_DrawLine(0,DOG_END+2, 320,DOG_END+2, FRAME_COLOR_0);                // 命令行顶横线
      LCD_DrawLine(0,DOG_END+2, 0,MODE_0_START_X - 1, FRAME_COLOR_0);         // 命令行左侧横线
      LCD_DrawLine(279,DOG_END+2, 279,MODE_0_START_X - 1, FRAME_COLOR_0);     // 命令行右侧横线

      /* 串口参数显示 */
      LCD_ShowString(5,MODE_0_START_X+3,(uint8_t *) "BaudRate:",WHITE,BLACK,24,0);// 字高24 
      LCD_ShowString(15,MODE_0_START_X+27,(uint8_t *) "TX:",WHITE,BLACK,16,0);// 字高16
      LCD_ShowString(117,MODE_0_START_X+27,(uint8_t *) "Time(s):",WHITE,BLACK,16,0);// 字高16
      break;

    default:
      break;
  }

}

void DRAW_DATA(uint8_t mode)
{
  uint8_t pos, i;
  static uint32_t t_cnt = 0;

  extern _Bool HEX_DISP; // 当前是否按照16进制格式显示

  switch (mode)
  {
    case 0x00:
      LCD_ShowIntNum(129, MODE_0_START_X+3, 11200, 7, WHITE, BLACK, 24);
      LCD_ShowIntNum(54, MODE_0_START_X+27, 32, 5, WHITE, BLACK, 16);
      LCD_ShowIntNum(185, MODE_0_START_X+27, (++t_cnt)/5, 5, WHITE, BLACK, 16);
      break;

    default:
      break;
  }
}
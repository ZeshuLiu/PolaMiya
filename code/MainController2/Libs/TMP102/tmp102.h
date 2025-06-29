#ifndef __TMP102_H__
#define __TMP102_H__

#include "main.h"

#define TMP102_ADDR      (0x48 << 1)  // 左移1位用于HAL库的8位地址格式
#define TMP102_TEMP_REG  0x00         // 温度寄存器地址
#define TMP102_I2C_TIMEOUT_MS 10

float TMP102_ReadTemperature(void);

#endif
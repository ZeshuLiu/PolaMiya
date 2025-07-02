#include "tmp102.h"
#include <stdint.h>
#include "zui.h"

extern I2C_HandleTypeDef hi2c2;

float tmp102Temp = 0;

float TMP102_ReadTemperature(void)
{
    uint8_t temp_raw[2] = {0};
    uint8_t tmpreg = TMP102_TEMP_REG;
    extern UI_Element BoardTempVal_elm_char16;
    extern char Boardtemp_vals[3];

    // 向 TMP102 写入要读取的寄存器地址
    if (HAL_I2C_Master_Transmit(&hi2c2, TMP102_ADDR, (uint8_t *)&tmpreg, 1, TMP102_I2C_TIMEOUT_MS) != HAL_OK)
    {
        return -1000.0f; // 错误值
    }

    // 读取两个字节的温度数据
    if (HAL_I2C_Master_Receive(&hi2c2, TMP102_ADDR, temp_raw, 2, TMP102_I2C_TIMEOUT_MS) != HAL_OK)
    {
        return -1000.0f; // 错误值
    }

    // 合并温度数据（12位左对齐）
    int16_t temp_combined = (temp_raw[0] << 4) | (temp_raw[1] >> 4);

    // 处理负温度（补码表示）
    if (temp_combined & 0x800)
    {
        temp_combined |= 0xF000; // 扩展符号位
    }

    tmp102Temp = temp_combined * 0.0625f; // 每个 LSB 为 0.0625°C

    Boardtemp_vals[0] = ( (uint8_t) tmp102Temp % 100)/10 +'0';
    Boardtemp_vals[1] = ( (uint8_t) tmp102Temp % 10)/1 +'0';
    BoardTempVal_elm_char16.dirty = 1;
    return tmp102Temp;
}
#include "sdm18.h"
#include "usart.h"
#include "zui.h"

const uint8_t start_meter_cmd[SDM18_CMD_LEN] = {0xa5, 0x03, 0x20, 0x01, 0x00, 0x00, 0x00, 0x02, 0x6e};
const uint8_t stop_meter_cmd[SDM18_CMD_LEN] = {0xa5, 0x03, 0x20, 0x02, 0x00, 0x00, 0x00, 0x46, 0x6e};
uint8_t sdm18_receive_data[SDM18_DATA_LEN+2] = {0x00};
float distance = 0;

void start_meter()
{
    HAL_UART_Transmit(&SDM18_USART, stop_meter_cmd, SDM18_CMD_LEN, 0xffff);
    // HAL_Delay(100);

		HAL_UART_Receive(&SDM18_USART, sdm18_receive_data, 60, 0xffff);
    HAL_UART_Transmit(&SDM18_USART, start_meter_cmd, SDM18_CMD_LEN, 0xfff);
    HAL_UART_Receive_IT(&SDM18_USART, sdm18_receive_data, SDM18_DATA_LEN);
    // HAL_UART_Receive(&SDM18_USART, sdm18_receive_data, SDM18_DATA_LEN,0xffffff);
    // HAL_Delay(2);
}


void parse_meter(void)
{
    uint16_t temp;
    extern char distanceval_vals[6];
    extern UI_Element distanceval_elm_char16;

    HAL_UART_Receive_IT(&SDM18_USART, sdm18_receive_data, SDM18_DATA_LEN);
    temp = (sdm18_receive_data[14]<<8) + sdm18_receive_data[13];
    distance = (float) temp / 1000.0;
    distanceval_vals[0] = temp/10000 + '0';
    temp %= 10000;
    distanceval_vals[1] = (temp - temp/10000)/1000 + '0';
    temp %= 1000;
    distanceval_vals[3] = (temp - temp/1000)/100 + '0';
    temp %= 100;
    distanceval_vals[4] = (temp - temp/100)/10 + '0';
    temp %= 10;
    distanceval_elm_char16.dirty = 1;
}

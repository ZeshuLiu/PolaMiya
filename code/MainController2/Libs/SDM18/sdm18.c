#include "sdm18.h"
#include "usart.h"
#include "zui.h"

const uint8_t start_meter_cmd[SDM18_CMD_LEN] = {0xa5, 0x03, 0x20, 0x01, 0x00, 0x00, 0x00, 0x02, 0x6e};
const uint8_t stop_meter_cmd[SDM18_CMD_LEN] = {0xa5, 0x03, 0x20, 0x02, 0x00, 0x00, 0x00, 0x46, 0x6e};
uint8_t sdm18_receive_data[SDM18_DATA_LEN+2] = {0x00};
float distance = 0;
_Bool single_meter = 0;
volatile HAL_StatusTypeDef sdm_tmp;

void sdm18_start_meter()
{
  sdm18_stop_meter();

  HAL_UART_Transmit(&SDM18_USART, start_meter_cmd, SDM18_CMD_LEN, 0xfff);
  // HAL_UART_Receive_IT(&SDM18_USART, sdm18_receive_data, SDM18_DATA_LEN);
  HAL_UARTEx_ReceiveToIdle_IT(&SDM18_USART, sdm18_receive_data, SDM18_DATA_LEN);
}

void sdm18_stop_meter()
{
  uint16_t dalen;

  sdm_tmp = HAL_UART_Abort(&SDM18_USART);

  sdm_tmp = HAL_UART_Transmit(&SDM18_USART, stop_meter_cmd, SDM18_CMD_LEN, 0xff);
  sdm_tmp = HAL_UARTEx_ReceiveToIdle(&SDM18_USART, sdm18_receive_data, 10, &dalen, 0xff);
}

void sdm18_single_meter()
{
  // volatile HAL_StatusTypeDef sdm_tmp;
  uint16_t dalen;
  single_meter = 1;
  sdm18_stop_meter();

  sdm_tmp = HAL_UART_Transmit(&SDM18_USART, start_meter_cmd, SDM18_CMD_LEN, 0xfff);
  HAL_NVIC_EnableIRQ(USART2_IRQn);
  // HAL_UART_Receive_IT(&SDM18_USART, sdm18_receive_data, SDM18_DATA_LEN);
  sdm_tmp = HAL_UARTEx_ReceiveToIdle_IT(&SDM18_USART, sdm18_receive_data, SDM18_DATA_LEN);
  // sdm_tmp = HAL_UARTEx_ReceiveToIdle(&SDM18_USART, sdm18_receive_data, SDM18_DATA_LEN, &dalen, 0xff);
  // sdm18_stop_meter();
}

void parse_meter(void)
{
    uint16_t temp;
    extern char distanceval_vals[6];
    extern UI_Element distanceval_elm_char16;

    // HAL_UART_Receive_IT(&SDM18_USART, sdm18_receive_data, SDM18_DATA_LEN);
    if (sdm18_receive_data[0] != 0xA5 || sdm18_receive_data[1] != 0x03 ){
      return;
    }
    
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

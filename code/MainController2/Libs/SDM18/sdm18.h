#ifndef __SDM18_H__
#define __SDM18_H__

#define SDM18_CMD_LEN       9
#define SDM18_DATA_LEN      23
#define SDM18_USART         huart2

void sdm18_start_meter();
void sdm18_stop_meter();
void sdm18_single_meter();
void parse_meter(void);

#endif
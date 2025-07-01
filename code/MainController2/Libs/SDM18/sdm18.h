#ifndef __SDM18_H__
#define __SDM18_H__

#define SDM18_CMD_LEN       9
#define SDM18_DATA_LEN      23
#define SDM18_USART         huart2

void start_meter();
void parse_meter(void);

#endif
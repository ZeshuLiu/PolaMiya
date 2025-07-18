#include "motor.h"
#include "DataStore.h"

uint8_t motor_state = 0;
uint8_t motor_start_cnt = 0;
static uint32_t motor_count;

void motor_init(void)
{
    HAL_GPIO_WritePin(EJCMT_EN_GPIO_Port, EJCMT_EN_Pin, GPIO_PIN_RESET);
    motor_state = 0;
}

void motor_start(void)
{
    motor_start_cnt = 0;
    HAL_GPIO_WritePin(EJCMT_EN_GPIO_Port, EJCMT_EN_Pin, GPIO_PIN_SET);
    motor_state = 1;
    HAL_NVIC_EnableIRQ(EXTI0_IRQn);
    motor_count = HAL_GetTick();
}

void motor_end(void)
{
    HAL_GPIO_WritePin(EJCMT_EN_GPIO_Port, EJCMT_EN_Pin, GPIO_PIN_RESET);
    motor_state = 0;
    motor_count = HAL_GetTick() - motor_count;
    data_set_mtl(motor_count);
}
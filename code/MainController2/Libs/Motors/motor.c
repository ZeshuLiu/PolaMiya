#include "motor.h"
#include "DataStore.h"
#include "tim.h"
#include "main.h"
#include "sdm18.h"

uint8_t motor_state = 0;
uint8_t motor_start_cnt = 0;
static uint32_t motor_count;
uint16_t motor_pwm_width = 0;

void motor_init(void)
{
    HAL_GPIO_WritePin(EJCMT_EN_GPIO_Port, EJCMT_EN_Pin, GPIO_PIN_RESET);
    motor_state = 0;
    motor_pwm_width = 0;
}

void motor_start(void)
{
    sdm18_stop_meter();

    motor_start_cnt = 0;
    motor_pwm_width = 000;
    __HAL_TIM_SET_COMPARE(&htim16,TIM_CHANNEL_1, motor_pwm_width);    // 0-999
    HAL_TIM_Base_Start_IT(&htim16);
    HAL_TIM_PWM_Start_IT(&htim16,TIM_CHANNEL_1);
    // HAL_GPIO_WritePin(EJCMT_EN_GPIO_Port, EJCMT_EN_Pin, GPIO_PIN_SET);
    motor_state = 1;
    HAL_NVIC_EnableIRQ(EXTI0_IRQn);
    motor_count = HAL_GetTick();
}

void motor_end(void)
{
    HAL_TIM_PWM_Stop_IT(&htim16,TIM_CHANNEL_1);
    HAL_TIM_Base_Stop_IT(&htim16);
    motor_pwm_width = 0;
    __HAL_TIM_SET_COMPARE(&htim16,TIM_CHANNEL_1, motor_pwm_width);    // 0-999
    HAL_GPIO_WritePin(EJCMT_EN_GPIO_Port, EJCMT_EN_Pin, GPIO_PIN_RESET);
    motor_state = 0;
    motor_count = HAL_GetTick() - motor_count;
    data_set_mtl(motor_count);
}
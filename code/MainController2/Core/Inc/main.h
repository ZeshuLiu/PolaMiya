/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.h
  * @brief          : Header for main.c file.
  *                   This file contains the common defines of the application.
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2025 STMicroelectronics.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the LICENSE file
  * in the root directory of this software component.
  * If no LICENSE file comes with this software, it is provided AS-IS.
  *
  ******************************************************************************
  */
/* USER CODE END Header */

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __MAIN_H
#define __MAIN_H

#ifdef __cplusplus
extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "stm32g4xx_hal.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */

/* USER CODE END Includes */

/* Exported types ------------------------------------------------------------*/
/* USER CODE BEGIN ET */

/* USER CODE END ET */

/* Exported constants --------------------------------------------------------*/
/* USER CODE BEGIN EC */
// #define TIM_INTCNT_10MS       1                     // 10ms中断
#define TIM_INT10MS_MASK      (0x02)
#define TIM_INTCNT_200MS       (20)                     // 200ms中断
#define TIM_INT200MS_MASK      (0x04)
#define TIM_INTCNT_1S         (100)               // 1s中断
#define TIM_INT1S_MASK        0x01
#define TIM_INTCNT_300MS         (30)               // 300ms中断
#define TIM_INT300MS_MASK        (0x08)

#define TIM_INTCNT_MAX TIM_INTCNT_1S                // 1s

#define KEY_DEBOUNCE_TIME 10
#define KEY_LONG_PRESS_TIME 1500
/* USER CODE END EC */

/* Exported macro ------------------------------------------------------------*/
/* USER CODE BEGIN EM */

/* USER CODE END EM */

/* Exported functions prototypes ---------------------------------------------*/
void Error_Handler(void);

/* USER CODE BEGIN EFP */

/* USER CODE END EFP */

/* Private defines -----------------------------------------------------------*/
#define PWR_SENS_Pin GPIO_PIN_3
#define PWR_SENS_GPIO_Port GPIOC
#define KEY0_Pin GPIO_PIN_0
#define KEY0_GPIO_Port GPIOA
#define KEY0_EXTI_IRQn EXTI0_IRQn
#define KEY1_Pin GPIO_PIN_1
#define KEY1_GPIO_Port GPIOA
#define KEY1_EXTI_IRQn EXTI1_IRQn
#define KEY2_Pin GPIO_PIN_4
#define KEY2_GPIO_Port GPIOA
#define KEY2_EXTI_IRQn EXTI4_IRQn
#define SHUT_Trig_Pin GPIO_PIN_5
#define SHUT_Trig_GPIO_Port GPIOA
#define SHUT_Trig_EXTI_IRQn EXTI9_5_IRQn
#define EJCMT_EN_Pin GPIO_PIN_6
#define EJCMT_EN_GPIO_Port GPIOA
#define KEY3_Pin GPIO_PIN_7
#define KEY3_GPIO_Port GPIOA
#define PWR_EN2_Pin GPIO_PIN_4
#define PWR_EN2_GPIO_Port GPIOC
#define P2C0_Pin GPIO_PIN_12
#define P2C0_GPIO_Port GPIOB
#define P2C0_EXTI_IRQn EXTI15_10_IRQn
#define P2C1_Pin GPIO_PIN_13
#define P2C1_GPIO_Port GPIOB
#define P2C1_EXTI_IRQn EXTI15_10_IRQn
#define C2P0_Pin GPIO_PIN_14
#define C2P0_GPIO_Port GPIOB
#define C2P1_Pin GPIO_PIN_15
#define C2P1_GPIO_Port GPIOB
#define FLASH_Trig_Pin GPIO_PIN_15
#define FLASH_Trig_GPIO_Port GPIOA
#define PD2_Pin GPIO_PIN_2
#define PD2_GPIO_Port GPIOD
#define D_SCL_Pin GPIO_PIN_3
#define D_SCL_GPIO_Port GPIOB
#define D_SDA_Pin GPIO_PIN_5
#define D_SDA_GPIO_Port GPIOB
#define D_RES_Pin GPIO_PIN_6
#define D_RES_GPIO_Port GPIOB
#define D_DC_Pin GPIO_PIN_7
#define D_DC_GPIO_Port GPIOB
#define D_CS_Pin GPIO_PIN_9
#define D_CS_GPIO_Port GPIOB

/* USER CODE BEGIN Private defines */
#define MY_DISP_HOR_RES    280
#define MY_DISP_VER_RES    240
#define USR_KEY_COUNT      3
/* USER CODE END Private defines */

#ifdef __cplusplus
}
#endif

#endif /* __MAIN_H */

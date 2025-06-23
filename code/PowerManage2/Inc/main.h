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
#include "stm32l0xx_hal.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */

/* USER CODE END Includes */

/* Exported types ------------------------------------------------------------*/
/* USER CODE BEGIN ET */

/* USER CODE END ET */

/* Exported constants --------------------------------------------------------*/
/* USER CODE BEGIN EC */
#define PWR_UP_DELAY      200U      // Start up delay in ms
#define PRESS_HOLD_TIME   1000U     // delay after usr pin pressed in ms
#define LED_PWRUP_DUR     200U      // led light up duration after pwr up
#define LED_USR_DUR       2000U     // led light up duration after usr pin valid
/* USER CODE END EC */

/* Exported macro ------------------------------------------------------------*/
/* USER CODE BEGIN EM */

/* USER CODE END EM */

/* Exported functions prototypes ---------------------------------------------*/
void Error_Handler(void);

/* USER CODE BEGIN EFP */

/* USER CODE END EFP */

/* Private defines -----------------------------------------------------------*/
#define PWR_LED_Pin GPIO_PIN_14
#define PWR_LED_GPIO_Port GPIOC
#define PWR_EN_Pin GPIO_PIN_15
#define PWR_EN_GPIO_Port GPIOC
#define WKUP_Pin GPIO_PIN_0
#define WKUP_GPIO_Port GPIOA
#define P2C0_Pin GPIO_PIN_1
#define P2C0_GPIO_Port GPIOA
#define P2C1_Pin GPIO_PIN_4
#define P2C1_GPIO_Port GPIOA
#define PWR_KEY_Pin GPIO_PIN_7
#define PWR_KEY_GPIO_Port GPIOA
#define C2P0_Pin GPIO_PIN_9
#define C2P0_GPIO_Port GPIOA
#define C2P1_Pin GPIO_PIN_10
#define C2P1_GPIO_Port GPIOA

/* USER CODE BEGIN Private defines */

/* USER CODE END Private defines */

#ifdef __cplusplus
}
#endif

#endif /* __MAIN_H */

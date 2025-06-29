/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.c
  * @brief          : Main program body
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
/* Includes ------------------------------------------------------------------*/
#include "main.h"
#include "adc.h"
#include "dma.h"
#include "i2c.h"
#include "usart.h"
#include "rtc.h"
#include "spi.h"
#include "tim.h"
#include "usb_device.h"
#include "gpio.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include "lcd_usr.h"
#include "zui.h"
#include "zui_usr.h"
#include "tmp102.h"
/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */

/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/

/* USER CODE BEGIN PV */
uint8_t tim2_int_mask = 0;
extern double BAT_V, temperature_cd;
extern UI_Element zui_elm_char12;
extern UI_Element bat_elm_char16;
extern UI_Element batnum_elm_char16;
/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
/* USER CODE BEGIN PFP */

/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */

/* USER CODE END 0 */

/**
  * @brief  The application entry point.
  * @retval int
  */
int main(void)
{

  /* USER CODE BEGIN 1 */
  uint8_t a = 0;
	uint16_t * b = 0;
  /* USER CODE END 1 */

  /* MCU Configuration--------------------------------------------------------*/

  /* Reset of all peripherals, Initializes the Flash interface and the Systick. */
  HAL_Init();

  /* USER CODE BEGIN Init */

  /* USER CODE END Init */

  /* Configure the system clock */
  SystemClock_Config();

  /* USER CODE BEGIN SysInit */

  /* USER CODE END SysInit */

  /* Initialize all configured peripherals */
  MX_GPIO_Init();
  MX_DMA_Init();
  MX_ADC1_Init();
  MX_I2C2_Init();
  MX_SPI1_Init();
  MX_USART2_UART_Init();
  MX_USART3_UART_Init();
  MX_I2C3_Init();
  MX_RTC_Init();
  MX_SPI3_Init();
  MX_LPUART1_UART_Init();
  MX_TIM2_Init();
  MX_USB_Device_Init();
  MX_TIM1_Init();
  /* USER CODE BEGIN 2 */

  // 开始启动
  HAL_GPIO_WritePin(KEY3_GPIO_Port, KEY3_Pin, GPIO_PIN_RESET);
  HAL_Delay(100);
  HAL_GPIO_WritePin(KEY3_GPIO_Port, KEY3_Pin, GPIO_PIN_SET);

  // ADC 初始化
	HAL_ADCEx_Calibration_Start(&hadc1, ADC_SINGLE_ENDED);    //AD校准


  // 屏幕显示初始化
  HAL_TIM_Base_Start_IT(&htim2);
  zui_init();

  // float t = TMP102_ReadTemperature();

  // LCD_Fill(0,0,LCD_W,LCD_H,WHITE);
  // LCD_DrawArea(0, 0, LCD_W, LCD_H,WHITE);
  LCD_Fill(0,0,LCD_W,LCD_H,WHITE);
  // zui_elm_set_dirty(&batnum_elm_char16);
  // zui_elm_set_dirty(&bat_elm_char16);
  // zui_elm_set_dirty(&zui_elm_char12);
  zui_dirty_current_layer();
  zui_render_current_layer();
  
  HAL_Delay(100);
	HAL_GPIO_WritePin(KEY3_GPIO_Port, KEY3_Pin, GPIO_PIN_RESET);
	
  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  while (1)
  {

    if ( (tim2_int_mask & TIM_INT1S_MASK) != 0){
      tim2_int_mask &= (~TIM_INT1S_MASK);
      HAL_GPIO_TogglePin(KEY3_GPIO_Port, KEY3_Pin);
      Z_ADC_DMA_Start(&hadc1);
      TMP102_ReadTemperature();
      // HAL_GPIO_TogglePin(GPIOD, GPIO_PIN_2);
    }

    if ( (tim2_int_mask & TIM_INT10MS_MASK) != 0){
      tim2_int_mask &= (~TIM_INT10MS_MASK);
      // DRAW_FRAME(0);
      // DRAW_DATA(0);
    }

    if ( (tim2_int_mask & TIM_INT200MS_MASK) != 0) {
      tim2_int_mask &= (~TIM_INT200MS_MASK);
			// zui_elm_set_dirty(&pure_element);
      zui_render_current_layer();
      // DRAW_FRAME(0);
      // DRAW_DATA(0);
    }


    if (a == 1){
      HAL_GPIO_WritePin(EJCMT_EN_GPIO_Port, EJCMT_EN_Pin, GPIO_PIN_SET);
    }
    else{
      HAL_GPIO_WritePin(EJCMT_EN_GPIO_Port, EJCMT_EN_Pin, GPIO_PIN_RESET);
    }
    /* USER CODE END WHILE */

    /* USER CODE BEGIN 3 */
  }
  /* USER CODE END 3 */
}

/**
  * @brief System Clock Configuration
  * @retval None
  */
void SystemClock_Config(void)
{
  RCC_OscInitTypeDef RCC_OscInitStruct = {0};
  RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};

  /** Configure the main internal regulator output voltage
  */
  HAL_PWREx_ControlVoltageScaling(PWR_REGULATOR_VOLTAGE_SCALE1);

  /** Configure LSE Drive Capability
  */
  HAL_PWR_EnableBkUpAccess();
  __HAL_RCC_LSEDRIVE_CONFIG(RCC_LSEDRIVE_LOW);

  /** Initializes the RCC Oscillators according to the specified parameters
  * in the RCC_OscInitTypeDef structure.
  */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSI48|RCC_OSCILLATORTYPE_HSE
                              |RCC_OSCILLATORTYPE_LSE;
  RCC_OscInitStruct.HSEState = RCC_HSE_ON;
  RCC_OscInitStruct.LSEState = RCC_LSE_ON;
  RCC_OscInitStruct.HSI48State = RCC_HSI48_ON;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSE;
  RCC_OscInitStruct.PLL.PLLM = RCC_PLLM_DIV2;
  RCC_OscInitStruct.PLL.PLLN = 24;
  RCC_OscInitStruct.PLL.PLLP = RCC_PLLP_DIV2;
  RCC_OscInitStruct.PLL.PLLQ = RCC_PLLQ_DIV6;
  RCC_OscInitStruct.PLL.PLLR = RCC_PLLR_DIV2;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    Error_Handler();
  }

  /** Initializes the CPU, AHB and APB buses clocks
  */
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                              |RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV1;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_4) != HAL_OK)
  {
    Error_Handler();
  }
}

/* USER CODE BEGIN 4 */

/* USER CODE END 4 */

/**
  * @brief  This function is executed in case of error occurrence.
  * @retval None
  */
void Error_Handler(void)
{
  /* USER CODE BEGIN Error_Handler_Debug */
  /* User can add his own implementation to report the HAL error return state */
  __disable_irq();
  while (1)
  {
  }
  /* USER CODE END Error_Handler_Debug */
}

#ifdef  USE_FULL_ASSERT
/**
  * @brief  Reports the name of the source file and the source line number
  *         where the assert_param error has occurred.
  * @param  file: pointer to the source file name
  * @param  line: assert_param error line source number
  * @retval None
  */
void assert_failed(uint8_t *file, uint32_t line)
{
  /* USER CODE BEGIN 6 */
  /* User can add his own implementation to report the file name and line number,
     ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */
  /* USER CODE END 6 */
}
#endif /* USE_FULL_ASSERT */

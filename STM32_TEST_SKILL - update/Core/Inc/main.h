/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.h
  * @brief          : Header for main.c file.
  *                   This file contains the common defines of the application.
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2023 STMicroelectronics.
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
#include "stm32f4xx_hal.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */

/* USER CODE END Includes */

/* Exported types ------------------------------------------------------------*/
/* USER CODE BEGIN ET */

/* USER CODE END ET */

/* Exported constants --------------------------------------------------------*/
/* USER CODE BEGIN EC */

/* USER CODE END EC */

/* Exported macro ------------------------------------------------------------*/
/* USER CODE BEGIN EM */

/* USER CODE END EM */

/* Exported functions prototypes ---------------------------------------------*/
void Error_Handler(void);

/* USER CODE BEGIN EFP */

/* USER CODE END EFP */

/* Private defines -----------------------------------------------------------*/
#define RIGHT_Pin GPIO_PIN_3
#define RIGHT_GPIO_Port GPIOE
#define LEFT_Pin GPIO_PIN_4
#define LEFT_GPIO_Port GPIOE
#define MODE_Pin GPIO_PIN_0
#define MODE_GPIO_Port GPIOA
#define SCLK_Pin GPIO_PIN_1
#define SCLK_GPIO_Port GPIOA
#define LATCH_Pin GPIO_PIN_4
#define LATCH_GPIO_Port GPIOA
#define DIN_Pin GPIO_PIN_6
#define DIN_GPIO_Port GPIOA
#define LED_D3_Pin GPIO_PIN_7
#define LED_D3_GPIO_Port GPIOA
#define ROW0_Pin GPIO_PIN_0
#define ROW0_GPIO_Port GPIOD
#define COL0_Pin GPIO_PIN_1
#define COL0_GPIO_Port GPIOD
#define ROW1_Pin GPIO_PIN_2
#define ROW1_GPIO_Port GPIOD
#define COL1_Pin GPIO_PIN_3
#define COL1_GPIO_Port GPIOD
#define ROW2_Pin GPIO_PIN_4
#define ROW2_GPIO_Port GPIOD
#define COL2_Pin GPIO_PIN_5
#define COL2_GPIO_Port GPIOD
#define ROW3_Pin GPIO_PIN_6
#define ROW3_GPIO_Port GPIOD
#define COL3_Pin GPIO_PIN_7
#define COL3_GPIO_Port GPIOD

/* USER CODE BEGIN Private defines */

/* USER CODE END Private defines */

#ifdef __cplusplus
}
#endif

#endif /* __MAIN_H */
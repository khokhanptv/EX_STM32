/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.c
  * @brief          : Main program body
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
  **************************************************************************y7
/* USER CODE END Header */
/* Includes ------------------------------------------------------------------*/
#include "main.h"
#include "iwdg.h"
#include "tim.h"
#include "wwdg.h"
#include "gpio.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include "stdio.h"
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
		int st0,st1;
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

  /* USER CODE END 1 */

  /* MCU Configuration--------------------------------------------------------*/

  /* Reset of all peripherals, Initializes the Flash interface and the Systick. */
  HAL_Init();

  /* USER CODE BEGIN Init */
//		st0 = HAL_GPIO_ReadPin(Key0_GPIO_Port,Key0_Pin);
//		st1 = HAL_GPIO_ReadPin(Key1_GPIO_Port,Key1_Pin);
  /* USER CODE END Init */

  /* Configure the system clock */
  SystemClock_Config();

  /* USER CODE BEGIN SysInit */

  /* USER CODE END SysInit */

  /* Initialize all configured peripherals */
  MX_GPIO_Init();
  MX_IWDG_Init();
  MX_TIM1_Init();
  MX_WWDG_Init();
  /* USER CODE BEGIN 2 */
		// HAL_TIM_PWM_Start (&htim1,TIM_CHANNEL_1);//khoi tao timer1
  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
			//__HAL_TIM_SET_COMPARE(&htim1,TIM_CHANNEL_1,50);	
		//	int var =100;
  while (1)
  {
    /* USER CODE END WHILE */

    /* USER CODE BEGIN 3 */
	
			st0 = HAL_GPIO_ReadPin(Key0_GPIO_Port,Key0_Pin);
			st1 = HAL_GPIO_ReadPin(Key1_GPIO_Port,Key1_Pin);
		printf("gia tri key0 la: %d \n", st0  );
		
//code nut nhan Key_0
			if(st0==0)
			{
				
				HAL_Delay(20);
				if(st0==0)
					{
//								HAL_Delay(50);
//								var=var+100;
//								if (var >1000)
//									{
//										var=100;
//									}
//								__HAL_TIM_SET_COMPARE(&htim1,TIM_CHANNEL_1,var);
								HAL_GPIO_WritePin(Led_D2_GPIO_Port,Led_D2_Pin,GPIO_PIN_RESET);
								HAL_GPIO_WritePin(Led_D3_GPIO_Port,Led_D3_Pin,GPIO_PIN_SET);
								HAL_Delay(500); // 
								HAL_GPIO_WritePin(Led_D2_GPIO_Port,Led_D2_Pin,GPIO_PIN_SET);
								HAL_GPIO_WritePin(Led_D3_GPIO_Port,Led_D3_Pin,GPIO_PIN_RESET);
								HAL_Delay(500);					
					}			
					
			}
			
		 
//code nut nhan Key_1
			if(st1==0)
			{
				HAL_Delay(20);
				if(st1==0)
					{
//						HAL_Delay (20);
//						var=1000-var;
//						if(var==0)
//								{
//									var=1000;
//								}
//						__HAL_TIM_SET_COMPARE(&htim1,TIM_CHANNEL_1,var);			
						for(int i=0;i<=4;i++)
							{
								HAL_GPIO_WritePin(Led_D2_GPIO_Port,Led_D2_Pin,GPIO_PIN_RESET);
								HAL_Delay(200);
								HAL_GPIO_WritePin(Led_D2_GPIO_Port,Led_D2_Pin,GPIO_PIN_SET);
								HAL_Delay(200);
							}
						for(int a=0;a<=4;a++)
							{
								HAL_GPIO_WritePin(Led_D3_GPIO_Port,Led_D3_Pin,GPIO_PIN_RESET);
								HAL_Delay(200);
								HAL_GPIO_WritePin(Led_D3_GPIO_Port,Led_D3_Pin,GPIO_PIN_SET);
								HAL_Delay(200);
							}
					}
			}		
		
			
	
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
  __HAL_RCC_PWR_CLK_ENABLE();
  __HAL_PWR_VOLTAGESCALING_CONFIG(PWR_REGULATOR_VOLTAGE_SCALE1);

  /** Initializes the RCC Oscillators according to the specified parameters
  * in the RCC_OscInitTypeDef structure.
  */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_LSI|RCC_OSCILLATORTYPE_HSE;
  RCC_OscInitStruct.HSEState = RCC_HSE_ON;
  RCC_OscInitStruct.LSIState = RCC_LSI_ON;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSE;
  RCC_OscInitStruct.PLL.PLLM = 4;
  RCC_OscInitStruct.PLL.PLLN = 168;
  RCC_OscInitStruct.PLL.PLLP = RCC_PLLP_DIV2;
  RCC_OscInitStruct.PLL.PLLQ = 4;
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
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV4;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV2;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_5) != HAL_OK)
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

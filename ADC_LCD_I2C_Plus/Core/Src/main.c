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
  ******************************************************************************
  */
/* USER CODE END Header */
/* Includes ------------------------------------------------------------------*/
#include "main.h"
#include "adc.h"
#include "i2c.h"
#include "gpio.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include <stdio.h>
#include "i2c-lcd.h"
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
uint32_t value_adc1 = 0;// khoi tao tung bien , gan vao chanel
uint32_t value_adc2 = 0;
uint32_t value_adc3 = 0;
int Vol,Temp =0 ;	
int dem=0;
int rank1=0;
int number;
int number1;
int thousands;
int hundreds ;
int tens ;
int units;
char so_xxxx[4];
 
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

  /* USER CODE END Init */

  /* Configure the system clock */
  SystemClock_Config();

  /* USER CODE BEGIN SysInit */

  /* USER CODE END SysInit */

  /* Initialize all configured peripherals */
  MX_GPIO_Init();
  MX_ADC1_Init();
  MX_I2C1_Init();
  /* USER CODE BEGIN 2 */
	 HAL_Delay(1000);
	 lcd_init ();

  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  while (1)
	{
    /* USER CODE END WHILE */

    /* USER CODE BEGIN 3 */
		unsigned short data_adc[16]; // regular 16 yc chuyen doi
		HAL_ADC_Start(&hadc1);// DUNG regular, DA SET UP 3 KENH , CHANEL 0>3
		HAL_ADC_PollForConversion(&hadc1,HAL_MAX_DELAY);
		for(int i =0; i< 2;i++)
		{
			dem ++;
			value_adc1 = HAL_ADC_GetValue(&hadc1);
			data_adc[i] = value_adc1;
			HAL_Delay(100);
		}
		HAL_ADC_Stop(&hadc1);// ket thuc regular
		
		HAL_ADCEx_InjectedStart(&hadc1);// DANG DUNG Injected, 1 KENH CHANEL 0
		rank1 = HAL_ADCEx_InjectedGetValue(&hadc1,ADC_INJECTED_RANK_1); 
		
		// phia duoi la in gia tri
		printf("Gia tri dem:  %d \n",dem);// dem
		printf("Gia tri VOL: %d \n",rank1); 	// gia tri rank1 cua injected
		printf("Gia tri vol:  %d \n",data_adc[0]); // gia tri chanel 0 cua regular
		printf("Gia tri nhiet do: %d \n",data_adc[1]);// gia tri chanel 1cua regular
		printf("Gia tri VREFINT: %d \n",data_adc[2]);// gia tri chanel 2cua regular
		printf ("--------------\n");
		HAL_Delay(100);
		 
		// HIEN THI  LEN LCD
		Temp = (int)data_adc[1];
		Vol = (int) rank1;
		
	// HIEN THI binh thuong  bien tro LEN LCD
		char buffer[20];
		number = Vol;
		lcd_put_cur(0,0);
		sprintf (buffer,"D:%d ", Vol); 
		lcd_send_string (buffer);
		HAL_Delay(500);
		
		//4 so ,co so 0, bien tro LEN LCD
		lcd_put_cur(0,7);
		sprintf (buffer,"DA:%d ", number);
		thousands = number / 1000;
		hundreds = (number % 1000) / 100;
		tens = (number % 100) / 10;
		units = number % 10;
		lcd_send_data(thousands + '0');
		lcd_send_data(hundreds + '0');
		lcd_send_data(tens + '0');
		lcd_send_data(units + '0');
		HAL_Delay(500);
		
		//xoa so 0, bien tro LEN LCD
		lcd_put_cur(0,12);
		sprintf (buffer,"DA:%d ", number);
		thousands = number / 1000;
		hundreds = (number % 1000) / 100;
	  tens = (number % 100) / 10;
	  units = number % 10;
		so_xxxx[0] = thousands + '0';
		so_xxxx[1] = hundreds + '0';
		so_xxxx[2] = tens + '0';
		so_xxxx[3] = units + '0';
		if (thousands == 0)
		{
			so_xxxx[0] = 32;
			if (hundreds == 0)
			{
				so_xxxx[1] = 32;
				if (tens == 0)
				{
					so_xxxx[2] = 32;
				}	
			}
		}
		lcd_send_data(so_xxxx[0]);
		lcd_send_data(so_xxxx[1]);
		lcd_send_data(so_xxxx[2]);
		lcd_send_data(so_xxxx[3]);
		HAL_Delay(500);
		
		
		
		
		 // HIEN THI BINH THUONG NHIET DO LEN LCD
		number1 = Temp;
		sprintf (buffer,"T:%d", Temp);
		lcd_put_cur(1,0);
		lcd_send_string (buffer);
		HAL_Delay(500);
		
		//4 so ,co so 0, CUA NHIET DO LEN LCD
		lcd_put_cur(1,6);
		sprintf (buffer,"DA:%d ", number1);
		thousands = number1 / 1000;
		hundreds = (number1 % 1000) / 100;
		tens = (number1 % 100) / 10;
		units = number1 % 10;
		lcd_send_data(thousands + '0');
		lcd_send_data(hundreds + '0');
		lcd_send_data(tens + '0');
		lcd_send_data(units + '0');
		HAL_Delay(500);
		
		//4 so ,XOA SO 0 CUA NHIET DO LEN LCD
		lcd_put_cur(1,10);
		sprintf (buffer,"DA:%d ", number);
		thousands = number1 / 1000;
		hundreds = (number1 % 1000) / 100;
	  tens = (number1 % 100) / 10;
	  units = number1 % 10;
		so_xxxx[0] = thousands + '0';
		so_xxxx[1] = hundreds + '0';
		so_xxxx[2] = tens + '0';
		so_xxxx[3] = units + '0';
		if (thousands == 0)
		{
			so_xxxx[0] = 32;
			if (hundreds == 0)
			{
				so_xxxx[1] = 32;
				if (tens == 0)
				{
					so_xxxx[2] = 32;
				}	
			}
		}
		lcd_send_data(so_xxxx[0]);
		lcd_send_data(so_xxxx[1]);
		lcd_send_data(so_xxxx[2]);
		lcd_send_data(so_xxxx[3]);
		HAL_Delay(500);
	
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
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSI;
  RCC_OscInitStruct.HSIState = RCC_HSI_ON;
  RCC_OscInitStruct.HSICalibrationValue = RCC_HSICALIBRATION_DEFAULT;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSI;
  RCC_OscInitStruct.PLL.PLLM = 8;
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

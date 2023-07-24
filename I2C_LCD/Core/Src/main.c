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
//char cc1[] = {0x00, 0x00, 0x0A, 0x00, 0x11, 0x0E, 0x00, 0x00};  // smiley
//char cc2[] = {0x0E, 0x0E, 0x04, 0x0E, 0x15, 0x04, 0x0A, 0x0A};  // nguoi
//char cc3[] = {0x08, 0x0C, 0x0E, 0x0F, 0x0E, 0x0C, 0x08, 0x00};  // arrow
//char cc4[] = {0x10, 0x04, 0x0E, 0x0E, 0x0E, 0x1F, 0x04, 0x00};  // bell
//char cc5[] = {0x00, 0x00, 0x0A, 0x15, 0x11, 0x0E, 0x04, 0x00};  // Heart
//char cc6[] = {0x00, 0x0E, 0x11, 0x11, 0x11, 0x0A, 0x1B, 0x00};  // omega
//char cc7[] = {0x0E, 0x10, 0x17, 0x12, 0x12, 0x12, 0x10, 0x0E};  // CT
//char cc8[] = {0x04, 0x04, 0x1F, 0x04, 0x04, 0x00, 0x1F, 0x00};  // +-
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
  MX_I2C1_Init();
  /* USER CODE BEGIN 2 */
	lcd_init ();
//	lcd_send_cmd(0x40);
//  for (int i=0; i<8; i++) lcd_send_data(cc1[i]);

//  lcd_send_cmd(0x40+8);
//  for (int i=0; i<8; i++) lcd_send_data(cc2[i]);

//  lcd_send_cmd(0x40+16);
//  for (int i=0; i<8; i++) lcd_send_data(cc3[i]);

//  lcd_send_cmd(0x40+24);
//  for (int i=0; i<8; i++) lcd_send_data(cc4[i]);

//  lcd_send_cmd(0x40+32);
//  for (int i=0; i<8; i++) lcd_send_data(cc5[i]);

//  lcd_send_cmd(0x40+40);
//  for (int i=0; i<8; i++) lcd_send_data(cc6[i]);

//  lcd_send_cmd(0x40+48);
//  for (int i=0; i<8; i++) lcd_send_data(cc7[i]);

//  lcd_send_cmd(0x40+56);
//  for (int i=0; i<8; i++) lcd_send_data(cc8[i]);
	
	lcd_send_cmd (0X50);
	 
	lcd_send_data(0X10);
	lcd_send_data(0X08);
	lcd_send_data(0X04);
	lcd_send_data(0X02);
	lcd_send_data(0X01);
	lcd_send_data(0X02);
	lcd_send_data(0X04);
	lcd_send_data(0X08);
	
	lcd_put_cur(1, 0);
	// SO 3 NGUOC
	lcd_send_cmd (0x78);
	
	lcd_send_data(0X0F);
	lcd_send_data(0X10);
	lcd_send_data(0X10);
	lcd_send_data(0X1F);
	lcd_send_data(0X10);
	lcd_send_data(0X10);
	lcd_send_data(0X10);
	lcd_send_data(0X0F);
	
	lcd_put_cur(1, 0);
  lcd_send_data(7);  
	lcd_send_data(2); 
	
//	lcd_put_cur(1, 0);
//  lcd_send_data(0);  // at 0x40 (smiley)
//	lcd_put_cur(1, 2);
//  lcd_send_data(1);  // at 0x40+56 (nguoi)
//	lcd_put_cur(1, 4);
//  lcd_send_data(2);  // at 0x40+56 (mui ten)
//	lcd_put_cur(1, 6);
//  lcd_send_data(3);  // at 0x40+56  chuong
//	lcd_put_cur(1, 8);
//  lcd_send_data(4);  // at 0x40+56 (trai tim )
//	lcd_put_cur(1, 10);
//  lcd_send_data(5);  // at 0x40+56 (+-)
//	lcd_put_cur(1, 12);
//  lcd_send_data(6);  // at 0x40+56 (+-)
//	lcd_put_cur(1, 12);
//  lcd_send_data(7);  // at 0x40+56 (+-)
	HAL_Delay(100000);
	
	// code scan tim dia chi I2C( thiet bi ket noi), ney khong biet dia chi do la bao nhieu
//	int ret = 0;
//	for(int i=1; i<128; i++)
//	{
//			ret = HAL_I2C_IsDeviceReady(&hi2c1, (uint16_t)(i<<1), 3, 5);
//			if (ret != HAL_OK) /* No ACK Received At That Address */
//			{
//					printf("khong co tai dia chi 0x%X \n",i);
//			}
//			else if(ret == HAL_OK)
//			{
//					printf("dia chi 0x%X \n",i);
//					break;
//			}
//	}

		char string_lcd[20];
		int count =0;
		lcd_clear ();
		int status_button =0;
  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  while (1)
  {
    /* USER CODE END WHILE */

    /* USER CODE BEGIN 3 */
//			lcd_put_cur(0, 0); // VI TRI HANG0 COT 0
//			lcd_send_string ("ThaiTn");// gui ky tu
//			HAL_Delay(10);
//			lcd_put_cur(1, 3);        // code xoa hang chuc
//			lcd_send_string ("  ---");// code xoa hang chuc
//			lcd_put_cur(1, 0);
//			sprintf (string_lcd, "So: %d",count); // 
//			lcd_send_string (string_lcd); //  gui bien
//			HAL_Delay(100);
//			if (HAL_GPIO_ReadPin(Key_0_GPIO_Port,Key_0_Pin) ==0)	
//			{	
//				HAL_Delay(100);
//				if(HAL_GPIO_ReadPin(Key_0_GPIO_Port,Key_0_Pin) ==0)
//				{
//					count = count +1 ;
//					if(count ==30)
//					{
//						count =0;
//					}
//					
//				}
//				
//			}
//				
//			if (HAL_GPIO_ReadPin(Key_1_GPIO_Port,Key_1_Pin) ==0)	
//			{	
//				HAL_Delay(100);
//				if(HAL_GPIO_ReadPin(Key_1_GPIO_Port,Key_1_Pin) ==0)
//				{
//					count = count -1 ;
//					if (count <0)
//					{
//						count =0;
// 		 
//					}
//				}
//			}
			
//		lcd_clear();
//		lcd_put_cur(0,3);
//		lcd_send_string("TN1");
//		lcd_put_cur(0, 1);
//		lcd_send_data(2); 
//		lcd_put_cur(0, 14);
//    lcd_send_data(4);
//		lcd_put_cur(1,3);
//		lcd_send_string("I LOVE YOU ");
//		lcd_put_cur(1, 1);
//    lcd_send_data(4); 
//		lcd_put_cur(1, 14);
//    lcd_send_data(4); 
//			
//		HAL_Delay(20000);
//		lcd_clear();
		
	 
//		lcd_clear();
 
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
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSE;
  RCC_OscInitStruct.HSEState = RCC_HSE_ON;
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

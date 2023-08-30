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
#include "usart.h"
#include "gpio.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include "i2c-lcd.h"
#include <stdio.h>
/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */
#define SLAVE_ADDRESS_EEPROM 0x50  
void write_eeprom (int address ,int value)
{
	uint8_t data_t[2];

	data_t[0] = address;  
	data_t[1] = value;  
	HAL_I2C_Master_Transmit (&hi2c2, SLAVE_ADDRESS_EEPROM << 1,(uint8_t *) data_t, 2, HAL_MAX_DELAY);
	HAL_Delay(10); // tWR Write Cycle Time 5 ms
	printf("write_eeprom at 0x%X \r\n",address);
}

uint8_t read_eeprom(uint16_t register_pointer)
{
	HAL_StatusTypeDef status = HAL_OK;
	uint8_t return_value = 0;

	status = HAL_I2C_Mem_Read(&hi2c2, SLAVE_ADDRESS_EEPROM << 1, (uint16_t)register_pointer, I2C_MEMADD_SIZE_8BIT, &return_value, 1, 100);

	/* Check the communication status */
	if(status != HAL_OK)
	{
			
	}
	printf("Read EEPROM at [%d] : %d \r\n",register_pointer, return_value);
	return return_value;
}

void reset_eeprom()
{
	for(int i = 0 ; i < 20; i++)
	{
		write_eeprom(i,0+48);
		read_eeprom(i);
	}
}




typedef struct
{
	char password[6];
	char user[3][6];
	enum Mode {
		Mode_None = 0,
		Mode_Keypad = 1,
		Mode_NFC,
		Mode_Fingerprint,
		Mode_Sel,
		Mode_AddUser,
		Mode_ClearUser,
		Mode_Delete,
	} mode;
	enum Status {
		SMARTHOME_NG,
		SMARTHOME_OK,
	} status;
}SMARTHOME;
/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */
 
// quet thay dia chi la 0x50, dich trai 1 bit >> 0xA0
/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/

/* USER CODE BEGIN PV */

char buffer[20];
// su dung uart debug
	int fputc(int ch, FILE * f) 
	{		
		HAL_UART_Transmit( & huart2, (uint8_t * ) & ch, 1, HAL_MAX_DELAY);
		return ch;
	}



	int row,cow =0;
	uint32_t value_adc1 = 0;// khoi tao tung bien , gan vao chanel
	uint32_t value_adc2 = 0;
	uint32_t value_adc3 = 0;
	int Vol =0 ;	
	int so[3];
	int rank1=0;
	int number;
	char key;
	int dem=0;
	int gio,phut,giay;
	int status = 0;
	
	int MK ;
	char buffer[20];
	
	 
	int	matrix_row ;  //debug printf LCD
	int matrix_col;
	
	SMARTHOME smarthome;
/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
/* USER CODE BEGIN PFP */

/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */

GPIO_TypeDef* row_port[4] = {ROW0_GPIO_Port, ROW1_GPIO_Port, ROW2_GPIO_Port, ROW3_GPIO_Port};
GPIO_TypeDef* col_port[4] = {COL0_GPIO_Port, COL1_GPIO_Port, COL2_GPIO_Port, COL3_GPIO_Port};

 
char scan_key()
{
	char keys [4][4] =
	{
		{'1', '2', '3', 'A'},
		{'4', '5', '6', 'B'},
		{'7', '8', '9', 'C'},
		{'*', '0', '#', 'D'},
	};
	// PHIA DUOI LA MATRIX 4X4
	char col_pin[4]={COL0_Pin,COL1_Pin,COL2_Pin,COL3_Pin};// quet cot
	char row_pin[4]={ROW0_Pin,ROW1_Pin,ROW2_Pin,ROW3_Pin};// quet hang
	// quet cot
	for (int col =0;col<4; col++)
	{
		
		HAL_GPIO_WritePin(GPIOD,col_pin[col],0);
		//quet hang
		for ( int row=0;row<4 ;row++)
		{
			if ( HAL_GPIO_ReadPin(GPIOD,row_pin[row]) ==0) 
			{
				
				HAL_Delay(50);
				while ( HAL_GPIO_ReadPin(GPIOD,row_pin[row]) ==0)
				{ 
				}
					 
				HAL_GPIO_WritePin(GPIOD,col_pin[col],1);	
				matrix_row = row;  //debug printf LCD
				matrix_col = col;
				return keys [row][col];
				
			}
			
		}
		HAL_GPIO_WritePin(GPIOD,col_pin[col],1);
	}
 return 32;// 32 KY TU "SPACE" TRONG MA ACSII
}



int check_pass(int * sll_pass)
{
	smarthome.status = SMARTHOME_OK;
	
	for(int j = 0; j < *sll_pass; j++)
	{	
		smarthome.status = SMARTHOME_OK;
		printf("smarthome.password[%d]!=smarthome.user[0][%d] > %d : %d \r\n", j,j,smarthome.password[j],smarthome.user[0][j]);
		if (smarthome.password[j]!=smarthome.user[0][j])
		{
			printf("Usser 1: Saiiiiiiiiii \r\n");
			smarthome.status = SMARTHOME_NG;
			break;
		}
	}

	if (smarthome.status == SMARTHOME_OK)
	{
		lcd_put_cur(1,0);
		lcd_send_string("User 1: OK       ");
		printf("User 1: OK        \r\n");
		return smarthome.status;
	}
	
	return smarthome.status;
}


void display_key(void)
{
		lcd_put_cur(0,0);
		lcd_send_string("Row : ");
		lcd_send_data(matrix_row +48 );

		lcd_put_cur(1,0);
		lcd_send_string("Col : ");
		lcd_send_data(matrix_col + 48 );

	 
}

int status_mod =0;
void pass_fail(void)
{
			lcd_put_cur (1,0);
			lcd_send_string("PASS NOK");
			HAL_Delay(1000);
			lcd_put_cur (1,0);
			lcd_send_string("          ");
			HAL_Delay(1000);
	
	
	
}

int display_keypad()
{
		for(int i = 0; i <3; i++) //add test pass EEEPROM
		{
			smarthome.user[0][i] = '1';
			smarthome.user[1][i] = '2';
			smarthome.user[2][i] = '3';
		 
   	}
		
		int i = 0;
		
		while(smarthome.mode == Mode_Keypad)
		{
				lcd_put_cur(0,0);
				lcd_send_string("Nhap MK -> #");
				
				lcd_send_cmd (0x0F); //Display on/off control --> D = 1, C and B = 1. (Cursor and blink, last two bits)
				lcd_put_cur(1, i);
				
				key = scan_key();
				
				if ((47 < key) && (key < 58)) //phim so 48 -> 0, 57 -> 9
				{
					if (i < 6 ) // mode 6 so
					{
						printf("KeyNumber: %s \r\n", &key);
						smarthome.password[i] = key;
						lcd_send_data(key);
						i++;
					}
				}
				if ((i > 3) && (key == '#'))
				{
					printf("Kiem tra mat khau \r\n");
					
					if (check_pass(&i))
					{
						smarthome.status = SMARTHOME_OK;
						smarthome.mode = Mode_None;
						HAL_Delay(1000);
						lcd_clear();
						HAL_Delay(500);
						return smarthome.status;
					}
					else
					{
						printf("User 1: NG        \r\n");
						i = 0;
						lcd_put_cur(1,0);
						lcd_send_string("User 1: NG       ");
						HAL_Delay(1000);
						lcd_put_cur(1,0);
						lcd_send_string("                 ");
						HAL_Delay(500);
						smarthome.status = SMARTHOME_NG;
					}
				}
				HAL_Delay(100);
		}
		return smarthome.status;
}	
	
	
	




void setup_mod(void)
{
	if (key=='*')
	{
	
		if (status_mod > 0)
		{
			status_mod--;
		}
	}
	else if (key=='#')
	{
	
		if (status_mod <2) 
		{
			status_mod++;
		}
	}

}

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
  MX_ADC1_Init();
  MX_I2C2_Init();
  MX_USART2_UART_Init();
  /* USER CODE BEGIN 2 */
	lcd_init ();
	

  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
	int i=0;
	while(1)
	{
	key = scan_key();
	
	if(('0'<=key)&&(key<='9'))
	{
		  so[i]=key-48;
			printf("so thu %d la %d\r\n ",i,so[i]);
			i++;
	}
	if(key=='#')
	{

		write_eeprom(0,so[0]);
		write_eeprom(1,so[1]);
		write_eeprom(2,so[2]);
		i=0;		
		int read =100*read_eeprom(0) +10*read_eeprom(1) + read_eeprom(2);
		printf("mk nguoi dung 1 la %d\r\n",read);
	}
 
}
	 
  while (1)
  {
		
    /* USER CODE END WHILE */

    /* USER CODE BEGIN 3 */
		
				key = scan_key();
				 			 
   	if ((47 < key) && (key < 58)) //phim so 48 -> 0, 57 -> 9
		{
			printf("KeyNumber: %s \r\n", &key);

		}
		else if (key != 0x20)// cac phim chuc nang (loai tru 0x20 vi 0x20 la return khi k bam phim)
		{
			printf("KeyFunction: %s \r\n", &key);

			if (key == 'A')
			{	
				lcd_clear();
				smarthome.mode = Mode_Keypad;
				printf("smarthome.mode: Mode_Keypad \r\n");
			}
			else if (key == 'D')
			{
				lcd_clear();
				smarthome.mode = Mode_Delete;
				printf("smarthome.mode: Mode_Delete \r\n");
			}
			
		}

		if (smarthome.mode == Mode_None)
		{
			lcd_put_cur(0,0);
			lcd_send_string("A: Mode_Keypad");
			lcd_put_cur(1,0);
			lcd_send_string("D: Mode_Delete");
			
			
			// lock door
			HAL_GPIO_WritePin(LED_D2_GPIO_Port, LED_D2_Pin, 1); // lock
		}
		else if (smarthome.mode == Mode_Keypad)
		{
			printf("Status mo cua: %d \r\n",display_keypad());		
			if (display_keypad() == SMARTHOME_OK)
			{
				HAL_GPIO_WritePin(LED_D2_GPIO_Port, LED_D2_Pin,0 ); // open
				lcd_put_cur(0,0);
				lcd_send_string("Mode_Keypad:");
				lcd_put_cur(1,0);
				lcd_send_string("Open Door 5 Sec");
				HAL_Delay(5000);
				lcd_clear();
			}
			
		}
		else if (smarthome.mode == Mode_NFC)
		{
			//chua hoc
		}
		else if (smarthome.mode == Mode_Fingerprint)
		{
			// chua hoc
		}
		else if (smarthome.mode == Mode_Delete)
		{
			lcd_init();
			smarthome.mode = Mode_None;
			HAL_Delay(500);
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

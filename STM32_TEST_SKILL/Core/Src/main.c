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




typedef struct
{
	uint8_t status;
	uint8_t left;//khai bao nut bam 
	uint8_t right;//khai bao nut bam 
	 
	
	enum 
	{
		
		Mode_maytinh = 0,
		Mode_4led,
	};
	
} Global  ;
Global Global_NT;

typedef struct 
{
	int string_num[100];
	int num_1;
	int num_2;
	int vitri_pheptinh;
	int ketqua;
	int trangthai;
	
	
} Mode1;

Mode1 Maytinh;
 

typedef struct 
{
	//int number[4];
	//int num_2[2];
	
} Mode2;

 




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
	 	
	int so[6];
	char key;
	
	char buffer[20];
	int read;
	int maytinh;
	int status=0;
	int cal=0;
int a=0;
	
	char test;
	 
	int	matrix_row ;  //debug printf LCD
	int matrix_col;

/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
/* USER CODE BEGIN PFP */

/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */

GPIO_TypeDef* row_port[4] = {ROW0_GPIO_Port, ROW1_GPIO_Port, ROW2_GPIO_Port, ROW3_GPIO_Port};
GPIO_TypeDef* col_port[4] = {COL0_GPIO_Port, COL1_GPIO_Port, COL2_GPIO_Port, COL3_GPIO_Port};
/////////////////////////////////////// code led 7 DOAN
void shiftout(int data)
{
	for(int i=0;i <8;i++) // gui tin hieu 1 SO CAN HIEN THI L?N 1 LED 
	{
		if(data&0x80)
		{
			HAL_GPIO_WritePin(DIN_GPIO_Port,DIN_Pin,GPIO_PIN_SET);
		}
		else
		{
			HAL_GPIO_WritePin(DIN_GPIO_Port,DIN_Pin,GPIO_PIN_RESET);
		}
		data<<=1;
		HAL_GPIO_WritePin(SCLK_GPIO_Port,SCLK_Pin,GPIO_PIN_SET);
		HAL_GPIO_WritePin(SCLK_GPIO_Port,SCLK_Pin,GPIO_PIN_RESET);
				
	}
}
char led_index[9]={0};
void display_4led(int so_int)
{
	unsigned char MA7DOAN[] = 
	{
		0xC0,// 0
		0xF9,// 1
		0xA4,// 2
		0xB0,// 3
		0x99,// 4
		0x92,// 5
		0x82,// 6
		0xF8,// 7
		0x80,// 8
		0x90,// 9
		0x88,// A 10
		0x83,// B 11
		0xC6,// C 12
		0xA1,// D 13
		0x86,// E 14
		0x8E,// F 15
		0xff // NONE 16  
	};
	led_index[8] = so_int/1000;
	led_index[4] = so_int%1000/100;			
	led_index[2] = so_int%100/10;			
	led_index[1] = so_int%10;	
	if(led_index[8] == 0)
	{
		led_index[8] = 16; // MA7DOAN[16] -> tat led 7 doan
		if(led_index[4] == 0)
		{
			led_index[4] = 16; // MA7DOAN[16] -> tat led 7 doan
					if(led_index[2] == 0)
					{
						led_index[2] = 16; // MA7DOAN[16] -> tat led 7 doan
					}
		}
	}		

	for(int i = 1; i < 9; i = i*2)
	{
		HAL_GPIO_WritePin(LATCH_GPIO_Port,LATCH_Pin,GPIO_PIN_RESET);
		shiftout(MA7DOAN[led_index[i]]);
		shiftout(i);
		HAL_GPIO_WritePin(LATCH_GPIO_Port,LATCH_Pin,GPIO_PIN_SET);
	}
}


 //////////////////////////////////////////code matrix
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


int scan_mode(void)
{
	if (HAL_GPIO_ReadPin(Mode_GPIO_Port, Mode_Pin))
	{
		 
		HAL_Delay(20);
		if (HAL_GPIO_ReadPin(Mode_GPIO_Port, Mode_Pin))
		{
			// cho nay de chuyen mode, =0 thi mode nay, 1 mode khac
			Global_NT.status = ~Global_NT.status&0x01;
			printf("Global_NT.status %d \r\n",Global_NT.status);
			while (HAL_GPIO_ReadPin(Mode_GPIO_Port, Mode_Pin) == 1)
			{ 
			}
		}
	}
	return Global_NT.status;
}
int scan_left(void)
{
	if ((HAL_GPIO_ReadPin(LEFT_GPIO_Port, LEFT_Pin)==0)&&(a==0))
	{
		 
		HAL_Delay(20);
		if (HAL_GPIO_ReadPin(LEFT_GPIO_Port, LEFT_Pin)==0)
		{
			
			printf("gia tri truoc la %d\r\n",read_eeprom(0) );
			a=~a;
			while (HAL_GPIO_ReadPin(LEFT_GPIO_Port, LEFT_Pin) == 0)
			{ 
			}
		}
	}
	return Global_NT.left;
}


int scan_right(void)
{
	if (HAL_GPIO_ReadPin(RIGHT_GPIO_Port, RIGHT_Pin)==0)
	{
		 
		HAL_Delay(20);
		if (HAL_GPIO_ReadPin(RIGHT_GPIO_Port, RIGHT_Pin)==0)
		{
			 
			printf("gia tri hien tai la  %d\r\n",Maytinh.ketqua );
			while (HAL_GPIO_ReadPin(RIGHT_GPIO_Port, RIGHT_Pin) == 0)
			{ 
				
			}
		}
	}
	return Global_NT.right;
}


int fn_getNum(int array[5],int array_max)
{
	for(int i =0;i<5;i++)
	{
		printf("array[%d] %d \r\n", i, array[i]);
		
	}
	
		
	return 1;
}


void fn_Mode_maytinh(void)
{
	while(Global_NT.status == Mode_maytinh)
	{
		scan_mode();
		scan_left();
		scan_right();
		 
		key = scan_key();
		static int string_max = 0;
		lcd_put_cur (1, string_max);

		if((key == '#') && (string_max > 2))
		{
			for(int i = 0; i < string_max; i++)
			{
				//printf("Maytinh.string_num[i] %d\r\n", Maytinh.string_num[i]);
				if (( Maytinh.string_num[i] < 0) || \
					( Maytinh.string_num[i] > 9))
				{
					printf("VI TRI PHEP TINH LA %d\r\n", i);
					if (Maytinh.string_num[i] == (65-48)) // dau +
					{
						printf("PHEP TINH HIEN TAI LA + \r\n");
						Maytinh.trangthai=0;
						
					}
					else if (Maytinh.string_num[i] == (66-48)) // dau -
					{
						printf("PHEP TINH HIEN TAI LA - \r\n");
						Maytinh.trangthai=1;
					}
					else if (Maytinh.string_num[i] == (67-48)) // dau *
					{
						printf("PHEP TINH HIEN TAI LA * \r\n");
						Maytinh.trangthai=2;
					}
					Maytinh.vitri_pheptinh = i;
				}	 
			}
				
// lay so 1
			printf("Lay so 1:\r\n");
			Maytinh.num_1 = 0;
			for (int j = 0; j < Maytinh.vitri_pheptinh; j++) 
			{
				Maytinh.num_1 = Maytinh.num_1 * 10 + Maytinh.string_num[j];			
			}
			printf("Maytinh.num_1 1a: %d\r\n", Maytinh.num_1);
			// lay so 2
			printf("Lay so 2:\r\n");

			Maytinh.num_2 = 0;
			for (int j = Maytinh.vitri_pheptinh+1; j < string_max; j++) 
			{
				Maytinh.num_2 = Maytinh.num_2 * 10 + Maytinh.string_num[j];
			}
			
			printf("Maytinh.num_2 1a: %d\r\n", Maytinh.num_2);
			
			if(Maytinh.trangthai==0)
			{
				Maytinh.ketqua=Maytinh.num_1+Maytinh.num_2;
				write_eeprom(0,Maytinh.ketqua);
				a=~a;
				printf("ket qua phep cong la:%d\r\n",Maytinh.ketqua);
		
		 
			}
			
				if(Maytinh.trangthai==1)
			{
				Maytinh.ketqua=Maytinh.num_1-Maytinh.num_2;
				printf("ket qua phep tru la : %d\r\n",Maytinh.ketqua);
			
			}
				if(Maytinh.trangthai==2)
			{
				Maytinh.ketqua=Maytinh.num_1*Maytinh.num_2;
				printf("ket qua phep nhan la  : %d\r\n",Maytinh.ketqua);
			}
			
			string_max = 0;
			
	
			
			
		}
		else if(key != 32)
		{
		  Maytinh.string_num[string_max]=key-48;			
			printf("so thu %d la %d\r\n", string_max, Maytinh.string_num[string_max]);
			lcd_send_data(key);			
			string_max++;			
		}
	}
	
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
///////////////////////////CODE MODE 1 thaitn
char Mode_1(void)
{
  int k=0;
	int n=3;
	lcd_put_cur(0,0);
	lcd_send_string("MAY TINH");

	while (k<3)
	{
		key = scan_key();
		lcd_put_cur (1,k);
		if(('0'<=key)&&(key<='9'))
		{
		  so[k]=key-48;			
			printf("so thu %d la %d\r\n",k,so[k]);
			lcd_send_data(key);			
			k++;			
			if(k==2)
			{
				k=0;
			}
		
		}
		if(key=='A')
		{
		 lcd_put_cur(1,2);
		 lcd_send_string("+");	
		 cal=1;
		 break;
		}	
		
		if(key=='B')
		{
		 lcd_put_cur(1,2);
		 lcd_send_string("-");	
		 cal=2;
		 break;
		}
		
		if(key=='C')
		{
		 lcd_put_cur(1,2);
		 lcd_send_string("x");	
		 cal=3;
		 break;
		}	
		
  }
		
	while (n<5)
	{
		key = scan_key();
	
		lcd_put_cur (1,n);
		if(('0'<=key)&&(key<='9'))
		{
		  so[n]=key-48;
			printf("so thu %d la %d\r\n",n,so[n]);
			lcd_send_data(key);			
			n++;
				if(n==5)
				{
					n=3;
				}				
			
		}
		if(key=='#')
		{
			write_eeprom(0,so[0]);
			write_eeprom(1,so[1]);
			write_eeprom(3,so[3]);
			write_eeprom(4,so[4]);
			
			lcd_put_cur (1,5);
			lcd_send_string("=");
			HAL_Delay(100);
			if(cal==1)
			{
				maytinh=10*read_eeprom(0) +read_eeprom(1) + 10*read_eeprom(3) +read_eeprom(4);
			}
			
			if(cal==2)
			{
				maytinh=(10*read_eeprom(0) +read_eeprom(1))-(10*read_eeprom(3) +read_eeprom(4));
			}
			if(cal==3)
			{
				maytinh=(10*read_eeprom(0) +read_eeprom(1))*(10*read_eeprom(3) +read_eeprom(4));
			}
			printf("maytinh %d\r\n",maytinh);// EP KIEU DU LIEU TU INT >>CHAR
			lcd_put_cur (1,6);
			sprintf (buffer,"%d ", maytinh); 
			lcd_send_string (buffer);
			HAL_Delay(3000);
			
			break;
			 
		}
			
		
	}

	return maytinh;
 
}

///////////////////////////CODE MODE 2 thaitn

char Mode_2(void)
{
	int k=0;
	lcd_put_cur(0,0);
	lcd_send_string("nhap 4 so");
	printf("hang 0 : nhap 4 so\r\n");	
	while (k<4)
	{		
	key = scan_key();
	lcd_put_cur (1,k);
	if(('0'<=key)&&(key<='9'))
	{
		  so[k]=key-48;			
			printf("so thu %d la %d\r\n",k,so[k]);			 
			lcd_send_data(key);
			k++;	
			if(k==4)
				{
					k=0;
				}
	}
	
	if(key=='#')
	{
		write_eeprom(10,so[0]);
		write_eeprom(11,so[1]);
		write_eeprom(12,so[2]);
		write_eeprom(13,so[3]);
		k=0;		
		read =1000*read_eeprom(10) +100*read_eeprom(11) + 10*read_eeprom(12)+read_eeprom(13);	 
		printf("so da luu la: %d\r\n",read);
		lcd_init();

		lcd_put_cur(0,0);
		lcd_send_string("DA LUU EEPROM");

	}
	if(key=='*')
	{
		while(1)
		{
			key = scan_key();
			if(key=='A')
				{
					display_4led(0000);	
					lcd_init();
					lcd_put_cur(0,0);
					lcd_send_string("NHAP LAI");
					break;
				}
		display_4led(read);	

		}		
			
	}
	 
	}	 
return read;
	
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
  MX_I2C2_Init();
  MX_USART2_UART_Init();
  /* USER CODE BEGIN 2 */
	lcd_init ();
	

  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
	
  while (1)
  {
		
    /* USER CODE END WHILE */

    /* USER CODE BEGIN 3 */
		//READ STATUS IN EEPROM
		
		//
		scan_mode();
		 	
		if (Global_NT.status == Mode_maytinh)
		{
			printf("Global_NT.status  Mode_maytinh \r\n");
			// void fn_Mode_maytinh
			fn_Mode_maytinh();
		}
		else if (Global_NT.status == Mode_4led)
		{
			printf("Global_NT.status  Mode_4led \r\n");
			// void fn_Mode_4led
			
		}
		else
		{
			//k lam
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

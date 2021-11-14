/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.c
  * @brief          : Main program body
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; Copyright (c) 2021 STMicroelectronics.
  * All rights reserved.</center></h2>
  *
  * This software component is licensed by ST under BSD 3-Clause license,
  * the "License"; You may not use this file except in compliance with the
  * License. You may obtain a copy of the License at:
  *                        opensource.org/licenses/BSD-3-Clause
  *
  ******************************************************************************
  */
/* USER CODE END Header */
/* Includes ------------------------------------------------------------------*/
#include "main.h"
#include "tim.h"
#include "gpio.h"
#include "i2c.h"
#include "spi.h"
#include "usart.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include <math.h>
#include "delay.h"
#include "am2302.h"
#include "hmc5883.h"
#include "mpu9250.h"

#include "MahonyAHRS.h"
// #include "MadgwickAHRS.h"
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
bool reset = false;
/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
/* USER CODE BEGIN PFP */
void AHRSComputeAngles(float *pitch, float *roll, float *yaw);
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
  int tempCount = 0;
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
  MX_TIM5_Init();
  MX_I2C1_Init();
  MX_I2C2_Init();
  MX_SPI1_Init();
  MX_USART1_UART_Init();
  /* USER CODE BEGIN 2 */
  delay_ms(500);
  am2302_init();
  hmc5883_init();
  mpu_init_user();
  printf("System initialization finished\r\n");
  delay_ms(2000);
  hmc5883_calibrate();
  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  while (true)
  {
    // 获取温度数据
    if (++tempCount > 10)
    {
      tempCount = 0;
      float temp, humi;
      am2302_get_data(&temp, &humi);
      // printf("temperature: %.1fC humidity: %.1f%%\r\n", temp, humi);
      printf("T%.1f\r\nH%.1f\r\n", temp, humi);
    }
    // 获取磁力计数据
    short x, y, z;
    hmc5883_get_data(&x, &y, &z, NULL);
    // printf("comp: %d %d %d, angle: %f\r\n", x, y, z, 0.0f);
    printf("M%d %d %d\r\n", x, y, z);
    // 获取加速度计和陀螺仪数据
    float accel[3], gyro[3];
    mpu_get_data(accel, gyro);
    // printf("accel: %.2f %.2f %.2f\r\n", accel[0], accel[1], accel[2]);
    printf("C%.2f %.2f %.2f\r\n", accel[0], accel[1], accel[2]);
    // printf("gyro: %.2f %.2f %.2f\r\n", gyro[0], gyro[1], gyro[2]);
    printf("G%.2f %.2f %.2f\r\n", gyro[0], gyro[1], gyro[2]);
    
    // 计算欧拉角
    float pitch, roll, yaw;
    // 调用MPL库
    // inv_time_t timestamp;
    // get_tick_count(&timestamp);
    // mpl_build_external_compress(x, y, z, timestamp);
    // mpl_get_data(&pitch, &roll, &yaw);
    // 使用DMP
    // dmp_get_data(&pitch, &roll, &yaw);
    // 复位
    if (reset)
    {
      reset = false;
      MahonyAHRSReset();
      printf("Reset angles\r\n");
    }
    // 使用Mahony算法(互补滤波算法explicit complement filter)
    MahonyAHRSupdate(gyro[0], gyro[1], gyro[2], accel[0], accel[1], accel[2], y, -x, z);
    // 使用Madgwick算法(梯度下降算法gradient descent)
    // MadgwickAHRSupdate(gyro[0], gyro[1], gyro[2], accel[0], accel[1], accel[2], y, -x, z);
    AHRSComputeAngles(&pitch, &roll, &yaw);
    // printf("pitch: %.2f roll: %.2f yaw: %.2f\r\n", pitch, roll, yaw);
    printf("A%d %d %d\r\n", (int) pitch, (int) roll, (int) yaw);
    // 延时
    delay_ms(200);
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
  __HAL_RCC_PWR_CLK_ENABLE();
  __HAL_PWR_VOLTAGESCALING_CONFIG(PWR_REGULATOR_VOLTAGE_SCALE1);
  /** Initializes the RCC Oscillators according to the specified parameters
  * in the RCC_OscInitTypeDef structure.
  */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSE;
  RCC_OscInitStruct.HSEState = RCC_HSE_ON;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSE;
  RCC_OscInitStruct.PLL.PLLM = 8;
  RCC_OscInitStruct.PLL.PLLN = 336;
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
  /** Enables the Clock Security System
  */
  HAL_RCC_EnableCSS();
}

/* USER CODE BEGIN 4 */
void AHRSComputeAngles(float *pitch, float *roll, float *yaw)
{
    *pitch = asinf(-2.0f * (q1 * q3 - q0 * q2)) * 57.29578f;
    *roll = atan2f(q0 * q1 + q2 * q3, 0.5f - q1 * q1 - q2 * q2) * 57.29578f;
    *yaw = atan2f(q1 * q2 + q0 * q3, 0.5f - q2 * q2 - q3 * q3) * 57.29578f;
}

void key0_pressed(void)
{
    reset = true;
}

void uart_received(const char *data, uint16_t size)
{
    uart_println(data);
}

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

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/


/**
  ******************************************************************************
  * @file           : main.c
  * @brief          : Main program body
  ******************************************************************************
  ** This notice applies to any and all portions of this file
  * that are not between comment pairs USER CODE BEGIN and
  * USER CODE END. Other portions of this file, whether 
  * inserted by the user or by software development tools
  * are owned by their respective copyright owners.
  *
  * COPYRIGHT(c) 2018 STMicroelectronics
  *
  * Redistribution and use in source and binary forms, with or without modification,
  * are permitted provided that the following conditions are met:
  *   1. Redistributions of source code must retain the above copyright notice,
  *      this list of conditions and the following disclaimer.
  *   2. Redistributions in binary form must reproduce the above copyright notice,
  *      this list of conditions and the following disclaimer in the documentation
  *      and/or other materials provided with the distribution.
  *   3. Neither the name of STMicroelectronics nor the names of its contributors
  *      may be used to endorse or promote products derived from this software
  *      without specific prior written permission.
  *
  * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
  * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
  * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
  * DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE
  * FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
  * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
  * SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
  * CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
  * OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
  * OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
  *
  ******************************************************************************
  */
/* Includes ------------------------------------------------------------------*/
#include "main.h"
#include "stm32f4xx_hal.h"

/* USER CODE BEGIN Includes */
#include  <errno.h>
#include  <stdio.h>
#include  <sys/stat.h>
#include  <sys/unistd.h>
#include "x_nucleo_ihmxx.h"
#include "l6206.h"
#include "x_nucleo_ihm04a1_stm32f4xx.h"
/* USER CODE END Includes */

/* Private variables ---------------------------------------------------------*/
TIM_HandleTypeDef htim2;
TIM_HandleTypeDef htim3;

UART_HandleTypeDef huart2;

/* USER CODE BEGIN PV */
/* Private variables ---------------------------------------------------------*/
#define MAX_STEPS (9)
/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
static void MX_GPIO_Init(void);
static void MX_USART2_UART_Init(void);
static void MX_TIM3_Init(void);
static void MX_TIM2_Init(void);                                    
void HAL_TIM_MspPostInit(TIM_HandleTypeDef *htim);
                                
                                

/* USER CODE BEGIN PFP */
/* Private function prototypes -----------------------------------------------*/
void my_Error_Handler(uint16_t error);

static volatile uint16_t gLastError;
static volatile bool gButtonPressed = FALSE;
static volatile uint8_t gStep = 0;


deviceParams_t initDeviceParameters =
{
		 L6206_CONF_PARAM_PARALLE_BRIDGES,
		 {L6206_CONF_PARAM_FREQ_PWM1A, L6206_CONF_PARAM_FREQ_PWM2A, L6206_CONF_PARAM_FREQ_PWM1B, L6206_CONF_PARAM_FREQ_PWM2B},
		 {100,100,100,100},
		 {FORWARD,FORWARD,BACKWARD,FORWARD},
		 {INACTIVE,INACTIVE,INACTIVE,INACTIVE},
		 {FALSE,FALSE}
};
/* USER CODE END PFP */

/* USER CODE BEGIN 0 */
int _write(int file, char *data, int len)
{
    HAL_StatusTypeDef ret = HAL_ERROR;

    if ((file != STDOUT_FILENO) && (file != STDERR_FILENO))
    {
        errno = EBADF;
        return -1;
    }
    ret = HAL_UART_Transmit(&huart2, (uint8_t*)data, (uint16_t)len, 100);

    if (ret == HAL_OK)
    	return len;
    return -1;
}

static void MyFlagInterruptHandler(void);
void ButtonHandler(void);


void avance()
{
	/*********** Step 0  ************/
	/* Set speed of motor 0 to 100 % */
	BSP_MotorControl_SetMaxSpeed(0,30);
	BSP_MotorControl_SetMaxSpeed(1,30);
	/* start motor 0 to run forward*/
	BSP_MotorControl_Run(1, FORWARD);
	/* start motor 0 to run forward*/
	BSP_MotorControl_Run(0, FORWARD);
	HAL_Delay(500);
	BSP_MotorControl_SetMaxSpeed(0,50);
	BSP_MotorControl_SetMaxSpeed(1,50);
	HAL_Delay(500);
	BSP_MotorControl_SetMaxSpeed(0,70);
	BSP_MotorControl_SetMaxSpeed(1,70);
	HAL_Delay(1000);
	BSP_MotorControl_SetMaxSpeed(0,50);
	BSP_MotorControl_SetMaxSpeed(1,50);
	HAL_Delay(500);
	BSP_MotorControl_SetMaxSpeed(0,30);
	BSP_MotorControl_SetMaxSpeed(1,30);
	HAL_Delay(500);
	BSP_MotorControl_SetMaxSpeed(0,0);
	BSP_MotorControl_SetMaxSpeed(1,0);
	HAL_Delay(100);
	BSP_MotorControl_HardStop(0);
	BSP_MotorControl_HardStop(1);
}
void recul()
{
	/*********** Step 0  ************/
	/* Set speed of motor 0 to 100 % */
	BSP_MotorControl_SetMaxSpeed(0,30);
	BSP_MotorControl_SetMaxSpeed(1,30);
	/* start motor 0 to run forward*/
	BSP_MotorControl_Run(1, BACKWARD);
	/* start motor 0 to run forward*/
	BSP_MotorControl_Run(0, BACKWARD);
	HAL_Delay(500);
	BSP_MotorControl_SetMaxSpeed(0,50);
	BSP_MotorControl_SetMaxSpeed(1,50);
	HAL_Delay(500);
	BSP_MotorControl_SetMaxSpeed(0,70);
	BSP_MotorControl_SetMaxSpeed(1,70);
	HAL_Delay(1000);
	BSP_MotorControl_SetMaxSpeed(0,50);
	BSP_MotorControl_SetMaxSpeed(1,50);
	HAL_Delay(500);
	BSP_MotorControl_SetMaxSpeed(0,30);
	BSP_MotorControl_SetMaxSpeed(1,30);
	HAL_Delay(500);
	BSP_MotorControl_SetMaxSpeed(0,0);
	BSP_MotorControl_SetMaxSpeed(1,0);
	HAL_Delay(100);
	BSP_MotorControl_HardStop(0);
	BSP_MotorControl_HardStop(1);
}

void tourne()
{

		/*********** Step 0  ************/
		/* Set speed of motor 0 to 100 % */
		BSP_MotorControl_SetMaxSpeed(0,30);
		BSP_MotorControl_SetMaxSpeed(1,30);
		/* start motor 0 to run forward*/
		BSP_MotorControl_Run(1, FORWARD);
		/* start motor 0 to run forward*/
		BSP_MotorControl_Run(0, FORWARD);
		HAL_Delay(500);
		BSP_MotorControl_SetMaxSpeed(0,50);
		HAL_Delay(500);
		BSP_MotorControl_SetMaxSpeed(1,50);
		BSP_MotorControl_SetMaxSpeed(0,70);
		HAL_Delay(500);
		BSP_MotorControl_SetMaxSpeed(1,70);
		BSP_MotorControl_SetMaxSpeed(0,50);
		HAL_Delay(500);
		BSP_MotorControl_SetMaxSpeed(1,50);
		BSP_MotorControl_SetMaxSpeed(0,30);
		HAL_Delay(500);
		BSP_MotorControl_SetMaxSpeed(1,30);
		HAL_Delay(500);
		BSP_MotorControl_SetMaxSpeed(0,0);
		BSP_MotorControl_SetMaxSpeed(1,0);
		HAL_Delay(100);
		BSP_MotorControl_HardStop(0);
		BSP_MotorControl_HardStop(1);

}

/* USER CODE END 0 */

/**
  * @brief  The application entry point.
  *
  * @retval None
  */
int main(void)
{
  /* USER CODE BEGIN 1 */
	uint32_t Step_size = 10;
	uint32_t dir = FORWARD;
  /* USER CODE END 1 */

  /* MCU Configuration----------------------------------------------------------*/

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
  MX_USART2_UART_Init();
  MX_TIM3_Init();
  MX_TIM2_Init();
  /* USER CODE BEGIN 2 */
  printf("Hello world !!\n");
  /* Configure KEY Button */
  BSP_PB_Init(BUTTON_KEY, BUTTON_MODE_EXTI);

  HAL_NVIC_SetPriorityGrouping(NVIC_PRIORITYGROUP_4);
  /* Set Systick Interrupt to the highest priority to have HAL_Delay working*/
  /* under the user button handler */
  HAL_NVIC_SetPriority(SysTick_IRQn, 0x0, 0x0);

  //----- Init of the Motor control library
  /* Set the L6208 library to use 1 device */
  BSP_MotorControl_SetNbDevices(BSP_MOTOR_CONTROL_BOARD_ID_L6206, 2);
  /* When BSP_MotorControl_Init is called with NULL pointer,                  */
  /* the L6206 parameters are set with the predefined values from file        */
  /* l6206_target_config.h, otherwise the parameters are set using the        */
  /* initDeviceParameters structure values.                                   */
  BSP_MotorControl_Init(BSP_MOTOR_CONTROL_BOARD_ID_L6206, &initDeviceParameters);

  /* Select the configuration with paralleling of brigde input IN1A with IN2A,
  and with paralleling of brigde input IN1B with IN2B with the use of one
  bidirectionnal motor */
  BSP_MotorControl_SetDualFullBridgeConfig(PARALLELING_NONE___1_BIDIR_MOTOR_BRIDGE_A__1_BIDIR_MOTOR_BRIDGE_B);

  /* Attach the function MyFlagInterruptHandler (defined below) to the flag interrupt */
  BSP_MotorControl_AttachFlagInterrupt(MyFlagInterruptHandler);

  /* Attach the function Error_Handler (defined below) to the error Handler*/
  BSP_MotorControl_AttachErrorHandler(my_Error_Handler);

  /* Set PWM Frequency of bridge A inputs to 10000 Hz */
  BSP_MotorControl_SetBridgeInputPwmFreq(0,10000);

  /* Set PWM Frequency of bridge B inputs to 10000 Hz */
  BSP_MotorControl_SetBridgeInputPwmFreq(1,10000);
  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  while (1)
  {
	  /* Each time the user button is pressed, the step is increased by 1 */

	      if (gButtonPressed)
	      {
	        gButtonPressed = FALSE;
	        gStep++;
	        if (gStep * Step_size > 100)
	        {
	          gStep = 0;

	        }
	        if (dir == FORWARD)
	        	dir = BACKWARD;
		    else
		    	dir = FORWARD;
	        printf("Setting motor %s at %lu ratio\n", (dir == FORWARD ? "Forward" : "Backward"), gStep*Step_size);
	        //avance();
	        tourne();

	        //recul();
	        BSP_MotorControl_CmdHardHiZ(0);
	        BSP_MotorControl_CmdHardHiZ(1);
			printf("stop\n");

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

  RCC_OscInitTypeDef RCC_OscInitStruct;
  RCC_ClkInitTypeDef RCC_ClkInitStruct;

    /**Configure the main internal regulator output voltage 
    */
  __HAL_RCC_PWR_CLK_ENABLE();

  __HAL_PWR_VOLTAGESCALING_CONFIG(PWR_REGULATOR_VOLTAGE_SCALE2);

    /**Initializes the CPU, AHB and APB busses clocks 
    */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSI;
  RCC_OscInitStruct.HSIState = RCC_HSI_ON;
  RCC_OscInitStruct.HSICalibrationValue = 16;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSI;
  RCC_OscInitStruct.PLL.PLLM = 16;
  RCC_OscInitStruct.PLL.PLLN = 336;
  RCC_OscInitStruct.PLL.PLLP = RCC_PLLP_DIV4;
  RCC_OscInitStruct.PLL.PLLQ = 7;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    _Error_Handler(__FILE__, __LINE__);
  }

    /**Initializes the CPU, AHB and APB busses clocks 
    */
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                              |RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV2;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_2) != HAL_OK)
  {
    _Error_Handler(__FILE__, __LINE__);
  }

    /**Configure the Systick interrupt time 
    */
  HAL_SYSTICK_Config(HAL_RCC_GetHCLKFreq()/1000);

    /**Configure the Systick 
    */
  HAL_SYSTICK_CLKSourceConfig(SYSTICK_CLKSOURCE_HCLK);

  /* SysTick_IRQn interrupt configuration */
  HAL_NVIC_SetPriority(SysTick_IRQn, 0, 0);
}

/* TIM2 init function */
static void MX_TIM2_Init(void)
{

  TIM_MasterConfigTypeDef sMasterConfig;
  TIM_OC_InitTypeDef sConfigOC;

  htim2.Instance = TIM2;
  htim2.Init.Prescaler = 0;
  htim2.Init.CounterMode = TIM_COUNTERMODE_UP;
  htim2.Init.Period = 0;
  htim2.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;
  if (HAL_TIM_PWM_Init(&htim2) != HAL_OK)
  {
    _Error_Handler(__FILE__, __LINE__);
  }

  sMasterConfig.MasterOutputTrigger = TIM_TRGO_RESET;
  sMasterConfig.MasterSlaveMode = TIM_MASTERSLAVEMODE_DISABLE;
  if (HAL_TIMEx_MasterConfigSynchronization(&htim2, &sMasterConfig) != HAL_OK)
  {
    _Error_Handler(__FILE__, __LINE__);
  }

  sConfigOC.OCMode = TIM_OCMODE_PWM1;
  sConfigOC.Pulse = 0;
  sConfigOC.OCPolarity = TIM_OCPOLARITY_HIGH;
  sConfigOC.OCFastMode = TIM_OCFAST_DISABLE;
  if (HAL_TIM_PWM_ConfigChannel(&htim2, &sConfigOC, TIM_CHANNEL_1) != HAL_OK)
  {
    _Error_Handler(__FILE__, __LINE__);
  }

  if (HAL_TIM_PWM_ConfigChannel(&htim2, &sConfigOC, TIM_CHANNEL_2) != HAL_OK)
  {
    _Error_Handler(__FILE__, __LINE__);
  }

  HAL_TIM_MspPostInit(&htim2);

}

/* TIM3 init function */
static void MX_TIM3_Init(void)
{

  TIM_MasterConfigTypeDef sMasterConfig;
  TIM_OC_InitTypeDef sConfigOC;

  htim3.Instance = TIM3;
  htim3.Init.Prescaler = 0;
  htim3.Init.CounterMode = TIM_COUNTERMODE_UP;
  htim3.Init.Period = 0;
  htim3.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;
  if (HAL_TIM_PWM_Init(&htim3) != HAL_OK)
  {
    _Error_Handler(__FILE__, __LINE__);
  }

  sMasterConfig.MasterOutputTrigger = TIM_TRGO_RESET;
  sMasterConfig.MasterSlaveMode = TIM_MASTERSLAVEMODE_DISABLE;
  if (HAL_TIMEx_MasterConfigSynchronization(&htim3, &sMasterConfig) != HAL_OK)
  {
    _Error_Handler(__FILE__, __LINE__);
  }

  sConfigOC.OCMode = TIM_OCMODE_PWM1;
  sConfigOC.Pulse = 0;
  sConfigOC.OCPolarity = TIM_OCPOLARITY_HIGH;
  sConfigOC.OCFastMode = TIM_OCFAST_DISABLE;
  if (HAL_TIM_PWM_ConfigChannel(&htim3, &sConfigOC, TIM_CHANNEL_1) != HAL_OK)
  {
    _Error_Handler(__FILE__, __LINE__);
  }

  if (HAL_TIM_PWM_ConfigChannel(&htim3, &sConfigOC, TIM_CHANNEL_2) != HAL_OK)
  {
    _Error_Handler(__FILE__, __LINE__);
  }

  HAL_TIM_MspPostInit(&htim3);

}

/* USART2 init function */
static void MX_USART2_UART_Init(void)
{

  huart2.Instance = USART2;
  huart2.Init.BaudRate = 115200;
  huart2.Init.WordLength = UART_WORDLENGTH_8B;
  huart2.Init.StopBits = UART_STOPBITS_1;
  huart2.Init.Parity = UART_PARITY_NONE;
  huart2.Init.Mode = UART_MODE_TX_RX;
  huart2.Init.HwFlowCtl = UART_HWCONTROL_NONE;
  huart2.Init.OverSampling = UART_OVERSAMPLING_16;
  if (HAL_UART_Init(&huart2) != HAL_OK)
  {
    _Error_Handler(__FILE__, __LINE__);
  }

}

/** Configure pins as 
        * Analog 
        * Input 
        * Output
        * EVENT_OUT
        * EXTI
*/
static void MX_GPIO_Init(void)
{

  GPIO_InitTypeDef GPIO_InitStruct;

  /* GPIO Ports Clock Enable */
  __HAL_RCC_GPIOC_CLK_ENABLE();
  __HAL_RCC_GPIOH_CLK_ENABLE();
  __HAL_RCC_GPIOA_CLK_ENABLE();
  __HAL_RCC_GPIOB_CLK_ENABLE();

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(LD2_GPIO_Port, LD2_Pin, GPIO_PIN_RESET);

  /*Configure GPIO pin : B1_Pin */
  GPIO_InitStruct.Pin = B1_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_IT_FALLING;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  HAL_GPIO_Init(B1_GPIO_Port, &GPIO_InitStruct);

  /*Configure GPIO pin : PC1 */
  GPIO_InitStruct.Pin = GPIO_PIN_1;
  GPIO_InitStruct.Mode = GPIO_MODE_IT_RISING;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  HAL_GPIO_Init(GPIOC, &GPIO_InitStruct);

  /*Configure GPIO pin : LD2_Pin */
  GPIO_InitStruct.Pin = LD2_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(LD2_GPIO_Port, &GPIO_InitStruct);

  /*Configure GPIO pin : PA10 */
  GPIO_InitStruct.Pin = GPIO_PIN_10;
  GPIO_InitStruct.Mode = GPIO_MODE_IT_RISING;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

  /* EXTI interrupt init*/
  HAL_NVIC_SetPriority(EXTI1_IRQn, 0xf, 0);
  HAL_NVIC_EnableIRQ(EXTI1_IRQn);

  //HAL_NVIC_SetPriority(EXTI15_10_IRQn, 0xf, 0);
  //HAL_NVIC_EnableIRQ(EXTI15_10_IRQn);

}

/* USER CODE BEGIN 4 */
/**
  * @brief  This function is the User handler for the flag interrupt
  * @param  None
  * @retval None
  */
void MyFlagInterruptHandler(void)
{
  /* Code to be customised */
  /************************/
  /* Get the state of bridge A */
  uint16_t bridgeState  = BSP_MotorControl_CmdGetStatus(0);

  if (bridgeState == 0)
  {
    if (BSP_MotorControl_GetDeviceState(0) != INACTIVE)
    {
      /* Bridge A was disabling due to overcurrent or over temperature */
      /* When  motor was running */
    	my_Error_Handler(0XBAD0);
    }
  }
 }

/**
  * @brief  This function is executed in case of error occurrence.
  * @param  error number of the error
  * @retval None
  */
void my_Error_Handler(uint16_t error)
{
  /* Backup error number */
  gLastError = error;
  printf("Error !!! %x\n", error);
  /* Infinite loop */
  while(1)
  {
  }
}
/**
  * @brief  This function is executed when the Nucleo User button is pressed
  * @param  error number of the error
  * @retval None
  */
void ButtonHandler(void)
{
  gButtonPressed = TRUE;

  /* Let 300 ms before clearing the IT for key debouncing */
  HAL_Delay(300);
  __HAL_GPIO_EXTI_CLEAR_IT(KEY_BUTTON_PIN);
  HAL_NVIC_ClearPendingIRQ(KEY_BUTTON_EXTI_IRQn);
}
/* USER CODE END 4 */

/**
  * @brief  This function is executed in case of error occurrence.
  * @param  file: The file name as string.
  * @param  line: The line in file as a number.
  * @retval None
  */
void _Error_Handler(char *file, int line)
{
  /* USER CODE BEGIN Error_Handler_Debug */
  /* User can add his own implementation to report the HAL error return state */
  while(1)
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
void assert_failed(uint8_t* file, uint32_t line)
{ 
  /* USER CODE BEGIN 6 */
  /* User can add his own implementation to report the file name and line number,
     tex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */
  /* USER CODE END 6 */
}
#endif /* USE_FULL_ASSERT */

/**
  * @}
  */

/**
  * @}
  */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/

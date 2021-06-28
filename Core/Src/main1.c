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
  * This software component is licensed by ST under Ultimate Liberty license
  * SLA0044, the "License"; You may not use this file except in compliance with
  * the License. You may obtain a copy of the License at:
  *                             www.st.com/SLA0044
  *
  ******************************************************************************
  */
/* USER CODE END Header */
/* Includes ------------------------------------------------------------------*/
#include "main.h"
#include "cmsis_os.h"
#include "lwip.h"
#include "httpserver-netconn.h"
#include "heap_3.h"
#include "metroTask.h"
#include "st_device.h"
#include "app.h"
/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
//#define CRC_8 (0x07)
//#define STPM3x_FRAME_LEN (5)
//uint8_t frame_without_CRC[STPM3x_FRAME_LEN -1];
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
RTC_HandleTypeDef hrtc;

osThreadId defaultTaskHandle;
//osThreadId LED_taskHandle;
osThreadId bot_taskHandle;
UART_HandleTypeDef huart4;
/* USER CODE BEGIN PV */
TIM_HandleTypeDef htim3;
/* USER CODE END PV */
uint32_t uwPrescalerValue = 0; 
/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
static void MX_GPIO_Init(void);
static void MX_RTC_Init(void);
void StartDefaultTask(void const * argument);
void Task_HAL1(void const * argument);
void Task_HAL2(void const * argument);
static void MX_UART4_Init(void);
/* USER CODE BEGIN PFP */
static void Timer_Init();
static void MX_TIM3_Init(void);
/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */



/* Buffer used for reception */

/* USER CODE END 0 */
/**
  * @brief      Reverse byte
  * @param      one Byte
  * @retval     Byte reversed
  */
////static uint8_t Metro_HAL_byteReverse(uint8_t in_byte)
////{
////    in_byte = ((in_byte >> 1) & 0x55) | ((in_byte << 1) & 0xaa);
////    in_byte = ((in_byte >> 2) & 0x33) | ((in_byte << 2) & 0xcc);
////    in_byte = ((in_byte >> 4) & 0x0F) | ((in_byte << 4) & 0xF0);
////
////    return in_byte;
////}
/////**
////  * @brief      Calculate CRC of a byte
////  * @param      one Byte
////  * @retval     None
////  */
////static uint8_t CRC_u8Checksum;
////static void Metro_HAL_Crc8Calc (uint8_t in_Data)
////{
////    uint8_t loc_u8Idx;
////    uint8_t loc_u8Temp;
////    loc_u8Idx=0;
////    while(loc_u8Idx<8)
////    {
////        loc_u8Temp=in_Data^CRC_u8Checksum;
////        CRC_u8Checksum<<=1;
////        if(loc_u8Temp&0x80)
////        {
////            CRC_u8Checksum^=CRC_8;
////        }
////        in_Data<<=1;
////        loc_u8Idx++;
////    }
////}
/////**
////  * @brief      Calculate CRC of a frame
////  * @param      Buf Frame
////  * @retval     u8 checksum of the frame
////  */
////static uint8_t Metro_HAL_CalcCRC8(uint8_t *pBuf)
////{
////    uint8_t     i;
////    CRC_u8Checksum = 0x00;
////
////    for (i=0; i<STPM3x_FRAME_LEN-1; i++)
////    {
////        Metro_HAL_Crc8Calc(pBuf[i]);
////    }
////
////    return CRC_u8Checksum;
////}
////
////void send_data_uart (uint8_t * frame_with_CRC)
////{
////  uint32_t i;
////  uint8_t CRC_on_reversed_buf;
//// for (i=0;i<(STPM3x_FRAME_LEN-1);i++)
////     {
////       frame_without_CRC[i] = Metro_HAL_byteReverse(frame_with_CRC[i]);
////     }
////
////     /* Calculate CRC and put it at the end of the frame */
////     CRC_on_reversed_buf = Metro_HAL_CalcCRC8(frame_without_CRC);
////     frame_with_CRC[4] = Metro_HAL_byteReverse(CRC_on_reversed_buf);
////}

/**
  * @brief  The application entry point.
  * @retval int
  */
int main(void)
{
  /* USER CODE BEGIN 1 */
 init_system_heap();
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
//   set_relay(0);
// set_relay(1);
  MX_RTC_Init();
  
  MX_TIM3_Init();
  Timer_Init(); 
 /// METRO_Init();
  //  MX_UART4_Init();
  /* USER CODE BEGIN 2 */

  /* USER CODE END 2 */

  /* USER CODE BEGIN RTOS_MUTEX */
  /* add mutexes, ... */
  /* USER CODE END RTOS_MUTEX */

  /* USER CODE BEGIN RTOS_SEMAPHORES */
  /* add semaphores, ... */
  /* USER CODE END RTOS_SEMAPHORES */

  /* USER CODE BEGIN RTOS_TIMERS */
  /* start timers, add new ones, ... */
  /* USER CODE END RTOS_TIMERS */

  /* USER CODE BEGIN RTOS_QUEUES */
  /* add queues, ... */
  /* USER CODE END RTOS_QUEUES */

  /* Create the thread(s) */
  /* definition and creation of defaultTask */
  osThreadDef(defaultTask, StartDefaultTask, osPriorityNormal, 0, 128);
  defaultTaskHandle = osThreadCreate(osThread(defaultTask), NULL);

  /* definition and creation of LED_task */
  osThreadDef(LED_task, Task_HAL1, osPriorityIdle, 0, 128);  
  LED_taskHandle = osThreadCreate(osThread(LED_task), NULL);

   osThreadDef(bot_task, Task_HAL2, osPriorityIdle, 0, 128);
  bot_taskHandle = osThreadCreate(osThread(bot_task), NULL);
  /* USER CODE BEGIN RTOS_THREADS */
  /* add threads, ... */
  /* USER CODE END RTOS_THREADS */

  /* Start scheduler */
  osKernelStart();

  /* We should never get here as control is now taken by the scheduler */
  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  while (1)
  {
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
  RCC_PeriphCLKInitTypeDef PeriphClkInit = {0};

  /** Initializes the RCC Oscillators according to the specified parameters
  * in the RCC_OscInitTypeDef structure.
  */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSE|RCC_OSCILLATORTYPE_LSE;
  RCC_OscInitStruct.HSEState = RCC_HSE_ON;
  RCC_OscInitStruct.HSEPredivValue = RCC_HSE_PREDIV_DIV6;
  RCC_OscInitStruct.LSEState = RCC_LSE_ON;
  RCC_OscInitStruct.HSIState = RCC_HSI_ON;
  RCC_OscInitStruct.Prediv1Source = RCC_PREDIV1_SOURCE_PLL2;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSE;
  RCC_OscInitStruct.PLL.PLLMUL = RCC_PLL_MUL5;
  RCC_OscInitStruct.PLL2.PLL2State = RCC_PLL2_ON;
  RCC_OscInitStruct.PLL2.PLL2MUL = RCC_PLL2_MUL16;
  RCC_OscInitStruct.PLL2.HSEPrediv2Value = RCC_HSE_PREDIV2_DIV4;
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

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_1) != HAL_OK)
  {
    Error_Handler();
  }
  PeriphClkInit.PeriphClockSelection = RCC_PERIPHCLK_RTC;
  PeriphClkInit.RTCClockSelection = RCC_RTCCLKSOURCE_LSE;
  PeriphClkInit.PLLI2S.PLLI2SMUL = RCC_PLLI2S_MUL16;
  PeriphClkInit.PLLI2S.HSEPrediv2Value = RCC_HSE_PREDIV2_DIV4;
  if (HAL_RCCEx_PeriphCLKConfig(&PeriphClkInit) != HAL_OK)
  {
    Error_Handler();
  }
  HAL_RCC_MCOConfig(RCC_MCO, RCC_MCO1SOURCE_PLL3CLK, RCC_MCODIV_1);
  /** Configure the Systick interrupt time
  */
  __HAL_RCC_HSE_PREDIV2_CONFIG(RCC_HSE_PREDIV2_DIV4);
  /** Configure the Systick interrupt time
  */
  __HAL_RCC_PLLI2S_CONFIG(RCC_PLLI2S_MUL16);
  /** Configure the Systick interrupt time
  */
  __HAL_RCC_PLLI2S_ENABLE();
}
/**
  * @brief RTC Initialization Function
  * @param None
  * @retval None
  */
static void MX_RTC_Init(void)
{

  /* USER CODE BEGIN RTC_Init 0 */

  /* USER CODE END RTC_Init 0 */

  RTC_TimeTypeDef sTime = {0};
  RTC_DateTypeDef DateToUpdate = {0};

  /* USER CODE BEGIN RTC_Init 1 */

  /* USER CODE END RTC_Init 1 */
  /** Initialize RTC Only
  */
  hrtc.Instance = RTC;
  hrtc.Init.AsynchPrediv = RTC_AUTO_1_SECOND;
  hrtc.Init.OutPut = RTC_OUTPUTSOURCE_NONE;
  if (HAL_RTC_Init(&hrtc) != HAL_OK)
  {
    Error_Handler();
  }

  /* USER CODE BEGIN Check_RTC_BKUP */

  /* USER CODE END Check_RTC_BKUP */

  /** Initialize RTC and set the Time and Date
  */
  sTime.Hours = 0x10;
  sTime.Minutes = 0x0;
  sTime.Seconds = 0x0;

  if (HAL_RTC_SetTime(&hrtc, &sTime, RTC_FORMAT_BCD) != HAL_OK)
  {
    Error_Handler();
  }
  DateToUpdate.WeekDay = RTC_WEEKDAY_MONDAY;
  DateToUpdate.Month = RTC_MONTH_JANUARY;
  DateToUpdate.Date = 0x1;
  DateToUpdate.Year = 0x0;

  if (HAL_RTC_SetDate(&hrtc, &DateToUpdate, RTC_FORMAT_BCD) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN RTC_Init 2 */

  /* USER CODE END RTC_Init 2 */

}
static void MX_UART4_Init(void)
{

  /* USER CODE BEGIN UART4_Init 0 */

  /* USER CODE END UART4_Init 0 */

  /* USER CODE BEGIN UART4_Init 1 */

  /* USER CODE END UART4_Init 1 */
  huart4.Instance = UART4;
  huart4.Init.BaudRate = 9600;
  huart4.Init.WordLength = UART_WORDLENGTH_8B;
  huart4.Init.StopBits = UART_STOPBITS_1;
  huart4.Init.Parity = UART_PARITY_NONE;
  huart4.Init.Mode = UART_MODE_TX_RX;
  huart4.Init.HwFlowCtl = UART_HWCONTROL_NONE;
  huart4.Init.OverSampling = UART_OVERSAMPLING_16;
  if (HAL_UART_Init(&huart4) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN UART4_Init 2 */

  /* USER CODE END UART4_Init 2 */

}
/**
  * @brief GPIO Initialization Function
  * @param None
  * @retval None
  */
static void MX_GPIO_Init(void)
{
  GPIO_InitTypeDef GPIO_InitStruct = {0};
  /* GPIO Ports Clock Enable */
  __HAL_RCC_GPIOC_CLK_ENABLE();
  __HAL_RCC_GPIOA_CLK_ENABLE();
  __HAL_RCC_GPIOB_CLK_ENABLE();
  __HAL_RCC_GPIOD_CLK_ENABLE();
  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(PHY_RST_GPIO_Port, PHY_RST_Pin, GPIO_PIN_SET);
  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(LED_RED_GPIO_Port, LED_RED_Pin|LED_GREEN_Pin, GPIO_PIN_RESET);
  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(RELAY_TRIG_GPIO_Port, RELAY_INT_A1_Pin|RELAY_TRIG_Pin, GPIO_PIN_RESET);
  /*Configure GPIO pin : PHY_RST_Pin */
  GPIO_InitStruct.Pin = PHY_RST_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_OD;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_MEDIUM;
  HAL_GPIO_Init(PHY_RST_GPIO_Port, &GPIO_InitStruct);

  /*Configure GPIO pins : LED_RED_Pin LED_GREEN_Pin */
  GPIO_InitStruct.Pin = LED_RED_Pin|LED_GREEN_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(LED_RED_GPIO_Port, &GPIO_InitStruct);

  /*Configure GPIO pin : PA8 */
  GPIO_InitStruct.Pin = GPIO_PIN_8;
  GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
  HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

  /*Configure GPIO pins : RELAY_INT_A1_Pin RELAY_TRIG_Pin */
  GPIO_InitStruct.Pin = RELAY_INT_A1_Pin|RELAY_TRIG_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(RELAY_TRIG_GPIO_Port, &GPIO_InitStruct);

  /*Configure GPIO pin : IN_SWICH_Pin */
  GPIO_InitStruct.Pin = IN_SWICH_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  HAL_GPIO_Init(IN_SWICH_GPIO_Port, &GPIO_InitStruct);
}

/* USER CODE BEGIN 4 */

/* USER CODE END 4 */

/* USER CODE BEGIN Header_StartDefaultTask */
/**
  * @brief  Function implementing the defaultTask thread.
  * @param  argument: Not used
  * @retval None
  */
/* USER CODE END Header_StartDefaultTask */
void StartDefaultTask(void const * argument)
{
  /* init code for LWIP */
  MX_LWIP_Init();
  /* USER CODE BEGIN 5 */
  http_server_netconn_init();
  /* Infinite loop */
  for(;;)
  {
    osDelay(1);
  }
  /* USER CODE END 5 */
}
uint8_t flag_sost=0 ;
void set_relay(uint8_t sost)
{
if (sost==0)
{
  HAL_GPIO_WritePin(RELAY_INT_A1_GPIO_Port,RELAY_INT_A1_Pin,GPIO_PIN_RESET);
 
}
else
{
  HAL_GPIO_WritePin(RELAY_INT_A1_GPIO_Port,RELAY_INT_A1_Pin,GPIO_PIN_SET);

}
osDelay(100);
HAL_GPIO_WritePin(RELAY_TRIG_GPIO_Port,RELAY_TRIG_Pin,GPIO_PIN_SET);
osDelay(100);
HAL_GPIO_WritePin(RELAY_TRIG_GPIO_Port,RELAY_TRIG_Pin,GPIO_PIN_RESET);
}

/* USER CODE BEGIN Header_Task_HAL1 */
/**
* @brief Function implementing the LED_task thread.
* @param argument: Not used
* @retval None
*/
/* USER CODE END Header_Task_HAL1 */
uint8_t flag_sp=1;

void Task_HAL1(void const * argument)
{
  /* USER CODE BEGIN Task_HAL1 */
//HAL_GPIO_TogglePin(LED_RED_GPIO_Port,LED_RED_Pin);
  //  osDelay(1000);
  /* Infinite loop */
  for(;;)
  {

     if (metroData.metroTimerActive == 1)
    {
      HAL_GPIO_TogglePin(LED2_GPIO_type, LED2_GPIO_pin);
      METRO_Latch_Measures();
      METRO_Get_Measures();
      METRO_Update_Measures();
      metroData.metroTimerActive = 0;
    }
    
    
    
////    
////        osDelay(50*flag_sp);
////     HAL_GPIO_WritePin(LED_RED_GPIO_Port,LED_RED_Pin,GPIO_PIN_SET);
////	//  xPortGetFreeHeapSize();
//////HAL_GPIO_TogglePin(LED_RED_GPIO_Port,LED_RED_Pin);
////HAL_GPIO_TogglePin(LED_GREEN_GPIO_Port,LED_GREEN_Pin);

  }
  /* USER CODE END Task_HAL1 */
}

void Task_HAL2(void const * argument)
{
//  if(HAL_UART_Transmit(&huart4, (uint8_t*)(aTxBuffer0), 5, 100)!= HAL_OK)
//      {
//      //  Error_Handler();   
//      }
//     if(HAL_UART_Transmit(&huart4, (uint8_t*)(aTxBuffer0), 5, 100)!= HAL_OK)
//      {
//      //  Error_Handler();   
//      }
    

  for(;;)
  {
	//  xPortGetFreeHeapSize();
    if (HAL_GPIO_ReadPin(IN_SWICH_GPIO_Port,IN_SWICH_Pin)==0)
      {
        flag_sp=10;
        if (flag_sost==1)
        {  
         set_relay(1);
         flag_sost=0;
        }
      }
    else
      {
         if (flag_sost==0)
        {  
         set_relay(0);
         flag_sost=1;
        }
         flag_sp=1;
      }
    
    osDelay(40);
//////   
//////    if ( flag_tx==0)
//////    {
//////   //  flag_tx++;
//////    send_data_uart(aTxBuffer0);
//////    
//////    if(HAL_UART_Transmit(&huart4, (uint8_t*)(aTxBuffer0), 5, 100)!= HAL_OK)
//////      {
//////      //  Error_Handler();   
//////      }
////////     if(HAL_UART_Transmit(&huart4, (uint8_t*)(aTxBuffer0), 5, 100)!= HAL_OK)
////////      {
////////      //  Error_Handler();   
////////      }
//////    
//////    }
//////    
//////    if ( flag_tx==1)
//////    {
//////    // flag_tx++;
//////    send_data_uart(aTxBuffer1);
//////    
//////    if(HAL_UART_Transmit(&huart4, (uint8_t*)(aTxBuffer1), 5, 100)!= HAL_OK)
//////      {
//////      //  Error_Handler();   
//////      }
//////    
////////     if(HAL_UART_Transmit(&huart4, (uint8_t*)(aTxBuffer1), 5, 100)!= HAL_OK)
////////      {
////////      //  Error_Handler();   
////////      }
//////    
//////    }
//////    
    
    
    
    
//    if ( flag_tx==0)
//    {
//     flag_tx++;
//     send_data_uart(aTxBuffer0);
//    
//     if(HAL_UART_Transmit(&huart4, (uint8_t*)(&aTxBuffer0[0]), 1, 100)!= HAL_OK)
//      {
//        Error_Handler();   
//      }
//     HAL_UART_Receive(&huart4, (uint8_t *)(&aRxBuffer2[0]), 1, 100);
//     
//      if(HAL_UART_Transmit(&huart4, (uint8_t*)(&aTxBuffer0[1]), 1, 100)!= HAL_OK)
//      {
//        Error_Handler();   
//      }
//     HAL_UART_Receive(&huart4, (uint8_t *)(&aRxBuffer2[1]), 1, 100);
//     
//      if(HAL_UART_Transmit(&huart4, (uint8_t*)(&aTxBuffer0[2]), 1, 100)!= HAL_OK)
//      {
//        Error_Handler();   
//      }
//     HAL_UART_Receive(&huart4, (uint8_t *)(&aRxBuffer2[2]), 1, 100);
//     
//     
//      if(HAL_UART_Transmit(&huart4, (uint8_t*)(&aTxBuffer0[3]), 1, 100)!= HAL_OK)
//      {
//        Error_Handler();   
//      }
//     HAL_UART_Receive(&huart4, (uint8_t *)(&aRxBuffer2[3]), 1, 100);
//     
//      if(HAL_UART_Transmit(&huart4, (uint8_t*)(&aTxBuffer0[4]), 1, 100)!= HAL_OK)
//      {
//        Error_Handler();   
//      }
//     HAL_UART_Receive(&huart4, (uint8_t *)(&aRxBuffer2[4]), 1, 100);
//     
//    
//    }
//    
// if ( flag_tx==1)
//    {
//     flag_tx=0;
//    send_data_uart(aTxBuffer1);
//    
//         if(HAL_UART_Transmit(&huart4, (uint8_t*)(&aTxBuffer1[0]), 1, 100)!= HAL_OK)
//      {
//        Error_Handler();   
//      }
//     HAL_UART_Receive(&huart4, (uint8_t *)(&aRxBuffer3[0]), 1, 100);
//     
//      if(HAL_UART_Transmit(&huart4, (uint8_t*)(&aTxBuffer1[1]), 1, 100)!= HAL_OK)
//      {
//        Error_Handler();   
//      }
//     HAL_UART_Receive(&huart4, (uint8_t *)(&aRxBuffer3[1]), 1, 100);
//     
//      if(HAL_UART_Transmit(&huart4, (uint8_t*)(&aTxBuffer1[2]), 1, 100)!= HAL_OK)
//      {
//        Error_Handler();   
//      }
//     HAL_UART_Receive(&huart4, (uint8_t *)(&aRxBuffer3[2]), 1, 100);
//     
//     
//      if(HAL_UART_Transmit(&huart4, (uint8_t*)(&aTxBuffer1[3]), 1, 100)!= HAL_OK)
//      {
//        Error_Handler();   
//      }
//     HAL_UART_Receive(&huart4, (uint8_t *)(&aRxBuffer3[3]), 1, 100);
//     
//      if(HAL_UART_Transmit(&huart4, (uint8_t*)(&aTxBuffer1[4]), 1, 100)!= HAL_OK)
//      {
//        Error_Handler();   
//      }
//     HAL_UART_Receive(&huart4, (uint8_t *)(&aRxBuffer3[4]), 1, 100);
//     
//    }
//    
    
    
    
    
  
     osDelay(100);
  }
  /* USER CODE END Task_HAL1 */
}

 /**
  * @brief  Period elapsed callback in non blocking mode
  * @note   This function is called  when TIM1 interrupt took place, inside
  * HAL_TIM_IRQHandler(). It makes a direct call to HAL_IncTick() to increment
  * a global variable "uwTick" used as application time base.
  * @param  htim : TIM handle
  * @retval None
  */
//void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim)
//{
//  /* USER CODE BEGIN Callback 0 */
//
//  /* USER CODE END Callback 0 */
//  if (htim->Instance == TIM1) {
//    HAL_IncTick();
//  }
//  /* USER CODE BEGIN Callback 1 */
// metroData.metroTimerActive = 1;
//  /* USER CODE END Callback 1 */
//}

void MX_TIM3_Init(void)
{

  TIM_ClockConfigTypeDef sClockSourceConfig;
  TIM_MasterConfigTypeDef sMasterConfig;

  htim3.Instance = TIM3;
  htim3.Init.Prescaler = 0;
  htim3.Init.CounterMode = TIM_COUNTERMODE_UP;
  htim3.Init.Period = 0;
  htim3.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;
  HAL_TIM_Base_Init(&htim3);

  sClockSourceConfig.ClockSource = TIM_CLOCKSOURCE_INTERNAL;
  HAL_TIM_ConfigClockSource(&htim3, &sClockSourceConfig);

  sMasterConfig.MasterOutputTrigger = TIM_TRGO_RESET;
  sMasterConfig.MasterSlaveMode = TIM_MASTERSLAVEMODE_DISABLE;
  HAL_TIMEx_MasterConfigSynchronization(&htim3, &sMasterConfig);

}
static void Timer_Init()
{
    /*##-1- Configure the TIM peripheral #######################################*/
  /* -----------------------------------------------------------------------
    In this example TIM3 input clock (TIM3CLK)  is set to APB1 clock (PCLK1) x2,
    since APB1 prescaler is set to 4 (0x100).
       TIM3CLK = PCLK1*2
       PCLK1   = HCLK/2
    => TIM3CLK = PCLK1*2 = (HCLK/2)*2 = HCLK = SystemCoreClock
    To get TIM3 counter clock at 10 KHz, the Prescaler is computed as following:
    Prescaler = (TIM3CLK / TIM3 counter clock) - 1
    Prescaler = (SystemCoreClock /10 KHz) - 1

    Note:
     SystemCoreClock variable holds HCLK frequency and is defined in system_stm32f1xx.c file.
     Each time the core clock (HCLK) changes, user had to update SystemCoreClock
     variable value. Otherwise, any configuration based on this variable will be incorrect.
     This variable is updated in three ways:
      1) by calling CMSIS function SystemCoreClockUpdate()
      2) by calling HAL API function HAL_RCC_GetSysClockFreq()
      3) each time HAL_RCC_ClockConfig() is called to configure the system clock frequency
  ----------------------------------------------------------------------- */

  /* Compute the prescaler value to have TIMx counter clock equal to 10000 Hz */
  uwPrescalerValue = (uint32_t)(SystemCoreClock / 10000) - 1;

  /* Set TIMx instance */
  htim3.Instance = TIM3;

  /* Initialize TIMx peripheral as follows:
       + Period = 10000 - 1
       + Prescaler = (SystemCoreClock/10000) - 1
       + ClockDivision = 0
       + Counter direction = Up
  */
  htim3.Init.Period            = 10000 - 1;
  htim3.Init.Prescaler         = uwPrescalerValue;
  htim3.Init.ClockDivision     = 0;
  htim3.Init.CounterMode       = TIM_COUNTERMODE_UP;
  htim3.Init.RepetitionCounter = 0;

  if (HAL_TIM_Base_Init(&htim3) != HAL_OK)
  {
    /* Initialization Error */
    Error_Handler();
  }
  
}
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

/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.c
  * @brief          : Main program body
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; Copyright (c) 2020 STMicroelectronics.
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
#include "ethernetif.h"
#include "lwip/netif.h"
#include "lwip/tcpip.h"
#include "flash_if.h"
//#include "heap_5.h"
#include "httpserver-netconn.h"
#include "lwip.h"
#include "dns.h"
#include "LOGS.h"
#include "html_page.h"
#include <stdio.h>
/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#define VECT_TAB_OFFSET 0x08005000
#define logoff_time 200*10
/* USER CODE END Includes */

#pragma segment="HEAP"
/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */

/* USER CODE END PTD */
////__root const uint16_t  CRC_APP  @ A_CRC_APP;
////__root const uint16_t  CRC_BOOT  @ A_CRC_BOOT;
////__root const uint8_t IP_CONFIG[4] @ A_IP_ADRESS;
////
////__root const uint8_t IP_MASK[4] @ A_IP_MASK;
////
////__root const uint8_t IP_GET[4] @ A_GET_ADRESS;
////
////__root const uint8_t FW1_VER[4] @ A_FW1_VER ;
////
////__root const uint32_t FW1_LEN @ A_FW1_LEN ;
////
////__root const uint8_t FW_LOADER_VER[4] @ A_FW_LOADER_VER;
////__root const char LOG[2000] @ A_LOG;
/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */
/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/
RTC_HandleTypeDef hrtc;

osThreadId defaultTaskHandle;
osThreadId LED_taskHandle;
osThreadId logs_task_nameHandle;
osThreadId  IO_CNTRLHandle;
uint8_t flag_set_ip=0;
uint16_t ct_dns_time=0;
uint8_t status_dns=0;
HeapRegion_t pxHeapRegions_f107;
/* USER CODE BEGIN PV */

/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
static void MX_GPIO_Init(void);
static void MX_RTC_Init(void);
void StartDefaultTask(void const * argument);
void Task_HAL1(void const * argument);
void logs_task(void const * argument);
void IO_CNRL_APP(void const * argument);
void set_out_port(uint8_t sost,uint8_t canal);
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
  SCB->VTOR = FLASH_BASE | VECT_TAB_OFFSET;
  
//void vPortDefineHeapRegions( const HeapRegion_t * const pxHeapRegions )
 FLASH_If_Init();
  /* USER CODE END 2 */
 load_struct_flash_data();
 
   MX_GPIO_Init();
 
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

  
   pxHeapRegions_f107.pucStartAddress=(uint8_t*)(__segment_begin( "HEAP"));
  pxHeapRegions_f107.xSizeInBytes= (size_t)((unsigned char *)__segment_end( "HEAP") - (unsigned char *)__segment_begin( "HEAP")); 
 
//  pxHeapRegions_f107->pucStartAddress = (uint8_t*)(__segment_begin( "HEAP"));
//  pxHeapRegions_f107->xSizeInBytes = (size_t)((unsigned char *)__segment_end( "HEAP") - (unsigned char *)__segment_begin( "HEAP")); 
  vPortDefineHeapRegions(&pxHeapRegions_f107);
  /* USER CODE BEGIN 1 */
// init_system_heap();
  /* USER CODE END 1 */

 

 
  MX_RTC_Init();
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
  osThreadDef(defaultTask, StartDefaultTask, osPriorityNormal, 0, 256);
  defaultTaskHandle = osThreadCreate(osThread(defaultTask), NULL);

  /* definition and creation of LED_task */
  osThreadDef(LED_task, Task_HAL1, osPriorityIdle, 0, 256);
  LED_taskHandle = osThreadCreate(osThread(LED_task), NULL);

  osThreadDef(logs_task_name, logs_task, osPriorityLow, 0, 256);
  logs_task_nameHandle = osThreadCreate(osThread(logs_task_name), NULL);
  /* USER CODE BEGIN RTOS_THREADS */
  /* add threads, ... */
  /* USER CODE END RTOS_THREADS */
  /* definition and creation of IO_CNTRL */
 osThreadDef(IO_CNTRL, IO_CNRL_APP, osPriorityLow, 0, 128);
 IO_CNTRLHandle = osThreadCreate(osThread(IO_CNTRL), NULL);
 
 
  
  //void GET_reple (uint8_t event,log_reple_t* reple)
   form_reple_to_save(POWER_ON);
  GET_reple(0,&start_time);
  
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
  RCC_OscInitStruct.HSEPredivValue = RCC_HSE_PREDIV_DIV8;
  RCC_OscInitStruct.LSEState = RCC_LSE_ON;
  RCC_OscInitStruct.HSIState = RCC_HSI_ON;
  RCC_OscInitStruct.Prediv1Source = RCC_PREDIV1_SOURCE_PLL2;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSE;
  RCC_OscInitStruct.PLL.PLLMUL = RCC_PLL_MUL8;
  RCC_OscInitStruct.PLL2.PLL2State = RCC_PLL2_ON;
  RCC_OscInitStruct.PLL2.PLL2MUL = RCC_PLL2_MUL10;
  RCC_OscInitStruct.PLL2.HSEPrediv2Value = RCC_HSE_PREDIV2_DIV5;
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

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_2) != HAL_OK)
  {
    Error_Handler();
  }
  PeriphClkInit.PeriphClockSelection = RCC_PERIPHCLK_RTC;
  PeriphClkInit.RTCClockSelection = RCC_RTCCLKSOURCE_LSE;
  if (HAL_RCCEx_PeriphCLKConfig(&PeriphClkInit) != HAL_OK)
  {
    Error_Handler();
  }
  HAL_RCC_MCOConfig(RCC_MCO, RCC_MCO1SOURCE_PLL2CLK, RCC_MCODIV_1);
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
  
  
  
  HAL_RTC_GetTime (&hrtc,&sTime,RTC_FORMAT_BIN);
  
  
//  sTime.Hours = 0x10;
//  sTime.Minutes = 0x12;
//  sTime.Seconds = 0x22;

  if (HAL_RTC_SetTime(&hrtc, &sTime, RTC_FORMAT_BIN) != HAL_OK)
  {
    Error_Handler();
  }
  
  
  HAL_RTC_GetDate (&hrtc,&DateToUpdate,RTC_FORMAT_BIN);
//  DateToUpdate.WeekDay = RTC_WEEKDAY_FRIDAY;
//  DateToUpdate.Month = RTC_MONTH_DECEMBER;
//  DateToUpdate.Date = 0x12;
if (DateToUpdate.Year <20)
{
  DateToUpdate.Year = 20;
}
  if (HAL_RTC_SetDate(&hrtc, &DateToUpdate, RTC_FORMAT_BIN) != HAL_OK)
  {
    Error_Handler();
  }
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
  HAL_GPIO_WritePin(GPIOC, LED_RED_Pin, GPIO_PIN_RESET);
  
  GPIO_InitStruct.Pin = LED_GREEN_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOC, &GPIO_InitStruct);
  
  //set_out_port(HAL_RTCEx_BKUPRead(&hrtc,1),1);
  if (HAL_RTCEx_BKUPRead(&hrtc,1)==0)
  {
    if (FW_data.V_TYPE_OUT==0)
    {
      HAL_GPIO_WritePin(LED_GREEN_GPIO_Port,LED_GREEN_Pin,0);    
    }
    if (FW_data.V_TYPE_OUT==1)
    {
      HAL_GPIO_WritePin(LED_GREEN_GPIO_Port,LED_GREEN_Pin,1);    
    }    
  }
  
  if (HAL_RTCEx_BKUPRead(&hrtc,1)==1)
  {
    if (FW_data.V_TYPE_OUT==0)
    {
      HAL_GPIO_WritePin(LED_GREEN_GPIO_Port,LED_GREEN_Pin,1);    
    }
    if (FW_data.V_TYPE_OUT==1)
    {
      HAL_GPIO_WritePin(LED_GREEN_GPIO_Port,LED_GREEN_Pin,0);    
    }    
  }
  
  /*Configure GPIO pins : LED_GREEN_Pin LED_RED_Pin */
 
  
  /*Configure GPIO pin : PHY_RST_Pin */
  GPIO_InitStruct.Pin = PHY_RST_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_OD;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_MEDIUM;
  HAL_GPIO_Init(PHY_RST_GPIO_Port, &GPIO_InitStruct);
  
   /*Configure GPIO pins : LED_GREEN_Pin LED_RED_Pin */
  GPIO_InitStruct.Pin = LED_RED_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOC, &GPIO_InitStruct);

  /*Configure GPIO pin : PA8 */
  GPIO_InitStruct.Pin = GPIO_PIN_8;
  GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
  HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

}

/* USER CODE BEGIN 4 */

/* USER CODE END 4 */
void smtp_serverFound (const char * name, struct ip_addr * ipaddr, void * arg) 
{ 
//  if ((ipaddr) && (ipaddr-> addr)) 
//  { 
//    smtp.serverIP.addr = ipaddr-> addr; 
//    smtp.state = SMTP_NAME_RESOLVED; 
//    ???? (smtp_connect () == ERR_OK) 
//      ???????; 
//    smtp.lastError = SMTP_CONNECT_FAILED; 
//  } 
//  ??? 
//    smtp.lastError = SMTP_UNKNOWN_HOST; 
//  smtp.state = SMTP_IDLE; 
//  while (1)
//  {
//      osDelay(10);
//  }
}
/* USER CODE BEGIN Header_StartDefaultTask */
/**
  * @brief  Function implementing the defaultTask thread.
  * @param  argument: Not used
  * @retval None
  */
/* USER CODE END Header_StartDefaultTask */
void StartDefaultTask(void const * argument)
{
ip4_addr_t ipdns1;
  /* init code for LWIP */
  MX_LWIP_Init();
  /* USER CODE BEGIN 5 */
    //  udpecho_init();
  http_server_netconn_init();
  /* Infinite loop */
  for(;;)
  {
    osDelay(1);
   if (ct_dns_time<DNS_TMR_INTERVAL) 
    {
    
    dns_tmr();
   
    ct_dns_time=0;
    }
   else
   {
   ct_dns_time++;
   }
    if ((gnetif.ip_addr.addr!=0)&&(flag_set_ip!=1))
      {
       FW_data.V_IP_CONFIG[0]=gnetif.ip_addr.addr&0x000000ff;
       FW_data.V_IP_CONFIG[1]=(gnetif.ip_addr.addr&0x0000ff00)>>8;
       FW_data.V_IP_CONFIG[2]=(gnetif.ip_addr.addr&0x00ff0000)>>16;
       FW_data.V_IP_CONFIG[3]=(gnetif.ip_addr.addr&0xff000000)>>24;
       FW_data.V_IP_MASK[0]=(gnetif.netmask.addr&0x000000ff);
       FW_data.V_IP_MASK[1]=(gnetif.netmask.addr&0x0000ff00)>>8;
       FW_data.V_IP_MASK[2]=(gnetif.netmask.addr&0x00ff0000)>>16;
       FW_data.V_IP_MASK[3]=(gnetif.netmask.addr&0xff000000)>>24;
       FW_data.V_IP_GET[0]=(gnetif.gw.addr&0x000000ff);
       FW_data.V_IP_GET[1]=(gnetif.gw.addr&0x0000ff00)>>8;
       FW_data.V_IP_GET[2]=(gnetif.gw.addr&0x00ff0000)>>16;
       FW_data.V_IP_GET[3]=(gnetif.gw.addr&0xff000000)>>24;
       flag_set_ip=1;

     //  struct ip_addr resolved;
//   if (flag_set_ip==1)
//      {
//      status_dns= dns_gethostbyname("www.netsmartswich.com", (ip_addr_t*)&(gnetif.ip_addr.addr), (dns_found_callback) smtp_serverFound, NULL);
//      }
      
     
//        {}
       snmp_ex_init();
      }
//    ipdns1=*(dns_getserver (0));
//   FW_data.V_IP_DNS[0]=(ipdns1.addr&0x000000ff);
//       FW_data.V_IP_DNS[1]=(ipdns1.addr&0x0000ff00)>>8;
//       FW_data.V_IP_DNS[2]=(ipdns1.addr&0x00ff0000)>>16;
//       FW_data.V_IP_DNS[3]=(ipdns1.addr&0xff000000)>>24;
//   
  }
  /* USER CODE END 5 */
}
void set_out_port(uint8_t sost,uint8_t canal)
{
  if (canal==1)
  {
  if (sost==0)
  {
    if (FW_data.V_TYPE_OUT==0)
    {
      HAL_GPIO_WritePin(LED_GREEN_GPIO_Port,LED_GREEN_Pin,0);    
    }
    if (FW_data.V_TYPE_OUT==1)
    {
      HAL_GPIO_WritePin(LED_GREEN_GPIO_Port,LED_GREEN_Pin,1);    
    }    
  }
  
  if (sost==1)
  {
    if (FW_data.V_TYPE_OUT==0)
    {
      HAL_GPIO_WritePin(LED_GREEN_GPIO_Port,LED_GREEN_Pin,1);    
    }
    if (FW_data.V_TYPE_OUT==1)
    {
      HAL_GPIO_WritePin(LED_GREEN_GPIO_Port,LED_GREEN_Pin,0);    
    }    
  }
  
  
  if (sost>1)
  {
    if ((FW_data.V_TYPE_OUT==2)||(FW_data.V_TYPE_OUT==1)||(FW_data.V_TYPE_OUT==0))
    {
      HAL_GPIO_WritePin(LED_GREEN_GPIO_Port,LED_GREEN_Pin,0);    
      osDelay(1000);
      HAL_GPIO_WritePin(LED_GREEN_GPIO_Port,LED_GREEN_Pin,1);    
    }
    
    if (FW_data.V_TYPE_OUT==3)
    {
      HAL_GPIO_WritePin(LED_GREEN_GPIO_Port,LED_GREEN_Pin,1);    
      osDelay(1000);
      HAL_GPIO_WritePin(LED_GREEN_GPIO_Port,LED_GREEN_Pin,0); 
    }    
  }
  
  
 // void HAL_GPIO_WritePin(GPIO_TypeDef *GPIOx, uint16_t GPIO_Pin, GPIO_PinState PinState)
  }  
    
 //HAL_GPIO_TogglePin(LED_GREEN_GPIO_Port,LED_GREEN_Pin);    

}
/* USER CODE END Header_IO_CNRL_APP */
void IO_CNRL_APP(void const * argument)
{
 uint16_t data=0;
 /* USER CODE BEGIN IO_CNRL_APP */
 /* Infinite loop */
 for(;;)
 {
//   uint32_t OID_out[]={1,3,6,1,4,1,2022,1,1};
   uint32_t OID_out[]={1,2020,1,0};

   if ((flag_global_swich_out==SWICH_ON_WEB)||(flag_global_swich_out==SWICH_OFF_WEB)||(flag_global_swich_out==SWICH_TOLG_WEB)||
       (flag_global_swich_out==SWICH_ON_SNMP)||(flag_global_swich_out==SWICH_OFF_SNMP)||(flag_global_swich_out==SWICH_TOLG_SNMP))
   {


      data= HAL_RTCEx_BKUPRead(&hrtc,1);

     
      if(data==0)
        {
          send_mess_trap(OID_out,FW_data.V_ON_MESS,strlen(FW_data.V_ON_MESS));
          set_out_port(data,1);
          flag_global_swich_out=0;
        }
      
        if(data==1)
        {
          OID_out[3]=2;
          send_mess_trap(OID_out,FW_data.V_ON_MESS,strlen(FW_data.V_ON_MESS));
          send_mess_trap();
            set_out_port(data,1);
            flag_global_swich_out=0;
        }
       if (data>1)
       {
         OID_out[3]=3;
         send_mess_trap(OID_out,FW_data.V_ON_MESS,strlen(FW_data.V_ON_MESS));
         send_mess_trap();
          set_out_port(2,1); 
          flag_global_swich_out=0;
       }
   }
  
  
  
   osDelay(100);
 }
 /* USER CODE END IO_CNRL_APP */
}
/* USER CODE BEGIN Header_Task_HAL1 */
/**
* @brief Function implementing the LED_task thread.
* @param argument: Not used
* @retval None
*/
/* USER CODE END Header_Task_HAL1 */
void Task_HAL1(void const * argument)
{
 /// log_reple_t reple1;
 
  
  
  /* USER CODE BEGIN Task_HAL1 */
  /* Infinite loop */
  for(;;)
  {
   //  termIO();
    if (flag_logon==1)
      {
       if (ct_logoff_time<logoff_time) 
       {
        ct_logoff_time++;
       }
       else
       {
        flag_logon=0;
        ct_logoff_time=0;
       }
      }
    else
      {
       ct_logoff_time=0;
      }
  ///   GET_reple(SWICH_ON_WEB,&reple1);
     HAL_GPIO_TogglePin (LED_RED_GPIO_Port, LED_RED_Pin);
//     save_reple_log(reple1);
     osDelay(100);
     
  }
  /* USER CODE END Task_HAL1 */
}
void logs_task(void const * argument)
{
  /* USER CODE BEGIN logs_task */
  /* Infinite loop */
  for(;;)
  {
    if (flag_global_save_log==1)
      {
        save_reple_log(reple_to_save);
        flag_global_save_log=0;
      }
     if ((flag_global_save_data==1)&&(flag_global_save_log==0))
      {
      // save  all data        
       save_data_flash();
       flag_global_save_data=0;
 //       jamp_to_app();
      }
     if ((flag_global_load_def==1)&&(flag_global_save_log==0))
      { // load_def_data
        
        load_def_data();      
        flag_global_load_def=0;
      }
      if ((flag_global_boot_mode==1)&&(flag_global_save_log==0))
      { // load_def_data
        vTaskDelay(1000);
       jamp_to_boot();
      
        flag_global_boot_mode=0;
      }
//      if ((flag_global_reset_mode==1)&&(flag_global_save_log==0))
//      { // load_def_data
//        vTaskDelay(1000);
//       jamp_to_app();
//      
//        flag_global_reset_mode=0;
//      }
    
  
       
    
    
    
    osDelay(100);
  }
  /* USER CODE END logs_task */
}
 /**
  * @brief  Period elapsed callback in non blocking mode
  * @note   This function is called  when TIM1 interrupt took place, inside
  * HAL_TIM_IRQHandler(). It makes a direct call to HAL_IncTick() to increment
  * a global variable "uwTick" used as application time base.
  * @param  htim : TIM handle
  * @retval None
  */
void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim)
{
  /* USER CODE BEGIN Callback 0 */

  /* USER CODE END Callback 0 */
  if (htim->Instance == TIM1) {
    HAL_IncTick();
  }
  /* USER CODE BEGIN Callback 1 */

  /* USER CODE END Callback 1 */
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

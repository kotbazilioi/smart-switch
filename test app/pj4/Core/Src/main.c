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
#include "ping.h"
#include "hard_config.h"
#include "app.h"
#include "ntp.h"
#include <stdio.h>
#define QUEUE_SIZE (uint32_t) 1    

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */


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

osMessageQId timeout_Queue;
HeapRegion_t pxHeapRegions_f107;
/* USER CODE BEGIN PV */

/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/


/* USER CODE BEGIN PFP */

/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */

/* USER CODE END 0 */
uint32_t sets = PBUF_POOL_BUFSIZE;

/**
  * @brief  The application entry point.
  * @retval int
  */
int main(void)
{
  SCB->VTOR = FLASH_BASE | VECT_TAB_OFFSET;
  sets++;
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
//  pxHeapRegions_f107.xSizeInBytes= 0x2000ffff/*(size_t)((unsigned char *)__segment_end( "HEAP")*/ - (unsigned char *)__segment_begin( "HEAP")); 
   pxHeapRegions_f107.xSizeInBytes= (size_t)((unsigned char *)0x2000ffff - (unsigned char *)__segment_begin( "HEAP")); 
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
  osThreadDef(defaultTask, StartDefaultTask, osPriorityNormal, 0, DEFAULT_THREAD_STACKSIZE);
  defaultTaskHandle = osThreadCreate(osThread(defaultTask), NULL);

  
  osMessageQDef(timeout_Queue, QUEUE_SIZE, uint16_t);

  timeout_Queue = osMessageCreate(osMessageQ(timeout_Queue), NULL);
 
   osThreadDef(LED_task, LED_task, osPriorityLow, 0, DEFAULT_THREAD_STACKSIZE);
  LED_taskHandle = osThreadCreate(osThread(LED_task), NULL);

  osThreadDef(logs_task_name, logs_task, osPriorityLow, 0, DEFAULT_THREAD_STACKSIZE);
  logs_task_nameHandle = osThreadCreate(osThread(logs_task_name), NULL);
  /* USER CODE BEGIN RTOS_THREADS */
  /* add threads, ... */
  /* USER CODE END RTOS_THREADS */
  /* definition and creation of IO_CNTRL */
 osThreadDef(IO_CNTRL, IO_CNRL_APP, osPriorityLow, 0, DEFAULT_THREAD_STACKSIZE);
 IO_CNTRLHandle = osThreadCreate(osThread(IO_CNTRL), NULL);
 

  osThreadDef(rasp_task, rasp_task, osPriorityLow, 0, DEFAULT_THREAD_STACKSIZE);
  rasp_task_id = osThreadCreate(osThread(rasp_task), NULL);
  
#ifdef IWDT_EN  
  osThreadDef(iwdt_task, iwdt_task, osPriorityRealtime, 0, DEFAULT_THREAD_STACKSIZE);
  iwdt_task_id = osThreadCreate(osThread(iwdt_task), NULL); 
#endif


 osThreadDef(ntp_thread, ntp_thread, osPriorityLow, 0, DEFAULT_THREAD_STACKSIZE);
 ntp_task_id = osThreadCreate(osThread(ntp_thread), NULL);
  //void GET_reple (uint8_t event,log_reple_t* reple)

  
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


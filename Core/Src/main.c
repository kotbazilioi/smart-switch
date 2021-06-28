/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.c
  * @brief          : Main program body
  ******************************************************************************/

/* Includes ------------------------------------------------------------------*/
#include "main.h"
#include "cmsis_os.h"
#include "ethernetif.h"
#include "lwip/netif.h"
#include "lwip/tcpip.h"
#include "flash_if.h"
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
#include "heap_5.h"
#define QUEUE_SIZE (uint32_t) 1    

#pragma segment="HEAP"

//******************************************************************//

//******************************************************************//

osMessageQId timeout_Queue;
HeapRegion_t pxHeapRegions_f107;
uint32_t sets = PBUF_POOL_BUFSIZE;

/**
  * @brief  The application entry point.
  * @retval int
  */
int main(void)
{
  SCB->VTOR = FLASH_BASE | VECT_TAB_OFFSET;
  sets++;
  FLASH_If_Init();
   MX_GPIO_Init();
  load_struct_flash_data();

  
  
  FW_data.V_FW1_VER[0]=Platform;
  FW_data.V_FW1_VER[1]=rev;
  FW_data.V_FW1_VER[2]=Assembly;
  FW_data.V_FW1_VER[3]=Bild;

  HAL_Init();
  SystemClock_Config();
 

  pxHeapRegions_f107.pucStartAddress=(uint8_t*)(__segment_begin( "HEAP"));
  pxHeapRegions_f107.xSizeInBytes= (size_t)((unsigned char *)0x2000ffff - (unsigned char *)__segment_begin( "HEAP")); 
  

  vPortDefineHeapRegions(&pxHeapRegions_f107);
  

 
  MX_RTC_Init();

  osThreadDef(defaultTask, StartDefaultTask, osPriorityNormal, 0, DEFAULT_THREAD_STACKSIZE);
  defaultTaskHandle = osThreadCreate(osThread(defaultTask), NULL);
  
  osMessageQDef(timeout_Queue, QUEUE_SIZE, uint16_t);
  timeout_Queue = osMessageCreate(osMessageQ(timeout_Queue), NULL); 
  
  osThreadDef(LED_task, LED_task, osPriorityLow, 0, DEFAULT_THREAD_STACKSIZE);
  LED_taskHandle = osThreadCreate(osThread(LED_task), NULL);
  
  osThreadDef(logs_task_name, logs_task, osPriorityLow, 0, DEFAULT_THREAD_STACKSIZE);
  logs_task_nameHandle = osThreadCreate(osThread(logs_task_name), NULL);
  
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
    
  /* Start scheduler */
  osKernelStart();
 
  
  while (1)
  {
  
  }
  
}


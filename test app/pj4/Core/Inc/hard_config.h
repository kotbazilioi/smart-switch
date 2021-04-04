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

#include <stdio.h>

void SystemClock_Config(void);
void MX_RTC_Init(void);
void MX_GPIO_Init(void);
void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim);
void Error_Handler(void);
extern RTC_HandleTypeDef hrtc;
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
#include <stdio.h>


void StartDefaultTask(void const * argument);
void LED_task(void const * argument);
void logs_task(void const * argument);
void IO_CNRL_APP(void const * argument);
void set_out_port(uint8_t sost,uint8_t canal);
void rasp_task(void const * argument);


extern osThreadId defaultTaskHandle;
extern osThreadId LED_taskHandle;
extern osThreadId logs_task_nameHandle;
extern osThreadId  IO_CNTRLHandle;
extern osThreadId rasp_task_id;
extern   uint8_t flag_err_dates;
extern uint8_t flag_set_ip;
extern char mess_smtp[256]; 
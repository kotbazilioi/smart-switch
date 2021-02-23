/**
  ******************************************************************************
  * @file    IAP_Main/Inc/flash_if.h 
  * @author  MCD Application Team
  * @brief   This file provides all the headers of the flash_if functions.
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; Copyright (c) 2016 STMicroelectronics.
  * All rights reserved.</center></h2>
  *
  * This software component is licensed by ST under BSD 3-Clause license,
  * the "License"; You may not use this file except in compliance with the
  * License. You may obtain a copy of the License at:
  *                        opensource.org/licenses/BSD-3-Clause
  *
  ******************************************************************************
  */

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __FLASH_IF_H
#define __FLASH_IF_H

/* Includes ------------------------------------------------------------------*/
#include "stm32f1xx_hal.h"

/* Exported types ------------------------------------------------------------*/
/* Exported constants --------------------------------------------------------*/

/* Base address of the Flash sectors */
#define ADDR_START_APP     ((uint32_t)0x08005000) 
#define ADDR_END_APP     ((uint32_t)0x08020000) 

////#define A_CRC_APP 0X8004000
////#define A_CRC_BOOT 0X8004002
////#define A_IP_ADRESS 0X8004004
////#define A_IP_MASK 0X8004008
////#define A_GET_ADRESS 0X800400c
////#define A_FW1_VER 0X8004010
////#define A_FW1_LEN 0X8004014
////#define A_FW_LOADER_VER 0X8004018
////#define A_LOG 0X800401C

#define A_START_BOOT 0x08000000
#define A_BOOT_FW 0x08000004
#define A_CRC_APP 0x8004000
#define A_CRC_DATA_BOOT 0x8004002
//*******************************
#define A_IP_ADRESS 0x8004004
#define A_IP_MASK 0x8004008
#define A_GET_ADRESS 0x800400c
#define A_FW_VER 0x8004010
#define A_FW_LEN 0x8004014
#define A_BOOT_VER 0x8004018
#define A_CRC_DATA 0x800401C
#define A_DHCP 0x800401E
//*******************************
#define A_LOGIN A_DHCP+2
#define A_PASSWORD A_LOGIN+16 
#define A_DNS A_PASSWORD+16
#define A_WEB_PORT A_DNS+4 
#define A_SNMP A_WEB_PORT+4 
#define A_HTTP_IP A_SNMP+4
#define A_ON_MESS A_HTTP_IP+2
#define A_OFF_MESS A_ON_MESS+32
#define A_FLAG_EN_MAN A_OFF_MESS+32
#define A_FLAG_EN_WEB A_FLAG_EN_MAN+1
#define A_FLAG_EN_RASP A_FLAG_EN_WEB+1
#define A_FLAG_EN_WATCHMAN A_FLAG_EN_RASP+1
#define A_EMAIL_ERR A_FLAG_EN_WATCHMAN+1
#define A_D_TIME A_EMAIL_ERR+32

#define A_RD1_TIME A_D_TIME+40

#define A_IP_PING A_RD1_TIME+12
#define A_TIME_SEND A_IP_PING+4
#define A_TIME_READ A_TIME_SEND+2
#define A_N_RERID A_TIME_READ+2
#define A_TIME_RESET A_N_RERID+2
#define A_DELAY_PING A_TIME_RESET+2
#define A_SOST_RESET A_DELAY_PING+2
#define A_N_PING A_SOST_RESET+1
#define A_IP_NTP1 A_N_PING+2
#define A_IP_NTP2 A_IP_NTP1+4
#define A_PORT_NTP A_IP_NTP2+4
#define A_NAME_SMTP A_PORT_NTP+2
#define A_PORT_SNMP A_NAME_SMTP+32
#define A_LOGIN_SMTP A_PORT_SNMP+2
#define A_PASSWORD_SMTP A_LOGIN_SMTP+16
#define A_GEOM_NAME A_PASSWORD_SMTP+16  
#define A_ID_MAC A_GEOM_NAME+32        
#define A_NAME_DEV A_ID_MAC+8
#define A_CALL_DATA A_NAME_DEV+16
#define A_IP_SNMP A_CALL_DATA+16
#define A_NTP_CIRCL A_IP_SNMP+4 
#define A_TYPE_OUT A_NTP_CIRCL+1
#define A_RESV V_TYPE_OUT+1
    
    
    
    
    
#define A_CRC_LOG 0x8004800
#define A_LOG A_CRC_LOG+2
#define A_START_APP 0x8005000
#define A_FW_APP 0x8005004
#define A_HTTP_DUMP 0x8005000
#define A_END_APP     ((uint32_t)0x08020000) 

#define BOOT_VER_FW 0X00010001



enum logs_events_t {
  NO_RUN,
  RESETL,
  UPDATE_FW,
  SWICH_ON_WEB,
  SWICH_OFF_WEB,
  SWICH_TOLG_WEB,
  SWICH_ON_SNMP,
  SWICH_OFF_SNMP,
  SWICH_TOLG_SNMP,
  SWICH_ON_RASP,
  SWICH_OFF_RASP,
  SWICH_TOLG_RASP,
  SWICH_ON_WATCH,
  SWICH_OFF_WATCH,
  SWICH_TOLG_WATCH,  
  SWICH_ON_HTTP,
  SWICH_OFF_HTTP,
  SWICH_TOLG_HTTP,
  POWER_ON,
  LOAD_DEF_DATA, 
  SAVE_DATA_SETT  
};


enum dweek_t 
{
Mon,
Ts,
Wed,
Th,
Fri,
Sat,
Sun 
};
/* Error code */
enum 
{
  FLASHIF_OK = 0,
  FLASHIF_ERASEKO,
  FLASHIF_WRITINGCTRL_ERROR,
  FLASHIF_WRITING_ERROR,
  FLASHIF_PROTECTION_ERRROR
};

/* protection type */  
enum{
  FLASHIF_PROTECTION_NONE         = 0,
  FLASHIF_PROTECTION_PCROPENABLED = 0x1,
  FLASHIF_PROTECTION_WRPENABLED   = 0x2,
  FLASHIF_PROTECTION_RDPENABLED   = 0x4,
};

/* protection update */
enum {
	FLASHIF_WRP_ENABLE,
	FLASHIF_WRP_DISABLE
};

/* Define the address from where user application will be loaded.
   Note: this area is reserved for the IAP code                  */
#define FLASH_PAGE_STEP         FLASH_PAGE_SIZE           /* Size of page : 2 Kbytes */
#define APPLICATION_ADDRESS     ADDR_START_APP      /* Start user code address: ADDR_FLASH_PAGE_8 */

/* Notable Flash addresses */
#define USER_FLASH_BANK1_START_ADDRESS  0x08000000
#define USER_FLASH_BANK2_START_ADDRESS  0x08080000
#define USER_FLASH_BANK1_END_ADDRESS    FLASH_BANK1_END
#define USER_FLASH_BANK2_END_ADDRESS    FLASH_BANK2_END

/* Define the user application size */
#define USER_FLASH_SIZE               ((uint32_t)0x00003000) /* Small default template application */

/* Define bitmap representing user flash area that could be write protected (check restricted to pages 8-39). */
#define FLASH_PAGE_TO_BE_PROTECTED (OB_WRP_PAGES8TO9 | OB_WRP_PAGES10TO11 | OB_WRP_PAGES12TO13 | OB_WRP_PAGES14TO15 | \
                                    OB_WRP_PAGES16TO17 | OB_WRP_PAGES18TO19 | OB_WRP_PAGES20TO21 | OB_WRP_PAGES22TO23 | \
                                    OB_WRP_PAGES24TO25 | OB_WRP_PAGES26TO27 | OB_WRP_PAGES28TO29 | OB_WRP_PAGES30TO31 | \
                                    OB_WRP_PAGES32TO33 | OB_WRP_PAGES34TO35 | OB_WRP_PAGES36TO37 | OB_WRP_PAGES38TO39  )  


/* Exported macro ------------------------------------------------------------*/
/* ABSoulute value */
#define ABS_RETURN(x,y)               (((x) < (y)) ? (y) : (x))

/* Get the number of sectors from where the user program will be loaded */
#define FLASH_SECTOR_NUMBER           ((uint32_t)(ABS_RETURN(APPLICATION_ADDRESS,FLASH_START_BANK1))>>12)

/* Compute the mask to test if the Flash memory, where the user program will be
  loaded, is write protected */
#define FLASH_PROTECTED_SECTORS       (~(uint32_t)((1 << FLASH_SECTOR_NUMBER) - 1))
/* Exported functions ------------------------------------------------------- */
void FLASH_If_Init(void);
uint32_t FLASH_If_Erase(uint32_t StartSector);
uint32_t FLASH_If_GetWriteProtectionStatus(void);
uint32_t FLASH_If_Write(uint32_t destination, uint32_t *p_source, uint32_t length);
uint32_t FLASH_If_WriteProtectionConfig(uint32_t protectionstate);
extern uint8_t flag_app_start;
extern   uint32_t timer_to_app;

extern uint16_t *idBase0;
extern uint16_t *idBase1;
extern uint32_t *idBase2;
extern  uint32_t *idBase3;

typedef struct 
{
uint16_t nblock_in;
uint32_t nblock_inflash;
uint16_t nframe16;
uint32_t size_file;
uint16_t temp_block_flash;
uint16_t temp_block;
uint16_t index;
uint8_t data_to_block[2048];
}file_data_t;

typedef struct 
{
uint8_t dicr ;//0
uint8_t type_event;//1
uint8_t reple_hours;//2
uint8_t reple_minuts;//3
uint8_t reple_seconds;//4
uint8_t day;//5
uint8_t dweek;//6
uint8_t month;//7
uint16_t year;//9

}log_reple_t;

typedef struct 
{
 uint16_t CRC16;
 log_reple_t log_reple[200];
} logs_t;

typedef struct 
{ 
uint16_t  V_CRC_APP  ;
////#define A_CRC_APP 0x8004000
uint16_t  V_CRC_BOOT  ;
////#define A_CRC_DATA_BOOT 0x8004002
//////*******************************
uint8_t V_IP_CONFIG[4] ;
////#define A_IP_ADRESS 0x8004004
uint8_t V_IP_MASK[4] ;
////#define A_IP_MASK 0x8004008
uint8_t V_IP_GET[4] ;
////#define A_GET_ADRESS 0x800400c
uint8_t V_FW1_VER[4] ;
////#define A_FW_VER 0x8004010
uint32_t V_FW1_LEN ;
////#define A_FW_LEN 0x8004014
uint32_t V_BOOT_VER ;
////#define A_BOOT_VER 0x8004018


//*******************************
uint16_t V_CRC_DATA;
////#define A_CRC_DATA 0x800401C

uint8_t V_DHCP;
////#define A_DHCP 0x800401C
char V_LOGIN[16];
////#define A_LOGIN 0x8004021
char V_PASSWORD[16];
////#define A_PASSWORD 0x8004031
uint8_t V_IP_DNS[4] ;
////#define A_DNS 0x8004041
uint16_t V_WEB_PORT;
////#define A_WEB_PORT 0x8004045
uint16_t V_SNMP;
////#define A_SNMP 0x8004047
uint16_t V_HTTP_IP; 
////#define A_HTTP_IP 0x8004049
char V_ON_MESS[32];
////#define A_ON_MESS 0x800404B
char V_OFF_MESS[32];
////#define A_OFF_MESS 0x800406B
uint8_t V_FLAG_EN_MAN;
////#define A_FLAG_EN_MAN 0x800408B
uint8_t V_FLAG_EN_WEB;
////#define A_FLAG_EN_WEB 0x800408C
uint8_t V_FLAG_EN_RASP;
////#define A_FLAG_EN_RASP 0x800408D
uint8_t V_FLAG_EN_WATCHMAN;
////#define A_FLAG_EN_WATCHMAN 0x800408E
uint8_t V_EMAIL_ERR[32];
////#define A_EMAIL_ERR 0x800408F
uint16_t V_D_TIME[10][2];
////#define A_D1_TIME 0x80040AF
////#define A_D2_TIME 0x80040B3
////#define A_D3_TIME 0x80040B7
////#define A_D4_TIME 0x80040BB
////#define A_D5_TIME 0x80040BF
////#define A_D6_TIME 0x80040C3
////#define A_D7_TIME 0x80040C7
////#define A_D8_TIME 0x80040CB
////#define A_D9_TIME 0x80040CF
////#define A_D10_TIME 0x80040D3
uint16_t V_RD_TIME[3][2];
////#define A_RD1_TIME 0x80040D7
////#define A_RD2_TIME 0x80040DB
////#define A_RD3_TIME 0x80040DF
uint16_t V_IP_PING_TIME;
////#define A_IP_PING_TIME 0x80040E3
uint16_t V_TIME_SEND;
////#define A_TIME_SEND 0x80040E5
uint16_t V_TIME_READ;
////#define A_TIME_READ 0x80040E7
uint16_t V_N_RERID;
////#define A_N_RERID 0x80040E9
uint16_t V_TIME_RESET;
////#define A_TIME_RESET 0x80040EB
uint16_t V_DELAY_PING;
////#define A_DELAY_PING 0x80040ED
uint8_t V_SOST_RESET;
////#define A_SOST_RESET 0x80040EF
uint8_t V_N_PING;
////#define A_N_PING 0x80040F0
uint8_t V_IP_NTP1[4];
////#define A_IP_NTP1 0x80040F1
uint8_t V_IP_NTP2[4];
////#define A_IP_NTP2 0x80040F5
uint16_t V_PORT_NTP;
////#define A_PORT_NTP 0x80040F9
char V_NAME_SMTP[16];
////#define A_NAME_SNMP 0x80040FB
uint16_t V_PORT_SNMP;
////#define A_PORT_SNMP 0x800411B
char V_LOGIN_SMTP[16];
////#define A_LOGIN_SNMP 0x800411D
char V_PASSWORD_SMTP[16];
////#define A_PASSWORD_SNMP 0x800412B
char V_GEOM_NAME[32];
uint8_t V_ID_MAC[8];
char V_Name_dev[16];
char V_CALL_DATA[16];
uint8_t V_IP_SNMP[4];
signed char V_NTP_CIRCL;
uint8_t V_TYPE_OUT;
uint8_t V_resv[1669];

////#define A_RESV 0x800413B
logs_t V_logs_struct;
//uint16_t V_CRC_LOG;
////#define A_CRC_LOG 0x8004800
//char V_LOG[50][40]; 
////#define A_LOG 0x8004804
////#define A_START_APP 0x8005000
////#define A_FW_APP 0x8005004
////#define A_HTTP_DUMP 0x8005000
////#define A_END_APP     ((uint32_t)0x08020000) 
}FW_data_t;


extern file_data_t data_file;
extern FW_data_t FW_data;
extern uint16_t *idBase0;
extern uint16_t *idBase1;
extern uint32_t *idBase2 ;
extern uint32_t *idBase3 ;
extern log_reple_t start_time;
extern log_reple_t real_time;
extern uint8_t flag_global_save_data;
extern uint8_t flag_global_save_log;
extern uint8_t flag_global_load_def;
extern uint8_t flag_global_swich_out;
extern uint8_t flag_global_boot_mode;
extern uint8_t flag_global_reset_mode;
extern uint16_t HTTPD_SERVER_PORT;
#endif  /* __FLASH_IF_H */
typedef  void (*pFunction)(void);
/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
uint8_t frame_in_to_flash(uint8_t* data, uint8_t* nframe, uint16_t len_frame);
void jamp_to_app (void);
void jamp_to_boot (void);

uint8_t load_struct_flash_data (void);
uint8_t save_data_blok (uint8_t N_sector,uint32_t* struct_to);
void Save_CRC_APP (void);
uint8_t Comp_CRC_APP(void);
void termIO (void);
uint8_t save_data_flash(void);
uint8_t load_def_data(void);
      
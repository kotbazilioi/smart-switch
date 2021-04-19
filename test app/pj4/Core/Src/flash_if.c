/**
  ******************************************************************************
  * @file    IAP_Main/Src/flash_if.c 
  * @author  MCD Application Team
  * @brief   This file provides all the memory related operation functions.
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

/** @addtogroup STM32F1xx_IAP
  * @{
  */

/* Includes ------------------------------------------------------------------*/
    
#include "flash_if.h"
#include "main.h"
#include "stm32f107xc.h"
#include "stm32f1xx_hal_def.h"
#include "stm32_hal_legacy.h"
#include "stm32f1xx_hal_gpio.h"
#include  "stm32f1xx_hal_eth.h"
#include <string.h>
#include <stdint.h>
#include "crc16.h"
#include <stdio.h>
/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
pFunction JumpToApplication;
uint32_t JumpAddress;
uint32_t FlashProtection = 0;
uint8_t flag_app_start=0;
uint32_t timer_to_app=0;
uint8_t flag_global_save_data;
uint8_t flag_global_load_def;
uint8_t flag_global_save_log;
uint8_t flag_global_swich_out;
uint8_t flag_global_boot_mode;
uint8_t flag_global_reset_mode;
uint8_t flag_my_smtp_test;
uint8_t status_NTP_activ;
log_reple_t start_time;
log_reple_t real_time;
file_data_t data_file;
uint16_t HTTPD_SERVER_PORT;
//FW_data_t *FW_data = (FW_data_t*)(A_CRC_APP);
FW_data_t FW_data;

uint16_t *idBase0 = (uint16_t*)(UID_BASE);
uint16_t *idBase1 = (uint16_t*)(UID_BASE + 0x02);
uint32_t *idBase2 = (uint32_t*)(UID_BASE + 0x04);
uint32_t *idBase3 = (uint32_t*)(UID_BASE + 0x08);

//////  hetho.Init.MACAddr[0] =   (uint16_t)idBase0[0];
//////  hetho.Init.MACAddr[1] =   ((uint16_t)idBase0[0])>>8;
//////  hetho.Init.MACAddr[2] =   (uint16_t)idBase1[0];
//////  hetho.Init.MACAddr[3] =   ((uint16_t)idBase1[0])>>8;
//////  hetho.Init.MACAddr[4] =   (uint16_t)idBase2[0];
//////  hetho.Init.MACAddr[5] =   ((uint16_t)idBase2[0])>>8;
  
  

//__root const uint16_t  CRC_APP  @ A_CRC_APP;
////__root const uint16_t  CRC_BOOT  @ A_CRC_DATA_BOOT;
////__root const uint8_t IP_CONFIG[4] @ A_IP_ADRESS;
////
////__root const uint8_t IP_MASK[4] @ A_IP_MASK;
////
////__root const uint8_t IP_GET[4] @ A_GET_ADRESS;
////
////__root const uint8_t FW_VER[4] @ A_FW_VER ;
////
//__root const uint32_t FW_LEN @ A_FW_LEN ;
////
////__root const uint8_t BOOT_VER[4] @ A_BOOT_VER;
////__root const char LOG[2000] @ A_LOG;


//#define UID_BASE 0x1FFFF7E8

////uint8_t aFileName[FILE_NAME_LENGTH];
/* Private variables ---------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/

/* Private functions ---------------------------------------------------------*/

/**
  * @brief  Unlocks Flash for write access
  * @param  None
  * @retval None
  */
void FLASH_If_Init(void)
{
  /* Unlock the Program memory */
  HAL_FLASH_Unlock();

  /* Clear all FLASH flags */
  __HAL_FLASH_CLEAR_FLAG(FLASH_FLAG_EOP | FLASH_FLAG_PGERR | FLASH_FLAG_WRPERR);
  /* Unlock the Program memory */
  HAL_FLASH_Lock();
}

/**
  * @brief  This function does an erase of all user flash area
  * @param  start: start of user flash area
  * @retval FLASHIF_OK : user flash area successfully erased
  *         FLASHIF_ERASEKO : error occurred
  */
uint32_t FLASH_If_Erase(uint32_t start)
{
  uint32_t NbrOfPages = 0;
  uint32_t PageError = 0;
  FLASH_EraseInitTypeDef pEraseInit;
  HAL_StatusTypeDef status = HAL_OK;

  /* Unlock the Flash to enable the flash control register access *************/ 
  status=HAL_FLASH_Unlock();

  /* Get the sector where start the user flash area */
  if (start < USER_FLASH_BANK1_END_ADDRESS)
  {
    NbrOfPages = ((USER_FLASH_BANK1_END_ADDRESS + 1) - start)/FLASH_PAGE_SIZE;
    pEraseInit.TypeErase = FLASH_TYPEERASE_PAGES;
    pEraseInit.PageAddress = start;
    pEraseInit.Banks = FLASH_BANK_1;
    pEraseInit.NbPages = NbrOfPages;
    status = HAL_FLASHEx_Erase(&pEraseInit, &PageError);

////    NbrOfPages = (USER_FLASH_BANK2_END_ADDRESS - USER_FLASH_BANK1_END_ADDRESS)/FLASH_PAGE_SIZE;
////    pEraseInit.TypeErase = FLASH_TYPEERASE_PAGES;
////    pEraseInit.PageAddress = USER_FLASH_BANK2_START_ADDRESS;
////    pEraseInit.Banks = FLASH_BANK_2;
////    pEraseInit.NbPages = NbrOfPages;
////    status = HAL_FLASHEx_Erase(&pEraseInit, &PageError);
  }
////  else if (start < USER_FLASH_BANK2_END_ADDRESS)
////  {
////    NbrOfPages = ((USER_FLASH_BANK2_END_ADDRESS + 1) - start)/FLASH_PAGE_SIZE;
////    pEraseInit.TypeErase = FLASH_TYPEERASE_PAGES;
////    pEraseInit.PageAddress = start;
////    pEraseInit.Banks = FLASH_BANK_2;
////    pEraseInit.NbPages = NbrOfPages;
////    status = HAL_FLASHEx_Erase(&pEraseInit, &PageError);
////  }

  /* Lock the Flash to disable the flash control register access (recommended
     to protect the FLASH memory against possible unwanted operation) *********/
   status=HAL_FLASH_Lock();

  if (status != HAL_OK)
  {
    /* Error occurred while page erase */
    return FLASHIF_ERASEKO;
  }

  return FLASHIF_OK;
}

/* Public functions ---------------------------------------------------------*/
/**
  * @brief  This function writes a data buffer in flash (data are 32-bit aligned).
  * @note   After writing data buffer, the flash content is checked.
  * @param  destination: start address for target location
  * @param  p_source: pointer on buffer with data to write
  * @param  length: length of data buffer (unit is 32-bit word)
  * @retval uint32_t 0: Data successfully written to Flash memory
  *         1: Error occurred while writing data in Flash memory
  *         2: Written Data in flash memory is different from expected one
  */
uint32_t FLASH_If_Write(uint32_t destination, uint32_t *p_source, uint32_t length)
{
  uint32_t i = 0;

  /* Unlock the Flash to enable the flash control register access *************/
  HAL_FLASH_Unlock();

  for (i = 0; (i < length) && (destination <= (USER_FLASH_BANK1_END_ADDRESS-4)); i++)
  {
    /* Device voltage range supposed to be [2.7V to 3.6V], the operation will
       be done by word */ 
    if (HAL_FLASH_Program(FLASH_TYPEPROGRAM_WORD, destination, *(uint32_t*)(p_source+i)) == HAL_OK)      
    {
     /* Check the written value */
      if (*(uint32_t*)destination != *(uint32_t*)(p_source+i))
      {
        /* Flash content doesn't match SRAM content */
        return(FLASHIF_WRITINGCTRL_ERROR);
      }
      /* Increment FLASH destination address */
      destination += 4;
    }
    else
    {
      /* Error occurred while writing data in Flash memory */
      return (FLASHIF_WRITING_ERROR);
    }
  }

  /* Lock the Flash to disable the flash control register access (recommended
     to protect the FLASH memory against possible unwanted operation) *********/
  HAL_FLASH_Lock();

  return (FLASHIF_OK);
}

/**
  * @brief  Returns the write protection status of application flash area.
  * @param  None
  * @retval If a sector in application area is write-protected returned value is a combinaison
            of the possible values : FLASHIF_PROTECTION_WRPENABLED, FLASHIF_PROTECTION_PCROPENABLED, ...
  *         If no sector is write-protected FLASHIF_PROTECTION_NONE is returned.
  */
uint32_t FLASH_If_GetWriteProtectionStatus(void)
{
  uint32_t ProtectedPAGE = FLASHIF_PROTECTION_NONE;
  FLASH_OBProgramInitTypeDef OptionsBytesStruct;

  /* Unlock the Flash to enable the flash control register access *************/
  HAL_FLASH_Unlock();

  /* Check if there are write protected sectors inside the user flash area ****/
  HAL_FLASHEx_OBGetConfig(&OptionsBytesStruct);

  /* Lock the Flash to disable the flash control register access (recommended
     to protect the FLASH memory against possible unwanted operation) *********/
  HAL_FLASH_Lock();

  /* Get pages already write protected ****************************************/
  ProtectedPAGE = ~(OptionsBytesStruct.WRPPage) & FLASH_PAGE_TO_BE_PROTECTED;

  /* Check if desired pages are already write protected ***********************/
  if(ProtectedPAGE != 0)
  {
    /* Some sectors inside the user flash area are write protected */
    return FLASHIF_PROTECTION_WRPENABLED;
  }
  else
  { 
    /* No write protected sectors inside the user flash area */
    return FLASHIF_PROTECTION_NONE;
  }
}

/**
  * @brief  Configure the write protection status of user flash area.
  * @param  protectionstate : FLASHIF_WRP_DISABLE or FLASHIF_WRP_ENABLE the protection
  * @retval uint32_t FLASHIF_OK if change is applied.
  */
uint32_t FLASH_If_WriteProtectionConfig(uint32_t protectionstate)
{
  uint32_t ProtectedPAGE = 0x0;
  FLASH_OBProgramInitTypeDef config_new, config_old;
  HAL_StatusTypeDef result = HAL_OK;
  

  /* Get pages write protection status ****************************************/
  HAL_FLASHEx_OBGetConfig(&config_old);

  /* The parameter says whether we turn the protection on or off */
  config_new.WRPState = (protectionstate == FLASHIF_WRP_ENABLE ? OB_WRPSTATE_ENABLE : OB_WRPSTATE_DISABLE);

  /* We want to modify only the Write protection */
  config_new.OptionType = OPTIONBYTE_WRP;
  
  /* No read protection, keep BOR and reset settings */
  config_new.RDPLevel = OB_RDP_LEVEL_0;
  config_new.USERConfig = config_old.USERConfig;  
  /* Get pages already write protected ****************************************/
  ProtectedPAGE = config_old.WRPPage | FLASH_PAGE_TO_BE_PROTECTED;

  /* Unlock the Flash to enable the flash control register access *************/ 
  HAL_FLASH_Unlock();

  /* Unlock the Options Bytes *************************************************/
  HAL_FLASH_OB_Unlock();
  
  /* Erase all the option Bytes ***********************************************/
  result = HAL_FLASHEx_OBErase();
    
  if (result == HAL_OK)
  {
    config_new.WRPPage    = ProtectedPAGE;
    result = HAL_FLASHEx_OBProgram(&config_new);
  }
  
  return (result == HAL_OK ? FLASHIF_OK: FLASHIF_PROTECTION_ERRROR);
}
/**
  * @}
  */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
void jamp_to_app (void)
{
  uint32_t addr_met =APPLICATION_ADDRESS;
  uint32_t data_met;
  data_met= (uint32_t)(*(uint32_t*)addr_met);
  __disable_irq ();
  GPIO_InitTypeDef GPIO_InitStruct = {0};
 if ((data_met!=0xFFFFFFFF)&&(data_met!=0))
 {
 // HAL_ETH_DeInit(&hetho);
 /// HAL_DMA_DeInit(&hdma_ETHtomem_dma1_channel1);

     /*Configure GPIO pin : PHY_RST_Pin */
  GPIO_InitStruct.Pin = PHY_RST_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_OD;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_MEDIUM;
  HAL_GPIO_DeInit(PHY_RST_GPIO_Port, (uint32_t)&GPIO_InitStruct);

  /*Configure GPIO pins : LED_GREEN_Pin LED_RED_Pin */
  GPIO_InitStruct.Pin = LED_GREEN_Pin|LED_RED_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_DeInit(GPIOC, (uint32_t)&GPIO_InitStruct);

  /*Configure GPIO pin : PA8 */
  GPIO_InitStruct.Pin = GPIO_PIN_8;
  GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
  HAL_GPIO_DeInit(GPIOA, (uint32_t)&GPIO_InitStruct);
 // DeSystemClock_Config();
  HAL_RCC_DeInit();// Clock deinit
  HAL_DeInit();
 // NVIC_SetVectorTable(NVIC_VectTab_FLASH, 0x08004000);
  SCB->VTOR = FLASH_BASE | APPLICATION_ADDRESS; /* Vector Table Relocation in Internal FLASH */
  typedef  void (*pFunction)(void);
  uint32_t jumpAddress = *((volatile uint32_t*)(APPLICATION_ADDRESS + 4));
  pFunction Jump_To_Application = (pFunction) jumpAddress;
  Jump_To_Application();
 }
 else
 {
  timer_to_app=0;
  flag_app_start=0;
 }
}
 extern ETH_HandleTypeDef heth;
void jamp_to_boot (void)
{
  uint32_t addr_met =0x08000000;
  uint32_t data_met;
  data_met= (uint32_t)(*(uint32_t*)addr_met);
  HAL_SuspendTick();
  __disable_irq();
  GPIO_InitTypeDef GPIO_InitStruct = {0};
// if ((data_met!=0xFFFFFFFF)&&(data_met!=0))
// {
 
//  ETH_MACReceptionDisable(&heth);
//  ETH_DMATransmissionDisable(&heth);
//  ETH_DMAReceptionDisable(&heth);
  HAL_ETH_Stop(&heth);
   HAL_ETH_DeInit(&heth);  
//  HAL_ETH_MspDeInit(&heth);
  //HAL_DMA_DeInit(&hdma_ETHtomem_dma1_channel1);
__HAL_RCC_LSI_DISABLE();
     /*Configure GPIO pin : PHY_RST_Pin */
  GPIO_InitStruct.Pin = PHY_RST_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_OD;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_MEDIUM;
  HAL_GPIO_DeInit(PHY_RST_GPIO_Port, (uint32_t)&GPIO_InitStruct);

  /*Configure GPIO pins : LED_GREEN_Pin LED_RED_Pin */
  GPIO_InitStruct.Pin = LED_GREEN_Pin|LED_RED_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_DeInit(GPIOC, (uint32_t)&GPIO_InitStruct);

  /*Configure GPIO pin : PA8 */
  GPIO_InitStruct.Pin = GPIO_PIN_8;
  GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
  HAL_GPIO_DeInit(GPIOA, (uint32_t)&GPIO_InitStruct);
 // DeSystemClock_Config();
  HAL_RCC_DeInit();// Clock deinit
  HAL_DeInit();
   // NVIC_SetVectorTable(NVIC_VectTab_FLASH, 0x08004000);
  SCB->VTOR = FLASH_BASE | 0x08000000; /* Vector Table Relocation in Internal FLASH */
  typedef  void (*pFunction)(void);
  uint32_t jumpAddress = *((volatile uint32_t*)(0x08000000 + 4));
  pFunction Jump_To_Application = (pFunction) jumpAddress;
  Jump_To_Application();
// }

}

uint8_t save_data_blok (uint8_t N_sector,uint32_t* struct_to)
{
  uint8_t status=0;
  uint8_t blok=0;
  uint32_t start_addr=0;
  if(N_sector ==0)
  {
   blok=1;
   start_addr=A_CRC_APP;
  }
  else
  {
  if (N_sector==1)
    {
    blok=1;
    start_addr=A_CRC_LOG;
    
    }
    else
    {
    blok=2;
    start_addr=A_CRC_APP;
    }
  }
    FLASH_EraseInitTypeDef pEraseInit;
    uint32_t PageError = 0;
    status=HAL_FLASH_Unlock();
    pEraseInit.TypeErase = FLASH_TYPEERASE_PAGES;
    pEraseInit.PageAddress = start_addr;
    pEraseInit.Banks = FLASH_BANK_1;
    pEraseInit.NbPages = blok;
    status = HAL_FLASHEx_Erase(&pEraseInit, &PageError);
    status=HAL_FLASH_Lock();
    status= FLASH_If_Write( (uint32_t)start_addr,struct_to,blok*512);
        
    memcpy((uint8_t*)struct_to, (uint8_t *)start_addr, blok*512);
    return status;
}

      uint8_t save_data_flash(void)
      {
       FW_data.V_CRC_BOOT=crc16_ccitt((uint8_t*)&(FW_data.V_IP_CONFIG[0]),24);     
       FW_data.V_logs_struct.CRC16 = crc16_ccitt((uint8_t*)&(FW_data.V_logs_struct.log_reple[0]),2000);
       FW_data.V_CRC_DATA=crc16_ccitt((uint8_t*)&(FW_data.V_DHCP),2018);
       
       save_data_blok(0,(uint32_t*)&FW_data.V_CRC_APP);        
       memcpy((uint8_t *)(&FW_data.V_CRC_APP), (uint8_t *)A_CRC_APP, 2048);
      }
      
      uint8_t load_def_data(void)
      {
        FW_data.V_CRC_APP=*((uint16_t *)A_CRC_APP);
      
     FW_data.V_IP_CONFIG[0]=192;
     FW_data.V_IP_CONFIG[1]=168;
     FW_data.V_IP_CONFIG[2]=3;
     FW_data.V_IP_CONFIG[3]=21;
     
     FW_data.V_IP_MASK[0]=255;
     FW_data.V_IP_MASK[1]=255;
     FW_data.V_IP_MASK[2]=255;
     FW_data.V_IP_MASK[3]=0;
     
     FW_data.V_IP_GET[0]=192;
     FW_data.V_IP_GET[1]=168;
     FW_data.V_IP_GET[2]=3;
     FW_data.V_IP_GET[3]=1; 
   
     FW_data.V_FW1_VER[0]=0;
     FW_data.V_FW1_VER[1]=0;
     FW_data.V_FW1_VER[2]=0;
     FW_data.V_FW1_VER[3]=1;
     FW_data.V_FW1_LEN=*((uint32_t *)A_FW_LEN);
     FW_data.V_BOOT_VER = *((uint32_t *)A_BOOT_VER);
     FW_data.V_CRC_DATA = 0;
     FW_data.V_DHCP = 0;
      memset((uint8_t*)&FW_data.V_LOGIN,0,16);
     memcpy((uint32_t*)&FW_data.V_LOGIN, (uint32_t *)"admin", 5);
      memset((uint8_t*)&FW_data.V_PASSWORD,0,16);
     memcpy((uint32_t*)&FW_data.V_PASSWORD, (uint32_t *)"admin", 5);
     FW_data.V_IP_DNS[0]=10;
     FW_data.V_IP_DNS[1]=0;
     FW_data.V_IP_DNS[2]=0;
     FW_data.V_IP_DNS[3]=18;
     FW_data.V_WEB_PORT = 80;
     HTTPD_SERVER_PORT =FW_data.V_WEB_PORT;
     FW_data.V_PORT_SNMP = 162;
     FW_data.V_HTTP_IP = 81;
     memset((uint8_t*)&FW_data.V_ON_MESS,0,32);
     memcpy((uint8_t*)&FW_data.V_ON_MESS, (uint8_t *)"Power swich ON",14);
     memset((uint8_t*)&FW_data.V_OFF_MESS,0,32);
     memcpy((uint8_t*)&FW_data.V_OFF_MESS, (uint8_t *)"Power swich OFF", 15);
     FW_data.V_FLAG_EN_MAN =1;
     FW_data.V_FLAG_EN_RASP =0;
     FW_data.V_FLAG_EN_WEB =1;
     FW_data.V_FLAG_EN_WATCHMAN =0;
     memset((uint8_t*)&FW_data.V_EMAIL_ERR,0,32);
     memset((uint8_t*)&FW_data.V_D_TIME,61,sizeof(FW_data.V_D_TIME));
     memset((uint8_t*)&FW_data.V_RD_DATA,61,sizeof(FW_data.V_RD_DATA));
     
     
     FW_data.V_IP_PING_TIME = 9999;
     FW_data.V_TIME_SEND = 9999;
     FW_data.V_TIME_READ = 9999;
     FW_data.V_N_RERID = 9999;
     FW_data.V_TIME_RESET = 9999;
     FW_data.V_DELAY_PING = 9999;
     FW_data.V_SOST_RESET = 0;
     FW_data.V_N_PING = 255;
     FW_data.V_IP_NTP1[0]=85;//    85.21.78.23
     FW_data.V_IP_NTP1[1]=21;
     FW_data.V_IP_NTP1[2]=78;
     FW_data.V_IP_NTP1[3]=23;
     
     FW_data.V_IP_NTP2[0]=194; 
     FW_data.V_IP_NTP2[1]=190; 
     FW_data.V_IP_NTP2[2]=168; 
     FW_data.V_IP_NTP2[3]=1; 
     FW_data.V_PORT_NTP  = 123;
     memset((uint8_t*)&FW_data.V_NAME_SMTP,0,32);
     memcpy((uint8_t*)&FW_data.V_NAME_SMTP, (uint8_t *)"Name SNMP Server",32);
     FW_data.V_PORT_SNMP = 162;
     memset((uint8_t*)&FW_data.V_LOGIN_SMTP,0,32);
     memcpy((uint32_t*)&FW_data.V_LOGIN_SMTP, (uint32_t *)"admin", 5);
     memset((uint8_t*)&FW_data.V_PASSWORD_SMTP,0,32);
     memcpy((uint32_t*)&FW_data.V_PASSWORD_SMTP, (uint32_t *)"admin", 5);
     FW_data.V_FLAG_EMAIL_PORT=25;
     
     memset((uint8_t*)&FW_data.V_EMAIL_ADDR,0,32);
     memcpy((uint32_t*)&FW_data.V_EMAIL_ADDR, (uint32_t *)"", 5);

     memset((uint8_t*)&FW_data.V_EMAIL_FROM,0,32);
     memcpy((uint32_t*)&FW_data.V_EMAIL_FROM, (uint32_t *)"", 5);

     memset((uint8_t*)&FW_data.V_EMAIL_TO,0,32);
     memcpy((uint32_t*)&FW_data.V_EMAIL_TO, (uint32_t *)"", 5);



     
     
     
     memcpy((uint32_t*)&FW_data.V_GEOM_NAME, (uint32_t *)"Moscow office", 13);    
     
    FW_data.V_ID_MAC[0] =   00;//(uint16_t)idBase0[0];
    FW_data.V_ID_MAC[1] =   ((uint16_t)idBase0[0])>>8;
    FW_data.V_ID_MAC[2] =   (uint16_t)idBase1[0];
    FW_data.V_ID_MAC[3] =   ((uint16_t)idBase1[0])>>8;
    FW_data.V_ID_MAC[4] =   (uint16_t)idBase2[0];
    FW_data.V_ID_MAC[5] =   ((uint16_t)idBase2[0])>>8;
    FW_data.V_ID_MAC[6] =    0x59;
    FW_data.V_ID_MAC[7] =    0xDC;
    memcpy((uint8_t*)&FW_data.V_Name_dev,(uint8_t *)"DKSF 59",7);    
    memcpy((uint8_t*)&FW_data.V_CALL_DATA,(uint8_t *)"netping.ru",10);   
    
//    memset((uint8_t*)&FW_data.V_resv,0,1659);
    FW_data.V_logs_struct.CRC16 = 0;
    memset((uint8_t*)&FW_data.V_logs_struct.log_reple,0,2000);
     
     FW_data.V_IP_SYSL[0]=0;//     62.117.76.142
     FW_data.V_IP_SYSL[1]=0;
     FW_data.V_IP_SYSL[2]=0;
     FW_data.V_IP_SYSL[3]=0;
    
    
     FW_data.V_IP_SNMP[0]=192;//     62.117.76.142
     FW_data.V_IP_SNMP[1]=168;
     FW_data.V_IP_SNMP[2]=0;
     FW_data.V_IP_SNMP[3]=152;
     FW_data.V_TYPE_OUT=0;
     
     FW_data.V_NTP_CIRCL = 4;
     
     FW_data.V_CRC_BOOT=*((uint16_t *)A_CRC_DATA_BOOT);  
     FW_data.V_logs_struct.CRC16 = crc16_ccitt((uint8_t*)&(FW_data.V_logs_struct.log_reple[0]),2000);
     FW_data.V_CRC_DATA=crc16_ccitt((uint8_t*)&(FW_data.V_DHCP),2018);
     
       FW_data.V_TYPE_LOGIC=0;
       FW_data.V_EN_WATCHDOG=0;
       FW_data.V_EN_WATCHDOG_CN_A=1;
       FW_data.V_EN_WATCHDOG_CN_B=1;
       FW_data.V_EN_WATCHDOG_CN_C=1;
       FW_data.V_IP_WDT_ADDR_CN_A[0]=192;
       FW_data.V_IP_WDT_ADDR_CN_A[1]=168;
       FW_data.V_IP_WDT_ADDR_CN_A[2]=0;
       FW_data.V_IP_WDT_ADDR_CN_A[3]=151;
      
       FW_data.V_IP_WDT_ADDR_CN_B[0]=192;
       FW_data.V_IP_WDT_ADDR_CN_B[1]=168;
       FW_data.V_IP_WDT_ADDR_CN_B[2]=0;
       FW_data.V_IP_WDT_ADDR_CN_B[3]=152;
      
       FW_data.V_IP_WDT_ADDR_CN_C[0]=192;
       FW_data.V_IP_WDT_ADDR_CN_C[1]=168;
       FW_data.V_IP_WDT_ADDR_CN_C[2]=0;
       FW_data.V_IP_WDT_ADDR_CN_C[3]=153;
       FW_data.V_CT_RES_ALLSTART=0;
       FW_data.V_T_SEND_PING=15;
       FW_data.V_TIME_RESEND_PING=1000;
       FW_data.V_MAX_REPID_PING=8;
       FW_data.V_TIME_RESET_PULSE=12;
       FW_data.V_PAUSE_RESET_TO_REPID=15;
       FW_data.V_MAX_RESEND_PACET_RESET=0;
     
     
     
     save_data_blok(3,(uint32_t*)&FW_data.V_CRC_APP); 
     

     
        return 1;
      }
uint8_t load_struct_flash_data (void)
{
uint16_t crc_in=((uint16_t)(*(uint32_t*)A_CRC_DATA));

      
   if ((crc_in!= crc16_ccitt((uint8_t *)&(*(uint32_t*)A_DHCP),2018))||(crc_in==0))   //(uint16_t)(*(uint32_t*)A_CRC_DATA_BOOT)
   {
   return  load_def_data();
   
  }
 else
    {
      memcpy((uint8_t *)(&FW_data.V_CRC_APP), (uint8_t *)A_CRC_APP, 2048);
      memcpy((void *)(&FW_data.V_logs_struct.log_reple[0]), (char *)A_LOG, 2000);
      return 0;
    }
}
void Save_CRC_APP (void)
{
uint16_t crcapp=crc16_ccitt((uint8_t *)(uint8_t*)A_START_APP,(uint32_t)((uint32_t*)A_FW_LEN));

 memcpy((uint32_t *)(&FW_data.V_CRC_APP), (uint32_t *)A_CRC_APP, 1024);
 
 FW_data.V_CRC_APP=crcapp;
 FW_data.V_CRC_BOOT=crc16_ccitt((uint8_t*)&(FW_data.V_IP_CONFIG[0]),24);    
 save_data_blok(3,(uint32_t*)&FW_data.V_CRC_APP); 

}
uint8_t Comp_CRC_APP(void)
{
  uint16_t crcapp=crc16_ccitt((uint8_t*)A_START_APP,(uint32_t)((uint32_t*)A_FW_LEN));
  memcpy((uint32_t *)(&FW_data.V_CRC_APP), (uint32_t *)A_CRC_APP, 1024);
  if ((crcapp==FW_data.V_CRC_APP)&&(crcapp!=0))
    {
      return 0;
    }
  else
    {
      return 1;
    }
}
void termIO (void)
{
    char PAGE_BODY[512];
    memset(PAGE_BODY, 0,512);
    osThreadList((unsigned char *)(PAGE_BODY + strlen(PAGE_BODY)));
    printf("%s\n\n\r", PAGE_BODY);
////    memset(PAGE_BODY, 0,512);
////    vTaskGetRunTimeStats ((unsigned char *)(PAGE_BODY + strlen(PAGE_BODY)));
    
}
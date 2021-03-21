/**
  ******************************************************************************
  * @file    LwIP/LwIP_HTTP_Server_Netconn_RTOS/Src/httpser-netconn.c 
  * @author  MCD Application Team
  * @brief   Basic http server implementation using LwIP netconn API  
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; Copyright (c) 2016 STMicroelectronics International N.V. 
  * All rights reserved.</center></h2>
  *
  * Redistribution and use in source and binary forms, with or without 
  * modification, are permitted, provided that the following conditions are met:
  *
  * 1. Redistribution of source code must retain the above copyright notice, 
  *    this list of conditions and the following disclaimer.
  * 2. Redistributions in binary form must reproduce the above copyright notice,
  *    this list of conditions and the following disclaimer in the documentation
  *    and/or other materials provided with the distribution.
  * 3. Neither the name of STMicroelectronics nor the names of other 
  *    contributors to this software may be used to endorse or promote products 
  *    derived from this software without specific written permission.
  * 4. This software, including modifications and/or derivative works of this 
  *    software, must execute solely and exclusively on microcontroller or
  *    microprocessor devices manufactured by or for STMicroelectronics.
  * 5. Redistribution and use of this software other than as permitted under 
  *    this license is void and will automatically terminate your rights under 
  *    this license. 
  *
  * THIS SOFTWARE IS PROVIDED BY STMICROELECTRONICS AND CONTRIBUTORS "AS IS" 
  * AND ANY EXPRESS, IMPLIED OR STATUTORY WARRANTIES, INCLUDING, BUT NOT 
  * LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY, FITNESS FOR A 
  * PARTICULAR PURPOSE AND NON-INFRINGEMENT OF THIRD PARTY INTELLECTUAL PROPERTY
  * RIGHTS ARE DISCLAIMED TO THE FULLEST EXTENT PERMITTED BY LAW. IN NO EVENT 
  * SHALL STMICROELECTRONICS OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT,
  * INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
  * LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, 
  * OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF 
  * LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING 
  * NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE,
  * EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
  *
  ******************************************************************************
  */

/* Includes ------------------------------------------------------------------*/
#include "lwip/opt.h"
#include "lwip/arch.h"
#include "lwip/api.h"
#include "lwip/apps/fs.h"
#include "lwip.h"
#include "string.h"
#include "httpserver-netconn.h"
#include "cmsis_os.h"
#include "stm32f1xx_hal.h"
#include <stdio.h>
#include "main.h"
#include "portmacro.h"
#include "flash_if.h"
#include "html_page.h"
#include "base64.h"
#include "LOGS.h"
/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
#define WEBSERVER_THREAD_PRIO    ( osPriorityAboveNormal )

void DynWebPageStr(struct netconn *conn);
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
u32_t nPageHits = 0;
char http_ok[] = {0x48,0x54,0x54,0x50,0x2f,0x31,0x2e,0x31,0x20,0x32,0x30,0x30,0x20,0x4f,0x4b,0x0d,0x0a};
uint8_t flag_req_logon=0;
  uint8_t page_n,page_sost=1;
/**
  * @brief serve tcp connection  
  * @param conn: pointer on connection structure 
  * @retval None
  */
    uint8_t scanf_time (char* data,uint16_t* ip_mass,uint8_t len_ip)
  {
     signed int  ct_char=0;
     uint16_t mul_k=1;
    ip_mass[2]=0;
    for(ct_char=len_ip-1;ct_char>=0;ct_char--)
      {
        if (data[ct_char]!='.')
          {
            ip_mass[2]=(data[ct_char]-0x30)*mul_k+ip_mass[2];
            mul_k=mul_k*10;  
            if (mul_k>10000)
            {// break;
              return 1;
            }
          }
        else
          {
          break;
          }
      }
    ct_char--;
    mul_k=1;
    ip_mass[1]=0;
    for(;ct_char>=0;ct_char--)
      {
        if (data[ct_char]!='.')
          {
            ip_mass[1]=(data[ct_char]-0x30)*mul_k+ip_mass[1];
            mul_k=mul_k*10;  
            if (mul_k>100)
            { 
              //break;
            return 1;
            }
          }
        else
          {
          break;
          }
      }
//    ct_char--;
//     mul_k=1;
//     for(;ct_char>=0;ct_char--)
//      {
//        if (data[ct_char]!='.')
//          {
//            ip_mass[0]=(data[ct_char]-0x30)*mul_k+ip_mass[0];
//            mul_k=mul_k*10;  
//            if (mul_k>1000)
//            { //break;
//            return 1;
//            }
//          }
//        else
//          {
//          break;
//          }
//      }
     ip_mass[0]=0;
     ct_char--;
      mul_k=1;
     for(;ct_char>=0;ct_char--)
      {
        if ((data[ct_char]!='.')||(ct_char!=0)||(data[ct_char]!='+')||(data[ct_char]!='&'))
          {
            ip_mass[0]=(data[ct_char]-0x30)*mul_k+ip_mass[0];
            mul_k=mul_k*10;  
            if (mul_k>100)
            {// break;
            return 1;
            }
          }
        else
          {
          break;
          }
      }
    return 0;
  }
  uint8_t scanf_ip (char* data,uint8_t* ip_mass,uint8_t len_ip)
  {
     signed int  ct_char=0;
     uint16_t mul_k=1;
     memset(ip_mass,0,4);
    for(ct_char=len_ip-1;ct_char>=0;ct_char--)
      {
        if (data[ct_char]!='.')
          {
            ip_mass[3]=(data[ct_char]-0x30)*mul_k+ip_mass[3];
            mul_k=mul_k*10;  
            if (mul_k>1000)
            {// break;
              return 1;
            }
          }
        else
          {
          break;
          }
      }
    ct_char--;
    mul_k=1;
    for(;ct_char>=0;ct_char--)
      {
        if (data[ct_char]!='.')
          {
            ip_mass[2]=(data[ct_char]-0x30)*mul_k+ip_mass[2];
            mul_k=mul_k*10;  
            if (mul_k>1000)
            { 
              //break;
            return 1;
            }
          }
        else
          {
          break;
          }
      }
    ct_char--;
     mul_k=1;
     for(;ct_char>=0;ct_char--)
      {
        if (data[ct_char]!='.')
          {
            ip_mass[1]=(data[ct_char]-0x30)*mul_k+ip_mass[1];
            mul_k=mul_k*10;  
            if (mul_k>1000)
            { //break;
            return 1;
            }
          }
        else
          {
          break;
          }
      }
     ct_char--;
      mul_k=1;
     for(;ct_char>=0;ct_char--)
      {
        if ((data[ct_char]!='.')||(ct_char!=0))
          {
            ip_mass[0]=(data[ct_char]-0x30)*mul_k+ip_mass[0];
            mul_k=mul_k*10;  
            if (mul_k>1000)
            {// break;
            return 1;
            }
          }
        else
          {
          break;
          }
      }
    return 0;
  }
  
  
   uint8_t scanf_port (char* data,uint16_t* ip_mass,uint8_t len_ip)
  {
     signed int  ct_char=0;
     uint16_t mul_k=1;

      mul_k=1;
      *ip_mass=0;
     for(ct_char=len_ip;ct_char>=0;ct_char--)
      {
        if (data[ct_char]>0x39)
          {
            return 1;
          }
        if (ct_char!=0)
          {
            *ip_mass=(data[ct_char-1]-0x30)*mul_k+(*ip_mass);
            mul_k=mul_k*10;  
            if (mul_k>10000)
            {// break;
            return 1;
            }
          }
        else
          {
          break;
          }
      }
    return 0;
  }
  
  
  
  
void get_addres_list(uint8_t* data,uint32_t lens)
{
uint16_t ct_char=0;
uint8_t ct_seg=0;

for (ct_char=16;ct_char<lens;ct_char++)
  {
    if ((data[ct_char]=='.')||(data[ct_char]=='&'))
      {
        FW_data.V_IP_CONFIG[ct_seg]=(data[ct_char-1]&0x0f);
        if (data[ct_char-2]!='.')
        {
          FW_data.V_IP_CONFIG[ct_seg]=FW_data.V_IP_CONFIG[ct_seg]+10*(data[ct_char-2]&0x0f);        
          if (data[ct_char-3]!='.')
            {
              FW_data.V_IP_CONFIG[ct_seg]=FW_data.V_IP_CONFIG[ct_seg]+100*(data[ct_char-3]&0x0f);        
            }
        }                       
                               
        ct_seg++;
         if (ct_seg==4)
          {
            ct_char=ct_char+9;
            ct_seg=0;
            break;
          }
      }    
  }

for (;ct_char<lens;ct_char++)
  {
    if ((data[ct_char]=='.')||(data[ct_char]=='&'))
      {
        FW_data.V_IP_MASK[ct_seg]=(data[ct_char-1]&0x0f);
        if (data[ct_char-2]!='.')
        {
          FW_data.V_IP_MASK[ct_seg]=FW_data.V_IP_MASK[ct_seg]+10*(data[ct_char-2]&0x0f);        
          if (data[ct_char-3]!='.')
            {
              FW_data.V_IP_MASK[ct_seg]=FW_data.V_IP_MASK[ct_seg]+100*(data[ct_char-3]&0x0f);        
            }
        }                       
                               
        ct_seg++;
        if (ct_seg==4)
          {
            ct_char=ct_char+9;
            ct_seg=0;
            break;
          }
      }    
  }
for (;ct_char<lens;ct_char++)
  {
    if ((data[ct_char]=='.')||(data[ct_char]==' '))
      {
        FW_data.V_IP_GET[ct_seg]=(data[ct_char-1]&0x0f);
        if (data[ct_char-2]!='.')
        {
          FW_data.V_IP_GET[ct_seg]=FW_data.V_IP_GET[ct_seg]+10*(data[ct_char-2]&0x0f);        
          if (data[ct_char-3]!='.')
            {
              FW_data.V_IP_GET[ct_seg]=FW_data.V_IP_GET[ct_seg]+100*(data[ct_char-3]&0x0f);        
            }
        }                       
                               
        ct_seg++;
        if (ct_seg==4)
          {
            ct_char=0;
            break;
          }
      }    
  }
    FLASH_EraseInitTypeDef pEraseInit;
    uint32_t PageError = 0;
    HAL_FLASH_Unlock();
    pEraseInit.TypeErase = FLASH_TYPEERASE_PAGES;
    pEraseInit.PageAddress = A_CRC_APP;
    pEraseInit.Banks = FLASH_BANK_1;
    pEraseInit.NbPages = 1;
    HAL_FLASHEx_Erase(&pEraseInit, &PageError);
    HAL_FLASH_Lock();
    FLASH_If_Write( (uint32_t)A_CRC_APP,(uint32_t*) FW_data.V_CRC_APP,512);
    
jamp_to_app();
//return 0;
};

////void set_addres_list(uint8_t* data,uint32_t lens)
////{
////uint16_t ct_char=0;
////uint8_t ct_seg=0;
////
////for (ct_char=449;ct_char<lens;ct_char++)
////  {
////    if ((data[ct_char]=='.')||(data[ct_char]=='"'))
////      {
////        data[ct_char-1]=FW_data.V_IP_CONFIG[ct_seg]-100*((uint8_t)FW_data.V_IP_CONFIG[ct_seg]/100);
////        data[ct_char-1]=data[ct_char-1] -10*((uint8_t)data[ct_char-1]/10)+0x30;
////        if (data[ct_char-2]!='.')
////        {
////          data[ct_char-2]=FW_data.V_IP_CONFIG[ct_seg]-100*(FW_data.V_IP_CONFIG[ct_seg]/100);
////             data[ct_char-2]=data[ct_char-2]/10+0x30;
////          if (data[ct_char-3]!='.')
////            {
////              data[ct_char-3]=0x30+(FW_data.V_IP_CONFIG[ct_seg]/100);
////            }
////        }                       
////                               
////        ct_seg++;
////         if (ct_seg==4)
////          {
////            ct_char=ct_char+132;
////            ct_seg=0;
////            break;
////          }
////      }    
////  }
////
////for (;ct_char<lens;ct_char++)
////  {
////     if ((data[ct_char]=='.')||(data[ct_char]=='"'))
////      {
////        data[ct_char-1]=FW_data.V_IP_MASK[ct_seg]-100*((uint8_t)FW_data.V_IP_MASK[ct_seg]/100);
////        data[ct_char-1]=data[ct_char-1] -10*((uint8_t)data[ct_char-1]/10)+0x30;
////        if (data[ct_char-2]!='.')
////        {
////          data[ct_char-2]=FW_data.V_IP_MASK[ct_seg]-100*(FW_data.V_IP_MASK[ct_seg]/100);
////             data[ct_char-2]=data[ct_char-2]/10+0x30;
////          if (data[ct_char-3]!='.')
////            {
////              data[ct_char-3]=0x30+(FW_data.V_IP_MASK[ct_seg]/100);
////            }
////        }                                
////     
////                           
////        ct_seg++;
////         if (ct_seg==4)
////          {
////            ct_char=ct_char+122;
////            ct_seg=0;
////            break;
////          }
////      }    
////  }
////                           
////for (;ct_char<lens;ct_char++)
////  {
////      if ((data[ct_char]=='.')||(data[ct_char]=='"'))
////      {
////        data[ct_char-1]=FW_data.V_IP_GET[ct_seg]-100*((uint8_t)FW_data.V_IP_GET[ct_seg]/100);
////        data[ct_char-1]=data[ct_char-1] -10*((uint8_t)data[ct_char-1]/10)+0x30;
////        if (data[ct_char-2]!='.')
////        {
////          data[ct_char-2]=FW_data.V_IP_GET[ct_seg]-100*(FW_data.V_IP_GET[ct_seg]/100);
////             data[ct_char-2]=data[ct_char-2]/10+0x30;
////          if (data[ct_char-3]!='.')
////            {
////              data[ct_char-3]=0x30+(FW_data.V_IP_GET[ct_seg]/100);
////            }
////        }                                 
////                 
////        ct_seg++;
////         if (ct_seg==4)
////          {
////            ct_char=ct_char+9;
////            ct_seg=0;
////            break;
////          }
////      }    
////  }
////  //out//449 22   
////             // 594 
////             //731
////
//////return 0;
////};
////
////  

/**********************************************/

/***********************************************/
typedef struct post_data_t
 {
   char name[32];
   char data[32];
 
   uint8_t len_par;
 }post_data_t;
void param_run(post_data_t* post_data,uint8_t index)
{
  uint8_t IP_buf[4];
  uint16_t port_n;
  uint16_t data_mass[3];
  uint8_t ct_temp0;
  uint8_t len_mess=0;
//  if (index==3)
//  {
    if (strncmp((char*)post_data->name,"name_dev", sizeof("name_dev")) == 0)
          {
           // FW_data.V_Name_dev
            len_mess=strlen(post_data->data);
            memset((char*)FW_data.V_Name_dev,0,strlen((char*)FW_data.V_Name_dev));
            for (ct_temp0=0;ct_temp0<len_mess;ct_temp0++)
              {
                if (post_data->data[ct_temp0]==0x2b)
                  {
                    post_data->data[ct_temp0]=0x20;
                  }
             
              }
            memcpy((char*)FW_data.V_Name_dev, (char*)post_data->data,len_mess );
          }
     else if (strncmp((char*)post_data->name,"check_res", sizeof("check_res")) == 0)
          {
//           len_mess=strlen(post_data->data);
//            if (scanf_ip ((char*) post_data->data,IP_buf,len_mess)==0)
//            {
//              memcpy((uint8_t*)FW_data.V_IP_NTP2, (char*)IP_buf,4 );
//            }
             form_reple_to_save(RESETL);
          //   flag_global_reset_mode=1;
             save_reple_log(reple_to_save);
             flag_global_save_log=0;
             vTaskDelay(100);
             jamp_to_app();
             
          }
     else if (strncmp((char*)post_data->name,"output_set", sizeof("output_set")) == 0)
          {
           // FW_data.V_Name_dev
            len_mess=strlen(post_data->data);
            if (post_data->data[0]==0x30)
            {
               form_reple_to_save(SWICH_ON_WEB);
               flag_global_swich_out=SWICH_ON_WEB;
               HAL_RTCEx_BKUPWrite(&hrtc,1,0);
            }
            else if (post_data->data[0]==0x31)
              {
                form_reple_to_save(SWICH_OFF_WEB);
                flag_global_swich_out=SWICH_OFF_WEB;
                 HAL_RTCEx_BKUPWrite(&hrtc,1,1);
              
              }
            else if (post_data->data[0]==0x32)
              {
                 form_reple_to_save(SWICH_TOLG_WEB);
                 flag_global_swich_out=SWICH_TOLG_WEB;
                 HAL_RTCEx_BKUPWrite(&hrtc,1,2);
              }
              
          
          }
      else if (strncmp((char*)post_data->name,"http_out_set", sizeof("http_out_set")) == 0)
          {
           // FW_data.V_Name_dev
            len_mess=strlen(post_data->data);
            if (post_data->data[0]==0x30)
            {  
               form_reple_to_save(SWICH_ON_HTTP);
               flag_global_swich_out=SWICH_ON_HTTP;
            }
            else if (post_data->data[0]==0x31)
              {
                form_reple_to_save(SWICH_OFF_HTTP);
                flag_global_swich_out=SWICH_OFF_HTTP;
              
              }
            else if (post_data->data[0]==0x32)
              {
                 form_reple_to_save(SWICH_TOLG_HTTP);
                 flag_global_swich_out=SWICH_TOLG_HTTP;
              
              }
              
          
          }
    else if (strncmp((char*)post_data->name,"geo_place", sizeof("geo_place")) == 0)
          {
           // FW_data.V_Name_dev
            len_mess=strlen(post_data->data);
              memset((char*)FW_data.V_GEOM_NAME,0,strlen((char*)FW_data.V_GEOM_NAME));
              for (ct_temp0=0;ct_temp0<len_mess;ct_temp0++)
              {
                if (post_data->data[ct_temp0]==0x2b)
                  {
                    post_data->data[ct_temp0]=0x20;
                  }
              }
            memcpy((char*)FW_data.V_GEOM_NAME, (char*)post_data->data,len_mess );
          }
    else if (strncmp((char*)post_data->name,"call_data", sizeof("call_data")) == 0)
          {
           // FW_data.V_Name_dev
            FW_data.V_DHCP=0;
            len_mess=strlen(post_data->data);
              memset((char*)FW_data.V_CALL_DATA,0,strlen((char*)FW_data.V_CALL_DATA));
              for (ct_temp0=0;ct_temp0<len_mess;ct_temp0++)
              {
                if (post_data->data[ct_temp0]==0x2b)
                  {
                    post_data->data[ct_temp0]=0x20;
                  }
              }
            memcpy((char*)FW_data.V_CALL_DATA, (char*)post_data->data,len_mess );
          }
      else if (strncmp((char*)post_data->name,"dhcp_flag", sizeof("dhcp_flag")) == 0)
          {
           // FW_data.V_Name_dev
            len_mess=strlen(post_data->data);
            if (post_data->data[0]==0x31)
            {
              FW_data.V_DHCP=1;
            }
          
          }
    
    else if (strncmp((char*)post_data->name,"ip_addr", sizeof("ip_addr")) == 0)
          {
           // FW_data.V_Name_dev
            len_mess=strlen(post_data->data);
            if (scanf_ip ((char*) post_data->data,IP_buf,len_mess)==0)
            {
              memcpy((uint8_t*)FW_data.V_IP_CONFIG, (char*)IP_buf,4 );
            }
          
          }/*****************************************/
     else if (strncmp((char*)post_data->name,"V_IP_WDT_ADDR_CN_A", sizeof("V_IP_WDT_ADDR_CN_A")) == 0)
          {
           // FW_data.V_Name_dev
            len_mess=strlen(post_data->data);
            if (scanf_ip ((char*) post_data->data,IP_buf,len_mess)==0)
            {
              memcpy((uint8_t*)FW_data.V_IP_WDT_ADDR_CN_A, (char*)IP_buf,4 );
            }
          
          }
     else if (strncmp((char*)post_data->name,"V_IP_WDT_ADDR_CN_B", sizeof("V_IP_WDT_ADDR_CN_B")) == 0)
          {
           // FW_data.V_Name_dev
            len_mess=strlen(post_data->data);
            if (scanf_ip ((char*) post_data->data,IP_buf,len_mess)==0)
            {
              memcpy((uint8_t*)FW_data.V_IP_WDT_ADDR_CN_B, (char*)IP_buf,4 );
            }
          
          }
     else if (strncmp((char*)post_data->name,"V_IP_WDT_ADDR_CN_C", sizeof("V_IP_WDT_ADDR_CN_C")) == 0)
          {
           // FW_data.V_Name_dev
            len_mess=strlen(post_data->data);
            if (scanf_ip ((char*) post_data->data,IP_buf,len_mess)==0)
            {
              memcpy((uint8_t*)FW_data.V_IP_WDT_ADDR_CN_C, (char*)IP_buf,4 );
            }
          
          }
      else if (strncmp((char*)post_data->name,"V_EN_WATCHDOG", sizeof("V_EN_WATCHDOG")) == 0)
          {
            FW_data.V_EN_WATCHDOG_CN_A=0;
            FW_data.V_EN_WATCHDOG_CN_B=0;
            FW_data.V_EN_WATCHDOG_CN_C=0;
          //  len_mess=strlen(post_data->data);
            if (strncmp((char*)post_data->data,"ON", sizeof("ON")) == 0)//(scanf_port ((char*) post_data->data,&port_n,len_mess)==0)
            {
              FW_data.V_EN_WATCHDOG=1;
            }
            else
            {
              FW_data.V_EN_WATCHDOG=0;
            }
          }
      else if (strncmp((char*)post_data->name,"V_EN_WATCHDOG_CN_A", sizeof("V_EN_WATCHDOG_CN_A")) == 0)
          {
            len_mess=strlen(post_data->data);
            if (scanf_port ((char*) post_data->data,&port_n,len_mess)==0)
            {
              FW_data.V_EN_WATCHDOG_CN_A=port_n;
            }
          }
     else if (strncmp((char*)post_data->name,"V_EN_WATCHDOG_CN_B", sizeof("V_EN_WATCHDOG_CN_B")) == 0)
          {
            len_mess=strlen(post_data->data);
            if (scanf_port ((char*) post_data->data,&port_n,len_mess)==0)
            {
              FW_data.V_EN_WATCHDOG_CN_B=port_n;
            }
          }
    else if (strncmp((char*)post_data->name,"V_EN_WATCHDOG_CN_C", sizeof("V_EN_WATCHDOG_CN_C")) == 0)
          {
            len_mess=strlen(post_data->data);
            if (scanf_port ((char*) post_data->data,&port_n,len_mess)==0)
            {
              FW_data.V_EN_WATCHDOG_CN_C=port_n;
            }
          }
    else if (strncmp((char*)post_data->name,"V_CT_RES_ALLSTART", sizeof("V_CT_RES_ALLSTART")) == 0)
          {
            len_mess=strlen(post_data->data);
            if (scanf_port ((char*) post_data->data,&port_n,len_mess)==0)
            {
             // FW_data.V_CT_RES_ALLSTART=port_n;
            }
          }
    else if (strncmp((char*)post_data->name,"V_T_SEND_PING", sizeof("V_T_SEND_PING")) == 0)
          {
            len_mess=strlen(post_data->data);
            if (scanf_port ((char*) post_data->data,&port_n,len_mess)==0)
            {
              FW_data.V_T_SEND_PING=port_n;
            }
          }
    else if (strncmp((char*)post_data->name,"V_TIME_RESEND_PING", sizeof("V_TIME_RESEND_PING")) == 0)
          {
            len_mess=strlen(post_data->data);
            if (scanf_port ((char*) post_data->data,&port_n,len_mess)==0)
            {
              FW_data.V_TIME_RESEND_PING=port_n;
            }
          }
     else if (strncmp((char*)post_data->name,"V_MAX_REPID_PING", sizeof("V_MAX_REPID_PING")) == 0)
          {
            len_mess=strlen(post_data->data);
            if (scanf_port ((char*) post_data->data,&port_n,len_mess)==0)
            {
              FW_data.V_MAX_REPID_PING=port_n;
            }
          }
     else if (strncmp((char*)post_data->name,"V_TIME_RESET_PULSE", sizeof("V_TIME_RESET_PULSE")) == 0)
          {
            len_mess=strlen(post_data->data);
            if (scanf_port ((char*) post_data->data,&port_n,len_mess)==0)
            {
              FW_data.V_TIME_RESET_PULSE=port_n;
            }
          }
     else if (strncmp((char*)post_data->name,"V_PAUSE_RESET_TO_REPID", sizeof("V_PAUSE_RESET_TO_REPID")) == 0)
          {
            len_mess=strlen(post_data->data);
            if (scanf_port ((char*) post_data->data,&port_n,len_mess)==0)
            {
              FW_data.V_PAUSE_RESET_TO_REPID=port_n;
            }
          }
     else if (strncmp((char*)post_data->name,"V_MAX_RESEND_PACET_RESET", sizeof("V_MAX_RESEND_PACET_RESET")) == 0)
          {
            len_mess=strlen(post_data->data);
            if (scanf_port ((char*) post_data->data,&port_n,len_mess)==0)
            {
              FW_data.V_MAX_RESEND_PACET_RESET=port_n;
            }
          }
     else if (strncmp((char*)post_data->name,"output_logic", sizeof("output_logic")) == 0)
          {
            len_mess=strlen(post_data->data);
            if (scanf_port ((char*) post_data->data,&port_n,len_mess)==0)
            {
              FW_data.V_TYPE_LOGIC=port_n;
            }
          }    
    else if (strncmp((char*)post_data->name,"mask_addr", sizeof("mask_addr")) == 0)
          {
             len_mess=strlen(post_data->data);
            if (scanf_ip ((char*) post_data->data,IP_buf,len_mess)==0)
            {
              memcpy((uint8_t*)FW_data.V_IP_MASK, (char*)IP_buf,4 );
            }
          }
    else if (strncmp((char*)post_data->name,"getway_addr", sizeof("getway_addr")) == 0)
          {
             len_mess=strlen(post_data->data);
            if (scanf_ip ((char*) post_data->data,IP_buf,len_mess)==0)
            {
              memcpy((uint8_t*)FW_data.V_IP_GET, (char*)IP_buf,4 );
            }
          }
    else if (strncmp((char*)post_data->name,"dns_addr", sizeof("dns_addr")) == 0)
          {
             len_mess=strlen(post_data->data);
            if (scanf_ip ((char*) post_data->data,IP_buf,len_mess)==0)
            {
              memcpy((uint8_t*)FW_data.V_IP_DNS, (char*)IP_buf,4 );
            }
          }
    else if (strncmp((char*)post_data->name,"port_http", sizeof("port_http")) == 0)
          {
            len_mess=strlen(post_data->data);
            if (scanf_port ((char*) post_data->data,&port_n,len_mess)==0)
            {
              FW_data.V_WEB_PORT=port_n;
            }
          }
    else if (strncmp((char*)post_data->name,"login", sizeof("login")) == 0)
          {
           // FW_data.V_Name_dev
            len_mess=strlen(post_data->data);
             memset((char*)FW_data.V_LOGIN,0,strlen((char*)FW_data.V_LOGIN));
            memcpy((char*)FW_data.V_LOGIN, (char*)post_data->data,len_mess );
          }
    else if (strncmp((char*)post_data->name,"password", sizeof("password")) == 0)
          {
           // FW_data.V_Name_dev
            len_mess=strlen(post_data->data);
            memset((char*)FW_data.V_PASSWORD,0,strlen((char*)FW_data.V_PASSWORD));
            memcpy((char*)FW_data.V_PASSWORD, (char*)post_data->data,len_mess );
          }
    else if (strncmp((char*)post_data->name,"snmp_addr", sizeof("snmp_addr")) == 0)
          {
             len_mess=strlen(post_data->data);
            if (scanf_ip ((char*) post_data->data,IP_buf,len_mess)==0)
            {
              memcpy((uint8_t*)FW_data.V_IP_SNMP, (char*)IP_buf,4 );
            }
          }
    else if (strncmp((char*)post_data->name,"port_snmp", sizeof("port_snmp")) == 0)
          {
            len_mess=strlen(post_data->data);
            if (scanf_port ((char*) post_data->data,&port_n,len_mess)==0)
            {
              FW_data.V_PORT_SNMP=port_n;
            }
          }
    else if (strncmp((char*)post_data->name,"ntp1_addr", sizeof("ntp1_addr")) == 0)
          {
            len_mess=strlen(post_data->data);
            if (scanf_ip ((char*) post_data->data,IP_buf,len_mess)==0)
            {
              memcpy((uint8_t*)FW_data.V_IP_NTP1, (char*)IP_buf,4 );
            }
          }
    else if (strncmp((char*)post_data->name,"ntp2_addr", sizeof("ntp2_addr")) == 0)
          {
           len_mess=strlen(post_data->data);
            if (scanf_ip ((char*) post_data->data,IP_buf,len_mess)==0)
            {
              memcpy((uint8_t*)FW_data.V_IP_NTP2, (char*)IP_buf,4 );
            }
          }
    else if (strncmp((char*)post_data->name,"time_circl", sizeof("time_circl")) == 0)
          {
           len_mess=strlen(post_data->data);
        //
           
           if (post_data->data[0]=='-')
           {
            post_data->data[0]=post_data->data[1];
            post_data->data[1]=post_data->data[2];
            len_mess--;
            if (scanf_port ((char*) post_data->data,&port_n,len_mess)==0)
            {
              
              FW_data.V_NTP_CIRCL=(signed char)port_n-2*port_n;
            }
           }
           else
           {
            if (scanf_port ((char*) post_data->data,&port_n,len_mess)==0)
            {
              
              FW_data.V_NTP_CIRCL=(signed char)port_n;
            }
           
           }
           
          }
    else if (strncmp((char*)post_data->name,"time_set", sizeof("dey_set")) == 0)
          {
     
            len_mess=strlen(post_data->data);
            if (scanf_time ((char*) post_data->data,data_mass,len_mess)==0)
            {
              //hrtc HAL_StatusTypeDef HAL_RTC_SetTime(RTC_HandleTypeDef *hrtc, RTC_TimeTypeDef *sTime, uint32_t Format)
              times.Hours=data_mass[0];
              times.Minutes=data_mass[1];
              times.Seconds=data_mass[2]; 
              HAL_RTC_SetTime(&hrtc,&times,RTC_FORMAT_BIN);
              
              
              //memcpy((uint8_t*)FW_data., (char*)IP_buf,3 );
            }
//            len_mess=strlen(post_data->data);
          //  memcpy((char*)FW_data.V_PASSWORD, (char*)post_data->data,len_mess );
          }
       else if (strncmp((char*)post_data->name,"dey_set", sizeof("time_set")) == 0)
          {
           // FW_data.V_Name_dev
            len_mess=strlen(post_data->data);
          //  memcpy((char*)FW_data.V_PASSWORD, (char*)post_data->data,len_mess );
              if (scanf_time ((char*) post_data->data,data_mass,len_mess)==0)
            {
             dates.Year=data_mass[2]-2000;
             dates.Month=data_mass[1]; 
             dates.Date=data_mass[0];
             HAL_RTC_SetDate(&hrtc,&dates,RTC_FORMAT_BIN);
           //  RTC_DateUpdate(&hrtc,)
            }
          }
    
    else if (strncmp((char*)post_data->name,"mess_on", sizeof("mess_on")) == 0)
          {
           // FW_data.V_Name_dev
            len_mess=strlen(post_data->data);
                memset((char*)FW_data.V_ON_MESS,0,strlen((char*)FW_data.V_ON_MESS));
              for (ct_temp0=0;ct_temp0<len_mess;ct_temp0++)
              {
                if (post_data->data[ct_temp0]==0x2b)
                  {
                    post_data->data[ct_temp0]=0x20;
                  }
              }
            memcpy((char*)FW_data.V_ON_MESS, (char*)post_data->data,len_mess );
          }
    else if (strncmp((char*)post_data->name,"mess_off", sizeof("mess_off")) == 0)
          {
           // FW_data.V_Name_dev
            len_mess=strlen(post_data->data);
            memset((char*)FW_data.V_OFF_MESS,0,strlen((char*)FW_data.V_OFF_MESS));
              for (ct_temp0=0;ct_temp0<len_mess;ct_temp0++)
              {
                if (post_data->data[ct_temp0]==0x2b)
                  {
                    post_data->data[ct_temp0]=0x20;
                  }
              }
            memcpy((char*)FW_data.V_OFF_MESS, (char*)post_data->data,len_mess );
          }
    else if (strncmp((char*)post_data->name,"output_type", sizeof("output_type")) == 0)
          {
           len_mess=strlen(post_data->data);
            if (scanf_port ((char*) post_data->data,&port_n,len_mess)==0)
            {
              FW_data.V_TYPE_OUT=port_n;
            }
          }
    else if (strncmp((char*)post_data->name,"save_all", sizeof("save_all")) == 0)
          {
            if (post_data->data[0]==0x31)
              {
                form_reple_to_save(SAVE_DATA_SETT);
               // while(flag_global_save_log==1){vTaskDelay(10);};
                vTaskDelay(100);
                flag_global_save_data=1;
              }
          }
    else if (strncmp((char*)post_data->name,"load_def", sizeof("load_def")) == 0)
          {
           if (post_data->data[0]==0x31)
              {
                form_reple_to_save(LOAD_DEF_DATA);
              //  while(flag_global_save_log==1){vTaskDelay(10);};
                vTaskDelay(100);
                flag_global_load_def=1;
              }
          }
    else if (strncmp((char*)post_data->name,"load_boot", sizeof("load_boot")) == 0)
          {
           form_reple_to_save(UPDATE_FW);           
          // while(flag_global_save_log==1){vTaskDelay(10);};
           vTaskDelay(100);
           flag_global_boot_mode=1;
          // jamp_to_boot();
          }
    else
     
    {
    
    }
    
  //}
  
}

void parser_post(char* buf_in,uint16_t buf_in_len,uint8_t index)
{
uint16_t ct_index,start_pars;
uint16_t start_par;
post_data_t elem_post_data;

 for(ct_index=buf_in_len;ct_index>0;ct_index--)
   {
     if (buf_in[ct_index]==0x0a)
       {
         if ((buf_in[ct_index-2]==0x0a)&&(buf_in[ct_index-1]==0x0d)&&(buf_in[ct_index-3]==0x0d))
           {
             start_pars=ct_index+1;
             ct_index=1;
           }
       }
   }
 start_par=start_pars;
 for(ct_index=start_pars;ct_index<=buf_in_len;ct_index++)
   {
    // elem_post_data=&post_mess[index_sec];
     if (buf_in[ct_index]=='=')
       {
         
         memset(elem_post_data.name,0,32);
         memcpy(elem_post_data.name,(uint8_t*)(buf_in+start_par),(ct_index-start_par));
         start_par=ct_index+1;
       }
     if ((buf_in[ct_index]=='&')||(ct_index==buf_in_len))
       {
         memset(elem_post_data.data,0,32);
         memcpy(elem_post_data.data,(uint8_t*)(buf_in+start_par),(ct_index-start_par));
         start_par=ct_index+1;
         param_run(&elem_post_data,index);
         //case_data_in
      //   elem_post_data.len_par=index_sec+1;
       // index_sec++;
         
       }
   }
 
}




 void page_html_swich(uint8_t index,struct netconn *conn,char* buf_list)
 {
   
  
  uint16_t len_buf_list;
  struct fs_file file;
  
  
    switch(index) {
          case 1:
            {
              fs_open(&file, "/img/netping.gif");
              netconn_write(conn, (const unsigned char*)(file.data), (size_t)file.len, NETCONN_NOCOPY);
              fs_close(&file);
            }
          break;
          case 2:
            {
              fs_open(&file, "/pass.html"); 
              netconn_write(conn, (const unsigned char*)(file.data), (size_t)file.len, NETCONN_NOCOPY);
              fs_close(&file);
            }
          break;
          case 3:
            { //watchdog.html
               vTaskDelay(10);
               len_buf_list=costr_watchdog1((char*)buf_list);
               netconn_write(conn, (char*)(buf_list), (size_t)len_buf_list, NETCONN_NOCOPY);               
               vTaskDelay(10);
               len_buf_list=costr_watchdog2((char*)buf_list);
               netconn_write(conn, (char*)(buf_list), (size_t)len_buf_list, NETCONN_NOCOPY);               
               vTaskDelay(10);
               len_buf_list=costr_watchdog3((char*)buf_list);
               netconn_write(conn, (char*)(buf_list), (size_t)len_buf_list, NETCONN_NOCOPY);               
               vTaskDelay(10);
                len_buf_list=costr_watchdog4((char*)buf_list);
               netconn_write(conn, (char*)(buf_list), (size_t)len_buf_list, NETCONN_NOCOPY);    
               vTaskDelay(10);
                len_buf_list=costr_watchdog5((char*)buf_list);
               netconn_write(conn, (char*)(buf_list), (size_t)len_buf_list, NETCONN_NOCOPY);       
               vTaskDelay(10);
            }
          break;
           case 4:
            {
              
                    
               vTaskDelay(10);
               len_buf_list=costr_page2((char*)buf_list);
               netconn_write(conn, (char*)(buf_list), (size_t)len_buf_list, NETCONN_NOCOPY);
                 vTaskDelay(10);
                  len_buf_list=costr_page2_1((char*)buf_list);
               netconn_write(conn, (char*)(buf_list), (size_t)len_buf_list, NETCONN_NOCOPY);
                 vTaskDelay(10);
                 
               len_buf_list=costr_page4((char*)buf_list);
               netconn_write(conn, (char*)(buf_list), (size_t)len_buf_list, NETCONN_NOCOPY);
                     
                   vTaskDelay(20);
            }
          break;
           case 5: //settings
            {
////               fs_open(&file, "/img/netping.gif");
////              netconn_write(conn, (const unsigned char*)(file.data), (size_t)file.len, NETCONN_NOCOPY);
////              fs_close(&file);
                    
                   vTaskDelay(10);
               len_buf_list=costr_page3((char*)buf_list);
               netconn_write(conn, (char*)(buf_list), (size_t)len_buf_list, NETCONN_NOCOPY);               
               vTaskDelay(10);
               
               len_buf_list=costr_page5((char*)buf_list);
               netconn_write(conn, (char*)(buf_list), (size_t)len_buf_list, NETCONN_NOCOPY);
                vTaskDelay(10);
               
               len_buf_list=costr_page6((char*)buf_list);
               netconn_write(conn, (char*)(buf_list), (size_t)len_buf_list, NETCONN_NOCOPY);               
                vTaskDelay(10);
               len_buf_list=costr_page6_1((char*)buf_list);
               netconn_write(conn, (char*)(buf_list), (size_t)len_buf_list, NETCONN_NOCOPY);               
                vTaskDelay(10);
               
               len_buf_list=costr_page7((char*)buf_list);
               netconn_write(conn, (char*)(buf_list), (size_t)len_buf_list, NETCONN_NOCOPY);
               
               
                   vTaskDelay(10);
               len_buf_list=costr_page7_1((char*)buf_list);
               netconn_write(conn, (char*)(buf_list), (size_t)len_buf_list, NETCONN_NOCOPY);
                     
                   vTaskDelay(20);
//               len_buf_list=costr_page4((char*)buf_list);
//               netconn_write(conn, (char*)(buf_list), (size_t)len_buf_list, NETCONN_NOCOPY);
            }
          break;
           case 6:  //logs
            {
                    
                   vTaskDelay(10);
                len_buf_list=costr_page8((char*)buf_list);
                netconn_write(conn, (char*)(buf_list), (size_t)len_buf_list, NETCONN_NOCOPY);               
                vTaskDelay(10);
                 uint16_t ct_mess;
                for(ct_mess=0;ct_mess<2048;ct_mess++)
                    {
                      if (FW_data.V_logs_struct.log_reple[ct_mess].dicr==0x7a)
                          {
                            memset(buf_list,0,sizeof(buf_list));
                            decode_reple(buf_list,&FW_data.V_logs_struct.log_reple[ct_mess]);
                            netconn_write(conn, (char*)(buf_list), (size_t)strlen(buf_list), NETCONN_NOCOPY);       
                            vTaskDelay(10);
                          }
                      else
                          {
                            break;
                          }
     
                    }
                len_buf_list=costr_page9((char*)buf_list);
                netconn_write(conn, (char*)(buf_list), (size_t)len_buf_list, NETCONN_NOCOPY);               
                vTaskDelay(20);
            }
          break;
           case 7:
            {
              fs_open(&file, "/404.html"); 
              netconn_write(conn, (const unsigned char*)(file.data), (size_t)file.len, NETCONN_NOCOPY);
              fs_close(&file);
            }
          break;
           case 8:
            {
              fs_open(&file, "/404.html"); 
              netconn_write(conn, (const unsigned char*)(file.data), (size_t)file.len, NETCONN_NOCOPY);
              fs_close(&file);
            }
          break;                         
          default  :
              {
              fs_open(&file, "/404.html"); 
              netconn_write(conn, (const unsigned char*)(file.data), (size_t)file.len, NETCONN_NOCOPY);
              fs_close(&file);
              }
          }
 }
// char buf_page[3000];
static void http_server_serve(struct netconn *conn1) 
{
   
 //post_data_t post_data[32];
  struct netbuf *inbuf;//=(struct netbuf*)pvPortMalloc(1500);
  err_t recv_err;
  char* buf;
   u16_t buflen;


        char* buf_page=(char*)pvPortMalloc(3000); 
//   char buf_page[3000];

  
 
  uint16_t len_buf_list;
  uint16_t ct_fpass=0;
  uint16_t start_key=0;

  
  /* Read the data from the port, blocking if nothing yet there. 
   We assume the request (the part we care about) is in one netbuf */
  recv_err = netconn_recv(conn1, &inbuf);
  
  if (recv_err == ERR_OK)
  {
    if (netconn_err(conn1) == ERR_OK) 
    {
      netbuf_data(inbuf, (void**)&buf, &buflen);
      
        if (flag_logon==1)
          {
            ct_logoff_time = 0;            
          }
        
       
        
     memset((void*)key_http,0,30);
      
      for (ct_fpass=0;ct_fpass<1024;ct_fpass++)
        {
          if (strncmp((char*)&buf[ct_fpass],"Basic",5) == 0)
            {
              start_key=ct_fpass+6;
              break;
            }
        }
      memcpy((char*)buf_page,(char*)&buf[start_key],16);  
      unbase64((char*)buf_page,16,&key_http_len,key_http);


      sprintf(buf_page,"%s:%s",FW_data.V_LOGIN,FW_data.V_PASSWORD);
      key_http_len=strlen(buf_page);
      
        if (strncmp(key_http,buf_page,key_http_len) != 0)
          {          
            len_buf_list=costr_pass((char*)buf_page);
            netconn_write(conn1, (char*)(buf_page), (size_t)len_buf_list, NETCONN_NOCOPY);
            vTaskDelay(20);
            flag_logon=0;
            flag_req_logon=1;
            // memcpy(key_http,"admin:admin",12); 
          }   
       else
          {
           if ((flag_logon==0)&&(strncmp(key_http,buf_page,key_http_len) == 0))
            {
              flag_logon=1;
              page_n=4;
            }
          }
      
      
  if (flag_logon==1)    
  {
      /* Is this an HTTP GET command? (only check the first 5 chars, since
      there are other formats for GET, and we're keeping it very simple )*/
      if ((buflen >=5) && (strncmp(buf, "GET /", 5) == 0))
      {        
            if (strncmp((char const *)buf,"GET /handler.php?login=",23)==0)
              {
                 page_n=4;
                 page_html_swich(page_n,conn1,buf_page);
              }
            else
              if ((strncmp((char const *)buf,"GET /index.html",15)==0)||(strncmp((char const *)buf,"GET / HTTP/1.1",14)==0))
             {
               page_n=4;
               page_sost=1;
               page_html_swich(page_n,conn1,buf_page);
             }
            else if (strncmp((char const *)buf,"GET /logs.html",14)==0)
             {
               page_n=6;
               page_sost=2;
               page_html_swich(page_n,conn1,buf_page);
             }
            else if (strncmp((char const *)buf,"GET /watchdog.html",18)==0)
             {
               page_n=3;
               page_sost=2;
               page_html_swich(page_n,conn1,buf_page);
             }
            else if (strncmp((char const *)buf,"GET /settings.html",14)==0)
              {
               page_n=5;
               page_sost=3;
               page_html_swich(page_n,conn1,buf_page);
              }
            else if (strncmp((char const *)buf,"GET /img/netping.gif",17)==0)
            {
              page_n=1;
              page_html_swich(page_n,conn1,buf_page);
            }
            else  
             {
              /* Load Error page */
               page_n=3;
               page_html_swich(page_n,conn1,buf_page);
             }
          //}        
      } 
      else
      {

            if ((buflen >=5) && (strncmp(buf, "POST /", 5) == 0))
              {
                if (strncmp(buf, "POST /", 5) == 0)
                    {      
                      if (page_sost==1)
                           {
                             page_n=4;
                             page_html_swich(page_n,conn1,buf_page);
                             vTaskDelay(20);
                             parser_post(buf,buflen,page_sost);
                           }         
                        else 
                           {  
                             if (page_sost==3)
                              {
                               parser_post(buf,buflen,page_sost);
                               page_n=5;
                               vTaskDelay(20);
                               page_html_swich(page_n,conn1,buf_page);
                              }
                             if(page_sost==2)
                               {
                                parser_post(buf,buflen,page_sost);
                                page_n=3;
                                vTaskDelay(20);
                                page_html_swich(page_n,conn1,buf_page);
                               }
                            }            
                    }
              }
        
      }
     }
    }
  }
//vTaskDelay(100);
  vPortFree(buf_page);
 //   netbuf_delete(buf_page);
  /* Close the connection (server closes in HTTP) */
  netconn_close(conn1);
  
  /* Delete the buffer (netconn_recv gives us ownership,
   so we have to make sure to deallocate the buffer) */
 // vPortFree(inbuf);
  netbuf_delete(inbuf);

          
}


/**
  * @brief  http server thread 
  * @param arg: pointer on argument(not used here) 
  * @retval None
  */
static void http_server_netconn_thread(void *arg)
{ 
  struct netconn *conn, *newconn;
  err_t err, accept_err;
  for(;;)
  {
  /* Create a new TCP connection handle */
  conn = netconn_new(NETCONN_TCP);
  
  if (conn!= NULL)
  {
    /* Bind to port 80 (HTTP) with default IP address */
    err = netconn_bind(conn, NULL, FW_data.V_WEB_PORT);
    //  err = netconn_bind(conn, NULL,80);
    if (err == ERR_OK)
    {
      /* Put the connection into LISTEN state */
      netconn_listen(conn);
  
      while(1) 
      {
        accept_err = netconn_accept(conn, &newconn);
        if(accept_err == ERR_OK)
        {
          /* serve connection */
          vTaskDelay(50);
          http_server_serve(newconn);
          vTaskDelay(50);
           /* delete connection */
          netconn_delete(newconn);          
        }
      }
    }
  }
}
}
/**
  * @brief  Initialize the HTTP server (start its thread) 
  * @param  none
  * @retval None
  */
void http_server_netconn_init()
{
  sys_thread_new("HTTP", http_server_netconn_thread, NULL,1*1024, osPriorityHigh);
}

/**
  * @brief  Create and send a dynamic Web Page. This page contains the list of 
  *         running tasks and the number of page hits. 
  * @param  conn pointer on connection structure 
  * @retval None
  */

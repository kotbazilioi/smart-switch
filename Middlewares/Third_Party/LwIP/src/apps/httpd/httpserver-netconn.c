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
  * 4. This software, including modificatios and/or derivative works of this 
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
#include "app.h"
#include "smtp.h"
#include "heap_5.h"
#include "utf8_code.h"
#define delay_send 5
/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
#define WEBSERVER_THREAD_PRIO    ( osPriorityAboveNormal )
typedef struct post_data_t
 {
   char name[32];
   char data[256];
 
   uint8_t len_par;
 }post_data_t;


/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
u32_t nPageHits = 0;
char http_ok[] = {0x48,0x54,0x54,0x50,0x2f,0x31,0x2e,0x31,0x20,0x32,0x30,0x30,0x20,0x4f,0x4b,0x0d,0x0a};
uint8_t flag_req_logon=0;
uint8_t page_n,page_sost=1;

/* Private function prototypes -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/
static const unsigned char PAGE_HEADER_200_OK[] = {
  //"HTTP/1.1 200 OK"
  0x48,0x54,0x54,0x50,0x2f,0x31,0x2e,0x30,0x20,0x32,0x30,0x30,0x20,0x4f,0x4b,0x0d,
  0x0a,
  //zero
  0x00
};
const unsigned char PAGE_HEADER_303_OK[] = 
"HTTP/1.1 303 See Other\n\r"
"Location: /index.html\n\r" ;
const unsigned char PAGE_HEADER_303_settings[] = 
"HTTP/1.1 303 See Other\n\r"
"Location: /settings.html\n\r" ;
const unsigned char PAGE_HEADER_303_rasp[] = 
"HTTP/1.1 303 See Other\n\r"
"Location: /rasp.html\n\r" ;
const unsigned char PAGE_HEADER_303_watchdog[] = 
"HTTP/1.1 303 See Other\n\r"
"Location: /watchdog.html\n\r" ;
const unsigned char PAGE_HEADER_303_email[] = 
"HTTP/1.1 303 See Other\n\r"
"Location: /email.html\n\r" ;
static const unsigned char PAGE_HEADER_SERVER[] = {
  //"Server: lwIP/1.3.1 (http://savannah.nongnu.org/projects/lwip)"
  0x53,0x65,0x72,0x76,0x65,0x72,0x3a,0x20,0x6c,0x77,0x49,0x50,0x2f,0x31,0x2e,0x33,
  0x2e,0x31,0x20,0x28,0x68,0x74,0x74,0x70,0x3a,0x2f,0x2f,0x73,0x61,0x76,0x61,0x6e,
  0x6e,0x61,0x68,0x2e,0x6e,0x6f,0x6e,0x67,0x6e,0x75,0x2e,0x6f,0x72,0x67,0x2f,0x70,
  0x72,0x6f,0x6a,0x65,0x63,0x74,0x73,0x2f,0x6c,0x77,0x69,0x70,0x29,0x0d,0x0a,
  //zero
  0x00
};
static const unsigned char PAGE_HEADER_CONTENT_TEXT[] = {
  //"Content-type: text/html"
  0x43,0x6f,0x6e,0x74,0x65,0x6e,0x74,0x2d,0x74,0x79,0x70,0x65,0x3a,0x20,0x74,0x65,
  0x78,0x74,0x2f,0x68,0x74,0x6d,0x6c,0x0d,0x0a,0x0d,0x0a,
  //zero
  0x00
};
//*
static const unsigned char PAGE_HEADER_CONTENT_STREAM[] = {
  //"Content-Type: application/octet-stream"
  0x43,0x6f,0x6e,0x74,0x65,0x6e,0x74,0x2d,0x54,0x79,0x70,0x65,0x3a,0x20,0x61,0x70,
  0x70,0x6c,0x69,0x63,0x61,0x74,0x69,0x6f,0x6e,0x2f,0x6f,0x63,0x74,0x65,0x74,0x2d,
  0x73,0x74,0x72,0x65,0x61,0x6d,0x0d,0x0a,
  //zero
  0x00
};
static const unsigned char PAGE_HEADER_LEN[] = {
  //"Content-Length: "
  0x43,0x6f,0x6e,0x74,0x65,0x6e,0x74,0x2d,0x4c,0x65,0x6e,0x67,0x74,0x68,0x3a,0x20,
  //zero
  0x00
};
static const unsigned char PAGE_HEADER_BYTES[] = {
  //"Accept-Ranges: bytes"
  0x41,0x63,0x63,0x65,0x70,0x74,0x2d,0x52,0x61,0x6e,0x67,0x65,0x73,0x3a,0x20,0x62,
  0x79,0x74,0x65,0x73,0x0d,0x0a,0x0d,0x0a,
  //zero
  0x00
};
void DynWebPageStr(struct netconn *conn);
void param_utf8_dec(post_data_t* post_data);
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
        if ((data[ct_char]!='.')&&(data[ct_char]!=','))
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
        if ((data[ct_char]!='.')&&(data[ct_char]!=','))
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
        if ((data[ct_char]!='.')&&(data[ct_char]!=','))
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
        if (((data[ct_char]!='.')&&(data[ct_char]!=','))||(ct_char!=0))
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


void param_utf8_dec(post_data_t* post_data)
{
  char temp_mess[256]={0};
  char temp_beakup[256]={0};
  uint16_t temp_hex={0};
  uint8_t ct_char,ct_mess,temp;
  uint8_t len_mess = strlen(post_data->data);
  ct_mess=0;
  temp=0;
//  memcpy(temp_beakup,post_data->data,sizeof(post_data->data));
 
      for(ct_char=0;ct_char<len_mess;ct_char++)
      {
         if(post_data->data[ct_char]=='%')
          {
           ct_char=ct_char+2;
           ct_mess=ct_mess+2;
          }
         else
          {
           temp_beakup[ct_mess]=1;
           temp++;
           ct_mess++;
          }
           

      }
     
     ct_mess=0;
   
     for(ct_char=0;ct_char<len_mess;ct_char++)
      {
         if(post_data->data[ct_char]=='%')
          {
           ct_char++;
          }
        temp_mess[ct_mess]=post_data->data[ct_char];
       
        ct_mess++;
      }
     
     len_mess = strlen(temp_mess);

    for(ct_char=0;ct_char<len_mess;ct_char++)
      {
        if (temp_beakup[ct_char]==0)
        {
         if (temp_mess[ct_char]<0x40)
          {
            temp_mess[ct_char]=temp_mess[ct_char]-0x30;
          }
         if (temp_mess[ct_char]>0x39)
          {
            temp_mess[ct_char]=temp_mess[ct_char]-0x37;
          }
        }
      }
     memset(post_data->data,0,128);
    len_mess=len_mess-temp;
    len_mess=(len_mess/2)+temp;
    ct_mess=0;
    for(ct_char=0;ct_char<len_mess;ct_char++)
      {
        if ((temp_mess[ct_mess]<0x30)&&(temp_mess[ct_mess]!='+')&&(temp_mess[ct_mess]!='_')&&(temp_mess[ct_mess]!='-')&&(temp_mess[ct_mess]!='.')&&(temp_mess[ct_mess]!=','))
         {
           
          temp_hex=temp_mess[ct_mess]<<4;
          temp_hex=temp_hex|(temp_mess[ct_mess+1]);
          post_data->data[ct_char]=temp_hex;
          ct_mess++;
         // ct_char++;
         }
        else
        {
          post_data->data[ct_char]=temp_mess[ct_mess];  
          
        }              
      
       ct_mess++;
      }
  
    
    
      
}


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
             HAL_RTCEx_BKUPWrite(&hrtc,3,1);
             save_reple_log(reple_to_save);
             flag_global_save_log=0;
             vTaskDelay(3000);
             jamp_to_app();
             
          }
     else if (strncmp((char*)post_data->name,"output_set", sizeof("output_set")) == 0)
          {
           // FW_data.V_Name_dev
            len_mess=strlen(post_data->data);
            if (post_data->data[0]==0x30)
            {
              if (flag_runtime_out[1]==1)
               {
               form_reple_to_save(SWICH_OFF_WEB);
               flag_global_swich_out=SWICH_OFF_WEB;
               HAL_RTCEx_BKUPWrite(&hrtc,1,0);
               }
              else
               {
                 form_reple_to_save(SWICH_OFF_WEB_N);
               }
            }
            else if (post_data->data[0]==0x31)
              {
                if (flag_runtime_out[1]==0)
                {
                form_reple_to_save(SWICH_ON_WEB);
                flag_global_swich_out=SWICH_ON_WEB;
                 HAL_RTCEx_BKUPWrite(&hrtc,1,1);
                } 
                else
                {
                  form_reple_to_save(SWICH_ON_WEB_N);
                }
              
              }
            else if (post_data->data[0]==0x32)
              {
                if (flag_runtime_out[1]==1)
                {
                 form_reple_to_save(SWICH_TOLG_WEB);
                 flag_global_swich_out=SWICH_TOLG_WEB;
                 HAL_RTCEx_BKUPWrite(&hrtc,1,2);
                }
                else
                {
                 form_reple_to_save(SWICH_TOLG_WEB_N);
                }
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
     else if (strncmp((char*)post_data->name,"syslog_addr", sizeof("syslog_addr")) == 0)
          {
             len_mess=strlen(post_data->data);
            if (scanf_ip ((char*) post_data->data,IP_buf,len_mess)==0)
            {
              memcpy((uint8_t*)FW_data.V_IP_SYSL, (char*)IP_buf,4 );
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
    
     else if (strncmp((char*)post_data->name,"V_D_d",5) == 0)
          {
                FW_data.V_D_TIME[post_data->name[5]-0x30].set_up_day=1;
          }
     else if (strncmp((char*)post_data->name,"V_DT",4) == 0)
          {
            uint8_t u_d=0;
            uint8_t slot=0;
            uint8_t n_day=0;
            u_d=post_data->name[4]-0x30;
            n_day=post_data->name[5]-0x30;
            slot=post_data->name[6]-0x30;
            if ((u_d==0)&&((slot==0)||(slot==3))&&(n_day==0))
            {
              FW_data.V_D_TIME[0].set_up_day=0;
              FW_data.V_D_TIME[1].set_up_day=0;
              FW_data.V_D_TIME[2].set_up_day=0;
              FW_data.V_D_TIME[3].set_up_day=0;
              FW_data.V_D_TIME[4].set_up_day=0;
              FW_data.V_D_TIME[5].set_up_day=0;
              FW_data.V_D_TIME[6].set_up_day=0;
            }
            if(u_d==0)
            {
              FW_data.V_D_TIME[n_day].time_data[slot].on_swich_h=(post_data->data[0]-0x30)*10+(post_data->data[1]-0x30);
              if (FW_data.V_D_TIME[n_day].time_data[slot].on_swich_h>61)
                {
                 FW_data.V_D_TIME[n_day].time_data[slot].on_swich_h=61;
                }
              FW_data.V_D_TIME[n_day].time_data[slot].on_swich_m=(post_data->data[5]-0x30)*10+(post_data->data[6]-0x30);  
              if (FW_data.V_D_TIME[n_day].time_data[slot].on_swich_m>61)
                {
                 FW_data.V_D_TIME[n_day].time_data[slot].on_swich_m=61;
                }
            }
            else
            {
              FW_data.V_D_TIME[n_day].time_data[slot].off_swich_h=(post_data->data[0]-0x30)*10+(post_data->data[1]-0x30);
              if (FW_data.V_D_TIME[n_day].time_data[slot].off_swich_h>61)
                {
                 FW_data.V_D_TIME[n_day].time_data[slot].off_swich_h=61;
                }
              
              FW_data.V_D_TIME[n_day].time_data[slot].off_swich_m=(post_data->data[5]-0x30)*10+(post_data->data[6]-0x30);                
              if (FW_data.V_D_TIME[n_day].time_data[slot].off_swich_m>61)
                {
                 FW_data.V_D_TIME[n_day].time_data[slot].off_swich_m=61;
                }
            }

          }
     else if (strncmp((char*)post_data->name,"V_FLAG_EN_RASP", sizeof("V_FLAG_EN_RASP")) == 0)
          {
           if (strncmp((char*)post_data->data,"ON", sizeof("ON")) == 0)
            {
              FW_data.V_FLAG_EN_RASP=1;
            }
           else
            {
              FW_data.V_FLAG_EN_RASP=0;
            }
          }
     else if (strncmp((char*)post_data->name,"V_FLAG_EN_EMAIL", sizeof("V_FLAG_EN_EMAIL")) == 0)
          {
             FW_data.V_FLAG_DEF_EMAIL=0;
           if (strncmp((char*)post_data->data,"ON", sizeof("ON")) == 0)
            {
              FW_data.V_FLAG_EN_EMAIL=1;
              
            }
           else
            {
              FW_data.V_FLAG_EN_EMAIL=0;
            }
          }
       else if (strncmp((char*)post_data->name,"V_FLAG_DEF_EMAIL", sizeof("V_FLAG_DEF_EMAIL")) == 0)
          {
           if (strncmp((char*)post_data->data,"1", sizeof("1")) == 0)
            {
              FW_data.V_FLAG_DEF_EMAIL=1;
               memcpy(FW_data.V_EMAIL_ADDR,"mail.smtp2go.com",sizeof("mail.smtp2go.com"));
               FW_data.V_FLAG_EMAIL_PORT=25;
               memcpy(FW_data.V_LOGIN_SMTP,"sys.microtech@mail.ru",sizeof("sys.microtech@mail.ru"));
               memcpy(FW_data.V_PASSWORD_SMTP,"dG40NmJ3MDVmNzAw",sizeof("dG40NmJ3MDVmNzAw"));
//               memset(FW_data.V_EMAIL_FROM,0,32);
//               memset(FW_data.V_EMAIL_TO,0,32);
//               memset(FW_data.V_EMAIL_CC1,0,32);
//               memset(FW_data.V_EMAIL_CC2,0,32);
//               memset(FW_data.V_EMAIL_CC3,0,32);
            }
           else
            {
              FW_data.V_FLAG_DEF_EMAIL=0;
            }
          }  
         else if (strncmp((char*)post_data->name,"V_LOGIN_SMTP", sizeof("V_LOGIN_SMTP")) == 0)
          {
           // FW_data.V_Name_dev
            if(FW_data.V_FLAG_DEF_EMAIL==0)
            {
            len_mess=strlen(post_data->data);
            memset((char*)FW_data.V_LOGIN_SMTP,0,32);
            memcpy((char*)FW_data.V_LOGIN_SMTP, (char*)post_data->data,len_mess );
            }
          }
    else if (strncmp((char*)post_data->name,"V_PASSWORD_SMTP", sizeof("V_PASSWORD_SMTP")) == 0)
          {
            if(FW_data.V_FLAG_DEF_EMAIL==0)
            {
           // FW_data.V_Name_dev
            len_mess=strlen(post_data->data);
            memset((char*)FW_data.V_PASSWORD_SMTP,0,32);
            memcpy((char*)FW_data.V_PASSWORD_SMTP, (char*)post_data->data,len_mess );
            }
          }
    else if (strncmp((char*)post_data->name,"V_EMAIL_ADDR", sizeof("V_EMAIL_ADDR")) == 0)
          {
            if(FW_data.V_FLAG_DEF_EMAIL==0)
            {
           // FW_data.V_Name_dev
            len_mess=strlen(post_data->data);
            memset((char*)FW_data.V_EMAIL_ADDR,0,32);
            memcpy((char*)FW_data.V_EMAIL_ADDR, (char*)post_data->data,len_mess );
            }
          }
    else if (strncmp((char*)post_data->name,"V_FLAG_EMAIL_PORT", sizeof("V_FLAG_EMAIL_PORT")) == 0)
          {
            if(FW_data.V_FLAG_DEF_EMAIL==0)
            {
             len_mess=strlen(post_data->data);
            if (scanf_port ((char*) post_data->data,&port_n,len_mess)==0)
            {
              FW_data.V_FLAG_EMAIL_PORT=port_n;
            }
            }
          }
    else if (strncmp((char*)post_data->name,"V_EMAIL_FROM", sizeof("V_EMAIL_FROM")) == 0)
          {
//            if(FW_data.V_FLAG_DEF_EMAIL==0)
//            {
           // FW_data.V_Name_dev
            len_mess=strlen(post_data->data);
            memset((char*)FW_data.V_EMAIL_FROM,0,32);
            memcpy((char*)FW_data.V_EMAIL_FROM, (char*)post_data->data,len_mess );
//            }
          }
     else if (strncmp((char*)post_data->name,"V_EMAIL_TO", sizeof("V_EMAIL_TO")) == 0)
          {
//            if(FW_data.V_FLAG_DEF_EMAIL==0)
//            {
           
            len_mess=strlen(post_data->data);
            memset((char*)FW_data.V_EMAIL_TO,0,32);
            memcpy((char*)FW_data.V_EMAIL_TO, (char*)post_data->data,len_mess );
           // }
          }
     else if (strncmp((char*)post_data->name,"V_EMAIL_CC1", sizeof("V_EMAIL_CC1")) == 0)
          {
//            if(FW_data.V_FLAG_DEF_EMAIL==0)
//            {
           // FW_data.V_Name_dev
            len_mess=strlen(post_data->data);
            memset((char*)FW_data.V_EMAIL_CC1,0,32);
            memcpy((char*)FW_data.V_EMAIL_CC1, (char*)post_data->data,len_mess );
          //  }
          }
    else if (strncmp((char*)post_data->name,"V_EMAIL_CC2", sizeof("V_EMAIL_CC2")) == 0)
          {
//            if(FW_data.V_FLAG_DEF_EMAIL==0)
//            {
           // FW_data.V_Name_dev
            len_mess=strlen(post_data->data);
            memset((char*)FW_data.V_EMAIL_CC2,0,32);
            memcpy((char*)FW_data.V_EMAIL_CC2, (char*)post_data->data,len_mess );
       //     }
          }
    else if (strncmp((char*)post_data->name,"V_EMAIL_CC3", sizeof("V_EMAIL_CC3")) == 0)
          {
//            if(FW_data.V_FLAG_DEF_EMAIL==0)
//            {
           // FW_data.V_Name_dev
            len_mess=strlen(post_data->data);
            memset((char*)FW_data.V_EMAIL_CC3,0,32);
            memcpy((char*)FW_data.V_EMAIL_CC3, (char*)post_data->data,len_mess );
//            }
          } 
    
    else if (strncmp((char*)post_data->name,"V_SOST_ERR_RASP", sizeof("V_SOST_ERR_RASP")) == 0)
          {
           if (strncmp((char*)post_data->data,"%D0%91%D0%B3", 11) == 0)
            {
              FW_data.V_SOST_ERR_RASP=0;
            }
           if (strncmp((char*)post_data->data,"%D0%92%D0%BA%", 11) == 0)
            {
              FW_data.V_SOST_ERR_RASP=1;
            }           
            if (strncmp((char*)post_data->data,"%D0%92%D1%8B%", 11) == 0)
            {
              FW_data.V_FLAG_EN_RASP=2;
            }
          }
    else if (strncmp((char*)post_data->name,"V_RD_DATA_DAY", 12) == 0)
          {
            uint8_t u_d=0;
            u_d=post_data->name[14]-0x30;
            FW_data.V_RD_DATA.data[u_d].year = (post_data->data[2]-0x30)*10+(post_data->data[3]-0x30);
            FW_data.V_RD_DATA.data[u_d].month=(post_data->data[5]-0x30)*10+(post_data->data[6]-0x30)-1;
            FW_data.V_RD_DATA.data[u_d].day=(post_data->data[8]-0x30)*10+(post_data->data[9]-0x30)-1;  
            
            
            if (FW_data.V_RD_DATA.data[u_d].year>61)
              {
                FW_data.V_RD_DATA.data[u_d].year =61;
              }
            
            if (FW_data.V_RD_DATA.data[u_d].day>61)
              {
                FW_data.V_RD_DATA.data[u_d].day =61;
              }
            if (FW_data.V_RD_DATA.data[u_d].month>61)
              {
                FW_data.V_RD_DATA.data[u_d].month =61;
              }
          }

     else if (strncmp((char*)post_data->name,"V_RD_DATA_PODM", 12) == 0)
          {
            uint8_t u_d=0;
            u_d=post_data->name[14]-0x30;
             if (strncmp((char*)post_data->data,"%D0%9F%D0%9D",12) == 0)
            {
              FW_data.V_RD_DATA.restore_day[u_d]=RTC_WEEKDAY_MONDAY;
            }
            if (strncmp((char*)post_data->data,"%D0%92%D0%A2",12) == 0)
            {
              FW_data.V_RD_DATA.restore_day[u_d]=RTC_WEEKDAY_TUESDAY;
            }
            if (strncmp((char*)post_data->data,"%D0%A1%D0%A0",12) == 0)
            {
              FW_data.V_RD_DATA.restore_day[u_d]=RTC_WEEKDAY_WEDNESDAY;
            }            
            if (strncmp((char*)post_data->data,"%D0%A7%D0%A2",12) == 0)
            {
              FW_data.V_RD_DATA.restore_day[u_d]=RTC_WEEKDAY_THURSDAY;
            }
            if (strncmp((char*)post_data->data,"%D0%9F%D0%A2",12) == 0)
            {
              FW_data.V_RD_DATA.restore_day[u_d]=RTC_WEEKDAY_FRIDAY;
            }
            if (strncmp((char*)post_data->data,"%D0%A1%D0%91",12) == 0)
            {
              FW_data.V_RD_DATA.restore_day[u_d]=RTC_WEEKDAY_SATURDAY;
            }
            if (strncmp((char*)post_data->data,"%D0%92%D0%A1",12) == 0)
            {
              FW_data.V_RD_DATA.restore_day[u_d]=RTC_WEEKDAY_SUNDAY;
            }
            if (strncmp((char*)post_data->data,"%D0%9F1",7) == 0)
            {
              FW_data.V_RD_DATA.restore_day[u_d]=7;
            }
            if (strncmp((char*)post_data->data,"%D0%9F2",7) == 0)
            {
              FW_data.V_RD_DATA.restore_day[u_d]=8;
            }
            if (strncmp((char*)post_data->data,"%D0%9F3",7) == 0)
            {
              FW_data.V_RD_DATA.restore_day[u_d]=9;
            }
            
          
          }
    
    
    else if (strncmp((char*)post_data->name,"login", sizeof("login")) == 0)
          {
           // FW_data.V_Name_dev
            len_mess=strlen(post_data->data);
             memset((char*)FW_data.V_LOGIN,0,16);
            memcpy((char*)FW_data.V_LOGIN, (char*)post_data->data,len_mess );
          }
    else if (strncmp((char*)post_data->name,"password", sizeof("password")) == 0)
          {
           // FW_data.V_Name_dev
            len_mess=strlen(post_data->data);
            memset((char*)FW_data.V_PASSWORD,0,16);
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
    else if ((strncmp((char*)post_data->name,"save_all", sizeof("save_all")) == 0)||(strncmp((char*)post_data->name,"save_rasp_sec", sizeof("save_rasp_sec")-2) == 0))
          {
            if (post_data->data[0]==0x31)
              {
                form_reple_to_save(SAVE_DATA_SETT);
               // while(flag_global_save_log==1){vTaskDelay(10);};
                
                vTaskDelay(100);
                flag_global_save_data=1;
                
               // form_reple_to_save(RESETL);
              //  flag_global_reset_mode=1;
              HAL_RTCEx_BKUPWrite(&hrtc,3,1);
            //    save_reple_log(reple_to_save);
                flag_global_save_log=0;
           //     vTaskDelay(1000);
//                jamp_to_app();
             
             
               // flag_global_reset_mode=1;
              }
          }
    else if (strncmp((char*)post_data->name,"test_email", sizeof("test_email")) == 0)
          {
            flag_my_smtp_test=1;
           //send_test_email();
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
           page_n=9;
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
uint8_t ct_find_a;
post_data_t elem_post_data;
uint8_t post_size=0;
uint8_t ct_post_ch=0;

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
         memset(elem_post_data.data,0,256);
         memcpy(elem_post_data.data,(uint8_t*)(buf_in+start_par),(ct_index-start_par));
             
         for(ct_find_a=0;ct_find_a<(ct_index-start_par);ct_find_a++)
         {
         if ((elem_post_data.data[ct_find_a]==0x25)&&(elem_post_data.data[ct_find_a+1]==0x34)&&(elem_post_data.data[ct_find_a+2]==0x30))
          {
            elem_post_data.data[ct_find_a]=0x40;
            memcpy((uint8_t*)(elem_post_data.data+ct_find_a+1),(uint8_t*)(elem_post_data.data+ct_find_a+3),((ct_index-start_par)-ct_find_a-2));
            elem_post_data.data[ct_index-start_par-1]=0;
            elem_post_data.data[ct_index-start_par-2]=0;
          }
         }
         start_par=ct_index+1;
         if ((memcmp(elem_post_data.name,"name_dev",sizeof("name_dev"))==0)||(memcmp(elem_post_data.name,"geo_place",sizeof("geo_place"))==0)||(memcmp(elem_post_data.name,"call_data",sizeof("call_data"))==0))
         {
         post_size=strlen(elem_post_data.data);
         for (ct_post_ch=0;ct_post_ch<post_size;ct_post_ch++)
            {
              if (elem_post_data.data[ct_post_ch]=='%')
                {  
                  param_utf8_dec(&elem_post_data);
                  break;
                }
            }
         }
         param_run(&elem_post_data,index);      
         
       }
   }
 
}


//uint8_t tempm[256]={0};
void HTTP_API_swich(struct netconn *conn,char* buf_list)
 { 
   uint8_t index=0;
   uint32_t len_buf_list=0;
   uint8_t relay_n=0;
   uint8_t data_s=0;
   uint8_t data=0;
   uint8_t delay_time=0;
   struct fs_file file;
     
   if (((buf_list[16]<0x32)&&(buf_list[16]>0x30))||(buf_list[16]=='n'))
    {
      relay_n=buf_list[16]-0x30;
      
      if (buf_list[17]!=0x20)
        {
          if (buf_list[18]==0x30)
            {
              data_s=0;
              index =2; 
            }
          if (buf_list[18]==0x31)
            {
              data_s=1;
              index =3; 
            }
          if (buf_list[18]=='f')
          {
            index=4;
            
            if ((buf_list[19]==',')&&(buf_list[20]<0x3a)&&(buf_list[21]<0x3a)&&(buf_list[20]>0x2f)&&(buf_list[21]>0x2f))
              { 
                  HTTP_pulse_time=((buf_list[20]-0x30)*10 +(buf_list[21]-0x30));
                  index =5; 
                  
               
              
              }
          }
        }
      else
        {
          index =1; 
        }
      
        
    }
     
   
   
   data= HAL_RTCEx_BKUPRead(&hrtc,1);
   
   switch(index) {
      case 0:
            {
              sprintf(buf_list,"%s%s%s relay_result('error')\;",PAGE_HEADER_200_OK,PAGE_HEADER_SERVER,PAGE_HEADER_CONTENT_TEXT);
              len_buf_list = strlen(buf_list);
              netconn_write(conn, (char*)(buf_list), (size_t)len_buf_list, NETCONN_COPY);      
              vTaskDelay(delay_send);                 
            }
          break;
          case 1:
            { 
           
              sprintf(buf_list,"%s%s%s relay_result('%s',%d,%d)\;",PAGE_HEADER_200_OK,PAGE_HEADER_SERVER,PAGE_HEADER_CONTENT_TEXT,"ok",flag_runtime_front[1],flag_runtime_out[1]);
              len_buf_list = strlen(buf_list);
              netconn_write(conn, (char*)(buf_list), (size_t)len_buf_list, NETCONN_COPY);      
              vTaskDelay(delay_send);                 
            }
          break;
           case 2:
            {
              if (flag_runtime_out[1]==1)
                {
                  flag_global_swich_out=SWICH_OFF_HTTP;
                  form_reple_to_save(SWICH_OFF_HTTP);
                  HAL_RTCEx_BKUPWrite(&hrtc,1,0);
                  sprintf(buf_list,"%s%s%s relay_result('%s')\;",PAGE_HEADER_200_OK,PAGE_HEADER_SERVER,PAGE_HEADER_CONTENT_TEXT,"ok");
                  len_buf_list = strlen(buf_list);
                  netconn_write(conn, (char*)(buf_list), (size_t)len_buf_list, NETCONN_COPY);      
                  vTaskDelay(delay_send);                 
                }
              else
                {
                   form_reple_to_save(SWICH_OFF_HTTP_N);
                  sprintf(buf_list,"%s%s%s relay_result('%s')\;",PAGE_HEADER_200_OK,PAGE_HEADER_SERVER,PAGE_HEADER_CONTENT_TEXT,"uneffecrtive");
                  len_buf_list = strlen(buf_list);
                  netconn_write(conn, (char*)(buf_list), (size_t)len_buf_list, NETCONN_COPY);      
                  vTaskDelay(delay_send);       
                
                }
            }
          break;
           case 3:
            {
              if (flag_runtime_out[1]==0)
                {
                  flag_global_swich_out=SWICH_ON_HTTP;
                   form_reple_to_save(SWICH_ON_HTTP);
                  HAL_RTCEx_BKUPWrite(&hrtc,1,1);
                  sprintf(buf_list,"%s%s%s relay_result('%s')\;",PAGE_HEADER_200_OK,PAGE_HEADER_SERVER,PAGE_HEADER_CONTENT_TEXT,"ok");
                  len_buf_list = strlen(buf_list);
                  netconn_write(conn, (char*)(buf_list), (size_t)len_buf_list, NETCONN_COPY);      
                  vTaskDelay(delay_send);                 
                }
               else
                {
                  form_reple_to_save(SWICH_ON_HTTP_N);
                  sprintf(buf_list,"%s%s%s relay_result('%s')\;",PAGE_HEADER_200_OK,PAGE_HEADER_SERVER,PAGE_HEADER_CONTENT_TEXT,"uneffecrtive");
                  len_buf_list = strlen(buf_list);
                  netconn_write(conn, (char*)(buf_list), (size_t)len_buf_list, NETCONN_COPY);      
                  vTaskDelay(delay_send);       
                
                }
            }
          break;
           case 4:
            {
              if (flag_runtime_out[1]==0)
                {
                  form_reple_to_save(SWICH_ON_HTTP);
                 flag_global_swich_out=SWICH_ON_HTTP;
                 form_reple_to_save(SWICH_ON_HTTP);
                 HAL_RTCEx_BKUPWrite(&hrtc,1,1);
                }
              else
              {
                form_reple_to_save(SWICH_OFF_HTTP);
                 flag_global_swich_out=SWICH_OFF_HTTP;
                 form_reple_to_save(SWICH_OFF_HTTP);
                 HAL_RTCEx_BKUPWrite(&hrtc,1,0);
              }           
              
              sprintf(buf_list,"%s%s%s relay_result('%s')\;",PAGE_HEADER_200_OK,PAGE_HEADER_SERVER,PAGE_HEADER_CONTENT_TEXT,"ok");
              len_buf_list = strlen(buf_list);
              netconn_write(conn, (char*)(buf_list), (size_t)len_buf_list, NETCONN_COPY);      
              vTaskDelay(delay_send);                 
            }
          break;
             case 5:
            {
               if (flag_runtime_out[1]!=0)
                {
                  flag_global_swich_out=SWICH_TOLG_HTTP;
                  form_reple_to_save(SWICH_TOLG_HTTP);
                  HAL_RTCEx_BKUPWrite(&hrtc,1,2);
                  sprintf(buf_list,"%s%s%s relay_result('%s')\;",PAGE_HEADER_200_OK,PAGE_HEADER_SERVER,PAGE_HEADER_CONTENT_TEXT,"ok");
                  len_buf_list = strlen(buf_list);
                  netconn_write(conn, (char*)(buf_list), (size_t)len_buf_list, NETCONN_COPY);      
                  vTaskDelay(delay_send);                 
                }
               else
                {
                  form_reple_to_save(SWICH_TOLG_HTTP_N);
                  sprintf(buf_list,"%s%s%s relay_result('%s')\;",PAGE_HEADER_200_OK,PAGE_HEADER_SERVER,PAGE_HEADER_CONTENT_TEXT,"uneffecrtive");
                  len_buf_list = strlen(buf_list);
                  netconn_write(conn, (char*)(buf_list), (size_t)len_buf_list, NETCONN_COPY);      
                  vTaskDelay(delay_send);   
                }
               
            }
          break;
          
            default  :
              {
              fs_open(&file, "/404.html"); 
              netconn_write(conn, (const unsigned char*)(file.data), (size_t)file.len, NETCONN_COPY);
               vTaskDelay(delay_send);
              fs_close(&file);
             
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
             // fs_open(&file, "/404.html"); 
              //memcpy(tempm,(const unsigned char*)(file.data),256);
              //156 - end
              //107 - length  
              netconn_write(conn, (const unsigned char*)(file.data), (size_t)file.len, NETCONN_COPY);
               vTaskDelay(delay_send);
              fs_close(&file);
             
            }
          break;
          case 2:
            {
              fs_open(&file, "/pass.html"); 
              netconn_write(conn, (const unsigned char*)(file.data), (size_t)file.len, NETCONN_COPY);
              vTaskDelay(delay_send);
              fs_close(&file);
              
            }
          break;
          case 3:
            { //watchdog.html
               vTaskDelay(delay_send);
               len_buf_list=costr_watchdog1((char*)buf_list);
               netconn_write(conn, (char*)(buf_list), (size_t)len_buf_list, NETCONN_COPY);               
               vTaskDelay(delay_send);
               len_buf_list=costr_watchdog2((char*)buf_list);
               netconn_write(conn, (char*)(buf_list), (size_t)len_buf_list, NETCONN_COPY);               
               vTaskDelay(delay_send);
               len_buf_list=costr_watchdog3((char*)buf_list);
               netconn_write(conn, (char*)(buf_list), (size_t)len_buf_list, NETCONN_COPY);               
               vTaskDelay(delay_send);
                len_buf_list=costr_watchdog4((char*)buf_list);
               netconn_write(conn, (char*)(buf_list), (size_t)len_buf_list, NETCONN_COPY);    
               vTaskDelay(delay_send);
                len_buf_list=costr_watchdog5((char*)buf_list);
               netconn_write(conn, (char*)(buf_list), (size_t)len_buf_list, NETCONN_COPY);       
               vTaskDelay(delay_send);
                
            }
          break;
           case 4: //index 
            {
               vTaskDelay(delay_send);
               len_buf_list=costr_page2_hdr((char*)buf_list);
               netconn_write(conn, (char*)(buf_list), (size_t)len_buf_list, NETCONN_COPY);               
             vTaskDelay(delay_send);
                    
               vTaskDelay(delay_send);
               len_buf_list=costr_page2((char*)buf_list);
               netconn_write(conn, (char*)(buf_list), (size_t)len_buf_list, NETCONN_COPY);
              vTaskDelay(delay_send);
                  len_buf_list=costr_page2_1((char*)buf_list);
               netconn_write(conn, (char*)(buf_list), (size_t)len_buf_list, NETCONN_COPY);
              vTaskDelay(delay_send);
                 
               len_buf_list=costr_page4((char*)buf_list);
               netconn_write(conn, (char*)(buf_list), (size_t)len_buf_list, NETCONN_COPY);
               vTaskDelay(delay_send);
               
                len_buf_list=costr_page4_1((char*)buf_list);
               netconn_write(conn, (char*)(buf_list), (size_t)len_buf_list, NETCONN_COPY);
                     
               vTaskDelay(delay_send);
               
              
               
            }
          break;
           case 5: //settings
            {
////               fs_open(&file, "/img/netping.gif");
////              netconn_write(conn, (const unsigned char*)(file.data), (size_t)file.len, NETCONN_COPY);
////              fs_close(&file);
                
               
             
               vTaskDelay(delay_send);
               len_buf_list=costr_page3((char*)buf_list);
               netconn_write(conn, (char*)(buf_list), (size_t)len_buf_list, NETCONN_COPY);               
             vTaskDelay(delay_send);
               
               len_buf_list=costr_page5((char*)buf_list);
               netconn_write(conn, (char*)(buf_list), (size_t)len_buf_list, NETCONN_COPY);
             vTaskDelay(delay_send);
               
               len_buf_list=costr_page6((char*)buf_list);
               netconn_write(conn, (char*)(buf_list), (size_t)len_buf_list, NETCONN_COPY);               
             vTaskDelay(delay_send);
               len_buf_list=costr_page6_1((char*)buf_list);
               netconn_write(conn, (char*)(buf_list), (size_t)len_buf_list, NETCONN_COPY);               
             vTaskDelay(delay_send);
               
               len_buf_list=costr_page7((char*)buf_list);
               netconn_write(conn, (char*)(buf_list), (size_t)len_buf_list, NETCONN_COPY);
               
               
              vTaskDelay(delay_send);
               len_buf_list=costr_page7_1((char*)buf_list);
               netconn_write(conn, (char*)(buf_list), (size_t)len_buf_list, NETCONN_COPY);
               vTaskDelay(delay_send);
//               len_buf_list=costr_page4((char*)buf_list);
//               netconn_write(conn, (char*)(buf_list), (size_t)len_buf_list, NETCONN_COPY);
            }
          break;
           case 6:  //logs
            {
                uint8_t end_mess=0;    
               vTaskDelay(delay_send);
                len_buf_list=costr_page8((char*)buf_list);
                netconn_write(conn, (char*)(buf_list), (size_t)len_buf_list, NETCONN_COPY);               
                vTaskDelay(delay_send);
                uint16_t ct_mess;
                 for(ct_mess=0;ct_mess<2048;ct_mess++)
                  {
                     if (FW_data.V_logs_struct.log_reple[ct_mess].dicr!=0x7a)
                      {
                         end_mess=ct_mess-1;
                         break;
                      }
                  }
                
                for(ct_mess=end_mess;ct_mess!=0;ct_mess--)
                    {
                    //  if (FW_data.V_logs_struct.log_reple[ct_mess].dicr==0x7a)
                      //  {
                            memset(buf_list,0,sizeof(buf_list));
                            decode_reple(buf_list,&FW_data.V_logs_struct.log_reple[ct_mess]);
                            netconn_write(conn, (char*)(buf_list), (size_t)strlen(buf_list), NETCONN_COPY);       
                            vTaskDelay(delay_send);
                    //    }
                     // else
                     //  {
                    //    break;
                       //}
     
                    }
                len_buf_list=costr_page9((char*)buf_list);
                netconn_write(conn, (char*)(buf_list), (size_t)len_buf_list, NETCONN_COPY);               
              vTaskDelay(delay_send);
            }
          break;
           case 7:  //rasp.html
            {
            vTaskDelay(delay_send);
               len_buf_list=costr_rasp_page1((char*)buf_list);
               netconn_write(conn, (char*)(buf_list), (size_t)len_buf_list, NETCONN_COPY);      
              vTaskDelay(delay_send);
               len_buf_list=costr_rasp_page2((char*)buf_list);
               netconn_write(conn, (char*)(buf_list), (size_t)len_buf_list, NETCONN_COPY);   
              vTaskDelay(delay_send);
               len_buf_list=costr_rasp_page3((char*)buf_list);
               netconn_write(conn, (char*)(buf_list), (size_t)len_buf_list, NETCONN_COPY);   
             vTaskDelay(delay_send);
                len_buf_list=costr_rasp_page4((char*)buf_list);
               netconn_write(conn, (char*)(buf_list), (size_t)len_buf_list, NETCONN_COPY);   
               vTaskDelay(delay_send);
                 len_buf_list=costr_rasp_page5((char*)buf_list);
               netconn_write(conn, (char*)(buf_list), (size_t)len_buf_list, NETCONN_COPY);   
               vTaskDelay(delay_send);
                 len_buf_list=costr_rasp_page6((char*)buf_list);
               netconn_write(conn, (char*)(buf_list), (size_t)len_buf_list, NETCONN_COPY);   
               vTaskDelay(delay_send);
                 len_buf_list=costr_rasp_page7((char*)buf_list);
               netconn_write(conn, (char*)(buf_list), (size_t)len_buf_list, NETCONN_COPY);   
               vTaskDelay(delay_send);
                 len_buf_list=costr_rasp_page8((char*)buf_list);
               netconn_write(conn, (char*)(buf_list), (size_t)len_buf_list, NETCONN_COPY);   
               vTaskDelay(delay_send);
                 len_buf_list=costr_rasp_page9((char*)buf_list);
               netconn_write(conn, (char*)(buf_list), (size_t)len_buf_list, NETCONN_COPY);   
             vTaskDelay(delay_send);
              
                len_buf_list=costr_rasp_page10((char*)buf_list);
               netconn_write(conn, (char*)(buf_list), (size_t)len_buf_list, NETCONN_COPY);   
               vTaskDelay(delay_send);
                len_buf_list=costr_rasp_page11((char*)buf_list);
               netconn_write(conn, (char*)(buf_list), (size_t)len_buf_list, NETCONN_COPY);   
               vTaskDelay(delay_send);
                len_buf_list=costr_rasp_page12((char*)buf_list);
               netconn_write(conn, (char*)(buf_list), (size_t)len_buf_list, NETCONN_COPY);   
              vTaskDelay(delay_send);
                len_buf_list=costr_rasp_page13((char*)buf_list);
               netconn_write(conn, (char*)(buf_list), (size_t)len_buf_list, NETCONN_COPY);   
               vTaskDelay(delay_send);
               
                len_buf_list=costr_rasp_page14((char*)buf_list);
               netconn_write(conn, (char*)(buf_list), (size_t)len_buf_list, NETCONN_COPY);   
               vTaskDelay(delay_send);
                len_buf_list=costr_rasp_page15((char*)buf_list);
               netconn_write(conn, (char*)(buf_list), (size_t)len_buf_list, NETCONN_COPY);   
              vTaskDelay(delay_send);
               len_buf_list=costr_rasp_page16((char*)buf_list);
               netconn_write(conn, (char*)(buf_list), (size_t)len_buf_list, NETCONN_COPY);   
               vTaskDelay(delay_send);
               len_buf_list=costr_rasp_page17((char*)buf_list);
               netconn_write(conn, (char*)(buf_list), (size_t)len_buf_list, NETCONN_COPY);   
               vTaskDelay(delay_send);
               len_buf_list=costr_rasp_page18((char*)buf_list);
               netconn_write(conn, (char*)(buf_list), (size_t)len_buf_list, NETCONN_COPY);   
               vTaskDelay(delay_send);
               len_buf_list=costr_rasp_page19((char*)buf_list);
               netconn_write(conn, (char*)(buf_list), (size_t)len_buf_list, NETCONN_COPY);   
              vTaskDelay(delay_send);
               len_buf_list=costr_rasp_page20((char*)buf_list);
               netconn_write(conn, (char*)(buf_list), (size_t)len_buf_list, NETCONN_COPY);   
              vTaskDelay(delay_send);
               len_buf_list=costr_rasp_page21((char*)buf_list);
               netconn_write(conn, (char*)(buf_list), (size_t)len_buf_list, NETCONN_COPY);   
               vTaskDelay(delay_send);
               len_buf_list=costr_rasp_page22((char*)buf_list);
               netconn_write(conn, (char*)(buf_list), (size_t)len_buf_list, NETCONN_COPY);   
              vTaskDelay(delay_send);
               len_buf_list=costr_rasp_page23((char*)buf_list);
               netconn_write(conn, (char*)(buf_list), (size_t)len_buf_list, NETCONN_COPY);   
               vTaskDelay(delay_send);
               len_buf_list=costr_rasp_page24((char*)buf_list);
               netconn_write(conn, (char*)(buf_list), (size_t)len_buf_list, NETCONN_COPY);   
               vTaskDelay(delay_send);
               len_buf_list=costr_rasp_page25((char*)buf_list);
               netconn_write(conn, (char*)(buf_list), (size_t)len_buf_list, NETCONN_COPY);   
               vTaskDelay(delay_send);
               len_buf_list=costr_rasp_page26((char*)buf_list);
               netconn_write(conn, (char*)(buf_list), (size_t)len_buf_list, NETCONN_COPY);   
               vTaskDelay(delay_send);
               len_buf_list=costr_rasp_page27((char*)buf_list);
               netconn_write(conn, (char*)(buf_list), (size_t)len_buf_list, NETCONN_COPY);   
               vTaskDelay(delay_send);
               len_buf_list=costr_rasp_page28((char*)buf_list);
               netconn_write(conn, (char*)(buf_list), (size_t)len_buf_list, NETCONN_COPY);   
               vTaskDelay(delay_send);
                len_buf_list=costr_rasp_page29((char*)buf_list);
               netconn_write(conn, (char*)(buf_list), (size_t)len_buf_list, NETCONN_COPY);   
               vTaskDelay(delay_send);
               len_buf_list=costr_rasp_page30((char*)buf_list);
               netconn_write(conn, (char*)(buf_list), (size_t)len_buf_list, NETCONN_COPY);   
               vTaskDelay(delay_send);
               len_buf_list=costr_rasp_page31((char*)buf_list);
               netconn_write(conn, (char*)(buf_list), (size_t)len_buf_list, NETCONN_COPY);   
               vTaskDelay(delay_send);
               len_buf_list=costr_rasp_page32((char*)buf_list);
               netconn_write(conn, (char*)(buf_list), (size_t)len_buf_list, NETCONN_COPY);   
              vTaskDelay(delay_send);
                len_buf_list=costr_rasp_page33((char*)buf_list);
               netconn_write(conn, (char*)(buf_list), (size_t)len_buf_list, NETCONN_COPY);   
//               len_buf_list=strlen(http_html_200);               
//               netconn_write(conn, (char*)(http_html_200), (size_t)len_buf_list, NETCONN_COPY);     
              vTaskDelay(delay_send);
               
            }
          break;
           case 8:
            {
               vTaskDelay(delay_send);
               len_buf_list=costr_email_page1((char*)buf_list);
               netconn_write(conn, (char*)(buf_list), (size_t)len_buf_list, NETCONN_COPY);      
               vTaskDelay(delay_send);
               len_buf_list=costr_email_page2((char*)buf_list);
               netconn_write(conn, (char*)(buf_list), (size_t)len_buf_list, NETCONN_COPY);   
               vTaskDelay(delay_send);
               len_buf_list=costr_email_page3((char*)buf_list);
               netconn_write(conn, (char*)(buf_list), (size_t)len_buf_list, NETCONN_COPY);   
               vTaskDelay(delay_send);
            }
          break;                         
             case 9:
            {
               vTaskDelay(delay_send);
               len_buf_list=costr_page_boot((char*)buf_list);
               netconn_write(conn, (char*)(buf_list), (size_t)len_buf_list, NETCONN_COPY);      
               vTaskDelay(delay_send);             
            }
          break; 
             case 10:
            {  
                

              if ((time_run[0]!=0)||(time_run[1]!=0)||(time_run[2]!=0)||(time_run[3]!=0)||(time_run[4]!=0)||(time_run[5]!=0))
              {
               sprintf(buf_list,"%s%s%s    %dг.  %dм. %dд. %dч. %dм. %dс.    ",PAGE_HEADER_200_OK,PAGE_HEADER_SERVER,PAGE_HEADER_CONTENT_TEXT,time_run[0],time_run[1],time_run[2],time_run[3],time_run[4],time_run[5]);
              }
              else
              {

                  sprintf(buf_list,"Время не установлено!",PAGE_HEADER_200_OK,PAGE_HEADER_SERVER,PAGE_HEADER_CONTENT_TEXT);
               }
            
              len_buf_list = strlen(buf_list);
              netconn_write(conn, (char*)(buf_list), (size_t)len_buf_list, NETCONN_COPY);      
              vTaskDelay(delay_send);             
                     
            }
          break; 
          case 11:
            {  
              uint8_t data;
             data= flag_runtime_out[1];
             if (data==0)
              {
               sprintf(buf_list,"%s%s%s%s",PAGE_HEADER_200_OK,PAGE_HEADER_SERVER,PAGE_HEADER_CONTENT_TEXT,FW_data.V_OFF_MESS);
              }
             if ((data==1))
              {
               sprintf(buf_list,"%s%s%s%s",PAGE_HEADER_200_OK,PAGE_HEADER_SERVER,PAGE_HEADER_CONTENT_TEXT,FW_data.V_ON_MESS);
              }              
              len_buf_list = strlen(buf_list);
              netconn_write(conn, (char*)(buf_list), (size_t)len_buf_list, NETCONN_COPY);      
              vTaskDelay(delay_send);             
            }
          break;
            case 12:
            {  
           
               

               sprintf(buf_list,"%s%s",PAGE_HEADER_303_OK,PAGE_HEADER_CONTENT_TEXT);
               len_buf_list = strlen(buf_list);
               netconn_write(conn, (char*)(buf_list), (size_t)len_buf_list, NETCONN_COPY);                     
               vTaskDelay(delay_send);
            }
          break;
          case 13:
            {  
           
               
               //sprintf(buf_list,"%s%s%s",PAGE_HEADER_303_OK,PAGE_HEADER_SERVER,PAGE_HEADER_CONTENT_TEXT);
               sprintf(buf_list,"%s%s",PAGE_HEADER_303_settings,PAGE_HEADER_CONTENT_TEXT);
               len_buf_list = strlen(buf_list);
               netconn_write(conn, (char*)(buf_list), (size_t)len_buf_list, NETCONN_COPY);                     
               vTaskDelay(delay_send);
            }
          break;
          case 14:
            {  
           
               
               //sprintf(buf_list,"%s%s%s",PAGE_HEADER_303_OK,PAGE_HEADER_SERVER,PAGE_HEADER_CONTENT_TEXT);
               sprintf(buf_list,"%s%s",PAGE_HEADER_303_rasp,PAGE_HEADER_CONTENT_TEXT);
               len_buf_list = strlen(buf_list);
               netconn_write(conn, (char*)(buf_list), (size_t)len_buf_list, NETCONN_COPY);                     
               vTaskDelay(delay_send);
            }
          break;
          case 15:
            {  
           
               
               //sprintf(buf_list,"%s%s%s",PAGE_HEADER_303_OK,PAGE_HEADER_SERVER,PAGE_HEADER_CONTENT_TEXT);
               sprintf(buf_list,"%s%s",PAGE_HEADER_303_watchdog,PAGE_HEADER_CONTENT_TEXT);
               len_buf_list = strlen(buf_list);
               netconn_write(conn, (char*)(buf_list), (size_t)len_buf_list, NETCONN_COPY);                     
               vTaskDelay(delay_send);
            }
          break;
          case 16:
            {  
           
               
               //sprintf(buf_list,"%s%s%s",PAGE_HEADER_303_OK,PAGE_HEADER_SERVER,PAGE_HEADER_CONTENT_TEXT);
               sprintf(buf_list,"%s%s",PAGE_HEADER_303_email,PAGE_HEADER_CONTENT_TEXT);
               len_buf_list = strlen(buf_list);
               netconn_write(conn, (char*)(buf_list), (size_t)len_buf_list, NETCONN_COPY);                     
               vTaskDelay(delay_send);
            }
          break;
          default  :
              {
              fs_open(&file, "/404.html"); 
              netconn_write(conn, (const unsigned char*)(file.data), (size_t)file.len, NETCONN_COPY);
               vTaskDelay(delay_send);
              fs_close(&file);
             
              }
          }
 }





//char buf_page[3000];
static void http_server_serve(struct netconn *conn1) 
{
   
 //post_data_t post_data[32];
 // struct netbuf *inbuf;//=(struct netbuf*)pvPortMalloc(1500);
  err_t recv_err;
 
   u16_t buflen;

  
       
        char* buf;
        struct netbuf *inbuf;//=( struct netbuf *)(buf_page+1500);//=(struct netbuf*)pvPortMalloc(1500);


  
 
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
     char* buf_page=(char*)pvPortMalloc(3000); 
      memset (buf_page,0,3000); 
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
      
          if (strncmp((char const *)buf,"GET /relay.cgi?r",16)==0)
            {
              HTTP_API_swich(conn1,buf);
            }
              else if (strncmp((char const *)buf,"GET /content.html",17)==0)
            {
              page_n=10;
              page_html_swich(page_n,conn1,buf_page);
            }
              else if (strncmp((char const *)buf,"GET /content1.html",18)==0)
            {
              page_n=11;
              page_html_swich(page_n,conn1,buf_page);
            }
             
             else
            {
      
#if (logon==0)
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
#else
 flag_logon=1;        
#endif      
      
  if (flag_logon==1)    
  {
    
      if ((buflen >=5) && (strncmp(buf, "POST /", 5) == 0))
              {
                if (strncmp(buf, "POST /", 5) == 0)
                    {      
                      if (page_sost==1)
                           {
                            page_n=12;
                            page_html_swich(page_n,conn1,buf_page);
                            vTaskDelay(20);
                            parser_post(buf,buflen,page_sost);    
                            
                           }         
                      if (page_sost==3)
                           {
                             page_n=13;
                             page_html_swich(page_n,conn1,buf_page);
                             vTaskDelay(20);
                             parser_post(buf,buflen,page_sost);
                             
                           }
                      if(page_sost==2)
                           {
                             page_n=15;
                             page_html_swich(page_n,conn1,buf_page);
                             vTaskDelay(20);
                             parser_post(buf,buflen,page_sost);
                             
                            
                           }
                      if(page_sost==4)
                           {
                            page_n=14;
                            page_html_swich(page_n,conn1,buf_page);
                            vTaskDelay(20);
                            parser_post(buf,buflen,page_sost);
                            
                            
                           }
                      if(page_sost==5)
                           {
                            page_n=16;
                            page_html_swich(page_n,conn1,buf_page);
                            vTaskDelay(20);                            
                            parser_post(buf,buflen,page_sost);
                          }
                          if (flag_global_reset_mode==1)//&&(flag_global_save_log==0))
     {
       
        
       
       
       //vTaskDelete(LED_taskHandle);
       //jamp_to_app();     
     }
                                        
                    }
              }
      

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
               memset (buf_page,0,3000); 
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
             else if (strncmp((char const *)buf,"GET /rasp.html",14)==0)
             {
               page_n=7;
               page_sost=4;
               page_html_swich(page_n,conn1,buf_page);
             }      
              else if (strncmp((char const *)buf,"GET /email.html",15)==0)
             {
               page_n=8;
               page_sost=5;
               page_html_swich(page_n,conn1,buf_page);
             }     
            else if (strncmp((char const *)buf,"GET /settings.html",14)==0)
              {
               page_n=5;
               page_sost=3;
               page_html_swich(page_n,conn1,buf_page);
              }
            else if ((strncmp((char const *)buf,"GET /img/netping.gif",17)==0)||(strncmp((char const *)buf,"GET /favicon.ico",16)==0))
            {
              page_n=1;
              page_html_swich(page_n,conn1,buf_page);
            }
//            else if (strncmp((char const *)buf,"GET /content.html",17)==0)
//            {
//              page_n=10;
//              page_html_swich(page_n,conn1,buf_page);
//            }
//              else if (strncmp((char const *)buf,"GET /content1.html",18)==0)
//            {
//              page_n=11;
//              page_html_swich(page_n,conn1,buf_page);
//            }
//             else if (strncmp((char const *)buf,"GET /?output_set=1&out_swich=1",30)==0)
//            {
//              page_n=4;
//              page_html_swich(page_n,conn1,buf_page);
//            }
            else  
             {
              /* Load Error page */
               page_n=3;
               page_html_swich(page_n,conn1,buf_page);
             }
          //}        
      } 
     

          
        
      
     }
      }
     vTaskDelay(4*delay_send);
    
     vPortFree(buf_page);
   
    }
     
  }
//vTaskDelay(100);
 
   vTaskDelay(4*delay_send);
    
   netbuf_delete(inbuf);
 //   netbuf_delete(buf_page);
  /* Close the connection (server closes in HTTP) */
   vTaskDelay(4*delay_send); 
  netconn_close(conn1);
  
  /* Delete the buffer (netconn_recv gives us ownership,
   so we have to make sure to deallocate the buffer) */
 // vPortFree(inbuf);


          
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
       //   vTaskDelay(50);
          http_server_serve(newconn);
       //   vTaskDelay(50);
           /* delete connection */
          netconn_delete(newconn);          
        }
////        else
////          {
////            while(1){}
////          }

            
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
  sys_thread_new("HTTP", http_server_netconn_thread, NULL,512, osPriorityHigh);
}

/**
  * @brief  Create and send a dynamic Web Page. This page contains the list of 
  *         running tasks and the number of page hits. 
  * @param  conn pointer on connection structure 
  * @retval None
  */
void DynWebPageStr(struct netconn *conn)
{
 
}
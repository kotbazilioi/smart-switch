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
/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
#define WEBSERVER_THREAD_PRIO    ( osPriorityAboveNormal )

void DynWebPageStr(struct netconn *conn);
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
u32_t nPageHits = 0;
char http_ok[] = {0x48,0x54,0x54,0x50,0x2f,0x31,0x2e,0x31,0x20,0x32,0x30,0x30,0x20,0x4f,0x4b,0x0d,0x0a};

  uint8_t page_n,page_sost;
/**
  * @brief serve tcp connection  
  * @param conn: pointer on connection structure 
  * @retval None
  */
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
        uint8_t ct_temp0;
  uint8_t len_mess=0;
  if (index==3)
  {
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
    else if (strncmp((char*)post_data->name,"ip_addr", sizeof("ip_addr")) == 0)
          {
           // FW_data.V_Name_dev
            len_mess=strlen(post_data->data);
            if (scanf_ip ((char*) post_data->data,IP_buf,len_mess)==0)
            {
              memcpy((uint8_t*)FW_data.V_IP_CONFIG, (char*)IP_buf,4 );
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
    else if (strncmp((char*)post_data->name,"time_set", sizeof("time_set")) == 0)
          {
           // FW_data.V_Name_dev
            len_mess=strlen(post_data->data);
          //  memcpy((char*)FW_data.V_PASSWORD, (char*)post_data->data,len_mess );
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
                flag_global_save_data=1;
              }
          }
    else if (strncmp((char*)post_data->name,"load_def", sizeof("load_def")) == 0)
          {
           if (post_data->data[0]==0x31)
              {
                flag_global_load_def=1;
              }
          }
    else if (strncmp((char*)post_data->name,"load_boot", sizeof("load_boot")) == 0)
          {
           jamp_to_boot();
          }
    else
     
    {
    
    }
    
  }
  
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
            {
              fs_open(&file, "/404.html"); 
              netconn_write(conn, (const unsigned char*)(file.data), (size_t)file.len, NETCONN_NOCOPY);
              fs_close(&file);
            }
          break;
           case 4:
            {
               len_buf_list=costr_page2((char*)buf_list);
               netconn_write(conn, (char*)(buf_list), (size_t)len_buf_list, NETCONN_NOCOPY);
                 vTaskDelay(10);
               len_buf_list=costr_page4((char*)buf_list);
               netconn_write(conn, (char*)(buf_list), (size_t)len_buf_list, NETCONN_NOCOPY);
            }
          break;
           case 5: //settings
            {
////               fs_open(&file, "/img/netping.gif");
////              netconn_write(conn, (const unsigned char*)(file.data), (size_t)file.len, NETCONN_NOCOPY);
////              fs_close(&file);
               len_buf_list=costr_page3((char*)buf_list);
               netconn_write(conn, (char*)(buf_list), (size_t)len_buf_list, NETCONN_NOCOPY);               
               vTaskDelay(10);
               
               len_buf_list=costr_page5((char*)buf_list);
               netconn_write(conn, (char*)(buf_list), (size_t)len_buf_list, NETCONN_NOCOPY);
                vTaskDelay(10);
               
               len_buf_list=costr_page6((char*)buf_list);
               netconn_write(conn, (char*)(buf_list), (size_t)len_buf_list, NETCONN_NOCOPY);               
                vTaskDelay(10);
               
               len_buf_list=costr_page7((char*)buf_list);
               netconn_write(conn, (char*)(buf_list), (size_t)len_buf_list, NETCONN_NOCOPY);
//               len_buf_list=costr_page4((char*)buf_list);
//               netconn_write(conn, (char*)(buf_list), (size_t)len_buf_list, NETCONN_NOCOPY);
            }
          break;
           case 6:  //logs
            {
               fs_open(&file, "/img/netping.gif");
              netconn_write(conn, (const unsigned char*)(file.data), (size_t)file.len, NETCONN_NOCOPY);
              fs_close(&file);
//               len_buf_list=costr_page2((char*)buf_list);
//               netconn_write(conn, (char*)(buf_list), (size_t)len_buf_list, NETCONN_NOCOPY);
//          
//               len_buf_list=costr_page4((char*)buf_list);
//               netconn_write(conn, (char*)(buf_list), (size_t)len_buf_list, NETCONN_NOCOPY);
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

static void http_server_serve(struct netconn *conn) 
{
   
 //post_data_t post_data[32];
  struct netbuf *inbuf;
  err_t recv_err;
  char* buf;
   u16_t buflen;
//f=(char*)malloc(3000);
 char buf_page[5000];
 
  uint16_t len_buf_list;
  uint16_t ct_fpass=0;
  uint16_t start_key=0;

  
  /* Read the data from the port, blocking if nothing yet there. 
   We assume the request (the part we care about) is in one netbuf */
  recv_err = netconn_recv(conn, &inbuf);
  
  if (recv_err == ERR_OK)
  {
    if (netconn_err(conn) == ERR_OK) 
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
      
       if ((flag_logon==0)&&(strncmp(key_http,buf_page,key_http_len) == 0))
            {
              flag_logon=1;
              page_n=4;
            }
          if (strncmp(key_http,buf_page,key_http_len) != 0)
          {          
            len_buf_list=costr_pass((char*)buf_page);
            netconn_write(conn, (const unsigned char*)(buf_page), (size_t)len_buf_list, NETCONN_NOCOPY);
            flag_logon=0;
            // memcpy(key_http,"admin:admin",12); 
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
                 page_html_swich(page_n,conn,buf_page);
              }
            else
              if ((strncmp((char const *)buf,"GET /index.html",15)==0)||(strncmp((char const *)buf,"GET / HTTP/1.1",14)==0))
             {
               page_n=4;
               page_sost=1;
               page_html_swich(page_n,conn,buf_page);
             }
            else if (strncmp((char const *)buf,"GET /logs.html",14)==0)
             {
               page_n=6;
               page_sost=2;
               page_html_swich(page_n,conn,buf_page);
             }
            else if (strncmp((char const *)buf,"GET /settings.html",14)==0)
              {
               page_n=5;
               page_sost=3;
               page_html_swich(page_n,conn,buf_page);
              }
            else if (strncmp((char const *)buf,"GET /img/netping.gif",17)==0)
            {
              page_n=1;
              page_html_swich(page_n,conn,buf_page);
            }
            else  
             {
              /* Load Error page */
               page_n=3;
               page_html_swich(page_n,conn,buf_page);
             }
          //}        
      } 
      else
      {
//       if (flag_logon==1)
//          {
            if ((buflen >=5) && (strncmp(buf, "POST /", 5) == 0))
              {
                if (strncmp(buf, "POST /", 5) == 0)
                    {                      
                      if (strncmp((char*)(buf+buflen-11),"check_res=1", 11)==0)
                        {                        
                       if (page_sost==1)
                         {
                         page_n=4; // pass
                         flag_logon=0;
                         page_html_swich(page_n,conn,buf_page);   
                         vTaskDelay(10);
                         jamp_to_app();
                         }
                        }         
                      else 
                        {  
                         if (page_sost==3)
                         {
                            parser_post(buf,buflen,page_sost);
                            page_n=5;
                            vTaskDelay(20);
                            page_html_swich(page_n,conn,buf_page);
                         }
                         if(page_sost==2)
                          {
                            
                          }
                        }                
 
                    
                    }
              }
        //  }
      }
     }
    }
  }
  // free(buf);
  /* Close the connection (server closes in HTTP) */
  netconn_close(conn);
  
  /* Delete the buffer (netconn_recv gives us ownership,
   so we have to make sure to deallocate the buffer) */
  
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
  
  /* Create a new TCP connection handle */
  conn = netconn_new(NETCONN_TCP);
  
  if (conn!= NULL)
  {
    /* Bind to port 80 (HTTP) with default IP address */
    err = netconn_bind(conn, NULL, 80);
    
    if (err == ERR_OK)
    {
      /* Put the connection into LISTEN state */
      netconn_listen(conn);
  
      while(1) 
      {
        /* accept any icoming connection */
        accept_err = netconn_accept(conn, &newconn);
        if(accept_err == ERR_OK)
        {
          /* serve connection */
          http_server_serve(newconn);

          /* delete connection */
          netconn_delete(newconn);
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
  sys_thread_new("HTTP", http_server_netconn_thread, NULL, 2*1024, WEBSERVER_THREAD_PRIO);
}

/**
  * @brief  Create and send a dynamic Web Page. This page contains the list of 
  *         running tasks and the number of page hits. 
  * @param  conn pointer on connection structure 
  * @retval None
  */
////void DynWebPage(struct netconn *conn)
////{
////  portCHAR PAGE_BODY[512];
////  portCHAR pagehits[10] = {0};
////
////  memset(PAGE_BODY, 0,512);
////
////  /* Update the hit count */
////  nPageHits++;
////  sprintf(pagehits, "%d", (int)nPageHits);
////  strcat(PAGE_BODY, pagehits);
////  strcat((char *)PAGE_BODY, "<pre><br>Name          State  Priority  Stack   Num" );
////  strcat((char *)PAGE_BODY, "<br>---------------------------------------------<br>");
////    
////  /* The list of tasks and their status */
////  osThreadList((unsigned char *)(PAGE_BODY + strlen(PAGE_BODY)));
////  strcat((char *)PAGE_BODY, "<br><br>---------------------------------------------");
////  strcat((char *)PAGE_BODY, "<br>B : Blocked, R : Ready, D : Deleted, S : Suspended<br>");
////
////  /* Send the dynamically generated page */
////  netconn_write(conn, PAGE_START, strlen((char*)PAGE_START), NETCONN_COPY);
////  netconn_write(conn, PAGE_BODY, strlen(PAGE_BODY), NETCONN_COPY);
////}
////
////void DynWebPageStr(struct netconn *conn)
////{
////  portCHAR PAGE_BODY[768];
////  uint16_t len = 0;
////  RTC_DateTypeDef sdatestructureget;
////  RTC_TimeTypeDef stimestructureget;
////  
////  /* Get the RTC current Time */
////  HAL_RTC_GetTime(&hrtc, &stimestructureget, RTC_FORMAT_BIN);
////  /* Get the RTC current Date */
////  HAL_RTC_GetDate(&hrtc, &sdatestructureget, RTC_FORMAT_BIN);
////  /* Display time Format : hh:mm:ss */
//// // sprintf((char*)showtime,"%02d:%02d:%02d",stimestructureget.Hours, stimestructureget.Minutes, stimestructureget.Seconds);
////  
////  
////  sprintf(PAGE_BODY,"%s%s%s    %d : %d : %d",PAGE_HEADER_200_OK,PAGE_HEADER_SERVER,PAGE_HEADER_CONTENT_TEXT,stimestructureget.Hours,stimestructureget.Minutes,stimestructureget.Seconds);
////  len = strlen(PAGE_BODY);
////  
//////osThreadList((uint8_t *)(PAGE_BODY + len));        
////netconn_write(conn, PAGE_BODY, strlen((char*)PAGE_BODY), NETCONN_COPY);
////}

//////////
//////////static void http_server_serve(struct netconn *conn) 
//////////{
//////////  struct netbuf *inbuf;
//////////  err_t recv_err;
//////////  char* buf;
//////////  u16_t buflen;
//////////  struct fs_file file;
//////////  uint8_t buf_list[1300];
//////////  
//////////  
//////////  /* Read the data from the port, blocking if nothing yet there. 
//////////   We assume the request (the part we care about) is in one netbuf */
//////////  recv_err = netconn_recv(conn, &inbuf);
//////////  
//////////  if (recv_err == ERR_OK)
//////////  {
//////////    if (netconn_err(conn) == ERR_OK) 
//////////    {
//////////      netbuf_data(inbuf, (void**)&buf, &buflen);
//////////    
//////////      /* Is this an HTTP GET command? (only check the first 5 chars, since
//////////      there are other formats for GET, and we're keeping it very simple )*/
//////////      if ((buflen >=5) && (strncmp(buf, "GET /", 5) == 0))
//////////      {
//////////        /* Check if request to get ST.gif */ 
//////////        if (strncmp((char const *)buf,"GET / HTTP/1.1",14)==0)
//////////        {
//////////          fs_open(&file, "/pass.html"); 
//////////          netconn_write(conn, (const unsigned char*)(file.data), (size_t)file.len, NETCONN_NOCOPY);
//////////////          memcpy (buf_list,(uint8_t*)(file.data),file.len);
//////////////          set_addres_list((uint8_t *)buf_list,file.len);
//////////////          netconn_write(conn, (const unsigned char*)(buf_list), (size_t)file.len, NETCONN_NOCOPY);
//////////          fs_close(&file);
//////////        }   
//////////        /* Check if request to get stm32.jpeg */
//////////////        else if (strncmp((char const *)buf,"GET /style.css",14)==0)
//////////////            {
//////////////              fs_open(&file, "/style.css");
//////////////              netconn_write(conn, (const unsigned char*)(file.data), (size_t)file.len, NETCONN_NOCOPY);
//////////////              fs_close(&file);
//////////////            }
//////////        else if (strncmp((char const *)buf,"GET /?ip_addres",15)==0)//
//////////          {
//////////            //out//449 22   
//////////             // 594 
//////////             //731
//////////            
//////////            //in// 16 40 66
//////////            memcpy (buf_list,(uint8_t*)buf,buflen);
//////////            get_addres_list((uint8_t *)buf,buflen);
//////////              fs_open(&file, "/index.html"); 
//////////             memcpy (buf_list,(uint8_t*)(file.data),file.len);
//////////              set_addres_list((uint8_t *)buf_list,file.len);
//////////              netconn_write(conn, (const unsigned char*)(buf_list), (size_t)file.len, NETCONN_NOCOPY);
//////////              // netconn_write(conn, (const unsigned char*)(file.data), (size_t)file.len, NETCONN_NOCOPY);
//////////              fs_close(&file);
//////////////          
//////////            }
//////////       else if (strncmp((char const *)buf,"GET /?Start_boot=boot",21)==0)
//////////            {
//////////              
//////////              fs_open(&file, "/index_end.html"); 
//////////              netconn_write(conn, (const unsigned char*)(file.data), (size_t)file.len, NETCONN_NOCOPY);
//////////              fs_close(&file);
//////////              vTaskDelay(100);
//////////              jamp_to_boot();
//////////            }
//////////      else if (strncmp((char const *)buf,"GET /img/netping.gif",17)==0)
//////////            {
//////////              fs_open(&file, "/img/netping.gif");
//////////              netconn_write(conn, (const unsigned char*)(file.data), (size_t)file.len, NETCONN_NOCOPY);
//////////              fs_close(&file);
//////////            }
//////////////      else if (strncmp((char const *)buf,"GET /IMG/bg03.png",17)==0)
//////////////            {
//////////////              fs_open(&file, "/IMG/bg03.png");
//////////////              netconn_write(conn, (const unsigned char*)(file.data), (size_t)file.len, NETCONN_NOCOPY);
//////////////              fs_close(&file);
//////////////            }
//////////////      else if (strncmp((char const *)buf,"GET /content.html",17)==0)
//////////////            {
//////////////              DynWebPageStr(conn);
//////////////            }
//////////////        else if((strncmp(buf, "GET /index.html", 15) == 0)||(strncmp(buf, "GET / ", 6) == 0)) 
//////////////        {
//////////////          /* Load STM32F7xx page */
//////////////////          fs_open(&file, "/index.html"); 
//////////////////          netconn_write(conn, (const unsigned char*)(file.data), (size_t)file.len, NETCONN_NOCOPY);
//////////////////          fs_close(&file);
//////////////        }
//////////        else 
//////////        {
//////////          /* Load Error page */
//////////          fs_open(&file, "/404.html"); 
//////////          netconn_write(conn, (const unsigned char*)(file.data), (size_t)file.len, NETCONN_NOCOPY);
//////////          fs_close(&file);
//////////        }
//////////      }      
//////////    }
//////////  }
//////////  /* Close the connection (server closes in HTTP) */
//////////  netconn_close(conn);
//////////  
//////////  /* Delete the buffer (netconn_recv gives us ownership,
//////////   so we have to make sure to deallocate the buffer) */
//////////  netbuf_delete(inbuf);
//////////}
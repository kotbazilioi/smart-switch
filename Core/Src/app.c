#include "app.h"
#include "ntp.h"
#include "syslog.h"
#include "smtp.h"
#include "snmp_ex.h"
#include "main.h"
#define logoff_time 200*10
extern size_t xFreeBytesRemaining;
extern size_t xMinimumEverFreeBytesRemaining;
char mess_smtp[256]={0}; 
osThreadId defaultTaskHandle;
osThreadId LED_taskHandle;
osThreadId logs_task_nameHandle;
osThreadId  IO_CNTRLHandle;
osThreadId rasp_task_id;
osThreadId iwdt_task_id;
uint8_t flag_set_ip=0;
uint16_t ct_dns_time=0;
uint8_t status_dns=0;
V_D_TIME_type slot_day,slot_repars[10];
uint8_t flag_err_dates=0;
IWDG_HandleTypeDef  iwdt_hdr;




void set_relay(uint8_t sost)
{
 if (sost==0)
  {
    HAL_GPIO_WritePin(RELAY_INT_A1_GPIO_Port,RELAY_INT_A1_Pin,GPIO_PIN_RESET);
  }
 else
  {
    HAL_GPIO_WritePin(RELAY_INT_A1_GPIO_Port,RELAY_INT_A1_Pin,GPIO_PIN_SET);
  }
 osDelay(100);
 HAL_GPIO_WritePin(RELAY_TRIG_GPIO_Port,RELAY_TRIG_Pin,GPIO_PIN_SET);
 osDelay(100);
 HAL_GPIO_WritePin(RELAY_TRIG_GPIO_Port,RELAY_TRIG_Pin,GPIO_PIN_RESET);
}


void smtp_serverFound (const char * name, struct ip_addr * ipaddr, void * arg) 
{ 

}

void StartDefaultTask(void const * argument)
{
 ip4_addr_t ipdns1;
 
 MX_LWIP_Init();
 http_server_netconn_init();
 ping_init();
 IP4_ADDR(&ip_syslog, FW_data.V_IP_SYSL[0], FW_data.V_IP_SYSL[1], FW_data.V_IP_SYSL[2], FW_data.V_IP_SYSL[3]);    
 syslog_init(local_syslog_ctx,ip_syslog);
 snmp_ex_init();
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
      }
  }
}
void set_out_port(uint8_t sost,uint8_t canal)
{
  if (canal==1)
   {
    if (sost==0)
     {
      if ((FW_data.V_TYPE_OUT==0)||(FW_data.V_TYPE_OUT==2))
       {
        set_relay(0);
       }
      if ((FW_data.V_TYPE_OUT==1)||(FW_data.V_TYPE_OUT==3))
       {
        set_relay(1);
       }    
     }
    if (sost==1)
     {
      if ((FW_data.V_TYPE_OUT==0)||(FW_data.V_TYPE_OUT==2))
       {
        set_relay(1);
       }
      if ((FW_data.V_TYPE_OUT==1)||(FW_data.V_TYPE_OUT==3))
       {
        set_relay(0);
       }    
     }  
    if (sost>1)
     {
       if ((FW_data.V_TYPE_OUT==0)||(FW_data.V_TYPE_OUT==2))
        {
         set_relay(0);
         osDelay(1000*FW_data.V_TIME_RESET_PULSE);
         set_relay(1);
        }    
       if ((FW_data.V_TYPE_OUT==1)||(FW_data.V_TYPE_OUT==3))
        {
          set_relay(1);
          osDelay(1000*FW_data.V_TIME_RESET_PULSE);
          set_relay(0);
        }    
     }
  }  
}


void IO_CNRL_APP(void const * argument)
{
 uint32_t OID_out[]={1,2020,1,0};
 uint16_t data=0;
 for(;;)
 {
  if ((flag_global_swich_out==SWICH_ON_WEB)||(flag_global_swich_out==SWICH_OFF_WEB)||(flag_global_swich_out==SWICH_TOLG_WEB)||(flag_global_swich_out==SWICH_ON_RASP)||(flag_global_swich_out==SWICH_OFF_RASP)||
     (flag_global_swich_out==SWICH_ON_SNMP)||(flag_global_swich_out==SWICH_OFF_SNMP)||(flag_global_swich_out==SWICH_TOLG_SNMP)||(flag_global_swich_out==SWICH_TOLG_WATCH))
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
      set_out_port(data,1);
      flag_global_swich_out=0;
     }
    if (data>1)
     {
      OID_out[3]=3;
      send_mess_trap(OID_out,FW_data.V_ON_MESS,strlen(FW_data.V_ON_MESS));
      set_out_port(2,1); 
      flag_global_swich_out=0;
     }
   }
  osDelay(100);
 }
 
}


void LED_task(void const * argument)
{
 HAL_GPIO_TogglePin (LED_RED_GPIO_Port, LED_RED_Pin);
 for(;;)
  {
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
   HAL_GPIO_TogglePin (LED_RED_GPIO_Port, LED_RED_Pin);
   osDelay(10);
   HAL_GPIO_TogglePin (LED_RED_GPIO_Port, LED_RED_Pin);
   osDelay(200);
   HAL_GPIO_TogglePin (LED_RED_GPIO_Port, LED_RED_Pin);
   osDelay(10);
   HAL_GPIO_TogglePin (LED_RED_GPIO_Port, LED_RED_Pin);
     
     
#if (LWIP_DEBUG)  
   printf("xFreeBytesRemaining=%d   ",xFreeBytesRemaining);
   printf("xMinimumEverFreeBytesRemaining=%d\n\r",xMinimumEverFreeBytesRemaining);
#endif
   if ((FW_data.V_EN_WATCHDOG==1)&&(flag_delay_ping==0))
    {      
     if (FW_data.V_TYPE_LOGIC==0)
      {
       if ( (ping_data.flag_err[0]!=0)||(ping_data.flag_err[1]!=0)||(ping_data.flag_err[2]!=0))
        {
         if (ct_max_res<FW_data.V_MAX_RESEND_PACET_RESET)
          {
           form_reple_to_save(SWICH_TOLG_WATCH);
           flag_global_swich_out=SWICH_TOLG_WATCH;
           HAL_RTCEx_BKUPWrite(&hrtc,1,2);  
           ct_max_res++;
           flag_delay_ping=1;
           ct_cn_a=0;
           ct_cn_b=0;
           ct_cn_c=0;
          }
         if (FW_data.V_MAX_RESEND_PACET_RESET==0)
          {
           form_reple_to_save(SWICH_TOLG_WATCH);
           flag_global_swich_out=SWICH_TOLG_WATCH;
           HAL_RTCEx_BKUPWrite(&hrtc,1,2);                
           flag_delay_ping=1;
           ct_cn_a=0;
           ct_cn_b=0;
           ct_cn_c=0;
          }
        }
       else
        {
         ct_max_res=0;
        }
      }      
     if (FW_data.V_TYPE_LOGIC==1)
      {
       if ( (ping_data.flag_err[0]!=0)&&(ping_data.flag_err[1]!=0)&&(ping_data.flag_err[2]!=0))
        {
         if (ct_max_res<FW_data.V_MAX_RESEND_PACET_RESET)
          {
           form_reple_to_save(SWICH_TOLG_WATCH);
           flag_global_swich_out=SWICH_TOLG_WATCH;
           HAL_RTCEx_BKUPWrite(&hrtc,1,2);  
           ct_max_res++;
           flag_delay_ping=1;
           ct_cn_a=0;
           ct_cn_b=0;
           ct_cn_c=0;
          }
         if (FW_data.V_MAX_RESEND_PACET_RESET==0)
          {
           form_reple_to_save(SWICH_TOLG_WATCH);
           flag_global_swich_out=SWICH_TOLG_WATCH;
           HAL_RTCEx_BKUPWrite(&hrtc,1,2);                
           flag_delay_ping=1;
           ct_cn_a=0;
           ct_cn_b=0;
           ct_cn_c=0;
          }
        }
       else
        {
         ct_max_res=0;
        }
      }
     if (FW_data.V_TYPE_LOGIC==2)
      {
       if ( ((ping_data.flag_err[0]!=0)&&(ping_data.flag_err[1]!=0))||((ping_data.flag_err[0]!=0)&&(ping_data.flag_err[2]!=0)))
        {
         if (ct_max_res<FW_data.V_MAX_RESEND_PACET_RESET)
          {
           form_reple_to_save(SWICH_TOLG_WATCH);
           flag_global_swich_out=SWICH_TOLG_WATCH;
           HAL_RTCEx_BKUPWrite(&hrtc,1,2);  
           ct_max_res++;
           flag_delay_ping=1;
           ct_cn_a=0;
           ct_cn_b=0;
           ct_cn_c=0;
          }
         if (FW_data.V_MAX_RESEND_PACET_RESET==0)
          {
           form_reple_to_save(SWICH_TOLG_WATCH);
           flag_global_swich_out=SWICH_TOLG_WATCH;
           HAL_RTCEx_BKUPWrite(&hrtc,1,2);                
           flag_delay_ping=1;
           ct_cn_a=0;
           ct_cn_b=0;
           ct_cn_c=0;
          }
        }
       else
        {
         ct_max_res=0;
        }
       }
     if (FW_data.V_TYPE_LOGIC==3)
       {
        if ( ((ping_data.flag_err[0]!=0)&&(ping_data.flag_err[1]!=1))&&((ping_data.flag_err[0]!=0)&&(ping_data.flag_err[2]!=1)))
         {
          if (ct_max_res<FW_data.V_MAX_RESEND_PACET_RESET)
           {
            form_reple_to_save(SWICH_TOLG_WATCH);
            flag_global_swich_out=SWICH_TOLG_WATCH;
            HAL_RTCEx_BKUPWrite(&hrtc,1,2);  
            ct_max_res++;
            flag_delay_ping=1;
            ct_cn_a=0;
            ct_cn_b=0;
            ct_cn_c=0;
           }
          if (FW_data.V_MAX_RESEND_PACET_RESET==0)
           {
            form_reple_to_save(SWICH_TOLG_WATCH);
            flag_global_swich_out=SWICH_TOLG_WATCH;
            HAL_RTCEx_BKUPWrite(&hrtc,1,2);                
            flag_delay_ping=1;
            ct_cn_a=0;
            ct_cn_b=0;
            ct_cn_c=0;
           }
         }
        else
         {
          ct_max_res=0;
         }
      }
   }
   osDelay(700);
  }
}

void logs_task(void const * argument)
{
  //osDelay(5000);
  form_reple_to_save(POWER_ON);
  GET_reple(0,&start_time);
  for(;;)
   {
    if (flag_my_smtp_test==1)
     {
      my_smtp_test();
      flag_my_smtp_test=0;
     }
    if (flag_global_save_log==1)
     {
      memset (mess_smtp,0,128);       
      save_reple_log(reple_to_save);
      decode_reple(mess_smtp, &reple_to_save);  
      send_smtp_mess(mess_smtp);
      flag_global_save_log=0;
     }
    if ((flag_global_save_data==1)&&(flag_global_save_log==0))
     {
      save_data_flash();
      flag_global_save_data=0;
     }
    if ((flag_global_load_def==1)&&(flag_global_save_log==0))
     {
      load_def_data();      
      flag_global_load_def=0;
     }
    if ((flag_global_boot_mode==1)&&(flag_global_save_log==0))
     {
      vTaskDelay(1000);
      jamp_to_boot();
      flag_global_boot_mode=0;
     }
     if ((flag_global_reset_mode==1)&&(flag_global_save_log==0))
     {
      vTaskDelay(1000);
      jamp_to_app();
      flag_global_reset_mode=0;
     }
    osDelay(100);
  }
}
void iwdt_task(void const * argument)
{
  #if (IWDT_EN) 
   __HAL_RCC_LSI_ENABLE();
   iwdt_hdr.Instance=IWDG;
   iwdt_hdr.Init.Prescaler=255;
   iwdt_hdr.Init.Reload=1*159;
   HAL_IWDG_Init(&iwdt_hdr);
  #endif
 while(1)
  {
   #if (IWDT_EN) 
    HAL_IWDG_Refresh(&iwdt_hdr);
   #endif
   vTaskDelay(200);
  }  
}

void rasp_task(void const * argument)
{
 uint8_t ct_podm,ct_slots;
 uint8_t start_flag=0;
 uint8_t old_WeekDay=0;
 uint8_t   flag_set_out_fasp=0;
 while(1)
  {
   if (FW_data.V_FLAG_EN_RASP!=0)
    {
     HAL_RTC_GetDate (&hrtc,&dates,RTC_FORMAT_BIN);
     HAL_RTC_GetTime (&hrtc,&times,RTC_FORMAT_BIN);
     if (dates.Year<21)
      {
       flag_err_dates=1;
      }
     else
      {
       flag_err_dates=0;
      }
       start_flag=1;
       old_WeekDay=dates.WeekDay;
       for (ct_slots=0;ct_slots<6;ct_slots++)
        {
         slot_repars[0].time_data[ct_slots].off_swich_h=FW_data.V_D_TIME[0].time_data[ct_slots].off_swich_h;
         slot_repars[0].time_data[ct_slots].off_swich_m=FW_data.V_D_TIME[0].time_data[ct_slots].off_swich_m;
         slot_repars[0].time_data[ct_slots].on_swich_h=FW_data.V_D_TIME[0].time_data[ct_slots].on_swich_h;
         slot_repars[0].time_data[ct_slots].on_swich_m=FW_data.V_D_TIME[0].time_data[ct_slots].on_swich_m;  
        }
       for (ct_podm=1;ct_podm<10;ct_podm++)
        {
         for (ct_slots=0;ct_slots<6;ct_slots++)
          {
           slot_repars[ct_podm].time_data[ct_slots].off_swich_h=FW_data.V_D_TIME[ct_podm].time_data[ct_slots].off_swich_h;
           slot_repars[ct_podm].time_data[ct_slots].off_swich_m=FW_data.V_D_TIME[ct_podm].time_data[ct_slots].off_swich_m;
           slot_repars[ct_podm].time_data[ct_slots].on_swich_h=FW_data.V_D_TIME[ct_podm].time_data[ct_slots].on_swich_h;
           slot_repars[ct_podm].time_data[ct_slots].on_swich_m=FW_data.V_D_TIME[ct_podm].time_data[ct_slots].on_swich_m;  
           if (FW_data.V_D_TIME[ct_podm].set_up_day!=0)
            {
             slot_repars[ct_podm].time_data[ct_slots].off_swich_h=slot_repars[ct_podm-1].time_data[ct_slots].off_swich_h;
             slot_repars[ct_podm].time_data[ct_slots].off_swich_m=slot_repars[ct_podm-1].time_data[ct_slots].off_swich_m;
             slot_repars[ct_podm].time_data[ct_slots].on_swich_h=slot_repars[ct_podm-1].time_data[ct_slots].on_swich_h;
             slot_repars[ct_podm].time_data[ct_slots].on_swich_m=slot_repars[ct_podm-1].time_data[ct_slots].on_swich_m;
            }
          }                
       }
      memcpy(&(slot_day),&(slot_repars[dates.WeekDay-1]),sizeof(slot_day));
      for (ct_podm=0;ct_podm<10;ct_podm++)
       {
        if ((dates.Date==FW_data.V_RD_DATA.data[ct_podm].day)&&(dates.Month==FW_data.V_RD_DATA.data[ct_podm].month)&&(dates.Year==FW_data.V_RD_DATA.data[ct_podm].year))
         {
          memcpy(slot_day,slot_repars[FW_data.V_RD_DATA.restore_day[ct_podm]],sizeof(slot_day));
         }
       }  
      for (ct_slots=0;ct_slots<6;ct_slots++)
       {
        if ((slot_day.time_data[ct_slots].on_swich_h==times.Hours)&&(slot_day.time_data[ct_slots].on_swich_m==times.Minutes)&&(flag_set_out_fasp==0))
         {
          flag_set_out_fasp=1;
          if (flag_err_dates==0)
           {
            form_reple_to_save(SWICH_ON_RASP);
            flag_global_swich_out=SWICH_ON_RASP;
            HAL_RTCEx_BKUPWrite(&hrtc,1,0);
           }
          else
           {
            if (FW_data.V_SOST_ERR_RASP==1)
             {
              form_reple_to_save(SWICH_ON_RASP);
              flag_global_swich_out=SWICH_ON_RASP;
              HAL_RTCEx_BKUPWrite(&hrtc,1,0);
             }
            if (FW_data.V_SOST_ERR_RASP==2)
             {
              form_reple_to_save(SWICH_OFF_RASP);
              flag_global_swich_out=SWICH_OFF_RASP;
              HAL_RTCEx_BKUPWrite(&hrtc,1,1);                      
             }
           }
         }
        if ((slot_day.time_data[ct_slots].off_swich_h==times.Hours)&&(slot_day.time_data[ct_slots].off_swich_m==times.Minutes)&&(flag_set_out_fasp==0))
         {
          flag_set_out_fasp=1;
          if (flag_err_dates==0)
           {
            form_reple_to_save(SWICH_OFF_RASP);
            flag_global_swich_out=SWICH_OFF_RASP;
            HAL_RTCEx_BKUPWrite(&hrtc,1,1);
           }
            else
           {
            if (FW_data.V_SOST_ERR_RASP==1)
             {
              form_reple_to_save(SWICH_ON_RASP);
              flag_global_swich_out=SWICH_ON_RASP;
              HAL_RTCEx_BKUPWrite(&hrtc,1,0);
             }
            if (FW_data.V_SOST_ERR_RASP==2)
             {
              form_reple_to_save(SWICH_OFF_RASP);
              flag_global_swich_out=SWICH_OFF_RASP;
              HAL_RTCEx_BKUPWrite(&hrtc,1,1);                      
             }
           }
         }
      }
      flag_set_out_fasp=0;        
     }
     vTaskDelay(60*1000);
    }
}
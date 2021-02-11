#include "main.h"
#include <string.h>
#include "LOGS.h"
#include "flash_if.h"

void logs_read (void)
  {
     memcpy((uint8_t *)(&FW_data.V_logs_struct.CRC16), (uint8_t *)A_CRC_LOG, 2002);
  }
void save_reple_log (log_reple_t reple2)
  {
    log_reple_t reple_temp;
    uint8_t ct_repl;
    logs_read();
    reple2.dicr= 0x5a;
    if (FW_data.V_logs_struct.log_reple[199].dicr==0x5a)
      {
         memcpy((uint8_t *)(&FW_data.V_logs_struct.log_reple[0]), (uint8_t *)(&FW_data.V_logs_struct.log_reple[1]), 1990);
         memcpy((uint8_t *)(&FW_data.V_logs_struct.log_reple[199]), (uint8_t *)(&reple2), 10);
         FW_data.V_logs_struct.CRC16 = crc16_ccitt((uint8_t*)&(FW_data.V_logs_struct.log_reple[0]),2000);
         
      }
    else
      {
         for (ct_repl=0;ct_repl<200;ct_repl++)
          {
            memcpy((uint8_t *)(&reple_temp),(uint8_t *)(&FW_data.V_logs_struct.log_reple[ct_repl]), 10);
            if (reple_temp.dicr!= 0x5a)
              {
                memcpy((uint8_t *)(&FW_data.V_logs_struct.log_reple[ct_repl]),(uint8_t *)(&reple2), 10);
                FW_data.V_logs_struct.CRC16 = crc16_ccitt((uint8_t*)&(FW_data.V_logs_struct.log_reple[0]),2000);
                ct_repl=200;
              }
          }
      }
    save_data_blok(1,(uint32_t*)&FW_data.V_logs_struct.CRC16);     
  }

void GET_reple (uint8_t event,log_reple_t* reple)
{
  RTC_DateTypeDef dates;
  RTC_TimeTypeDef times;
  HAL_RTC_GetDate (&hrtc,&dates,RTC_FORMAT_BIN);
  HAL_RTC_GetTime (&hrtc,&times,RTC_FORMAT_BIN);
  reple->type_event = event;
  reple->reple_hours = times.Hours;
  reple->reple_minuts = times.Minutes;
  reple->reple_seconds = times.Seconds;
  reple->dweek = dates.WeekDay;
  reple->day = dates.Date;
  reple->month = dates.Month;
  reple->year = 2000+dates.Year;
  reple->dicr = 0x5a;  
}

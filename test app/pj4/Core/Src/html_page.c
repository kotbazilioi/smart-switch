#include <string.h>
#include "html_page.h"
#include "flash_if.h"
#include "LOGS.h"
#include "app.h"
uint32_t ct_logoff_time;
uint8_t flag_logon=0;

//const char Name_dev[]="DKST 59";
const char contact_data[]="http://www.netping.ru/";
const char model_dev[]="UniPing port";
char str2[512]={0};
char str3[512]={0};
char str4[128]={0};
extern RTC_HandleTypeDef hrtc;
unsigned char key_http[30]="asfasdvas";
int key_http_len = 10;
char HTTP401[]={0x48,0x54,0x54,0x50,0x2f,0x31,0x2e,0x31,0x20,0x34,0x30,0x31,0x20,0x55,0x6e,0x61,   //HTTP/1.1 401 Una
0x75,0x74,0x68,0x6f,0x72,0x69,0x7a,0x65,0x64,0x0d,0x0a,0x57,0x57,0x57,0x2d,0x41,   //uthorized..WWW-A
0x75,0x74,0x68,0x65,0x6e,0x74,0x69,0x63,0x61,0x74,0x65,0x3a,0x20,0x42,0x61,0x73,   //uthenticate: Bas
0x69,0x63,0x20,0x72,0x65,0x61,0x6c,0x6d,0x3d,0x22}; 

char HTTP401end[]={0x22,0x0d,0x0a,0x0d,0x0a};
static const char http_logs_hdr[]="<style> textarea { width: 90%;height:400px;resize:none;}</style><center><form action>";
static const char http_logs_area[]="<p><textarea name=\"comment\" align=\"center\" readonly rows=\"10\" cols=\"2\" align=\"center\" disabled >";
static const char http_logs_end[] ="</textarea></p>  </center>";
static const char http_html_hdr[] = "HTTP/1.1 200 OK\r\nContent-type: text/html\r\n\r\n";


const char http_html_200[] = {
/* /404.html (10 chars) */
//0x2f,0x32,0x30,0x30,0x2e,0x68,0x74,0x6d,0x6c,0x00,0x00,0x00,

/* HTTP header */
/* "HTTP/1.1 200 OK
" (17 bytes) */
0x48,0x54,0x54,0x50,0x2f,0x31,0x2e,0x31,0x20,0x32,0x30,0x30,0x20,0x4f,0x4b,0x0d,
0x0a,
/* "Server: lwIP/1.3.1 (http://savannah.nongnu.org/projects/lwip)
" (63 bytes) */
0x53,0x65,0x72,0x76,0x65,0x72,0x3a,0x20,0x6c,0x77,0x49,0x50,0x2f,0x31,0x2e,0x33,
0x2e,0x31,0x20,0x28,0x68,0x74,0x74,0x70,0x3a,0x2f,0x2f,0x73,0x61,0x76,0x61,0x6e,
0x6e,0x61,0x68,0x2e,0x6e,0x6f,0x6e,0x67,0x6e,0x75,0x2e,0x6f,0x72,0x67,0x2f,0x70,
0x72,0x6f,0x6a,0x65,0x63,0x74,0x73,0x2f,0x6c,0x77,0x69,0x70,0x29,0x0d,0x0a,
/* "Content-Length: 3128
" (18+ bytes) */
0x43,0x6f,0x6e,0x74,0x65,0x6e,0x74,0x2d,0x4c,0x65,0x6e,0x67,0x74,0x68,0x3a,0x20,
};
const char http_html_200_end[] = {
0x0d,0x0a,
/* "Connection: Close
" (19 bytes) */
0x43,0x6f,0x6e,0x6e,0x65,0x63,0x74,0x69,0x6f,0x6e,0x3a,0x20,0x43,0x6c,0x6f,0x73,
0x65,0x0d,0x0a,
/* "Content-type: text/html

" (27 bytes) */
0x43,0x6f,0x6e,0x74,0x65,0x6e,0x74,0x2d,0x74,0x79,0x70,0x65,0x3a,0x20,0x74,0x65,
0x78,0x74,0x2f,0x68,0x74,0x6d,0x6c,0x0d,0x0a,0x0d,0x0a
};
  //char * data[]="<a href=\"settings.html\" target=\"_self\" rel=\"nofollow\">????????? &emsp; </a>";
//static const char http_html_start_constr[] = "\<!DOCTYPE html> <body onload=\"onload()\"\>  <a href=\"http:\/\/www.netping.ru/\"><img src=\"img/netping.gif\" height=\"59\" width=\"169\" border=\"0\" alt=\"netping logo\" title=\"200 OK\"></a> <html><head><meta http-equiv=\"Content-Type\" content=\"text/html; charset=utf-8\" />"
static const char http_html_start_constr[] = "\<!DOCTYPE html> <body onload=\"onload()\"\>  <a href=\"http:\/\/www.netping.ru/\"><img src=\"img/netping.gif\" height=\"59\" width=\"169\" border=\"0\" alt=\"netping logo\" title=\"200 OK\"></a> <html><head><meta http-equiv=\"Content-Type\" content=\"text/html; charset=utf-8\" />"
         "\<script\>"
	  "var xhr\;"
           "var idTimer1\";"
	   "function onload(){"
	   "xhr = new (XMLHttpRequest);"
	   "}"
//           
//	   "function swich1(){"
//           "xhr.open(\"GET\", \"swich?c=1\", true);"
//	   "xhr.responseType = \"text\";"
//	   "xhr.send(null);"
//	   "}"	
//	  "function swich2(){"
//	  "xhr.open(\"GET\", \"swich?c=2\", true);"
//	  "xhr.responseType = \"text\";"
//	  "xhr.send(null);"
//	  "}"
//	  "function swich3(){"
//	   "xhr.open(\"GET\", \"swich?c=3\", true);"
//	    "xhr.responseType = \"text\";"
//	    "xhr.send(null);"
//	  "}"
"</script>"
;


static const char http_html_style[] = 
"<title>NetPing</title>"
"  <style >"
"body {"
    "background: #F6FCF9;" /* Цвет фона */
    //"color: #fff;" /* Цвет текста */
   "}"
   "h1 {"
	"font-family: Tahoma;"//Courier;" /* Гарнитура шрифта */
	"src: url(fonts/Courier.ttf);" /* Путь к файлу со шрифтом */
        "text-align: center;"
          "font-size: 16pt;"
    "}"
    "h2 {"
      "font-family: Courier;"
	"font-size: 14pt;"
        "text-align: center;"
          
    "}"
      
     "h3 {"
       "font-family: Tahoma;"
       // "text-decoration:  underline 1px solid green;"
      //  "border-bottom: 2px  solid Seagreen;"/* Параметры линии под текстом */
        "font-weight: normal;" /* Убираем жирное начертание */
      //  "padding-bottom: 1px;" /* Расстояние от текста до линии */          
	"font-size: 10pt;"
        "text-align: center;"
      "}"
      "h4 {"
       "font-family: Tahoma;"
       // "text-decoration:  underline 1px solid green;"
      //  "border-bottom: 2px  solid Seagreen;"/* Параметры линии под текстом */
        "font-weight: normal;" /* Убираем жирное начертание */
      //  "padding-bottom: 1px;" /* Расстояние от текста до линии */          
	"font-size: 10pt;"
          "position: absolute;"
          "right: 11% ;"
      "}"

         
   "display: inline-block;"
  "</style>"
    "</head>";


void swich_mess_event (uint8_t event,char* mess)
{
 switch(event) {
          case NO_RUN:
            {
              
            }
          break;
          
          case RESETL:
            {
              sprintf(mess,"Сброс утройства произведен SN:%d %d %d \n\r",FW_data.V_ID_MAC[5],FW_data.V_ID_MAC[6],FW_data.V_ID_MAC[7]);
            }
          break;
          case UPDATE_FW:
            {
              sprintf(mess,"Произведен перевод в режим загрузки SN:%d %d %d \n\r",FW_data.V_ID_MAC[5],FW_data.V_ID_MAC[6],FW_data.V_ID_MAC[7]);
            }
          break;
          case SWICH_ON_WEB:
            {
              sprintf(mess,"Включена нагрузка с веб интерфейса SN:%d %d %d \n\r",FW_data.V_ID_MAC[5],FW_data.V_ID_MAC[6],FW_data.V_ID_MAC[7]);
            }
          break;
          case SWICH_OFF_WEB:
            {
              sprintf(mess,"Выключена нагрузка с веб интерфейса SN:%d %d %d \n\r",FW_data.V_ID_MAC[5],FW_data.V_ID_MAC[6],FW_data.V_ID_MAC[7]);
            }
          break;
          case SWICH_TOLG_WEB:
            {
              sprintf(mess,"Произведен импульсный сброс нагрузка с веб интерфейса SN:%d %d %d \n\r",FW_data.V_ID_MAC[5],FW_data.V_ID_MAC[6],FW_data.V_ID_MAC[7]);
            }
          break;
           case SWICH_ON_SNMP:
            {
              sprintf(mess,"Включена нагрузка с SNMP интерфейса SN:%d %d %d \n\r",FW_data.V_ID_MAC[5],FW_data.V_ID_MAC[6],FW_data.V_ID_MAC[7]);
            }
          break;
          case SWICH_OFF_SNMP:
            {
              sprintf(mess,"Выключена нагрузка с SNMP интерфейса SN:%d %d %d \n\r",FW_data.V_ID_MAC[5],FW_data.V_ID_MAC[6],FW_data.V_ID_MAC[7]);
            }
          break;
          case SWICH_TOLG_SNMP:
            {
              sprintf(mess,"Произведен импульсный сброс нагрузка с SNMP интерфейса SN:%d %d %d \n\r",FW_data.V_ID_MAC[5],FW_data.V_ID_MAC[6],FW_data.V_ID_MAC[7]);
            }
          break;
          case SWICH_ON_RASP:
            {
              sprintf(mess,"Включена нагрузка по расписанию SN:%d %d %d \n\r",FW_data.V_ID_MAC[5],FW_data.V_ID_MAC[6],FW_data.V_ID_MAC[7]);
            }
          break;
          case SWICH_OFF_RASP:
            {
              sprintf(mess,"Выключена нагрузка по расписанию SN:%d %d %d \n\r",FW_data.V_ID_MAC[5],FW_data.V_ID_MAC[6],FW_data.V_ID_MAC[7]);
            }
          break;
          case SWICH_TOLG_RASP:
            {
              sprintf(mess,"Произведен импульсный сброс нагрузка по расписанию SN:%d %d %d \n\r",FW_data.V_ID_MAC[5],FW_data.V_ID_MAC[6],FW_data.V_ID_MAC[7]);
            }
          break;
          case SWICH_ON_WATCH:
            {
              sprintf(mess,"Включена нагрузка по сторожу SN:%d %d %d \n\r",FW_data.V_ID_MAC[5],FW_data.V_ID_MAC[6],FW_data.V_ID_MAC[7]);
            }
          break;
          case SWICH_OFF_WATCH:
            {
              sprintf(mess,"Выключена нагрузка по сторожу SN:%d %d %d \n\r",FW_data.V_ID_MAC[5],FW_data.V_ID_MAC[6],FW_data.V_ID_MAC[7]);
            }
          break;
          case SWICH_TOLG_WATCH:
            {
              sprintf(mess,"Произведен импульсный сброс нагрузки по сторожу SN:%d %d %d \n\r",FW_data.V_ID_MAC[5],FW_data.V_ID_MAC[6],FW_data.V_ID_MAC[7]);
            }
          break;
          case SWICH_ON_HTTP:
            {
              sprintf(mess,"Включена нагрузка по HTTP API SN:%d %d %d \n\r",FW_data.V_ID_MAC[5],FW_data.V_ID_MAC[6],FW_data.V_ID_MAC[7]);
            }
          break;
          case SWICH_OFF_HTTP:
            {
              sprintf(mess,"Выключена нагрузка по HTTP API SN:%d %d %d \n\r",FW_data.V_ID_MAC[5],FW_data.V_ID_MAC[6],FW_data.V_ID_MAC[7]);
            }
          break;
          case SWICH_TOLG_HTTP:
            {
              sprintf(mess,"Произведен импульсный сброс нагрузки по HTTP API SN:%d %d %d \n\r",FW_data.V_ID_MAC[5],FW_data.V_ID_MAC[6],FW_data.V_ID_MAC[7]);
            }
          break;         
          case POWER_ON:
            {
              sprintf(mess,"Включением питание утройства SN:%d %d %d \n\r",FW_data.V_ID_MAC[5],FW_data.V_ID_MAC[6],FW_data.V_ID_MAC[7]);
            }
          break;
          case LOAD_DEF_DATA:
            {
              sprintf(mess,"Загружены настройки по умолчанию SN:%d %d %d \n\r",FW_data.V_ID_MAC[5],FW_data.V_ID_MAC[6],FW_data.V_ID_MAC[7]);
            }
          break;
          case SAVE_DATA_SETT:
            {
              sprintf(mess,"Сохранение настроек устройства SN:%d %d %d \n\r",FW_data.V_ID_MAC[5],FW_data.V_ID_MAC[6],FW_data.V_ID_MAC[7]);
            }
          break;
        }
}

void swich_mess_event_en (uint8_t event,char* mess)
{
 switch(event) {
          case NO_RUN:
            {
              
            }
          break;
          
          case RESETL:
            {
              sprintf(mess,"The device has been reset SN:%d %d %d \n\r",FW_data.V_ID_MAC[5],FW_data.V_ID_MAC[6],FW_data.V_ID_MAC[7]);
            }
          break;
          case UPDATE_FW:
            {
              sprintf(mess,"Transferred to download mode SN:%d %d %d \n\r",FW_data.V_ID_MAC[5],FW_data.V_ID_MAC[6],FW_data.V_ID_MAC[7]);
            }
          break;
          case SWICH_ON_WEB:
            {
              sprintf(mess,"Load from the web interface is enabled SN:%d %d %d \n\r",FW_data.V_ID_MAC[5],FW_data.V_ID_MAC[6],FW_data.V_ID_MAC[7]);
            }
          break;
          case SWICH_OFF_WEB:
            {
              sprintf(mess,"The load from the web interface is turned off SN:%d %d %d \n\r",FW_data.V_ID_MAC[5],FW_data.V_ID_MAC[6],FW_data.V_ID_MAC[7]);
            }
          break;
          case SWICH_TOLG_WEB:
            {
              sprintf(mess,"Impulse load dump was performed from the web interface SN:%d %d %d \n\r",FW_data.V_ID_MAC[5],FW_data.V_ID_MAC[6],FW_data.V_ID_MAC[7]);
            }
          break;
           case SWICH_ON_SNMP:
            {
              sprintf(mess,"Load from SNMP interface is enabled SN:%d %d %d \n\r",FW_data.V_ID_MAC[5],FW_data.V_ID_MAC[6],FW_data.V_ID_MAC[7]);
            }
          break;
          case SWICH_OFF_SNMP:
            {
              sprintf(mess,"Disabled load from SNMP interface SN:%d %d %d \n\r",FW_data.V_ID_MAC[5],FW_data.V_ID_MAC[6],FW_data.V_ID_MAC[7]);
            }
          break;
          case SWICH_TOLG_SNMP:
            {
              sprintf(mess,"Pulse load shedding from SNMP interface has been performed SN:%d %d %d \n\r",FW_data.V_ID_MAC[5],FW_data.V_ID_MAC[6],FW_data.V_ID_MAC[7]);
            }
          break;
          case SWICH_ON_RASP:
            {
              sprintf(mess,"Scheduled load is enabled SN:%d %d %d \n\r",FW_data.V_ID_MAC[5],FW_data.V_ID_MAC[6],FW_data.V_ID_MAC[7]);
            }
          break;
          case SWICH_OFF_RASP:
            {
              sprintf(mess,"Disabled load on schedule SN:%d %d %d \n\r",FW_data.V_ID_MAC[5],FW_data.V_ID_MAC[6],FW_data.V_ID_MAC[7]);
            }
          break;
          case SWICH_TOLG_RASP:
            {
              sprintf(mess,"Impulse reset of the load according to the schedule SN:%d %d %d \n\r",FW_data.V_ID_MAC[5],FW_data.V_ID_MAC[6],FW_data.V_ID_MAC[7]);
            }
          break;
          case SWICH_ON_WATCH:
            {
              sprintf(mess,"Watchdog load included SN:%d %d %d \n\r",FW_data.V_ID_MAC[5],FW_data.V_ID_MAC[6],FW_data.V_ID_MAC[7]);
            }
          break;
          case SWICH_OFF_WATCH:
            {
              sprintf(mess,"The watchdog load is turned off SN:%d %d %d \n\r",FW_data.V_ID_MAC[5],FW_data.V_ID_MAC[6],FW_data.V_ID_MAC[7]);
            }
          break;
          case SWICH_TOLG_WATCH:
            {
              sprintf(mess,"Impulse load shedding by the watchman SN:%d %d %d \n\r",FW_data.V_ID_MAC[5],FW_data.V_ID_MAC[6],FW_data.V_ID_MAC[7]);
            }
          break;
          case SWICH_ON_HTTP:
            {
              sprintf(mess,"HTTP API loading enabled SN:%d %d %d \n\r",FW_data.V_ID_MAC[5],FW_data.V_ID_MAC[6],FW_data.V_ID_MAC[7]);
            }
          break;
          case SWICH_OFF_HTTP:
            {
              sprintf(mess,"Disabled loading via HTTP API SN:%d %d %d \n\r",FW_data.V_ID_MAC[5],FW_data.V_ID_MAC[6],FW_data.V_ID_MAC[7]);
            }
          break;
          case SWICH_TOLG_HTTP:
            {
              sprintf(mess,"Pulse load shedding via HTTP API has been performed SN:%d %d %d \n\r",FW_data.V_ID_MAC[5],FW_data.V_ID_MAC[6],FW_data.V_ID_MAC[7]);
            }
          break;         
          case POWER_ON:
            {
              sprintf(mess,"Power on the device SN:%d %d %d \n\r",FW_data.V_ID_MAC[5],FW_data.V_ID_MAC[6],FW_data.V_ID_MAC[7]);
            }
          break;
          case LOAD_DEF_DATA:
            {
              sprintf(mess,"Loaded default settings SN:%d %d %d \n\r",FW_data.V_ID_MAC[5],FW_data.V_ID_MAC[6],FW_data.V_ID_MAC[7]);
            }
          break;
          case SAVE_DATA_SETT:
            {
              sprintf(mess,"Saving device settings SN:%d %d %d \n\r",FW_data.V_ID_MAC[5],FW_data.V_ID_MAC[6],FW_data.V_ID_MAC[7]);
            }
          break;
        }
}

uint32_t costr_pass(char* str1)
{
  uint32_t len_out;
  memset (str1,0, 200);
  memcpy(str1,(char*)(HTTP401),sizeof(HTTP401));
  strcat(str1,"Name_dev");
  strcat(str1,(char*)HTTP401end);
  len_out=strlen(str1);        
  return len_out;
}

uint16_t set_open_block (char* str1,char* name)
{ 

  sprintf(str1,"<%s>",name);
  return 0;
}
uint16_t reset_open_block (char* str1,char* name)
{ 
  
  sprintf(str1,"</%s>",name);
  return 0;
}
uint16_t set_space (char* str1,uint8_t n)
{ 
  uint16_t len=6*n;
  memset (str1,0, len);
  for(;n>0;n--)
   {
     strcat(str1,"&ensp;");
   }
  return len;
}

uint16_t set_br (char* str1,uint8_t n)
{ 
  uint16_t len=4*n;
  memset (str1,0, len);
  for(;n>0;n--)
   {
    strcat(str1,"<br>");
   }
return len;
}
uint16_t set_string_text  (char* str1,char* name,char* style, uint8_t n,uint8_t* dat)
  { 
    char str3[256];
    char str4[64];
    memset (str3,0, 256);
   
    for(;n>0;n--)
     {
      strcat(str3,"&ensp;");
     }
    sprintf(str4,"%d.%d.%d.%d",dat[0],dat[1],dat[2],dat[3]);
    sprintf(str1,"<%s> %s %s %s <%s/>",style,name,str3,str4,style);
    return 0;
  }

uint16_t set_text_area  (char* str1,char* name, uint8_t n,uint8_t c, uint8_t r,uint8_t* dat)
{ 
  sprintf(str1,"<textarea name=\"%s\" placeholder=\"%s\" cols=\"%d\" rows=\"%d\" maxlength=\"%d\" required></textarea> ",name,dat,c,r,n);
  return 0;
}
uint16_t set_text_input (char* str1,char* name, uint8_t n,uint8_t* dat)
{ 
 sprintf(str1,"<input size=\"%d\" name=\"%s\"  value=\"%s\" maxlength=\"%d\" >",n,name,dat,n);
 return 0;
}
uint16_t set_text_input_PW (char* str1,char* name, uint8_t n,uint8_t* dat)
{ 
  sprintf(str1,"<input type=\"password\" size=\"%d\" name=\"%s\"  value=\"%s\" maxlength=\"%d\" >",n,name,dat,n);
  return 0;
}

uint16_t set_intnum (char* str1,char* name,uint8_t size,char* value,int32_t min,int32_t  max)
{ 

 sprintf(str1,"<input type=\"number\" name=\"%s\"  size='%d' min=\"%d\" max=\"%d\" value=\"%s\">&ensp;",name,size,min,max,value);
 return 0;
}

uint16_t set_tab (char* str1,uint8_t n)
{ 
  uint16_t len=6*n;
  memset (str1,0, len);
  for(;n>0;n--)
   {
    strcat(str1,"&ensp;");
   }
  return len;
}


uint16_t set_link (char* str1,char* name,char* link)
{

 sprintf(str1,"<a href=\"%s\" target=\"_self\" rel=\"nofollow\">%s </a>",link,name);
 return 0;
}
uint16_t set_button (char* str1,char* name,char* value)
{ 
  sprintf(str1,"<button type=\"button\" name=\"%s\" formtarget=\"_self\" onclick=\"%s\">%s</button>\n\r",name,value,name);
  return 0;
}

uint16_t set_submit (char* str1,char* name,char* value,char* text,char* id)
{ 
  sprintf(str1,"<button type=\"submit\"  name=\"%s\" formtarget=\"_self\" value=\"%s\">%s</button>",name,value,text);
  return 0;
}
uint16_t set_checkbox (char* str1,char* name,char* value,char* id,uint8_t ck)
{ 
 if (ck==0)
  {
   sprintf(str1,"<input type=\"checkbox\"   name=\"%s\" id=\"%s\" value=\"%s\"> ",name,id,value);
  }
 else
  {
   sprintf(str1,"<input type=\"checkbox\"  name=\"%s\"  id=\"%s\" value=\"%s\" checked> ",name,id,value);
  }
return 0;
}

uint16_t set_checkonoff (char* str1,char* name,uint8_t ck)
 {  
  if (ck==1)
   {
    sprintf(str1,"<select name=\"%s\"><option selected >ON</option><option>OFF</option></select>",name);
   }
  else
   {
    sprintf(str1,"<select name=\"%s\"><option  >ON</option><option selected >OFF</option></select>",name);
   }
  return 0;
 }
 
 uint16_t set_menu3 (char* str1,char* mes1,char* mes2,char* mes3,char* name,uint8_t ck)
 {  
   if (ck==0)
   {
     sprintf(str1,"<select  name=\"%s\"><option selected >%s</option><option >%s</option><option>%s</option></select>",name,mes1,mes2,mes3);
   }
   if (ck==1)
   {
     sprintf(str1,"<select   name=\"%s\"><option >%s</option><option selected >%s</option><option>%s</option></select>",name,mes1,mes2,mes3);
   }
   if (ck==2)
   {
     sprintf(str1,"<select   name=\"%s\"><option  >%s</option><option >%s</option><option selected >%s</option></select>",name,mes1,mes2,mes3);
   }
   return 0;
 }

uint16_t set_menu10 (char* str1,char* name,uint8_t ck)
 {  
   if (ck==RTC_WEEKDAY_MONDAY)
    {
     sprintf(str1,"<select  name=\"%s\"><option selected >ПН</option>"
              "<option>ВТ</option>"
               "<option>СР</option>"
               "<option>ЧТ</option>"
               "<option>ПТ</option>"
               "<option>СБ</option>"
               "<option>ВС</option>"
               "<option>П1</option>"
               "<option>П2</option>"
               "<option>П3</option>"
               "</select>",name);
    }
   else
    {
     if (ck==RTC_WEEKDAY_TUESDAY)
    {
     sprintf(str1,"<select  name=\"%s\"><option  >ПН</option>"
               "<option selected>ВТ</option>"
               "<option>СР</option>"
               "<option>ЧТ</option>"
               "<option>ПТ</option>"
               "<option>СБ</option>"
               "<option>ВС</option>"
               "<option>П1</option>"
               "<option>П2</option>"
               "<option>П3</option>"
               "</select>",name);
    }
   else
    {
      if (ck==RTC_WEEKDAY_WEDNESDAY)
    {
   sprintf(str1,"<select  name=\"%s\"><option  >ПН</option>"
               "<option>ВТ</option>"
               "<option selected>СР</option>"
               "<option>ЧТ</option>"
               "<option>ПТ</option>"
               "<option>СБ</option>"
               "<option>ВС</option>"
               "<option>П1</option>"
               "<option>П2</option>"
               "<option>П3</option>"
               "</select>",name);
   }
  else
   {
    if (ck==RTC_WEEKDAY_THURSDAY)
     {
      sprintf(str1,"<select  name=\"%s\"><option  >ПН</option>"
               "<option>ВТ</option>"
               "<option>СР</option>"
               "<option selected>ЧТ</option>"
               "<option>ПТ</option>"
               "<option>СБ</option>"
               "<option>ВС</option>"
               "<option>П1</option>"
               "<option>П2</option>"
               "<option>П3</option>"
               "</select>",name);
     }
    else
     {
       if (ck==RTC_WEEKDAY_FRIDAY)
        {
         sprintf(str1,"<select  name=\"%s\"><option  >ПН</option>"
               "<option>ВТ</option>"
               "<option>СР</option>"
               "<option>ЧТ</option>"
               "<option selected>ПТ</option>"
               "<option>СБ</option>"
               "<option>ВС</option>"
               "<option>П1</option>"
               "<option>П2</option>"
               "<option>П3</option>"
               "</select>",name);
       }
      else
       {
        if (ck==RTC_WEEKDAY_SATURDAY)
         {
          sprintf(str1,"<select  name=\"%s\"><option  >ПН</option>"
               "<option>ВТ</option>"
               "<option>СР</option>"
               "<option>ЧТ</option>"
               "<option>ПТ</option>"
               "<option selected>СБ</option>"
               "<option>ВС</option>"
               "<option>П1</option>"
               "<option>П2</option>"
               "<option>П3</option>"
               "</select>",name);
         }
        else
         {
           if (ck==RTC_WEEKDAY_SUNDAY)
            {
             sprintf(str1,"<select  name=\"%s\"><option  >ПН</option>"
               "<option>ВТ</option>"
               "<option>СР</option>"
               "<option>ЧТ</option>"
               "<option>ПТ</option>"
               "<option>СБ</option>"
               "<option selected>ВС</option>"
               "<option>П1</option>"
               "<option>П2</option>"
               "<option>П3</option>"
               "</select>",name);
            }
           else
            {
             if (ck==7)
              {
               sprintf(str1,"<select  name=\"%s\"><option  >ПН</option>"
               "<option>ВТ</option>"
               "<option>СР</option>"
               "<option>ЧТ</option>"
               "<option>ПТ</option>"
               "<option>СБ</option>"
               "<option>ВС</option>"
               "<option selected>П1</option>"
               "<option>П2</option>"
               "<option>П3</option>"
               "</select>",name);
              }
             else
              { 
               if (ck==8)
                {
                 sprintf(str1,"<select  name=\"%s\"><option  >ПН</option>"
                 "<option>ВТ</option>"
                 "<option>СР</option>"
                 "<option>ЧТ</option>"
                 "<option>ПТ</option>"
                 "<option>СБ</option>"
                 "<option>ВС</option>"
                 "<option>П1</option>"
                 "<option selected>П2</option>"
                 "<option>П3</option>"
                 "</select>",name);
                }
               else
                {
                 if (ck==9)
                  {
                   sprintf(str1,"<select  name=\"%s\"><option >ПН</option>"
                   "<option>ВТ</option>"
                   "<option>СР</option>"
                   "<option>ЧТ</option>"
                   "<option>ПТ</option>"
                   "<option>СБ</option>"
                   "<option>ВС</option>"
                   "<option>П1</option>"
                   "<option>П2</option>"
                   "<option selected>П3</option>"
                   "</select>",name);
                  }
                 else
                  {
                    sprintf(str1,"<select  name=\"%s\"><option selected >ПН</option>"
                    "<option>ВТ</option>"
                    "<option>СР</option>"
                    "<option>ЧТ</option>"
                    "<option>ПТ</option>"
                    "<option>СБ</option>"
                    "<option>ВС</option>"
                    "<option>П1</option>"
                    "<option>П2</option>"
                    "<option>П3</option>"
                    "</select>",name);
                  }
                 }
                }
               }
              }
             }
            }
           }
          }
         }
   return 0;
 }

 uint16_t set_checkbox_bot (char* str1,char* name,char* value,char* mess,char* id)
{ 
  sprintf(str1,"<p style=\"text-align:center\"><input name=\"%s\" required=\"required\"    type=\"checkbox\" value=\"%s\" />&ensp; <input type=\"submit\" value=\"%s\"  /></p>",name,value,mess);
  return 0;
}



uint16_t set_intext (char* str1,char* name,char* size,char* value)
{ 

 sprintf(str1,"<input type=\"text\" name=\"%s\"  size='%s' value=\"%s\">&ensp;",name,size,value);
 return 0;
}

uint16_t set_table_string (char* str1,char* name,char* value)
{ 
 
 sprintf(str1,"<tr><td style=\"width: 80%; text-align: left ;border-bottom: 1px solid #36BA88; background: #BFE8BB;  \">&ensp;\%s</td><td style=\"width: 20%; font-size: 10pt; text-align: center;border-bottom: 1px solid #36BA88; background: #BFE8BB; \">%s</td></tr>",name,value);
 return 0;
}


uint16_t set_table_rasp_zagl (char* str1,char* str2,char* name)
{ 

 sprintf(str2,"<b><tr><td style=\"width: 3%; text-align: left ;border-bottom: 1px solid #36BA88; background: #BFE8BB; \">&ensp;\%s" ,name);
 strcat(str1,str2);
 
 sprintf(str2,"<td style=\"width: 3%; text-align: right ;border-bottom: 1px solid #36BA88; background: #BFE8BB; \">&ensp;\%s","Тайм");
 strcat(str1,str2);  
 
 sprintf(str2,"<td style=\"width: 3%; text-align: left ;border-bottom: 1px solid #36BA88; background: #BFE8BB; \">&ensp;\%s","слот №1");
 strcat(str1,str2);  

    
sprintf(str2,"<td style=\"width: 3%; text-align: right ;border-bottom: 1px solid #36BA88; background: #BFE8BB; \">&ensp;\%s","Тайм");
strcat(str1,str2);  
        


sprintf(str2,"<td style=\"width: 3%; text-align: left ;border-bottom: 1px solid #36BA88; background: #BFE8BB; \">&ensp;\%s","слот №2");
strcat(str1,str2); 

    
sprintf(str2,"<td style=\"width: 3%; text-align: right ;border-bottom: 1px solid #36BA88; background: #BFE8BB; \">&ensp;\%s","Тайм");
strcat(str1,str2);  
        


sprintf(str2,"<td style=\"width: 3%; text-align: left ;border-bottom: 1px solid #36BA88; background: #BFE8BB; \">&ensp;\%s","слот №3");
strcat(str1,str2); 


sprintf(str2,"</td><td style=\"width: 3%; font-size: 10pt; text-align: center;border-bottom: 1px solid #36BA88; background: #BFE8BB; \">%s</td></tr></b>" ,"Как в пред. день нед.");
strcat(str1,str2);

return 0;

}

uint16_t set_table_rasp_zag2 (char* str1,char* str2,char* name)
{ 


sprintf(str2,"<b><tr><td style=\"width: 3%; text-align: left ;border-bottom: 1px solid #36BA88; background: #BFE8BB; \">&ensp;\%s" ,name);
strcat(str1,str2);


    
sprintf(str2,"<td style=\"width: 3%; text-align: right ;border-bottom: 1px solid #36BA88; background: #BFE8BB; \">&ensp;\%s","Тайм");
strcat(str1,str2);  
        


sprintf(str2,"<td style=\"width: 3%; text-align: left ;border-bottom: 1px solid #36BA88; background: #BFE8BB; \">&ensp;\%s","слот №4");
strcat(str1,str2);  

    
sprintf(str2,"<td style=\"width: 3%; text-align: right ;border-bottom: 1px solid #36BA88; background: #BFE8BB; \">&ensp;\%s","Тайм");
strcat(str1,str2);  
        


sprintf(str2,"<td style=\"width: 3%; text-align: left ;border-bottom: 1px solid #36BA88; background: #BFE8BB; \">&ensp;\%s","слот №5");
strcat(str1,str2); 

    
sprintf(str2,"<td style=\"width: 3%; text-align: right ;border-bottom: 1px solid #36BA88; background: #BFE8BB; \">&ensp;\%s","Тайм");
strcat(str1,str2);  
        


sprintf(str2,"<td style=\"width: 3%; text-align: left ;border-bottom: 1px solid #36BA88; background: #BFE8BB; \">&ensp;\%s","слот №6");
strcat(str1,str2); 


sprintf(str2,"</td><td style=\"width: 3%; font-size: 10pt; text-align: center;border-bottom: 1px solid #36BA88; background: #BFE8BB; \">%s</td></tr></b>" ,"Как в пред. день нед.");
strcat(str1,str2);
return 0;

}


char data_time[3]={0};
char* dtimeh (uint8_t m )
{   data_time[0]=0;
    data_time[1]=0;
    data_time[2]=0;
    data_time[0]=(m/10)+0x30;
    if (data_time[0]>0x39)
    {
      data_time[0]=0x39;
    }
    data_time[1]=(m-(data_time[0]-0x30)*10)+0x30;
    if (data_time[1]>0x39)
    {
      data_time[1]=0x39;
    }
    data_time[2]=0;
    return data_time;
  }


char data_time1[3]={0};
char* dtime  (uint8_t m )
{   data_time1[0]=0;
    data_time1[1]=0;
    data_time1[2]=0;
    data_time1[0]=(m/10)+0x30;
    if (data_time1[0]>0x39)
    {
      data_time1[0]=0x39;
    }
    data_time1[1]=(m-(data_time1[0]-0x30)*10)+0x30;
    if (data_time1[1]>0x39)
    {
      data_time1[1]=0x39;
    }
    data_time1[2]=0;
    return data_time1;
  }
uint16_t set_table_rasp_str_1 (char* str1,char* str2,char* name,char* val_name,char* value,V_D_TIME_type* time,uint8_t N)
{ 

char str_data[250];
uint16_t len;
sprintf(str2,"<tr><td style=\"width: 3%; text-align: left ;border-bottom: 1px solid #36BA88; background: #BFE8BB; \">&ensp;\%s" ,name);
strcat(str1,str2);



sprintf(str_data,"<input type=\"time\" name=\"%s%d%d%d\"  value=\"%s:%s\">",val_name,0,N,3,dtimeh(time[N].time_data[3].on_swich_h),dtime(time[N].time_data[3].on_swich_m));        
sprintf(str2,"<td style=\"width: 3%; text-align: right ;border-bottom: 1px solid #36BA88; background: #BFE8BB; \">&ensp;\Включение-%s",str_data);
strcat(str1,str2);  
        

sprintf(str_data,"<input type=\"time\" name=\"%s%d%d%d\"  value=\"%s:%s\">",val_name,1,N,3,dtimeh(time[N].time_data[3].off_swich_h),dtime(time[N].time_data[3].off_swich_m));        
sprintf(str2,"<td style=\"width: 3%; text-align: left ;border-bottom: 1px solid #36BA88; background: #BFE8BB; \">&ensp;\%s-Выключение",str_data);
strcat(str1,str2); 



sprintf(str_data,"<input type=\"time\" name=\"%s%d%d%d\" value=\"%s:%s\">",val_name,0,N,4,dtimeh(time[N].time_data[4].on_swich_h),dtime(time[N].time_data[4].on_swich_m));        
sprintf(str2,"<td style=\"width: 3%; text-align: right ;border-bottom: 1px solid #36BA88; background: #BFE8BB; \">&ensp;\Включение-%s",str_data);
strcat(str1,str2);  
        

sprintf(str_data,"<input type=\"time\" name=\"%s%d%d%d\" value=\"%s:%s\">",val_name,1,N,4,dtimeh(time[N].time_data[4].off_swich_h),dtime(time[N].time_data[4].off_swich_m));        
sprintf(str2,"<td style=\"width: 3%; text-align: left ;border-bottom: 1px solid #36BA88; background: #BFE8BB; \">&ensp;\%s-Выключение",str_data);
strcat(str1,str2); 



sprintf(str_data,"<input type=\"time\" name=\"%s%d%d%d\" value=\"%s:%s\">",val_name,0,N,5,dtimeh(time[N].time_data[5].on_swich_h),dtime(time[N].time_data[5].on_swich_m));        
sprintf(str2,"<td style=\"width: 3%; text-align: right ;border-bottom: 1px solid #36BA88; background: #BFE8BB; \">&ensp;\Включение-%s",str_data);
strcat(str1,str2);  
        

sprintf(str_data,"<input type=\"time\" name=\"%s%d%d%d\" value=\"%s:%s\">",val_name,1,N,5,dtimeh(time[N].time_data[5].off_swich_h),dtime(time[N].time_data[5].off_swich_m));        
sprintf(str2,"<td style=\"width: 3%; text-align: left ;border-bottom: 1px solid #36BA88; background: #BFE8BB; \">&ensp;\%s-Выключение",str_data);
strcat(str1,str2); 


sprintf(str2,"</td><td style=\"width: 3%; font-size: 10pt; text-align: center;border-bottom: 1px solid #36BA88; background: #BFE8BB; \">%s</td></tr>" ,value);
strcat(str1,str2);

return len;

}

uint16_t set_table_rasp_str (char* str1,char* str2,char* name,char* val_name,char* value,V_D_TIME_type* time,uint8_t N)
{ 

char str_data[250];

sprintf(str2,"<tr><td style=\"width: 3%; text-align: left ;border-bottom: 1px solid #36BA88; background: #BFE8BB; \">&ensp;\%s" ,name);
strcat(str1,str2);


sprintf(str_data,"<input type=\"time\" name=\"%s%d%d%d\" value=\"%s:%s\">",val_name,0,N,0,dtimeh(time[N].time_data[0].on_swich_h),dtime(time[N].time_data[0].on_swich_m));        

sprintf(str2,"<td style=\"width: 3%; text-align: right ;border-bottom: 1px solid #36BA88; background: #BFE8BB; \">&ensp;\Включение-%s",str_data);
strcat(str1,str2);  
        

sprintf(str_data,"<input type=\"time\" name=\"%s%d%d%d\" value=\"%s:%s\">",val_name,1,N,0,dtimeh(time[N].time_data[0].off_swich_h),dtime(time[N].time_data[0].off_swich_m));        
sprintf(str2,"<td style=\"width: 3%; text-align: left ;border-bottom: 1px solid #36BA88; background: #BFE8BB; \">&ensp;\%s-Выключение",str_data);
strcat(str1,str2);  



sprintf(str_data,"<input type=\"time\" name=\"%s%d%d%d\" value=\"%s:%s\">",val_name,0,N,1,dtimeh(time[N].time_data[1].on_swich_h),dtime(time[N].time_data[1].on_swich_m));        
sprintf(str2,"<td style=\"width: 3%; text-align: right ;border-bottom: 1px solid #36BA88; background: #BFE8BB; \">&ensp;\Включение-%s",str_data);
strcat(str1,str2);  
        

sprintf(str_data,"<input type=\"time\" name=\"%s%d%d%d\" value=\"%s:%s\">",val_name,1,N,1,dtimeh(time[N].time_data[1].off_swich_h),dtime(time[N].time_data[1].off_swich_m));        
sprintf(str2,"<td style=\"width: 3%; text-align: left ;border-bottom: 1px solid #36BA88; background: #BFE8BB; \">&ensp;\%s-Выключение",str_data);
strcat(str1,str2); 



sprintf(str_data,"<input type=\"time\" name=\"%s%d%d%d\" value=\"%s:%s\">",val_name,0,N,2,dtimeh(time[N].time_data[2].on_swich_h),dtime(time[N].time_data[2].on_swich_m));        
sprintf(str2,"<td style=\"width: 3%; text-align: right ;border-bottom: 1px solid #36BA88; background: #BFE8BB; \">&ensp;\Включение-%s",str_data);
strcat(str1,str2);  
        

sprintf(str_data,"<input type=\"time\" name=\"%s%d%d%d\" value=\"%s:%s\">",val_name,1,N,2,dtimeh(time[N].time_data[2].off_swich_h),dtime(time[N].time_data[2].off_swich_m));        
sprintf(str2,"<td style=\"width: 3%; text-align: left ;border-bottom: 1px solid #36BA88; background: #BFE8BB; \">&ensp;\%s-Выключение",str_data);
strcat(str1,str2); 



sprintf(str2,"</td><td style=\"width: 3%; font-size: 10pt; text-align: center;border-bottom: 1px solid #36BA88; background: #BFE8BB; \">%s</td></tr>" ,value);
strcat(str1,str2);

return 0;

}


uint16_t set_table_podm_day_str1 (char* str1,char* str2,char* name,char* val_name,V_RD_DATA_type* time)
{ 

  char str_data[300]={0};
  char str_data1[20]={0};
uint8_t i;
uint16_t len;
sprintf(str2,"<tr><td style=\"width: 3%; text-align: left ;  border-bottom: 1px solid #36BA88; background: #BFE8BB; \">&ensp;\%s" ,name);
strcat(str1,str2);

for (i=0;i<5;i++)
{
  
sprintf(str_data1,"%s%d" ,val_name,i);
set_menu10(str_data,str_data1,time->restore_day[i]);
sprintf(str2,"<td style=\"width: 3%; text-align: center ;border-bottom: 1px solid #36BA88; background: #BFE8BB; \">&ensp;\%s",str_data);
strcat(str1,str2);  
}       

len=strlen(str1);

return len;

}


uint16_t set_table_podm_day_str2 (char* str1,char* str2,char* name,char* val_name,V_RD_DATA_type* time)
{ 

  char str_data[300]={0};
  char str_data1[20]={0};
uint8_t i;
uint16_t len;


for (i=5;i<10;i++)
{
  
sprintf(str_data1,"%s%d" ,val_name,i);
set_menu10(str_data,str_data1,time->restore_day[i]);
sprintf(str2,"<td style=\"width: 3%; text-align: center ;border-bottom: 1px solid #36BA88; background: #BFE8BB; \">&ensp;\%s",str_data);
strcat(str1,str2);  
}       



sprintf(str2,"</tr>");
strcat(str1,str2);

return len;

}





uint16_t set_table_podm_str (char* str1,char* str2,char* name,char* val_name,char* value,V_RD_DATA_type* time,uint8_t N)
{ 

char str_data[150];
uint8_t i;
uint16_t len;
uint16_t year,month,day;
sprintf(str2,"<tr><td style=\"width: 3%; text-align: left ;border-bottom: 1px solid #36BA88; background: #BFE8BB; \">&ensp;\%s" ,name);
strcat(str1,str2);

for (i=0;i<10;i++)
{
  year=time->data[i].year+2000;
  month=time->data[i].month+1;
  day=time->data[i].day+1;
  
  sprintf(str_data,"<input type=\"date\" name=\"%s%d%d\" value=\"%d-%s-%s\">",val_name,N,i,year,dtimeh(month),dtime(day));        

sprintf(str2,"<td style=\"width: 3%; text-align: center ;border-bottom: 1px solid #36BA88; background: #BFE8BB; \">&ensp;\%s",str_data);
strcat(str1,str2);  
}       



sprintf(str2,"</td><td style=\"width: 3%; font-size: 10pt; text-align: center;border-bottom: 1px solid #36BA88; background: #BFE8BB; \">%s</td></tr>" ,value);
strcat(str1,str2);

return len;

}




uint32_t costr_page_hdr(char* str1,uint32_t len_data)
{
  uint32_t len;
    memset (str1,0, sizeof(str1)); 
 
   
    memcpy((uint8_t *)str1,(uint8_t*)http_html_200,120);
    len=95;
    if (len_data>9999)
   {
    str1[len+1]=(len_data/10000);
    str1[len+2]=(len_data/1000)-str1[len+1]*10;
    str1[len+3]=(len_data/100)-str1[len+1]*100-str1[len+2]*10;
    str1[len+4]=(len_data/10)-str1[len+1]*1000-str1[len+2]*100-str1[len+3]*10;
    str1[len+5]=(len_data)-str1[len+1]*10000-str1[len+2]*1000-str1[len+3]*100-str1[len+4]*10;
    str1[len+1]=str1[len+1]+0x30;
    str1[len+2]=str1[len+2]+0x30;
    str1[len+3]=str1[len+3]+0x30;
    str1[len+4]=str1[len+4]+0x30;
    str1[len+5]=str1[len+5]+0x30;    
    memcpy((uint8_t *)(str1+101),(uint8_t*)http_html_200_end,47);
    len=149;
   }
   else
   {
    str1[len+1]=(len_data/1000);
    str1[len+2]=(len_data/100)-str1[len+1]*10;
    str1[len+3]=(len_data/10)-str1[len+1]*100-str1[len+2]*10;
    str1[len+4]=(len_data)-str1[len+1]*1000-str1[len+2]*100-str1[len+3]*10;    
    str1[len+1]=str1[len+1]+0x30;
    str1[len+2]=str1[len+2]+0x30;
    str1[len+3]=str1[len+3]+0x30;
    str1[len+4]=str1[len+4]+0x30;    
    memcpy((uint8_t *)(str1+100),(uint8_t*)http_html_200_end,47); 
    len=148;
    str1[len]=0;
    str1[len+1]=0;
   }
    
   len = strlen (str1);
  return len;
};
   



uint32_t costr_page2(char* str1)
{
  uint32_t len;
  


  memset (str1,0, sizeof(str1)); 
  memset (str2,0, sizeof(str2));
  memset (str3,0, sizeof(str3));
  memset (str4,0, sizeof(str4));
  
  
  strcat(str1,http_html_start_constr);
  

  strcat(str1,http_html_style);
  
  set_open_block(str2,"body");
  strcat(str1,str2);
  

  
 
  set_string_text(str2,"Интеллектуальная розетка DKST 59 v","h1",0,FW_data.V_FW1_VER);
  strcat(str1,str2);
    
  set_br(str2,1);
  strcat(str1,str2);
  
  
  set_open_block(str2,"h2");
  strcat(str1,str2);
  
  
  set_link(str2,"Главная","index.html");
  strcat(str1,str2);
  
  set_link(str2,"Настройки ","settings.html");
  strcat(str1,str2);
  
  set_link(str2,"Расписание ","rasp.html");
  strcat(str1,str2);
  
  set_link(str2,"Сторож","watchdog.html");
  strcat(str1,str2);
  
  
  set_link(str2,"E-mail","email.html");
  strcat(str1,str2);  

  
  set_link(str2,"Журнал","logs.html");
  strcat(str1,str2);  
    
  reset_open_block(str2,"h2");
  strcat(str1,str2);

 
  
  set_open_block(str2,"form action='/' method='POST'");
  strcat(str1,str2);
  
  set_open_block(str2,"ul");
  strcat(str1,str2);
  
   set_open_block(str2,"h2");
  strcat(str1,str2);
  
  set_br(str2,3);
  strcat(str1,str2);
         

  set_open_block(str2,"b>ОБЩАЯ ИНФОРМАЦИЯ</b");
  strcat(str1,str2);
  
  reset_open_block(str2,"h2");
  strcat(str1,str2);
  
  
  set_br(str2,2);
  strcat(str1,str2);
  
  set_open_block(str2,"h3");
  strcat(str1,str2);

 // Table   
   set_open_block(str2,"table border=\"1\" style=\"border-collapse: collapse; width: 80%;border: 1px solid #ffffff;margin: auto;\"><tbody");
   strcat(str1,str2);    

   set_table_string(str2,"Имя устройства",FW_data.V_Name_dev);
   strcat(str1,str2);
   
   set_table_string(str2,"Место установки устройства",FW_data.V_GEOM_NAME);  
   strcat(str1,str2);
   
   set_table_string(str2,"Контактные данные",FW_data.V_CALL_DATA);  
   strcat(str1,str2);
   
   
     len=strlen(str1);
     while(len>3000){}
  return len;
};
   
   
uint32_t costr_page2_1(char* str1)
{
  uint32_t len;
  memset (str1,0, sizeof(str1)); 
  memset (str2,0, sizeof(str2));
  memset (str3,0, sizeof(str3));
  memset (str4,0, sizeof(str4));
   
   memset(str3,0,128);
   sprintf(str3,"SN:%d %d %d",FW_data.V_ID_MAC[5],FW_data.V_ID_MAC[6],FW_data.V_ID_MAC[7]);
   
   set_table_string(str2,"Серийный номер",str3);  
   strcat(str1,str2);
   
   memset(str3,0,128);
   sprintf(str3,"%x:%x:%x:%x:%x:%x",FW_data.V_ID_MAC[0],FW_data.V_ID_MAC[1],FW_data.V_ID_MAC[2],FW_data.V_ID_MAC[3],FW_data.V_ID_MAC[4],FW_data.V_ID_MAC[5]);
   
   set_table_string(str2,"МАС адрес",str3);  
   strcat(str1,str2);
   
   set_table_string(str2,"Модель устройства ",(char*)model_dev);  
   strcat(str1,str2);
   
   memset(str3,0,128);
   sprintf(str3,"v%d.%d",FW_data.V_FW1_VER[2],FW_data.V_FW1_VER[3]);
 
   set_table_string(str2,"Версия ПО",str3);  
   strcat(str1,str2);
   
   memset(str3,0,128);
   sprintf(str3,"v%d.%d",FW_data.V_FW1_VER[0],FW_data.V_FW1_VER[1]);
 
   set_table_string(str2,"Версия аппаратной платформы",str3);  
   strcat(str1,str2);
   
   
   
   signed char time_run[6]={0};  
   uint8_t ct_time;
   GET_reple(0,&real_time);  
   

   
   time_run[0]=real_time.year-start_time.year;
   time_run[1]=real_time.month-start_time.month;
   time_run[2]=real_time.day-start_time.day;
   time_run[3]=real_time.reple_hours-start_time.reple_hours;
   time_run[4]=real_time.reple_minuts-start_time.reple_minuts;
   time_run[5]=real_time.reple_seconds-start_time.reple_seconds;
   
   ct_time=1;
      if (time_run[ct_time]<0)
        {
          time_run[ct_time-1]--;
          time_run[ct_time]=time_run[ct_time]+12;          
        }
      
      
   ct_time++;
      if (time_run[ct_time]<0)
        {
          time_run[ct_time-1]--;
          time_run[ct_time]=time_run[ct_time]+30;          
        }
      
      
   ct_time++;
      if (time_run[ct_time]<0)
        {
          time_run[ct_time-1]--;
          time_run[ct_time]=time_run[ct_time]+24;          
        }
      
      
      
   ct_time++;
      if (time_run[ct_time]<0)
        {
          time_run[ct_time-1]--;
          time_run[ct_time]=time_run[ct_time]+60;          
        }
      
      
   ct_time++;
      if (time_run[ct_time]<0)
        {
          time_run[ct_time-1]--;
          time_run[ct_time]=time_run[ct_time]+60;          
        }
   
    memset(str3,0,128);
   sprintf(str3,"%dг.  %dм. %dд. %dч. %dм. %dс.",time_run[0],time_run[1],time_run[2],time_run[3],time_run[4],time_run[5]);
      
   set_table_string(str2,"Время непрерывной работы",str3);  
   strcat(str1,str2);
  
   
   
  reset_open_block(str2,"h3");
  strcat(str1,str2);
  
  reset_open_block(str2,"tbody");
  strcat(str1,str2);
  
  reset_open_block(str2,"table");
  strcat(str1,str2);

  
  
  len=strlen(str1);
  while(len>3000){}
  return len;
};



uint32_t costr_page4(char* str1)
{
  uint32_t len;
  uint16_t data;
  
  memset (str1,0, sizeof(str1)); 
  memset (str2,0, sizeof(str2));
  memset (str3,0, sizeof(str3));
  memset (str4,0, sizeof(str4));
  
  
  set_open_block(str2,"h2");
  strcat(str1,str2);
  
  set_br(str2,1);
  strcat(str1,str2);
  
  set_open_block(str2,"b>НАСТРОЙКА  СЕТИ</b");
  strcat(str1,str2);
  
  
    reset_open_block(str2,"h2");
  strcat(str1,str2);
    


    set_open_block(str2,"h3");
  strcat(str1,str2);
  
 // Table   
  set_open_block(str2,"table border=\"1\" style=\"border-collapse: collapse; width: 80%;border: 1px solid #ffffff;margin: auto;\"><tbody");
  strcat(str1,str2);
    
   sprintf(str3,"%d\.%d\.%d\.%d",FW_data.V_IP_CONFIG[0],FW_data.V_IP_CONFIG[1],FW_data.V_IP_CONFIG[2],FW_data.V_IP_CONFIG[3]);

   set_table_string(str2,"IP адрес устройства",str3);
   strcat(str1,str2);
   
   sprintf(str3,"%d\.%d\.%d\.%d",FW_data.V_IP_MASK[0],FW_data.V_IP_MASK[1],FW_data.V_IP_MASK[2],FW_data.V_IP_MASK[3]);

   set_table_string(str2,"Маска подсети",str3);  
   strcat(str1,str2);
   
   sprintf(str3,"%d\.%d\.%d\.%d",FW_data.V_IP_GET[0],FW_data.V_IP_GET[1],FW_data.V_IP_GET[2],FW_data.V_IP_GET[3]);

   set_table_string(str2,"Шлюз",str3);  
   strcat(str1,str2);
    
   
   reset_open_block(str2,"h3");
  strcat(str1,str2);
    
    
  reset_open_block(str2,"tbody");
  strcat(str1,str2);
  
  reset_open_block(str2,"table");
  strcat(str1,str2);
  
  // End table 

  
  reset_open_block(str2,"ul");
  strcat(str1,str2);
  
  reset_open_block(str2,"form");
  strcat(str1,str2);
  
  
  set_br(str2,1);
  strcat(str1,str2);
  
  
  set_open_block(str2,"h2");
  strcat(str1,str2);
 
  set_open_block(str2,"b>УПРАВЛЕНИЯ НАГРУЗКОЙ</b");
  strcat(str1,str2);
  
  
  reset_open_block(str2,"h2");
  strcat(str1,str2);


  
    set_open_block(str2,"h3");
  strcat(str1,str2);
  
  
   set_open_block(str2,"form id=\"osf\" action='/' method='POST'");
  strcat(str1,str2);

  
  // Table   
    
 
  
  
   set_open_block(str2,"table border=\"1\" style=\"border-collapse: collapse; width: 78%; border: 1px solid #ffffff;margin: 0 8% 0 12%;\"><tbody");
   strcat(str1,str2);      
   data= HAL_RTCEx_BKUPRead(&hrtc,1);
  if (data==0)
  {
   sprintf(str3,"%s",FW_data.V_OFF_MESS);
  }
   if ((data==1)||(data==2))
  {
   sprintf(str3,"%s",FW_data.V_ON_MESS);
  }
   
   set_table_string(str2,"Текущее состояние ",str3);  
   strcat(str1,str2);
   
   
   set_open_block(str3,"input name=\"output_set\"  type=\"radio\" value=\"1\"checked");
   set_table_string(str2,"Включить ключ",str3);  
   strcat(str1,str2);

   set_open_block(str3,"input name=\"output_set\" type=\"radio\" value=\"0\"");
   set_table_string(str2,"Выключить ключ",str3);  
   strcat(str1,str2);
   
   set_open_block(str3,"input name=\"output_set\"  type=\"radio\" value=\"2\"");
   set_table_string(str2,"Выдать импульс",str3);  
   strcat(str1,str2);   
  
 
   
   set_submit(str3,"out_swich","1","Переключить","osf");
   set_table_string(str2,"Переключение выхода",str3);  
   strcat(str1,str2);
   
    reset_open_block(str2,"h3");
   strcat(str1,str2);
   

  

  reset_open_block(str2,"tbody"); 
  strcat(str1,str2); 
  
  reset_open_block(str2,"table");
  strcat(str1,str2);
  
  reset_open_block(str2,"form");
  strcat(str1,str2);

  
    len=strlen(str1);
       while(len>3000){}
  return len;
};
  

uint32_t costr_page4_1(char* str1)
{
  uint32_t len;

  
  memset (str1,0, sizeof(str1)); 
  memset (str2,0, sizeof(str2));
  memset (str3,0, sizeof(str3));
  memset (str4,0, sizeof(str4));
    
  set_open_block(str2,"form id=\"rsf\" action='/' method='POST'");
  strcat(str1,str2);
  
  set_open_block(str2,"ul");
  strcat(str1,str2);
  
   set_br(str2,2);
  strcat(str1,str2);
  
   set_open_block(str2,"h2");
  strcat(str1,str2);
  
  set_open_block(str2,"b>Выполнить перезагрузку</b");
  strcat(str1,str2);
  
  reset_open_block(str2,"h2");
  strcat(str1,str2);
  


  
  set_checkbox_bot(str2,"check_res","1","RESET","rsf");
  strcat(str1,str2); 

  
  set_space(str2,6);
  strcat(str1,str2); 
  
  reset_open_block(str2,"ul");
  strcat(str1,str2);
  
    reset_open_block(str2,"form");
  strcat(str1,str2);
  

  reset_open_block(str2,"html");
  strcat(str1,str2);
  
  
  
  len=strlen(str1);
       while(len>3000){}
  return len;
};


uint32_t costr_page3(char* str1)
{
  uint32_t len;
  
  memset (str1,0, sizeof(str1)); 
  memset (str2,0, sizeof(str2));
  memset (str3,0, sizeof(str3));
  memset (str4,0, sizeof(str4));

  strcat(str1,http_html_start_constr);
  

  strcat(str1,http_html_style);
  
  set_open_block(str2,"body");
  strcat(str1,str2);
  
  set_open_block(str2,"ul");
  strcat(str1,str2);
  
  

  
 
  set_string_text(str2,"Интеллектуальная розетка DKST 59 v","h1",0,FW_data.V_FW1_VER);
  strcat(str1,str2);

  
      set_br(str2,1);
  strcat(str1,str2);
  
  set_open_block(str2,"h2");
  strcat(str1,str2);
  
  
  set_link(str2,"Главная","index.html");
  strcat(str1,str2);
  
  set_link(str2,"Настройки ","settings.html");
  strcat(str1,str2);
  
  set_link(str2,"Расписание ","rasp.html");
  strcat(str1,str2);
  
  set_link(str2,"Сторож","watchdog.html");
  strcat(str1,str2);
  
  
  set_link(str2,"E-mail","email.html");
  strcat(str1,str2);  
  
  set_link(str2,"Журнал ","logs.html");
  strcat(str1,str2);  
    
  reset_open_block(str2,"h2");
  strcat(str1,str2);

  reset_open_block(str2,"ul");
  strcat(str1,str2);

  
  
  
  set_open_block(str2,"form id=\"sets\" action='/' method='POST'");
  strcat(str1,str2);
  
  set_open_block(str2,"h2");
  strcat(str1,str2);
  
  
  set_br(str2,2);
  strcat(str1,str2);

  set_open_block(str2,"b>ОБЩАЯ ИНФОРМАЦИЯ</b");
  strcat(str1,str2);
  
  set_br(str2,1);
  strcat(str1,str2);
  
  reset_open_block(str2,"h2");
  strcat(str1,str2);
  
  set_open_block(str2,"h3");
  strcat(str1,str2);
  // Table   
   set_open_block(str2,"table border=\"1\" style=\"border-collapse: collapse; width: 80%;border: 1px solid #ffffff;margin: auto;required\"><tbody");
   strcat(str1,str2);    

 
   
     
   set_text_input(str3,"name_dev",16,FW_data.V_Name_dev);  
   set_table_string(str2,"Имя устройства",str3);
   strcat(str1,str2);
   
   set_text_input(str3,"geo_place",16,FW_data.V_GEOM_NAME);  
   set_table_string(str2,"Место установки устройства",str3);  
   strcat(str1,str2);
   
   set_text_input(str3,"call_data",16,FW_data.V_CALL_DATA);  
   set_table_string(str2,"Контактные данные",str3);  
   strcat(str1,str2);
   

  set_open_block(str2,"h3");
  strcat(str1,str2);
   
 
   reset_open_block(str2,"tbody");
   strcat(str1,str2);
  
   reset_open_block(str2,"table");
   strcat(str1,str2);
   

  
  
  
 
  reset_open_block(str2,"tbody"); 
  strcat(str1,str2); 
  
  reset_open_block(str2,"table");
  strcat(str1,str2);
  



  
  len=strlen(str1);
       while(len>3000){}
  return len;
};



uint32_t costr_page5(char* str1)
{
  uint32_t len;
  
  memset (str1,0, sizeof(str1)); 
  memset (str2,0, sizeof(str2));
  memset (str3,0, sizeof(str3));
  memset (str4,0, sizeof(str4));


  
  
  set_br(str2,1);
  strcat(str1,str2);
  
  
    set_open_block(str2,"h2");
  strcat(str1,str2);
 
  set_open_block(str2,"b>НАСТРОЙКА СЕТИ</b");
  strcat(str1,str2);
  
  
  reset_open_block(str2,"h2");
  strcat(str1,str2);
  

  
  // Table   
   set_open_block(str2,"table border=\"1\" style=\"border-collapse: collapse; width: 80%;border: 1px solid #ffffff;margin: auto;required\"><tbody");
   strcat(str1,str2);    


   if (FW_data.V_DHCP==0)
   {
      set_open_block(str3,"input type=\"checkbox\" name=\"dhcp_flag\" value=\"1\" /");
   }
   else
   {
      set_open_block(str3,"input type=\"checkbox\" name=\"dhcp_flag\" value=\"1\" checked /");
   }
   set_table_string(str2,"Использовать DHCP для назначения IP адреса ",str3);
   strcat(str1,str2);

   
   
   
   
   
   sprintf(str4,"%d\.%d\.%d\.%d",FW_data.V_IP_CONFIG[0],FW_data.V_IP_CONFIG[1],FW_data.V_IP_CONFIG[2],FW_data.V_IP_CONFIG[3]);  
   set_text_input(str3,"ip_addr",16,str4);  
   set_table_string(str2,"IP адрес",str3);
   strcat(str1,str2);
   
   sprintf(str4,"%d\.%d\.%d\.%d",FW_data.V_IP_MASK[0],FW_data.V_IP_MASK[1],FW_data.V_IP_MASK[2],FW_data.V_IP_MASK[3]);
   set_text_input(str3,"mask_addr",16,str4);  
   set_table_string(str2,"Маска подсети ",str3);  
   strcat(str1,str2);
   
   sprintf(str4,"%d\.%d\.%d\.%d",FW_data.V_IP_GET[0],FW_data.V_IP_GET[1],FW_data.V_IP_GET[2],FW_data.V_IP_GET[3]);
   set_text_input(str3,"getway_addr",16,str4);  
   set_table_string(str2,"Шлюз",str3);  
   strcat(str1,str2);
   
   sprintf(str4,"%d\.%d\.%d\.%d",FW_data.V_IP_DNS[0],FW_data.V_IP_DNS[1],FW_data.V_IP_DNS[2],FW_data.V_IP_DNS[3]);
   set_text_input(str3,"dns_addr",16,str4);  
   set_table_string(str2,"DNS сервер",str3);  
   strcat(str1,str2);
   

   
   
   sprintf(str4,"%d",FW_data.V_WEB_PORT);
 //  set_text_input(str3,"port_http",16,str4); 
     set_intnum (str3,"port_http",5,str4,1,65635); 
   set_table_string(str2,"Порт HTTP сервера",str3);  
   strcat(str1,str2);
   
   sprintf(str4,"%d\.%d\.%d\.%d",FW_data.V_IP_SYSL[0],FW_data.V_IP_SYSL[1],FW_data.V_IP_SYSL[2],FW_data.V_IP_SYSL[3]);
   set_text_input(str3,"syslog_addr",16,str4);  
   set_table_string(str2,"Адрес сервера SysLog сообщений",str3);  
   strcat(str1,str2);
   


   
 
  reset_open_block(str2,"tbody"); 
  strcat(str1,str2); 
  
  reset_open_block(str2,"table");
  strcat(str1,str2);

   
  len=strlen(str1);
       while(len>3000){}
  return len;
};

uint32_t costr_page6(char* str1)
{
  uint32_t len;
1;
  
  memset (str1,0, sizeof(str1)); 
  memset (str2,0, sizeof(str2));
  memset (str3,0, sizeof(str3));
  memset (str4,0, sizeof(str4));


  
  
  set_br(str2,1);
  strcat(str1,str2);
  
  
  set_open_block(str2,"h2");
  strcat(str1,str2);
 
  set_open_block(str2,"b>НАСТРОЙКА ДОСТУПА</b");
  strcat(str1,str2);
  
  
  reset_open_block(str2,"h2");
  strcat(str1,str2);
  

  
  // Table   
   set_open_block(str2,"table border=\"1\" style=\"border-collapse: collapse; width: 80%;border: 1px solid #ffffff;margin: auto;required\"><tbody");
   strcat(str1,str2);    


     
     
   set_text_input(str3,"login",16,FW_data.V_LOGIN);  
   set_table_string(str2,"Имя пользователя",str3);
   strcat(str1,str2);
   
   set_text_input_PW(str3,"password",16,FW_data.V_PASSWORD);  
   set_table_string(str2,"Пароль",str3);  
   strcat(str1,str2);
   

  reset_open_block(str2,"tbody"); 
  strcat(str1,str2); 
  
  reset_open_block(str2,"table");
  strcat(str1,str2);
  

  
  

  
  
  set_br(str2,1);
  strcat(str1,str2);
  
  
 set_open_block(str2,"h2");
  strcat(str1,str2);
 
  set_open_block(str2,"b>SNMP TRAP</b");
  strcat(str1,str2);
  
  
  reset_open_block(str2,"h2");
  strcat(str1,str2);
  
  
  // Table   
   set_open_block(str2,"table border=\"1\" style=\"border-collapse: collapse; width: 80%;border: 1px solid #ffffff;margin: auto;required\"><tbody");
   strcat(str1,str2);    
   
   sprintf(str4,"%d\.%d\.%d\.%d",FW_data.V_IP_SNMP[0],FW_data.V_IP_SNMP[1],FW_data.V_IP_SNMP[2],FW_data.V_IP_SNMP[3]);  
   set_text_input(str3,"snmp_addr",16,str4);  
   set_table_string(str2,"Адрес для посылки Trap сообщений",str3);  
   strcat(str1,str2);
   
   sprintf(str4,"%d",FW_data.V_PORT_SNMP);
   set_intnum(str3,"port_snmp",5,str4,1,65635);  
   set_table_string(str2,"Порт SNMP агента",str3);  
   strcat(str1,str2);


   
     len=strlen(str1);
          while(len>3000){}
  return len;
};
   
   
   uint32_t costr_page6_1(char* str1)
{
  uint32_t len;

  
  memset (str1,0, sizeof(str1)); 
  memset (str2,0, sizeof(str2));
  memset (str3,0, sizeof(str3));
  memset (str4,0, sizeof(str4));
  
  
  
 
  reset_open_block(str2,"tbody"); 
  strcat(str1,str2); 
  
  reset_open_block(str2,"table");
  strcat(str1,str2);
  

  
  set_br(str2,1);
  strcat(str1,str2);
  
  
  set_open_block(str2,"h2");
  strcat(str1,str2);
 
  set_open_block(str2,"b>НАСТРОЙКА ВСТРОЕННЫХ ЧАСОВ</b");
  strcat(str1,str2);
  
  
  reset_open_block(str2,"h2");
  strcat(str1,str2);

  
  // Table   
   set_open_block(str2,"table border=\"1\" style=\"border-collapse: collapse; width: 80%;border: 1px solid #ffffff;margin: auto;required\"><tbody");
   strcat(str1,str2);    
   
   sprintf(str4,"%d\.%d\.%d\.%d",FW_data.V_IP_NTP1[0],FW_data.V_IP_NTP1[1],FW_data.V_IP_NTP1[2],FW_data.V_IP_NTP1[3]);  
   set_text_input(str3,"ntp1_addr",16,str4);  
   set_table_string(str2,"NTP сервер 1",str3);  
   strcat(str1,str2);
   
   sprintf(str4,"%d\.%d\.%d\.%d",FW_data.V_IP_NTP2[0],FW_data.V_IP_NTP2[1],FW_data.V_IP_NTP2[2],FW_data.V_IP_NTP2[3]);  
   set_text_input(str3,"ntp2_addr",16,str4);  
   set_table_string(str2,"NTP сервер 2",str3);  
   strcat(str1,str2);
   
   sprintf(str4,"%d",FW_data.V_NTP_CIRCL);
   set_intnum(str3,"time_circl",2,str4,-12,+12);  
   set_table_string(str2,"Часовой пояс UTC:-12...+12 ",str3);  
   strcat(str1,str2);
      
      GET_reple(0,&real_time);  
   

   
   
   sprintf(str4,"%d\.%d\.%d",real_time.day,real_time.month,real_time.year);  
   set_text_input(str3,"dey_set",16,str4);  
   set_table_string(str2,"Текущая дата:",str3);  
   strcat(str1,str2);
   
   
   sprintf(str4,"%d\.%d\.%d",real_time.reple_hours,real_time.reple_minuts,real_time.reple_seconds);  
   set_text_input(str3,"time_set",16,str4);  
   set_table_string(str2,"Текущее время: ",str3);  
   strcat(str1,str2);
   


   
 
  reset_open_block(str2,"tbody"); 
  strcat(str1,str2); 
  
  reset_open_block(str2,"table");
  strcat(str1,str2);
  
   
  len=strlen(str1);
       while(len>3000){}
  return len;
};

uint32_t costr_page7(char* str1)
{
  uint32_t len;

  
  memset (str1,0, sizeof(str1)); 
  memset (str2,0, sizeof(str2));
  memset (str3,0, sizeof(str3));
  memset (str4,0, sizeof(str4));

  
  
  set_br(str2,1);
  strcat(str1,str2);
  
  
  set_open_block(str2,"h2");
  strcat(str1,str2);
 
  set_open_block(str2,"b>НАСТРОЙКА УПРАВЛЕНИЯ НАГРУЗКОЙ</b");
  strcat(str1,str2);
  
  
  reset_open_block(str2,"h2");
  strcat(str1,str2);
  
  
  // Table   
   set_open_block(str2,"table border=\"1\" style=\"border-collapse: collapse; width: 80%;border: 1px solid #ffffff;margin: auto;required\"><tbody");
   strcat(str1,str2);   


     
     
   set_text_input(str3,"mess_on",32,FW_data.V_ON_MESS);  
   set_table_string(str2,"Сообщение при включении нагрузки",str3);
   strcat(str1,str2);
   
   set_text_input(str3,"mess_off",32,FW_data.V_OFF_MESS);  
   set_table_string(str2,"Сообщение при выключении нагрузки",str3);  
   strcat(str1,str2);
   
   
   set_open_block(str2,"h2");
   strcat(str1,str2);
   
   set_table_string(str2,"НАСТРОЙКИ УПРАВЛЕНИЯ ВЫХОДНОГО ДРАЙВЕРА:","");  
   
   strcat(str1,str2);
   
   
  reset_open_block(str2,"h2");
  strcat(str1,str2);
  
  
  
   if (FW_data.V_TYPE_OUT ==0)
   {
   set_open_block(str3,"input name=\"output_type\" type=\"radio\" value=\"0\"checked");
   set_table_string(str2,"Прямое управление ",str3);  
   strcat(str1,str2);
   }
   else
   {
   set_open_block(str3,"input name=\"output_type\" type=\"radio\" value=\"0\"");
   set_table_string(str2,"Прямое управление ",str3);  
   strcat(str1,str2);   
   }
   
   if (FW_data.V_TYPE_OUT ==1)
   {
    set_open_block(str3,"input name=\"output_type\" type=\"radio\" value=\"1\"checked");
   set_table_string(str2,"Инвертированное управление",str3);  
   strcat(str1,str2);
   }
   else
   {
   set_open_block(str3,"input name=\"output_type\" type=\"radio\" value=\"1\"");
   set_table_string(str2,"Инвертированное управление",str3);  
   strcat(str1,str2);
   
   }
   
     
  len=strlen(str1);
       while(len>3000){}
  return len;
};

   
   uint32_t costr_page7_1 (char* str1)
{
  uint32_t len;

  
  memset (str1,0, sizeof(str1)); 
  memset (str2,0, sizeof(str2));
  memset (str3,0, sizeof(str3));
  memset (str4,0, sizeof(str4));
  
  
  
   
 if (FW_data.V_TYPE_OUT ==2)
   {
   set_open_block(str3,"input name=\"output_type\" type=\"radio\" value=\"2\"checked");
   set_table_string(str2,"Импульс включения",str3);  
   strcat(str1,str2);
   }
   else
   {
      set_open_block(str3,"input name=\"output_type\" type=\"radio\" value=\"2\"");
   set_table_string(str2,"Импульс включения",str3);  
   strcat(str1,str2);   
   }
   
  if (FW_data.V_TYPE_OUT ==3)
   {   
   set_open_block(str3,"input name=\"output_type\" type=\"radio\" value=\"3\"checked");
   set_table_string(str2,"Импульс выключения",str3);  
   strcat(str1,str2);
   }
   else
   {
   set_open_block(str3,"input name=\"output_type\" type=\"radio\" value=\"3\"");
   set_table_string(str2,"Импульс выключения",str3);  
   strcat(str1,str2);
   }
   
   set_submit(str3,"save_all","1","Сохранить","sets");
   set_table_string(str2,"Сохранить настройки ",str3);  
   strcat(str1,str2);
   
   
    reset_open_block(str2,"form");
  strcat(str1,str2);
  
  
  
  
  
  
    set_open_block(str2,"form id=\"load_dat\" action='/' method='POST'");
  strcat(str1,str2);
   
    set_submit(str3,"load_def","1","Загрузить","load_dat");
   set_table_string(str2,"Загрузить исходные настройки ",str3);  
   strcat(str1,str2);
   
  reset_open_block(str2,"form");
  strcat(str1,str2);
   
    
  
  
  
  set_open_block(str2,"form id=\"boots\" action='/' method='POST'");
  strcat(str1,str2);
  
   set_submit(str3,"load_boot","1","Загрузка ПО","boots");
   set_table_string(str2,"Перевести устройство в режим обновления ПО",str3);  
   strcat(str1,str2);
   
  reset_open_block(str2,"form");
  strcat(str1,str2);
   
  reset_open_block(str2,"tbody"); 
  strcat(str1,str2); 
  
  reset_open_block(str2,"table");
  strcat(str1,str2);
  
  reset_open_block(str2,"html");
  strcat(str1,str2);
  
  len=strlen(str1);
       while(len>3000){}
  return len;
};



uint32_t costr_page8(char* str1)
{
  uint32_t len;
  
  memset (str1,0, sizeof(str1)); 
  memset (str2,0, sizeof(str2));
  memset (str3,0, sizeof(str3));
  memset (str4,0, sizeof(str4));

  strcat(str1,http_html_start_constr);
  

  strcat(str1,http_html_style);
  
  set_open_block(str2,"body");
  strcat(str1,str2);
  
  set_open_block(str2,"ul");
  strcat(str1,str2);
  

  
 
  set_string_text(str2,"Интеллектуальная розетка DKST 59 v","h1",0,FW_data.V_FW1_VER);
  strcat(str1,str2);
    

  
      set_br(str2,1);
  strcat(str1,str2);
  
  set_open_block(str2,"h2");
  strcat(str1,str2);
  
  
  set_link(str2,"Главная","index.html");
  strcat(str1,str2);
  
  set_link(str2,"Настройки ","settings.html");
  strcat(str1,str2);
  
  set_link(str2,"Расписание ","rasp.html");
  strcat(str1,str2);
  
  set_link(str2,"Сторож","watchdog.html");
  strcat(str1,str2);
  
  
  set_link(str2,"E-mail","email.html");
  strcat(str1,str2);  
  
  set_link(str2,"Журнал ","logs.html");
  strcat(str1,str2);  
    
  reset_open_block(str2,"h2");
  strcat(str1,str2);

  reset_open_block(str2,"ul");
  strcat(str1,str2);
  
  
   strcat(str1,http_logs_hdr);
  
  
   set_open_block(str2,"p>ЖУРНАЛ СОБЫТИЙ</p");
  strcat(str1,str2);
   

  strcat(str1,http_logs_area);
  
  len=strlen(str1);
       while(len>3000){}
  return len;
}  
  
  
uint32_t costr_page9(char* str1)
{
  uint32_t len;


  
  memset (str1,0, sizeof(str1)); 
  memset (str2,0, sizeof(str2));
  memset (str3,0, sizeof(str3));
  memset (str4,0, sizeof(str4));
  

  
  
  
  
  strcat(str1,http_logs_end);


  
  reset_open_block(str2,"form");
 strcat(str1,str2);


  reset_open_block(str2,"html");
  strcat(str1,str2);
  
  len=strlen(str1);
       while(len>3000){}
  return len;
};

uint32_t costr_watchdog1(char* str1)
{
  uint32_t len;
  
  memset (str1,0, sizeof(str1)); 
  memset (str2,0, sizeof(str2));
  memset (str3,0, sizeof(str3));
  memset (str4,0, sizeof(str4));

  strcat(str1,http_html_start_constr);
  

  strcat(str1,http_html_style);
  
  set_open_block(str2,"body");
  strcat(str1,str2);
  
  set_open_block(str2,"ul");
  strcat(str1,str2);
  

  
 
  set_string_text(str2,"Интеллектуальная розетка DKST 59 v","h1",0,FW_data.V_FW1_VER);
  strcat(str1,str2);
 
  set_br(str2,1);
  strcat(str1,str2);
  
  set_open_block(str2,"h2");
  strcat(str1,str2);
  
  
  set_link(str2,"Главная","index.html");
  strcat(str1,str2);
  
  set_link(str2,"Настройки ","settings.html");
  strcat(str1,str2);
  
  set_link(str2,"Расписание ","rasp.html");
  strcat(str1,str2);
  
  set_link(str2,"Сторож","watchdog.html");
  strcat(str1,str2);
  
  
  set_link(str2,"E-mail","email.html");
  strcat(str1,str2);  
  
  set_link(str2,"Журнал ","logs.html");
  strcat(str1,str2);  
    
  reset_open_block(str2,"h2");
  strcat(str1,str2);

  reset_open_block(str2,"ul");
  strcat(str1,str2);
    
 set_open_block(str2,"form id=\"sets\" action='/' method='POST'");
 strcat(str1,str2);
  

 set_open_block(str2,"h2");
  strcat(str1,str2);
  
  
  set_br(str2,2);
  strcat(str1,str2);

  set_open_block(str2,"b>НАСТРОЙКА РЕЖИМА СТОРОЖ</b");
  strcat(str1,str2);
  
  set_br(str2,1);
  strcat(str1,str2);
  
  reset_open_block(str2,"h2");
  strcat(str1,str2);
  
  set_open_block(str2,"h3");
  strcat(str1,str2);
  // Table   
   set_open_block(str2,"table border=\"1\" style=\"border-collapse: collapse; width: 80%;border: 1px solid #ffffff;margin: auto;required\"><tbody");
   strcat(str1,str2);    

   
   
   memset (str3,0, sizeof(str3));
 //  set_checkbox (str3,"V_EN_WATCHDOG","1","WDT",FW_data.V_EN_WATCHDOG);
   set_checkonoff (str3,"V_EN_WATCHDOG",FW_data.V_EN_WATCHDOG);// (char* str1,char* name,uint8_t ck)
   set_table_string(str2,"Включить режим 'Сторож'",str3);
   strcat(str1,str2);
   len=strlen(str1); 
   
   memset (str3,0, sizeof(str3));
   memset (str4,0, sizeof(str4));
   set_checkbox (str4,"V_EN_WATCHDOG_CN_A","1","A",FW_data.V_EN_WATCHDOG_CN_A);
   strcat(str3,str4);
   set_open_block(str4,"b>A    </b");
   strcat(str3,str4);
   len=strlen(str3);
    
   memset (str4,0, sizeof(str4));
   set_checkbox (str4,"V_EN_WATCHDOG_CN_B","1","B",FW_data.V_EN_WATCHDOG_CN_B);
   strcat(str3,str4);
   set_open_block(str4,"b>B    </b");
   strcat(str3,str4);
   len=strlen(str3);
    
   memset (str4,0, sizeof(str4));
   set_checkbox (str4,"V_EN_WATCHDOG_CN_C","1","C",FW_data.V_EN_WATCHDOG_CN_C);
   strcat(str3,str4);
   set_open_block(str4,"b>C    </b");
   strcat(str3,str4);
   len=strlen(str3);   
   
   set_table_string(str2,"Выбрать контролируемые адреса",str3);
   strcat(str1,str2);  
 
   
   len=strlen(str1);
        while(len>3000){}
  return len;
};  


uint32_t costr_watchdog2(char* str1)
{
  uint32_t len;
  
  memset (str1,0, sizeof(str1)); 
  memset (str2,0, sizeof(str2));
  memset (str3,0, sizeof(str3));
  memset (str4,0, sizeof(str4));

   sprintf(str4,"%d\.%d\.%d\.%d",FW_data.V_IP_WDT_ADDR_CN_A[0],FW_data.V_IP_WDT_ADDR_CN_A[1],FW_data.V_IP_WDT_ADDR_CN_A[2],FW_data.V_IP_WDT_ADDR_CN_A[3]);  
   set_text_input(str3,"V_IP_WDT_ADDR_CN_A",15,str4);  
   set_table_string(str2,"IP адресс канал А",str3);
   strcat(str1,str2);
   
   sprintf(str4,"%d\.%d\.%d\.%d",FW_data.V_IP_WDT_ADDR_CN_B[0],FW_data.V_IP_WDT_ADDR_CN_B[1],FW_data.V_IP_WDT_ADDR_CN_B[2],FW_data.V_IP_WDT_ADDR_CN_B[3]);
   set_text_input(str3,"V_IP_WDT_ADDR_CN_B",15,str4);  
   set_table_string(str2,"IP адресс канал B",str3);  
   strcat(str1,str2);
   
   sprintf(str4,"%d\.%d\.%d\.%d",FW_data.V_IP_WDT_ADDR_CN_C[0],FW_data.V_IP_WDT_ADDR_CN_C[1],FW_data.V_IP_WDT_ADDR_CN_C[2],FW_data.V_IP_WDT_ADDR_CN_C[3]);
   set_text_input(str3,"V_IP_WDT_ADDR_CN_C",15,str4);  
   set_table_string(str2,"IP адресс канал C",str3);  
   strcat(str1,str2);
   
   sprintf(str3,"<b>%d</b>",FW_data.V_CT_RES_ALLSTART);  

   set_table_string(str2,"Cчетчик сбросов канала (обнуляется при перезагрузке прошивки)",str3);
   strcat(str1,str2);
   
   sprintf(str4,"%d",FW_data.V_T_SEND_PING);
   set_intnum(str3,"V_T_SEND_PING",3,str4,10,300);  
   set_table_string(str2,"Период опроса пингов 10-300с",str3);  
   strcat(str1,str2);
   
   sprintf(str4,"%d",FW_data.V_TIME_RESEND_PING);
   set_intnum(str3,"V_TIME_RESEND_PING",4,str4,600,9000);  
   set_table_string(str2,"Таймаут перед повтором пинга  600-9000 мc",str3);  
   strcat(str1,str2);
   
     len=strlen(str1);
          while(len>3000){}
  return len;
};



uint32_t costr_watchdog3(char* str1)
{
  uint32_t len;
  
  memset (str1,0, sizeof(str1)); 
  memset (str2,0, sizeof(str2));
  memset (str3,0, sizeof(str3));
  memset (str4,0, sizeof(str4));
  
   sprintf(str4,"%d",FW_data.V_MAX_REPID_PING);  
   set_intnum(str3,"V_MAX_REPID_PING",3,str4,0,100);  
   set_table_string(str2,"Максимальное число перезапросов",str3);
   strcat(str1,str2);
   
   sprintf(str4,"%d",FW_data.V_TIME_RESET_PULSE);
   set_intnum(str3,"V_TIME_RESET_PULSE",3,str4,1,901);  
   set_table_string(str2,"Длительность сброса 1-900с",str3);  
   strcat(str1,str2);
   
   sprintf(str4,"%d",FW_data.V_PAUSE_RESET_TO_REPID);
   set_intnum(str3,"V_PAUSE_RESET_TO_REPID",4,str4,1,3600);  
   set_table_string(str2,"Пауза после сброса перед возобновлением пингов 1-3600с",str3);  
   strcat(str1,str2);
   
   sprintf(str4,"%d",FW_data.V_MAX_RESEND_PACET_RESET);
   set_intnum(str3,"V_MAX_RESEND_PACET_RESET",3,str4,1,255);  
   set_table_string(str2,"Ограничение числа идущих подряд сбросов 1-255",str3);  
   strcat(str1,str2);
   
     reset_open_block(str2,"tbody"); 
  strcat(str1,str2); 
  
  reset_open_block(str2,"table");
  strcat(str1,str2);
     
  set_br(str2,1);
  strcat(str1,str2);
  
  
  set_open_block(str2,"h2");
  strcat(str1,str2);
 
  set_open_block(str2,"b>НАСТРОЙКА РЕЖИМА ВЫХОДА</b");
  strcat(str1,str2);
  
  
  reset_open_block(str2,"h2");
  strcat(str1,str2);
  
  // Table   
   set_open_block(str2,"table border=\"1\" style=\"border-collapse: collapse; width: 80%;border: 1px solid #ffffff;margin: auto;required\"><tbody");
   strcat(str1,str2);     
   
     len=strlen(str1);
          while(len>3000){}
  return len;
};

uint32_t costr_watchdog4(char* str1)
{
  uint32_t len;
  
  memset (str1,0, sizeof(str1)); 
  memset (str2,0, sizeof(str2));
  memset (str3,0, sizeof(str3));
  memset (str4,0, sizeof(str4));

   if (FW_data.V_TYPE_OUT ==0)
   {
   set_open_block(str3,"input name=\"output_type\" type=\"radio\" value=\"0\"checked");
   set_table_string(str2,"Прямое управление ",str3);  
   strcat(str1,str2);
   }
   else
   {
   set_open_block(str3,"input name=\"output_type\" type=\"radio\" value=\"0\"");
   set_table_string(str2,"Прямое управление ",str3);  
   strcat(str1,str2);   
   }
   
   
   if (FW_data.V_TYPE_OUT ==1)
   {
    set_open_block(str3,"input name=\"output_type\" type=\"radio\" value=\"1\"checked");
   set_table_string(str2,"Инвертированное управление",str3);  
   strcat(str1,str2);
   }
   else
   {
   set_open_block(str3,"input name=\"output_type\" type=\"radio\" value=\"1\"");
   set_table_string(str2,"Инвертированное управление",str3);  
   strcat(str1,str2);
   
   }
   
    if (FW_data.V_TYPE_OUT ==2)
   {
   set_open_block(str3,"input name=\"output_type\" type=\"radio\" value=\"2\"checked");
   set_table_string(str2,"Импульс включения",str3);  
   strcat(str1,str2);
   }
   else
   {
      set_open_block(str3,"input name=\"output_type\" type=\"radio\" value=\"2\"");
   set_table_string(str2,"Импульс включения",str3);  
   strcat(str1,str2);   
   }
   
   
    if (FW_data.V_TYPE_OUT ==3)
   {   
   set_open_block(str3,"input name=\"output_type\" type=\"radio\" value=\"3\"checked");
   set_table_string(str2,"Импульс выключения",str3);  
   strcat(str1,str2);
   }
   else
   {
   set_open_block(str3,"input name=\"output_type\" type=\"radio\" value=\"3\"");
   set_table_string(str2,"Импульс выключения",str3);  
   strcat(str1,str2);
   }
   
     reset_open_block(str2,"tbody"); 
  strcat(str1,str2); 
  
  reset_open_block(str2,"table");
  strcat(str1,str2);
     
  set_br(str2,1);
  strcat(str1,str2);
  
  
  set_open_block(str2,"h2");
  strcat(str1,str2);
 
  set_open_block(str2,"b>НАСТРОЙКА ЛОГИКИ УПРАВЛЕНИЯ</b");
  strcat(str1,str2);
  
  
  reset_open_block(str2,"h2");
  strcat(str1,str2);
  
  

  
  // Table   
 
     len=strlen(str1);
          while(len>3000){}
  return len;
};
   
   uint32_t costr_watchdog5(char* str1)
{
  uint32_t len;
  
  memset (str1,0, sizeof(str1)); 
  memset (str2,0, sizeof(str2));
  memset (str3,0, sizeof(str3));
  memset (str4,0, sizeof(str4));
  
  
   set_open_block(str2,"table border=\"1\" style=\"border-collapse: collapse; width: 80%;border: 1px solid #ffffff;margin: auto;required\"><tbody");
   strcat(str1,str2);   

   
   if (FW_data.V_TYPE_LOGIC ==0)
   {
   set_open_block(str3,"input name=\"output_logic\" type=\"radio\" value=\"0\"checked");
   set_table_string(str2,"Не ответил хотя бы один опрашиваемый адрес (A,B,C) ",str3);  
   strcat(str1,str2);
   }
   else
   {
   set_open_block(str3,"input name=\"output_logic\" type=\"radio\" value=\"0\"");
   set_table_string(str2,"Не ответил хотя бы один опрашиваемый адрес (A,B,C)",str3);  
   strcat(str1,str2);   
   }
   
    if (FW_data.V_TYPE_LOGIC ==1)
   {
    set_open_block(str3,"input name=\"output_logic\" type=\"radio\" value=\"1\"checked");
   set_table_string(str2,"Не ответил ни один из опрашиваемых адресов (A,B,C)",str3);  
   strcat(str1,str2);
   }
   else
   {
   set_open_block(str3,"input name=\"output_logic\" type=\"radio\" value=\"1\"");
   set_table_string(str2,"Не ответил ни один из опрашиваемых адресов (A,B,C)",str3);  
   strcat(str1,str2);
   
   }
   
    if (FW_data.V_TYPE_LOGIC ==2)
   {
   set_open_block(str3,"input name=\"output_logic\" type=\"radio\" value=\"2\"checked");
   set_table_string(str2,"Не ответил адрес А и один из B или C",str3);  
   strcat(str1,str2);
   }
   else
   {
      set_open_block(str3,"input name=\"output_logic\" type=\"radio\" value=\"2\"");
   set_table_string(str2,"Не ответил адрес А и один из B или C",str3);  
   strcat(str1,str2);   
   }
   
   
    if (FW_data.V_TYPE_LOGIC ==3)
   {   
   set_open_block(str3,"input name=\"output_logic\" type=\"radio\" value=\"3\"checked");
   set_table_string(str2,"не ответил адрес А, однако ответил B или C",str3);  
   strcat(str1,str2);
   }
   else
   {
   set_open_block(str3,"input name=\"output_logic\" type=\"radio\" value=\"3\"");
   set_table_string(str2,"Не ответил адрес А, однако ответил B или C",str3);  
   strcat(str1,str2);
   }
   
  
   set_submit(str3,"save_all","1","Сохранить","sets");
   set_table_string(str2,"Сохранить настройки ",str3);  
   strcat(str1,str2);
   
  reset_open_block(str2,"h3");
  strcat(str1,str2);
   
 
   reset_open_block(str2,"tbody");
   strcat(str1,str2);
  
   reset_open_block(str2,"table");
   strcat(str1,str2);
   
//
  reset_open_block(str2,"form");
  strcat(str1,str2);
  
  


  
  len=strlen(str1);
       while(len>3000){}
  return len;
};


uint32_t costr_rasp_page1(char* str1)
{
  uint32_t len;
  
  memset (str1,0, sizeof(str1)); 
  memset (str2,0, sizeof(str2));
  memset (str3,0, sizeof(str3));
  memset (str4,0, sizeof(str4));

  strcat(str1,http_html_start_constr);
  

  strcat(str1,http_html_style);
  
  set_open_block(str2,"body");
  strcat(str1,str2);
  
  set_open_block(str2,"ul");
  strcat(str1,str2);
  
  

  
 
  set_string_text(str2,"Интеллектуальная розетка DKST 59 v","h1",0,FW_data.V_FW1_VER);
  strcat(str1,str2);
    

  
      set_br(str2,1);
  strcat(str1,str2);
  
  set_open_block(str2,"h2");
  strcat(str1,str2);
  
  
  set_link(str2,"Главная","index.html");
  strcat(str1,str2);
  
  set_link(str2,"Настройки ","settings.html");
  strcat(str1,str2);
  
  set_link(str2,"Расписание ","rasp.html");
  strcat(str1,str2);
  
  set_link(str2,"Сторож","watchdog.html");
  strcat(str1,str2);
  
  
  set_link(str2,"E-mail","email.html");
  strcat(str1,str2);  
  
  set_link(str2,"Журнал ","logs.html");
  strcat(str1,str2);  
    
  reset_open_block(str2,"h2");
  strcat(str1,str2);

  reset_open_block(str2,"ul");
  strcat(str1,str2);

  
  
  
  set_open_block(str2,"form id=\"sets1\" action='/' method='POST'");
 strcat(str1,str2);

 set_open_block(str2,"h2");
  strcat(str1,str2);
  
  
  set_br(str2,2);
  strcat(str1,str2);

  set_open_block(str2,"b>СТАТУС</b");
  strcat(str1,str2);
  
  set_br(str2,1);
  strcat(str1,str2);
  
  reset_open_block(str2,"h2");
  strcat(str1,str2);
  
  set_open_block(str2,"h3");
  strcat(str1,str2);
  // Table   
   set_open_block(str2,"table border=\"1\" style=\"border-collapse: collapse; width: 80%;border: 1px solid #ffffff;margin: auto;required\"><tbody");
   strcat(str1,str2);    


   HAL_RTC_GetDate (&hrtc,&dates,RTC_FORMAT_BIN);
   HAL_RTC_GetTime (&hrtc,&times,RTC_FORMAT_BIN);
  
   sprintf(str3,"%d.%d.%d  %d:%d:%d    ",dates.Date,dates.Month,2000+dates.Year,times.Hours,times.Minutes,times.Seconds);
   set_table_string(str2,"Локальное время в устройстве",str3);
   strcat(str1,str2);
   
   sprintf(str3,"UTC %d",FW_data.V_NTP_CIRCL);
   set_table_string(str2,"Сдвиг локального времени",str3);  
   strcat(str1,str2);
   
   if (status_NTP_activ==0)
   {
      sprintf(str3,"Синхронизация времени по NTP не выполнена");
   }
   else
    {
       sprintf(str3,"Синхронизация времени по NTP выполнена");
    }
   set_table_string(str2,"Статус NTP",str3);  
   strcat(str1,str2);
   

   reset_open_block(str2,"h3");
  strcat(str1,str2);
   
 
   reset_open_block(str2,"tbody");
   strcat(str1,str2);
  
   reset_open_block(str2,"table");
   strcat(str1,str2);
   
   
  
  len=strlen(str1);
       while(len>3000){}
  return len;
};

 uint32_t costr_rasp_page2(char* str1)
{
  uint32_t len;
  
  memset (str1,0, sizeof(str1)); 
  memset (str2,0, sizeof(str2));
  memset (str3,0, sizeof(str3));
  memset (str4,0, sizeof(str4));  
   
   set_open_block(str2,"h2");
  strcat(str1,str2);
  
    set_br(str2,2);
  strcat(str1,str2);

  set_open_block(str2,"b>НАСТРОЙКА</b");
  strcat(str1,str2);
  
  set_br(str2,1);
  strcat(str1,str2);
  
  reset_open_block(str2,"h2");
  strcat(str1,str2);
  
  set_open_block(str2,"h3");
  strcat(str1,str2);
  // Table   
   set_open_block(str2,"table border=\"1\" style=\"border-collapse: collapse; width: 80%;border: 1px solid #ffffff;margin: auto;required\"><tbody");
   strcat(str1,str2);    

  
   memset (str3,0, sizeof(str3));

   set_checkonoff (str3,"V_FLAG_EN_RASP",FW_data.V_FLAG_EN_RASP);// (char* str1,char* name,uint8_t ck)
   set_table_string(str2,"Включить режим 'Расписание'",str3);
   strcat(str1,str2);
   len=strlen(str1); 
   
    memset (str3,0, sizeof(str3));

 
     
   set_menu3(str3,"Игнорировать сбой","Вкл Постоянно ","Выкл Постоянно","V_SOST_ERR_RASP",FW_data.V_SOST_ERR_RASP);  
   set_table_string(str2,"Состояние реле при сбое установки часов",str3);
   strcat(str1,str2);
   len=strlen(str1); 
   
   if (flag_err_dates==0)
   {
     set_table_string(str2,"Ошибка настройки времени","Отсутствует");
   }
   else
   {
     set_table_string(str2,"Ошибка настройки времени","Ошибка активна");
   }
     
   strcat(str1,str2);
   len=strlen(str1); 
   
   set_submit(str3,"save_rasp_sec1","1","Применить изменения",0);
   set_table_string(str2,"",str3);  
   strcat(str1,str2);
   
   
  reset_open_block(str2,"h3");
  strcat(str1,str2);
   
 
   reset_open_block(str2,"tbody");
   strcat(str1,str2);
  
   reset_open_block(str2,"table");
   strcat(str1,str2);
   
   
   
   

  reset_open_block(str2,"form");
  strcat(str1,str2);
  
  
  

  
  len=strlen(str1);
       while(len>3000){}
  return len;
};

//Rasp SEC1-3 d1-d7
 uint32_t costr_rasp_page3(char* str1)
{
  uint32_t len;
  
  memset (str1,0, sizeof(str1)); 
  memset (str2,0, sizeof(str2));
  memset (str3,0, sizeof(str3));
  memset (str4,0, sizeof(str4));  
   
  set_open_block(str2,"form id=\"sets2\" action='/' method='POST'");

 strcat(str1,str2);
 
 
   set_open_block(str2,"h2");
  strcat(str1,str2);
  
    set_br(str2,2);
  strcat(str1,str2);

  set_open_block(str2,"b>НЕДЕЛЬНОЕ РАССПИСАНИЕ</b");
  strcat(str1,str2);
  
  set_br(str2,1);
  strcat(str1,str2);
  
  reset_open_block(str2,"h2");
  strcat(str1,str2);
  
  set_open_block(str2,"h3");
  strcat(str1,str2);
  // Table   
   set_open_block(str2,"table border=\"1\" style=\"border-collapse: collapse; width: 80%;border: 1px solid #ffffff;margin: auto;required\"><tbody");
   strcat(str1,str2);    
   

   set_table_rasp_zagl(str1,str2," ");
   
   
  
   
   
  len=strlen(str1);
       while(len>3000){}
  return len;
};




 uint32_t costr_rasp_page4(char* str1)
{
  uint32_t len;
  
  memset (str1,0, sizeof(str1)); 
  memset (str2,0, sizeof(str2));
  memset (str3,0, sizeof(str3));
  memset (str4,0, sizeof(str4));  

 memset (str3,0, sizeof(str3));
  
   set_table_rasp_str(str1,str2,"ПН","V_DT"," ",FW_data.V_D_TIME,0);
   
  len=strlen(str1);
       while(len>3000){}
  return len;
};
   
   
   

 uint32_t costr_rasp_page5(char* str1)
{
  uint32_t len;
  
  memset (str1,0, sizeof(str1)); 
  memset (str2,0, sizeof(str2));
  memset (str3,0, sizeof(str3));
  memset (str4,0, sizeof(str4));  
   
   
   memset (str3,0, sizeof(str3));
   set_checkbox (str4,"V_D_d1","1","C",FW_data.V_D_TIME[1].set_up_day);
   set_table_rasp_str(str1,str2,"ВТ","V_DT",str4,FW_data.V_D_TIME,1);
 
   len=strlen(str1);
       while(len>3000){}
  return len;
};


 uint32_t costr_rasp_page6(char* str1)
{
  uint32_t len;
  
  memset (str1,0, sizeof(str1)); 
  memset (str2,0, sizeof(str2));
  memset (str3,0, sizeof(str3));
  memset (str4,0, sizeof(str4));  
   
   
   memset (str3,0, sizeof(str3));
   set_checkbox (str4,"V_D_d2","1","C",FW_data.V_D_TIME[2].set_up_day);
   set_table_rasp_str(str1,str2,"СР","V_DT",str4,FW_data.V_D_TIME,2);
 
   len=strlen(str1);
       while(len>3000){}
  return len;
};


 uint32_t costr_rasp_page7(char* str1)
{
  uint32_t len;
  
  memset (str1,0, sizeof(str1)); 
  memset (str2,0, sizeof(str2));
  memset (str3,0, sizeof(str3));
  memset (str4,0, sizeof(str4));  
   
   
   memset (str3,0, sizeof(str3));
   set_checkbox (str4,"V_D_d3","1","C",FW_data.V_D_TIME[3].set_up_day);
   set_table_rasp_str(str1,str2,"ЧТ","V_DT",str4,FW_data.V_D_TIME,3);
 
   len=strlen(str1);
       while(len>3000){}
  return len;
};


 uint32_t costr_rasp_page8(char* str1)
{
  uint32_t len;
  
  memset (str1,0, sizeof(str1)); 
  memset (str2,0, sizeof(str2));
  memset (str3,0, sizeof(str3));
  memset (str4,0, sizeof(str4));  
   
   
   memset (str3,0, sizeof(str3));
   set_checkbox (str4,"V_D_d4","1","C",FW_data.V_D_TIME[4].set_up_day);
   set_table_rasp_str(str1,str2,"ПТ","V_DT",str4,FW_data.V_D_TIME,4);
 
   len=strlen(str1);
       while(len>3000){}
  return len;
};


 uint32_t costr_rasp_page9(char* str1)
{
  uint32_t len;
  
  memset (str1,0, sizeof(str1)); 
  memset (str2,0, sizeof(str2));
  memset (str3,0, sizeof(str3));
  memset (str4,0, sizeof(str4));  
   
   
   memset (str3,0, sizeof(str3));
   set_checkbox (str4,"V_D_d5","1","C",FW_data.V_D_TIME[5].set_up_day);
   set_table_rasp_str(str1,str2,"СБ","V_DT",str4,FW_data.V_D_TIME,5);
 
   len=strlen(str1);
       while(len>3000){}
  return len;
};
  
 uint32_t costr_rasp_page10(char* str1)
{
  uint32_t len;
  
  memset (str1,0, sizeof(str1)); 
  memset (str2,0, sizeof(str2));
  memset (str3,0, sizeof(str3));
  memset (str4,0, sizeof(str4));  
   
   
   memset (str3,0, sizeof(str3));
   set_checkbox (str4,"V_D_d6","1","C",FW_data.V_D_TIME[6].set_up_day);
   set_table_rasp_str(str1,str2,"ВС","V_DT",str4,FW_data.V_D_TIME,6);
 
   len=strlen(str1);
       while(len>3000){}
  return len;
};
  

 uint32_t costr_rasp_page11(char* str1)
{
  uint32_t len;
  
  memset (str1,0, sizeof(str1)); 
  memset (str2,0, sizeof(str2));
  memset (str3,0, sizeof(str3));
  memset (str4,0, sizeof(str4));  
  
  
   
   
  reset_open_block(str2,"h3");
  strcat(str1,str2);
   
 
   reset_open_block(str2,"tbody");
   strcat(str1,str2);
  
   reset_open_block(str2,"table");
   strcat(str1,str2);
   

  
  set_open_block(str2,"h4");
  strcat(str1,str2);
  
  
  set_submit(str3,"save_rasp_sec1","1","Применить изменения",0);
  strcat(str1,str3);
   
  set_br(str2,1);
  strcat(str1,str2);
   
     reset_open_block(str2,"h4");
  strcat(str1,str2);

  reset_open_block(str2,"form");
  strcat(str1,str2);
  
      set_br(str2,3);
  strcat(str1,str2);
  


  
  len=strlen(str1);

       while(len>3000){}
  return len;
};



//RASP SEC4-6* d1-d7*************************************************************

 uint32_t costr_rasp_page12(char* str1)
{
  uint32_t len;
  
  memset (str1,0, sizeof(str1)); 
  memset (str2,0, sizeof(str2));
  memset (str3,0, sizeof(str3));
  memset (str4,0, sizeof(str4));  
   
  set_open_block(str2,"form id=\"sets3\" action='/' method='POST'");
 
 strcat(str1,str2);
 

  
  set_open_block(str2,"h3");
  strcat(str1,str2);
  // Table   
   set_open_block(str2,"table border=\"1\" style=\"border-collapse: collapse; width: 80%;border: 1px solid #ffffff;margin: auto;required\"><tbody");
   strcat(str1,str2);    
   

   set_table_rasp_zag2(str1,str2," ");
   
   
  
   
   
  len=strlen(str1);
       while(len>3000){}
  return len;
};



 uint32_t costr_rasp_page13(char* str1)
{
  uint32_t len;
  
  memset (str1,0, sizeof(str1)); 
  memset (str2,0, sizeof(str2));
  memset (str3,0, sizeof(str3));
  memset (str4,0, sizeof(str4));  

 memset (str3,0, sizeof(str3));

   set_table_rasp_str_1(str1,str2,"ПН","V_DT"," ",FW_data.V_D_TIME,0);
   
  len=strlen(str1);
       while(len>3000){}
  return len;
};
   
   
   

 uint32_t costr_rasp_page14(char* str1)
{
  uint32_t len;
  
  memset (str1,0, sizeof(str1)); 
  memset (str2,0, sizeof(str2));
  memset (str3,0, sizeof(str3));
  memset (str4,0, sizeof(str4));  
   
   
   memset (str3,0, sizeof(str3));
   set_checkbox (str4,"V_D_d1","1","C",FW_data.V_D_TIME[1].set_up_day);
   set_table_rasp_str_1(str1,str2,"ВТ","V_DT",str4,FW_data.V_D_TIME,1);
 
   len=strlen(str1);
       while(len>3000){}
  return len;
};


 uint32_t costr_rasp_page15(char* str1)
{
  uint32_t len;
  
  memset (str1,0, sizeof(str1)); 
  memset (str2,0, sizeof(str2));
  memset (str3,0, sizeof(str3));
  memset (str4,0, sizeof(str4));  
   
   
   memset (str3,0, sizeof(str3));
   set_checkbox (str4,"V_D_d2","1","C",FW_data.V_D_TIME[2].set_up_day);
   set_table_rasp_str_1(str1,str2,"СР","V_DT",str4,FW_data.V_D_TIME,2);
 
   len=strlen(str1);
       while(len>3000){}
  return len;
};


 uint32_t costr_rasp_page16(char* str1)
{
  uint32_t len;
  
  memset (str1,0, sizeof(str1)); 
  memset (str2,0, sizeof(str2));
  memset (str3,0, sizeof(str3));
  memset (str4,0, sizeof(str4));  
   
   
   memset (str3,0, sizeof(str3));
   set_checkbox (str4,"V_D_d3","1","C",FW_data.V_D_TIME[3].set_up_day);
   set_table_rasp_str_1(str1,str2,"ЧТ","V_DT",str4,FW_data.V_D_TIME,3);
 
   len=strlen(str1);
       while(len>3000){}
  return len;
};


 uint32_t costr_rasp_page17(char* str1)
{
  uint32_t len;
  
  memset (str1,0, sizeof(str1)); 
  memset (str2,0, sizeof(str2));
  memset (str3,0, sizeof(str3));
  memset (str4,0, sizeof(str4));  
   
   
   memset (str3,0, sizeof(str3));
   set_checkbox (str4,"V_D_d4","1","C",FW_data.V_D_TIME[4].set_up_day);
   set_table_rasp_str_1(str1,str2,"ПТ","V_DT",str4,FW_data.V_D_TIME,4);
 
   len=strlen(str1);
       while(len>3000){}
  return len;
};


 uint32_t costr_rasp_page18(char* str1)
{
  uint32_t len;
  
  memset (str1,0, sizeof(str1)); 
  memset (str2,0, sizeof(str2));
  memset (str3,0, sizeof(str3));
  memset (str4,0, sizeof(str4));  
   
   
   memset (str3,0, sizeof(str3));
   set_checkbox (str4,"V_D_d5","1","C",FW_data.V_D_TIME[5].set_up_day);
   set_table_rasp_str_1(str1,str2,"СБ","V_DT",str4,FW_data.V_D_TIME,5);
 
   len=strlen(str1);
       while(len>3000){}
  return len;
};
  
 uint32_t costr_rasp_page19(char* str1)
{
  uint32_t len;
  
  memset (str1,0, sizeof(str1)); 
  memset (str2,0, sizeof(str2));
  memset (str3,0, sizeof(str3));
  memset (str4,0, sizeof(str4));  
   
   
   memset (str3,0, sizeof(str3));
   set_checkbox (str4,"V_D_d6","1","C",FW_data.V_D_TIME[6].set_up_day);
   set_table_rasp_str_1(str1,str2,"ВС","V_DT",str4,FW_data.V_D_TIME,6);
 
   len=strlen(str1);
       while(len>3000){}
  return len;
};
  



 uint32_t costr_rasp_page20(char* str1)
{
  uint32_t len;
  
  memset (str1,0, sizeof(str1)); 
  memset (str2,0, sizeof(str2));
  memset (str3,0, sizeof(str3));
  memset (str4,0, sizeof(str4));  
  
  
   
   
  reset_open_block(str2,"h3");
  strcat(str1,str2);
   
 
   reset_open_block(str2,"tbody");
   strcat(str1,str2);
  
   reset_open_block(str2,"table");
   strcat(str1,str2);
   
 
  set_open_block(str2,"h4");
  strcat(str1,str2);
  
  
  set_submit(str3,"save_rasp_sec2","1","Применить изменения",0);
  strcat(str1,str3);
   
  set_br(str2,1);
  strcat(str1,str2);
   
     reset_open_block(str2,"h4");
  strcat(str1,str2);

  reset_open_block(str2,"form");
  strcat(str1,str2);
  
        set_br(str2,3);
  strcat(str1,str2);
  


  
  len=strlen(str1);

       while(len>3000){}
  return len;
};

// RASP SEC1-3 P1-3

 uint32_t costr_rasp_page21(char* str1)
{
  uint32_t len;
  
  memset (str1,0, sizeof(str1)); 
  memset (str2,0, sizeof(str2));
  memset (str3,0, sizeof(str3));
  memset (str4,0, sizeof(str4));  
   
  set_open_block(str2,"form id=\"sets4\" action='/' method='POST'");
 
 strcat(str1,str2);
 
 
  set_open_block(str2,"h3");
  strcat(str1,str2);
  // Table   
   set_open_block(str2,"table border=\"1\" style=\"border-collapse: collapse; width: 80%;border: 1px solid #ffffff;margin: auto;required\"><tbody");
   strcat(str1,str2);    

   set_table_rasp_zagl(str1,str2," ");
   
   
  
   
   
  len=strlen(str1);
       while(len>3000){}
  return len;
};


 uint32_t costr_rasp_page22(char* str1)
{
  uint32_t len;
  
  memset (str1,0, sizeof(str1)); 
  memset (str2,0, sizeof(str2));
  memset (str3,0, sizeof(str3));
  memset (str4,0, sizeof(str4));  
   
   
   memset (str3,0, sizeof(str3));

   set_table_rasp_str(str1,str2,"П1","V_DT","",FW_data.V_D_TIME,7);
 
   len=strlen(str1);
       while(len>3000){}
  return len;
};
  

 uint32_t costr_rasp_page23(char* str1)
{
  uint32_t len;
  
  memset (str1,0, sizeof(str1)); 
  memset (str2,0, sizeof(str2));
  memset (str3,0, sizeof(str3));
  memset (str4,0, sizeof(str4));  
   
   
   memset (str3,0, sizeof(str3));

   set_table_rasp_str(str1,str2,"П2","V_DT","",FW_data.V_D_TIME,8);
 
   len=strlen(str1);
       while(len>3000){}
  return len;
};
  

 uint32_t costr_rasp_page24(char* str1)
{
  uint32_t len;
  
  memset (str1,0, sizeof(str1)); 
  memset (str2,0, sizeof(str2));
  memset (str3,0, sizeof(str3));
  memset (str4,0, sizeof(str4));  
   
   
   memset (str3,0, sizeof(str3));

   set_table_rasp_str(str1,str2,"П3","V_DT","",FW_data.V_D_TIME,9);
 
   len=strlen(str1);
       while(len>3000){}
  return len;
};
  

 uint32_t costr_rasp_page25(char* str1)
{
  uint32_t len;
  
  memset (str1,0, sizeof(str1)); 
  memset (str2,0, sizeof(str2));
  memset (str3,0, sizeof(str3));
  memset (str4,0, sizeof(str4));  
  
  
   
   
  reset_open_block(str2,"h3");
  strcat(str1,str2);
   
 
   reset_open_block(str2,"tbody");
   strcat(str1,str2);
  
   reset_open_block(str2,"table");
   strcat(str1,str2);
   

  
  set_open_block(str2,"h4");
  strcat(str1,str2);
  
  
  set_submit(str3,"save_rasp_sec3","1","Применить изменения",0);
  strcat(str1,str3);
   
  set_br(str2,1);
  strcat(str1,str2);
   
     reset_open_block(str2,"h4");
  strcat(str1,str2);

  reset_open_block(str2,"form");
  strcat(str1,str2);
  
  set_br(str2,3);
  strcat(str1,str2);
  


  
  len=strlen(str1);

       while(len>3000){}
  return len;
};

// RASP SEC4-6 P1-3

 uint32_t costr_rasp_page26(char* str1)
{
  uint32_t len;
  
  memset (str1,0, sizeof(str1)); 
  memset (str2,0, sizeof(str2));
  memset (str3,0, sizeof(str3));
  memset (str4,0, sizeof(str4));  
   
  set_open_block(str2,"form id=\"sets4\" action='/' method='POST'");
 
 strcat(str1,str2);
 
 
  set_open_block(str2,"h3");
  strcat(str1,str2);
  // Table   
   set_open_block(str2,"table border=\"1\" style=\"border-collapse: collapse; width: 80%;border: 1px solid #ffffff;margin: auto;required\"><tbody");
   strcat(str1,str2);    

   set_table_rasp_zag2(str1,str2," ");
   
   
  
   
   
  len=strlen(str1);
       while(len>3000){}
  return len;
};


 uint32_t costr_rasp_page27(char* str1)
{
  uint32_t len;
  
  memset (str1,0, sizeof(str1)); 
  memset (str2,0, sizeof(str2));
  memset (str3,0, sizeof(str3));
  memset (str4,0, sizeof(str4));  
   
   
   memset (str3,0, sizeof(str3));

   set_table_rasp_str_1(str1,str2,"П1","V_DT","",FW_data.V_D_TIME,7);
 
   len=strlen(str1);
       while(len>3000){}
  return len;
};
  

 uint32_t costr_rasp_page28(char* str1)
{
  uint32_t len;
  
  memset (str1,0, sizeof(str1)); 
  memset (str2,0, sizeof(str2));
  memset (str3,0, sizeof(str3));
  memset (str4,0, sizeof(str4));  
   
   
   memset (str3,0, sizeof(str3));

   set_table_rasp_str_1(str1,str2,"П2","V_DT","",FW_data.V_D_TIME,8);
 
   len=strlen(str1);
       while(len>3000){}
  return len;
};
  

 uint32_t costr_rasp_page29(char* str1)
{
  uint32_t len;
  
  memset (str1,0, sizeof(str1)); 
  memset (str2,0, sizeof(str2));
  memset (str3,0, sizeof(str3));
  memset (str4,0, sizeof(str4));  
   
   
   memset (str3,0, sizeof(str3));

   set_table_rasp_str_1(str1,str2,"П3","V_DT","",FW_data.V_D_TIME,9);
 
   len=strlen(str1);
       while(len>3000){}
  return len;
};
  

 uint32_t costr_rasp_page30(char* str1)
{
  uint32_t len;
  
  memset (str1,0, sizeof(str1)); 
  memset (str2,0, sizeof(str2));
  memset (str3,0, sizeof(str3));
  memset (str4,0, sizeof(str4));  
  
  
   
   
  reset_open_block(str2,"h3");
  strcat(str1,str2);
   
 
   reset_open_block(str2,"tbody");
   strcat(str1,str2);
  
   reset_open_block(str2,"table");
   strcat(str1,str2);
   

  
  set_open_block(str2,"h4");
  strcat(str1,str2);
  
  
  set_submit(str3,"save_rasp_sec4","1","Применить изменения",0);
  strcat(str1,str3);
   
  set_br(str2,1);
  strcat(str1,str2);
   
     reset_open_block(str2,"h4");
  strcat(str1,str2);

  reset_open_block(str2,"form");
  strcat(str1,str2);
  
    set_br(str2,3);
  strcat(str1,str2);
  
 


  
  len=strlen(str1);

       while(len>3000){}
  return len;
};


//sec4-6 p1-3


 uint32_t costr_rasp_page31(char* str1)
{
  uint32_t len;
  
  memset (str1,0, sizeof(str1)); 
  memset (str2,0, sizeof(str2));
  memset (str3,0, sizeof(str3));
  memset (str4,0, sizeof(str4));  
   
   set_open_block(str2,"form id=\"sets4\" action='/' method='POST'");
 strcat(str1,str2);
 
 
   set_open_block(str2,"h2");
  strcat(str1,str2);
  
    set_br(str2,2);
  strcat(str1,str2);

  set_open_block(str2,"b>ПОДМЕНА ПРАЗДНИКОВ</b");
  strcat(str1,str2);
  
  set_br(str2,1);
  strcat(str1,str2);
  
  reset_open_block(str2,"h2");
  strcat(str1,str2);
  
  set_open_block(str2,"h3");
  strcat(str1,str2);
  // Table   
   set_open_block(str2,"table border=\"1\" style=\"border-collapse: collapse; width: 80%;border: 1px solid #ffffff;margin: auto;required\"><tbody");
   strcat(str1,str2);    
     
   memset (str3,0, sizeof(str3));

   set_table_podm_str(str1,str2,"Дата","V_RD_DATA_DAY","",&(FW_data.V_RD_DATA),0);
 
   len=strlen(str1);
       while(len>3000){}
  return len;
};


 uint32_t costr_rasp_page32(char* str1)
{
  uint32_t len;
  
  memset (str1,0, sizeof(str1)); 
  memset (str2,0, sizeof(str2));
  memset (str3,0, sizeof(str3));
  memset (str4,0, sizeof(str4));  
   
   
   memset (str3,0, sizeof(str3));

   len=set_table_podm_day_str1(str1,str2,"Подм.","V_RD_DATA_PODM",&(FW_data.V_RD_DATA));
   


       while(len>3000){}
  return len;
};

 uint32_t costr_rasp_page33(char* str1)
{
  uint32_t len;
  
  memset (str1,0, sizeof(str1)); 
  memset (str2,0, sizeof(str2));
  memset (str3,0, sizeof(str3));
  memset (str4,0, sizeof(str4));  
   
   
   memset (str3,0, sizeof(str3));

   set_table_podm_day_str2(str1,str2,"Подм.","V_RD_DATA_PODM",&(FW_data.V_RD_DATA));
   
  reset_open_block(str2,"h3");
  strcat(str1,str2);
   
 
   reset_open_block(str2,"tbody");
   strcat(str1,str2);
  
   reset_open_block(str2,"table");
   strcat(str1,str2);
   

  
  set_open_block(str2,"h4");
  strcat(str1,str2);
  
  
  set_submit(str3,"save_rasp_sec5","1","Применить изменения",0);
  strcat(str1,str3);
   
  set_br(str2,1);
  strcat(str1,str2);
   
     reset_open_block(str2,"h4");
  strcat(str1,str2);

  reset_open_block(str2,"form");
  strcat(str1,str2);

  reset_open_block(str2,"html");
  strcat(str1,str2);
  
  len=strlen(str1);
       while(len>3000){}
  return len;
};


uint32_t costr_email_page1(char* str1)
{
  uint32_t len;
  
  memset (str1,0, sizeof(str1)); 
  memset (str2,0, sizeof(str2));
  memset (str3,0, sizeof(str3));
  memset (str4,0, sizeof(str4));

  strcat(str1,http_html_start_constr);
  

  strcat(str1,http_html_style);
  
  set_open_block(str2,"body");
  strcat(str1,str2);
  
  set_open_block(str2,"ul");
  strcat(str1,str2);
  
  

 
  set_string_text(str2,"Интеллектуальная розетка DKST 59 v","h1",0,FW_data.V_FW1_VER);
  strcat(str1,str2);
    

  
      set_br(str2,1);
  strcat(str1,str2);
  
  set_open_block(str2,"h2");
  strcat(str1,str2);
  
  
  set_link(str2,"Главная","index.html");
  strcat(str1,str2);
  
  set_link(str2,"Настройки ","settings.html");
  strcat(str1,str2);
  
  set_link(str2,"Расписание ","rasp.html");
  strcat(str1,str2);
  
  set_link(str2,"Сторож","watchdog.html");
  strcat(str1,str2);
  
  
  set_link(str2,"E-mail","email.html");
  strcat(str1,str2);  
  
  set_link(str2,"Журнал ","logs.html");
  strcat(str1,str2);  
    
  reset_open_block(str2,"h2");
  strcat(str1,str2);

  reset_open_block(str2,"ul");
  strcat(str1,str2);

  set_open_block(str2,"form id=\"sets1\" action='/' method='POST'");
 strcat(str1,str2);

   
   set_open_block(str2,"h2");
  strcat(str1,str2);
  
    set_br(str2,2);
  strcat(str1,str2);

  set_open_block(str2,"b>НАСТРОЙКА SMTP ДЛЯ ИСХОДЯЩИХ E-MAIL СООБЩИЩЕНИЙ</b");
  strcat(str1,str2);
  
  set_br(str2,1);
  strcat(str1,str2);
  
  reset_open_block(str2,"h2");
  strcat(str1,str2);
  
  set_open_block(str2,"h3");
  strcat(str1,str2);
  // Table   
   set_open_block(str2,"table border=\"1\" style=\"border-collapse: collapse; width: 80%;border: 1px solid #ffffff;margin: auto;required\"><tbody");
   strcat(str1,str2);    

  
   memset (str3,0, sizeof(str3));

   set_checkonoff (str3,"V_FLAG_EN_EMAIL",FW_data.V_FLAG_EN_EMAIL);// (char* str1,char* name,uint8_t ck)
   set_table_string(str2,"Включить отправку e-mail уведомлений ",str3);
   strcat(str1,str2);
   len=strlen(str1); 
   
    memset (str3,0, sizeof(str3));

   set_checkbox (str3,"V_FLAG_DEF_EMAIL","1","1",FW_data.V_FLAG_DEF_EMAIL);// (char* str1,char* name,uint8_t ck)
   set_table_string(str2,"Использовать SMTP сервер по умолчанию ",str3);
   strcat(str1,str2);
   len=strlen(str1); 
   
    memset (str3,0, sizeof(str3));



  
  len=strlen(str1);
       while(len>3000){}
  return len;
};

 uint32_t costr_email_page2(char* str1)
{
  uint32_t len;
  
  memset (str1,0, sizeof(str1)); 
  memset (str2,0, sizeof(str2));
  memset (str3,0, sizeof(str3));
  memset (str4,0, sizeof(str4));  
   

 
   

  
  
    memset (str3,0, sizeof(str3));
     
     
   set_text_input(str3,"V_EMAIL_ADDR",32,FW_data.V_EMAIL_ADDR);  
   set_table_string(str2,"Адрес SMTP сервера",str3);
   strcat(str1,str2);
   
    memset (str3,0, sizeof(str3));
     
     

   
    sprintf(str4,"%d",FW_data.V_FLAG_EMAIL_PORT);
   set_intnum(str3,"V_FLAG_EMAIL_PORT",4,str4,1,65535);  
   
   
   set_table_string(str2,"Порт SMTP сервера",str3);
   strcat(str1,str2);
   
    memset (str3,0, sizeof(str3));
     
     
   set_text_input(str3,"V_LOGIN_SMTP",32,FW_data.V_LOGIN_SMTP);  
   set_table_string(str2,"Имя пользователя",str3);
   strcat(str1,str2);
   
    memset (str3,0, sizeof(str3));
   
   set_text_input_PW(str3,"V_PASSWORD_SMTP",32,FW_data.V_PASSWORD_SMTP);  
   set_table_string(str2,"Пароль",str3);  
   strcat(str1,str2);
   
   
    memset (str3,0, sizeof(str3));
     
     
   set_text_input(str3,"V_EMAIL_FROM",32,FW_data.V_EMAIL_FROM);  
   set_table_string(str2,"От кого (From:)",str3);
   strcat(str1,str2);
   
    memset (str3,0, sizeof(str3));
     
     
   set_text_input(str3,"V_EMAIL_TO",32,FW_data.V_EMAIL_TO);  
   set_table_string(str2,"Кому (to:)",str3);
   strcat(str1,str2);
   
     
  len=strlen(str1);
       while(len>3000){}
  return len;
};
   
 uint32_t costr_email_page3(char* str1)
{
  uint32_t len;
  
  memset (str1,0, sizeof(str1)); 
  memset (str2,0, sizeof(str2));
  memset (str3,0, sizeof(str3));
  memset (str4,0, sizeof(str4));    
  
     
     
   set_text_input(str3,"V_EMAIL_CC1",32,FW_data.V_EMAIL_CC1);  
   set_table_string(str2,"Копия №1(cc:)",str3);
   strcat(str1,str2);
   
   
    memset (str3,0, sizeof(str3));
     
     
   set_text_input(str3,"V_EMAIL_CC2",32,FW_data.V_EMAIL_CC2);  
   set_table_string(str2,"Копия №2(cc:)",str3);
   strcat(str1,str2);
   
  
   
    memset (str3,0, sizeof(str3));
    
    set_text_input(str3,"V_EMAIL_CC3",32,FW_data.V_EMAIL_CC3);  
    set_table_string(str2,"Копия №3(cc:)",str3);
    strcat(str1,str2);
    
    set_submit(str3,"save_rasp_sec5 ","1","Применить изменения",0);
    set_table_string(str2,"",str3);  
    strcat(str1,str2);

   reset_open_block(str2,"h3");
   strcat(str1,str2);
   
 
   reset_open_block(str2,"tbody");
   strcat(str1,str2);
  
   reset_open_block(str2,"table");
   strcat(str1,str2);
   
   

   
   

  reset_open_block(str2,"form");
  strcat(str1,str2);
  
  set_open_block(str2,"form id=\"sets1\" action='/' method='POST'");
 strcat(str1,str2);
  
    set_open_block(str2,"h4");
  strcat(str1,str2);
  
    set_br(str2,1);
  strcat(str1,str2);
  
  
  set_submit(str3,"test_email","1","Тестовая отправка e-mail ",0);
  strcat(str1,str3);
   
  set_br(str2,1);
  strcat(str1,str2);
   
     reset_open_block(str2,"h4");
  strcat(str1,str2);
  
  reset_open_block(str2,"form");
  strcat(str1,str2);
  
  


  reset_open_block(str2,"html");
  strcat(str1,str2);
  
  len=strlen(str1);
       while(len>3000){}
  return len;
};

uint32_t costr_page_boot(char* str1)
{
  uint32_t len;
  

 // len=sizeof(str1)+1;
  memset (str1,0, sizeof(str1)); 
  memset (str2,0, sizeof(str2));
  memset (str3,0, sizeof(str3));
  memset (str4,0, sizeof(str4));
  
  
  strcat(str1,http_html_start_constr);
  

  strcat(str1,http_html_style);
  
  set_open_block(str2,"body");
  strcat(str1,str2);
  

  
 
  set_string_text(str2,"Интеллектуальная розетка DKST 59 v","h1",0,FW_data.V_FW1_VER);
  strcat(str1,str2);
    

  
    set_br(str2,1);
  strcat(str1,str2);
  
  
  set_open_block(str2,"h2");
  strcat(str1,str2);
  
  
  set_link(str2,"Главная","index.html");
  strcat(str1,str2);
  
  set_link(str2,"Настройки ","settings.html");
  strcat(str1,str2);
  
  set_link(str2,"Расписание ","rasp.html");
  strcat(str1,str2);
  
  set_link(str2,"Сторож","watchdog.html");
  strcat(str1,str2);
  
  
  set_link(str2,"E-mail","email.html");
  strcat(str1,str2);  

  
  set_link(str2,"Журнал","logs.html");
  strcat(str1,str2);  
    
  reset_open_block(str2,"h2");
  strcat(str1,str2);
    
  set_open_block(str2,"ul");
  strcat(str1,str2);
  
   set_open_block(str2,"h2");
  strcat(str1,str2);
  
  set_br(str2,3);
  strcat(str1,str2);
         

  set_open_block(str2,"b>УСТРОЙСТВО В РЕЖИМЕ ЗАГРУЗКИ ОБНОВЛЕНИЯ ПО</b");
  strcat(str1,str2);
  
   set_br(str2,2);
  strcat(str1,str2);
  
  set_open_block(str2,"b>Запустите утилиту отправки файлов по TFTP протоколу </b");
  strcat(str1,str2);
  
  set_br(str2,2);
  strcat(str1,str2);
  
  set_open_block(str2,"b>Адрес для загрузки:</b");
  strcat(str1,str2);
  
  sprintf(str2,"%d\.%d\.%d\.%d",FW_data.V_IP_CONFIG[0],FW_data.V_IP_CONFIG[1],FW_data.V_IP_CONFIG[2],FW_data.V_IP_CONFIG[3]);
  strcat(str1,str2);
  
  set_br(str2,2);
  strcat(str1,str2);
  
  set_open_block(str2,"b>Порт №: 69</b");
  strcat(str1,str2);
  
  set_br(str2,2);
  strcat(str1,str2);
  
  set_open_block(str2,"b>Размер блока: 512 байт</b");
  strcat(str1,str2);
  
  set_br(str2,2);
  strcat(str1,str2);
  
  set_open_block(str2,"b>После обновления перейдите по ссылке на главную страницу</b");
  strcat(str1,str2);
  
  set_br(str2,2);
  strcat(str1,str2);
  
  reset_open_block(str2,"h2");
  strcat(str1,str2);
  
 
  
  set_open_block(str2,"h3");
  strcat(str1,str2);

   
   
     len=strlen(str1);
     while(len>3000){}
     return len;
};
   
  
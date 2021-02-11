#include <string.h>
#include "html_page.h"
#include "flash_if.h"
#include "LOGS.h"
uint32_t ct_logoff_time;
uint8_t flag_logon=0;

//const char Name_dev[]="DKST 59";
const char contact_data[]="http://www.netping.ru/";
const char model_dev[]="UniPing port";

unsigned char key_http[30]="asfasdvas";
int key_http_len = 10;
char HTTP401[]={0x48,0x54,0x54,0x50,0x2f,0x31,0x2e,0x31,0x20,0x34,0x30,0x31,0x20,0x55,0x6e,0x61,   //HTTP/1.1 401 Una
0x75,0x74,0x68,0x6f,0x72,0x69,0x7a,0x65,0x64,0x0d,0x0a,0x57,0x57,0x57,0x2d,0x41,   //uthorized..WWW-A
0x75,0x74,0x68,0x65,0x6e,0x74,0x69,0x63,0x61,0x74,0x65,0x3a,0x20,0x42,0x61,0x73,   //uthenticate: Bas
0x69,0x63,0x20,0x72,0x65,0x61,0x6c,0x6d,0x3d,0x22}; 

char HTTP401end[]={0x22,0x0d,0x0a,0x0d,0x0a};

static const char http_html_hdr[] = "HTTP/1.1 200 OK\r\nContent-type: text/html\r\n\r\n";
  //char * data[]="<a href=\"settings.html\" target=\"_self\" rel=\"nofollow\">????????? &emsp; </a>";
static const char http_html_start_constr[] = "\<!DOCTYPE html> <body onload=\"onload()\"\>  <a href=\"http:\/\/www.netping.ru/\"><img src=\"img/netping.gif\" height=\"59\" width=\"169\" border=\"0\" alt=\"netping logo\" title=\"404 error\"></a> <html><head><meta http-equiv=\"Content-Type\" content=\"text/html; charset=utf-8\" />\<script\>"
	  "var xhr\;"
           "var idTimer1\";"
	   "function onload(){"
	   "xhr = new (XMLHttpRequest);"
	   "}"
           
	   "function swich1(){"
           "xhr.open(\"GET\", \"swich?c=1\", true);"
	   "xhr.responseType = \"text\";"
	   "xhr.send(null);"
	   "}"	
	  "function swich2(){"
	  "xhr.open(\"GET\", \"swich?c=2\", true);"
	  "xhr.responseType = \"text\";"
	  "xhr.send(null);"
	  "}"
	  "function swich3(){"
	   "xhr.open(\"GET\", \"swich?c=3\", true);"
	    "xhr.responseType = \"text\";"
	    "xhr.send(null);"
	  "}"
"</script>";


static const char http_html_style[] = 
"<title>NetPing</title>"
"  <style >"
"body {"
    "background: #F6FCF9;" /* Цвет фона */
    //"color: #fff;" /* Цвет текста */
   "}"
"    lf {"
	"font-family: Tahoma;"//Courier;" /* Гарнитура шрифта */
	"src: url(fonts/Courier.ttf);" /* Путь к файлу со шрифтом */
///////////////	"font-family: Courier, 'Comic Sans MS', cursive;"
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
        

         
   "display: inline-block;"
  "</style>"
    "</head>";

uint32_t costr_pass(char* str1)
{
uint32_t len_out;
memset (str1,0, 5000);
memcpy(str1,(char*)(HTTP401),sizeof(HTTP401));
strcat(str1,"Name_dev");
strcat(str1,(char*)HTTP401end);
len_out=strlen(str1);        
return len_out;
}
uint8_t pass_compar(char* in_buf)
{
uint8_t ct_pass;
uint8_t len_login,len_pass;
char login_in[32] = {0};
char pass_in[32]= {0};
for(ct_pass=23;ct_pass<120;ct_pass++)
  {
    if (((char)(*(in_buf+ct_pass)))=='&')
      {
       len_login = ct_pass-23;
      }
    if (in_buf[ct_pass]==' ')
      {
        len_pass=ct_pass-23-len_login-10;
        ct_pass=120;
      }
   
  }
 
    memcpy(login_in,(char*)(in_buf+23),len_login);
    memcpy(pass_in,(char*)(in_buf+23+len_login+10),len_pass);
    if (len_pass==0)len_pass++;
    if (len_login==0)len_login++;
    if (((strncmp((char*)FW_data.V_LOGIN,(char*)login_in,len_login))==0)&&((strncmp((char*)FW_data.V_PASSWORD,(char*)pass_in,len_pass))==0))
    {
      return 1;
    }
return 0;
}



uint16_t set_open_block (char* str1,char* name)
{ 

uint16_t len;
sprintf(str1,"<%s>",name);
return len;
}
uint16_t reset_open_block (char* str1,char* name)
{ 

uint16_t len;
sprintf(str1,"<%s/>",name);
return len;
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

uint16_t len;
for(;n>0;n--)
{
  strcat(str3,"&ensp;");
}
sprintf(str4,"%d.%d.%d.%d",dat[0],dat[1],dat[2],dat[3]);
sprintf(str1,"<%s> %s %s %s <%s/>",style,name,str3,str4,style);

return len;
}


//<textarea name="NAME_TEXT" placeholder="DATA_DEFAULT" cols="16" rows="1" maxlength="16" required></textarea>

uint16_t set_text_area  (char* str1,char* name, uint8_t n,uint8_t c, uint8_t r,uint8_t* dat)
{ 
////char str3[256];
////char str4[64];
////memset (str3,0, 256);


//sprintf(str4,"%d.%d.%d.%d",dat[0],dat[1],dat[2],dat[3]);
sprintf(str1,"<textarea name=\"%s\" placeholder=\"%s\" cols=\"%d\" rows=\"%d\" maxlength=\"%d\" required></textarea> ",name,dat,c,r,n);

return 0;
}
uint16_t set_text_input (char* str1,char* name, uint8_t n,uint8_t* dat)
{ 
////char str3[256];
////char str4[64];
////memset (str3,0, 256);


//sprintf(str4,"%d.%d.%d.%d",dat[0],dat[1],dat[2],dat[3]);
sprintf(str1,"<input name=\"%s\"  value=\"%s\" maxlength=\"%d\" >",name,dat,n);

return 0;
}

//<input type="text"  placeholder=\"%s\" maxlength="число">

////  len=len+set_open_block(str2,"h4");
////  strcat(str1,str2);
////  
////  len=len+sizeof("Драйвер лампы и фона HW2.0 ");
////  strcat(str1,"Драйвер лампы и фона HW2.0 ");
////  
////  len=len+reset_open_block(str2,"h4");
////  strcat(str1,str2);
////  
  
  

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
 
uint16_t len; 
sprintf(str1,"<a href=\"%s\" target=\"_self\" rel=\"nofollow\">%s </a>",link,name);


return len;
}
uint16_t set_button (char* str1,char* name,char* value)
{ 

//<button type="button" name="Save_power" formtarget="_self" onclick="Save_Power()" >Save Power </button>	

sprintf(str1,"<button type=\"button\" name=\"%s\" formtarget=\"_self\" onclick=\"%s\">%s</button>\n\r",name,value,name);

return 0;
}


 uint16_t set_submit (char* str1,char* name,char* value,char* text)
{ 

//<button type="button" name="Save_power" formtarget="_self" onclick="Save_Power()" >Save Power </button>	

sprintf(str1,"<button type=\"submit\" name=\"%s\" formtarget=\"_self\" value=\"%s\">%s</button>\n\r",name,value,text);

return 0;
}

//<p style="text-align:center"><input name="Start_boot" required="required" type="checkbox" value="boot" />&ensp; <input type="submit" value="Start frimware update? Start bootloader " /></p>
//<label><input type="checkbox" name="name"> messeg</label>
 uint16_t set_checkbox (char* str1,char* name,char* value)
{ 

//<button type="button" name="Save_power" formtarget="_self" onclick="Save_Power()" >Save Power </button>	

sprintf(str1,"<label><input type=\"checkbox\" name=\"%s\"> %s</label>",name,value);

return 0;
}

 uint16_t set_checkbox_bot (char* str1,char* name,char* value,char* mess)
{ 

//<p style="text-align:center"><input name="Start_boot" required="required" type="checkbox" value="boot" />&ensp; <input type="submit" value="Start frimware update? Start bootloader " /></p>

sprintf(str1,"<p style=\"text-align:center\"><input name=\"%s\" required=\"required\" type=\"checkbox\" value=\"%s\" />&ensp; <input type=\"submit\" value=\"%s\" /></p>",name,value,mess);

return 0;
}



uint16_t set_intext (char* str1,char* name,char* size,char* value)
{ 
///<input type="text" name="IP_addres"  size='15' value="192.168.000.122">&ensp;
///<input type="text" name=""  size='' value="">&ensp;
uint16_t len;
sprintf(str1,"<input type=\"text\" name=\"%s\"  size='%s' value=\"%s\">&ensp;",name,size,value);

return len;
}


uint16_t set_table_string (char* str1,char* name,char* value)
{ 
///<tr><td style="width: 90%; text-align: left ;border-bottom: 1px solid #36BA88; background: #BFE8BB;  "></td><td style="width: 10%; text-align: center;border-bottom: 1px solid #36BA88; background: #BFE8BB; "></td></tr> 
uint16_t len;
sprintf(str1,"<tr><td style=\"width: 80%; text-align: left ;border-bottom: 1px solid #36BA88; background: #BFE8BB;  \">&ensp;\%s</td><td style=\"width: 20%; font-size: 10pt; text-align: center;border-bottom: 1px solid #36BA88; background: #BFE8BB; \">%s</td></tr>",name,value);
return len;

}
//uint16_t set_submit (char* str1,char* name,char* value)
//{
//
////<button type="button" name="Save_power" formtarget="_self" oncl ick="Save_Power()" >Save Power </button>    
//
//sprintf(str1,"<button type=\"submit\" name=\"%s\" formtarget=\"_self\" value=\"%s\">%s</button>\n\r",name,value,name);
//
//return 0;
//}



uint32_t costr_page2(char* str1)
{
  uint32_t len;
  
  char str2[1000]={0};
    char str3[128]={0};
 // len=sizeof(str1)+1;
  
  memset (str1,0, 5000);

  strcat(str1,http_html_start_constr);
  

  strcat(str1,http_html_style);
  
  set_open_block(str2,"body");
  strcat(str1,str2);
  
  set_open_block(str2,"ul");
  strcat(str1,str2);
  
  
  set_open_block(str2,"lf");
  strcat(str1,str2);
  
 
  set_string_text(str2,"Интеллектуальная розетка DKST 59 v","h4",0,FW_data.V_FW1_VER);
  strcat(str1,str2);
    
  reset_open_block(str2,"lf");
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

  reset_open_block(str2,"ul");
  strcat(str1,str2);
  
  
  
  
  
  set_open_block(str2,"form action='/' method='POST'");
  strcat(str1,str2);
  
  set_open_block(str2,"ul");
  strcat(str1,str2);
  
  
  set_br(str2,3);
  strcat(str1,str2);
         

  set_open_block(str2,"b>ОБЩАЯ ИНФОРМАЦИЯ</b");
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
   sprintf(str3,"v%d.%d",FW_data.V_FW1_VER[0],FW_data.V_FW1_VER[1]);
 
   set_table_string(str2,"Версия ПО",str3);  
   strcat(str1,str2);
   
   memset(str3,0,128);
   sprintf(str3,"v%d.%d",FW_data.V_FW1_VER[2],FW_data.V_FW1_VER[3]);
 
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
   
   ct_time++;
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
  return len;
};




uint32_t costr_page4(char* str1)
{
  uint32_t len;
    char str3[128]={0};
  char str2[1000]={0};
 // len=sizeof(str1)+1;
  
  memset (str1,0, 5000);
    set_open_block(str2,"h2");
  strcat(str1,str2);
  
  set_br(str2,3);
  strcat(str1,str2);
  
  set_open_block(str2,"b>НАСТРОЙКА  СЕТИ</b");
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
;
  
  reset_open_block(str2,"ul");
  strcat(str1,str2);
  
  reset_open_block(str2,"form");
  strcat(str1,str2);
  
  
  
  
  
  set_open_block(str2,"form action='/' method='POST'");
  strcat(str1,str2);
  
  set_open_block(str2,"ul");
  strcat(str1,str2);
  
   set_br(str2,2);
  strcat(str1,str2); 
  
  set_open_block(str2,"b>Выполнить перезагрузку</b");
  strcat(str1,str2);
  
   set_space(str2,2);
  strcat(str1,str2); 
  
  
  
  set_checkbox_bot(str2,"check_res","1","RESET");
  strcat(str1,str2); 
////    set_checkbox(str2,"check_res","");
////  strcat(str1,str2);  
////  
////  
////  set_submit(str2,"RESET","1");
////  strcat(str1,str2);  
  
     set_space(str2,6);
  strcat(str1,str2); 
  
  reset_open_block(str2,"ul");
  strcat(str1,str2);
  
  reset_open_block(str2,"form");
  strcat(str1,str2);
  
  len=strlen(str1);
  return len;
};


uint32_t costr_page3(char* str1)
{
  uint32_t len;
  
  char str2[1000]={0};
    char str3[128]={0};
 // len=sizeof(str1)+1;
  
  memset (str1,0, 4000);

  strcat(str1,http_html_start_constr);
  

  strcat(str1,http_html_style);
  
  set_open_block(str2,"body");
  strcat(str1,str2);
  
  set_open_block(str2,"ul");
  strcat(str1,str2);
  
  
  set_open_block(str2,"lf");
  strcat(str1,str2);
  
 
  set_string_text(str2,"Интеллектуальная розетка DKST 59 v","h4",0,FW_data.V_FW1_VER);
  strcat(str1,str2);
    
  reset_open_block(str2,"lf");
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
////
////  set_open_block(str2,"form action='/' method='GET'");
////  strcat(str1,str2);
 //    len=strlen(str1);
  
  
  
  set_open_block(str2,"form action='/' method='POST'");
 strcat(str1,str2);
  
//  set_open_block(str2,"ul");
//  strcat(str1,str2);

  set_br(str2,2);
  strcat(str1,str2);

  set_open_block(str2,"b>ОБЩАЯ ИНФОРМАЦИЯ</b");
  strcat(str1,str2);
  
  set_br(str2,1);
  strcat(str1,str2);
  
  set_open_block(str2,"h3");
  strcat(str1,str2);
  // Table   
   set_open_block(str2,"table border=\"1\" style=\"border-collapse: collapse; width: 80%;border: 1px solid #ffffff;margin: auto;required\"><tbody");
   strcat(str1,str2);    

  //uint16_t set_text_area  (char* str1,char* name, uint8_t n,uint8_t c, uint8_t r,uint8_t* dat)
   
     
   set_text_input(str3,"name_dev",16,FW_data.V_Name_dev);  
   set_table_string(str2,"Имя устройства",str3);
   strcat(str1,str2);
   
   set_text_input(str3,"geo_place",32,FW_data.V_GEOM_NAME);  
   set_table_string(str2,"Место установки устройства",str3);  
   strcat(str1,str2);
   
   set_text_input(str3,"call_data",32,FW_data.V_CALL_DATA);  
   set_table_string(str2,"Контактные данные",str3);  
   strcat(str1,str2);
   
//   set_submit(str3,"save_geo","1","Сохранить");
//   set_table_string(str2,"Сохранить общие настройки ",str3);  
//   strcat(str1,str2);
  set_open_block(str2,"h3");
  strcat(str1,str2);
   
 
   reset_open_block(str2,"tbody");
   strcat(str1,str2);
  
   reset_open_block(str2,"table");
   strcat(str1,str2);
   

  reset_open_block(str2,"form");
  strcat(str1,str2);
  
  
  
 
  reset_open_block(str2,"tbody"); 
  strcat(str1,str2); 
  
  reset_open_block(str2,"table");
  strcat(str1,str2);
  

  
  reset_open_block(str2,"form");
 strcat(str1,str2);

  
  len=strlen(str1);
  return len;
};



uint32_t costr_page5(char* str1)
{
  uint32_t len;
  
  char str2[1000]={0};
  char str3[128]={0};
  char str4[128]={0};
 // len=sizeof(str1)+1;
  
  memset (str1,0, 4000);

  set_open_block(str2,"form action='/' method='POST'");
  strcat(str1,str2);
  
  
  set_br(str2,1);
  strcat(str1,str2);
  
  
    set_open_block(str2,"h2");
  strcat(str1,str2);
 
  set_open_block(str2,"b>НАСТРОЙКА СЕТИ</b");
  strcat(str1,str2);
  
  
  reset_open_block(str2,"h2");
  strcat(str1,str2);
  
  
//  set_br(str2,1);
//  strcat(str1,str2);
  
  // Table   
   set_open_block(str2,"table border=\"1\" style=\"border-collapse: collapse; width: 80%;border: 1px solid #ffffff;margin: auto;required\"><tbody");
   strcat(str1,str2);    

   //<input type="checkbox" name="option1" value="a1" checked>
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
   set_table_string(str2,"IP адресс",str3);
   strcat(str1,str2);
   
   sprintf(str4,"%d\.%d\.%d\.%d",FW_data.V_IP_MASK[0],FW_data.V_IP_MASK[1],FW_data.V_IP_MASK[2],FW_data.V_IP_MASK[3]);
   set_text_input(str3,"mask_addr",32,str4);  
   set_table_string(str2,"Маска подсети ",str3);  
   strcat(str1,str2);
   
   sprintf(str4,"%d\.%d\.%d\.%d",FW_data.V_IP_GET[0],FW_data.V_IP_GET[1],FW_data.V_IP_GET[2],FW_data.V_IP_GET[3]);
   set_text_input(str3,"getway_addr",32,str4);  
   set_table_string(str2,"Шлюз",str3);  
   strcat(str1,str2);
   
   sprintf(str4,"%d\.%d\.%d\.%d",FW_data.V_IP_DNS[0],FW_data.V_IP_DNS[1],FW_data.V_IP_DNS[2],FW_data.V_IP_DNS[3]);
   set_text_input(str3,"dns_addr",32,str4);  
   set_table_string(str2,"DNS сервер",str3);  
   strcat(str1,str2);
   
   sprintf(str4,"%d",FW_data.V_WEB_PORT);
   set_text_input(str3,"port_http",32,str4);  
   set_table_string(str2,"Порт HTTP сервера",str3);  
   strcat(str1,str2);
   

   
//   set_submit(str3,"save_net","1","Сохранить");
//   set_table_string(str2,"Сохранить настройки сети ",str3);  
//   strcat(str1,str2);

   
 
  reset_open_block(str2,"tbody"); 
  strcat(str1,str2); 
  
  reset_open_block(str2,"table");
  strcat(str1,str2);
  
  reset_open_block(str2,"form");
  strcat(str1,str2);
   
  len=strlen(str1);
  return len;
};

uint32_t costr_page6(char* str1)
{
  uint32_t len;
  
  char str2[1000]={0};
    char str3[128]={0};
     char str4[128]={0};
 // len=sizeof(str1)+1;
  
  memset (str1,0, 4000);

  set_open_block(str2,"form action='/' method='POST'");
  strcat(str1,str2);
  
  
  set_br(str2,1);
  strcat(str1,str2);
  
  
  set_open_block(str2,"h2");
  strcat(str1,str2);
 
  set_open_block(str2,"b>НАСТРОЙКА ДОСТУПА</b");
  strcat(str1,str2);
  
  
  reset_open_block(str2,"h2");
  strcat(str1,str2);
  
  
//  set_br(str2,1);
//  strcat(str1,str2);
  
  // Table   
   set_open_block(str2,"table border=\"1\" style=\"border-collapse: collapse; width: 80%;border: 1px solid #ffffff;margin: auto;required\"><tbody");
   strcat(str1,str2);    


     
     
   set_text_input(str3,"login",16,FW_data.V_LOGIN);  
   set_table_string(str2,"Имя пользователя",str3);
   strcat(str1,str2);
   
   set_text_input(str3,"password",32,FW_data.V_PASSWORD);  
   set_table_string(str2,"Пароль",str3);  
   strcat(str1,str2);
   

  reset_open_block(str2,"tbody"); 
  strcat(str1,str2); 
  
  reset_open_block(str2,"table");
  strcat(str1,str2);
  
  reset_open_block(str2,"form");
  strcat(str1,str2);
  
  
  
  
  
  set_open_block(str2,"form action='/' method='POST'");
  strcat(str1,str2);
  
  
  set_br(str2,1);
  strcat(str1,str2);
  
  
    reset_open_block(str2,"h2");
  strcat(str1,str2);
 
  set_open_block(str2,"b>SNMP TRAP</b");
  strcat(str1,str2);
  
  
  reset_open_block(str2,"h2");
  strcat(str1,str2);
  
  
  
//  set_br(str2,1);
//  strcat(str1,str2);
  
  // Table   
   set_open_block(str2,"table border=\"1\" style=\"border-collapse: collapse; width: 80%;border: 1px solid #ffffff;margin: auto;required\"><tbody");
   strcat(str1,str2);    
   
   sprintf(str4,"%d\.%d\.%d\.%d",FW_data.V_IP_SNMP[0],FW_data.V_IP_SNMP[1],FW_data.V_IP_SNMP[2],FW_data.V_IP_SNMP[3]);  
   set_text_input(str3,"snmp_addr",32,str4);  
   set_table_string(str2,"Адрес для посылки Trap сообщений",str3);  
   strcat(str1,str2);
   
   sprintf(str4,"%d",FW_data.V_PORT_SNMP);
   set_text_input(str3,"port_snmp",32,str4);  
   set_table_string(str2,"Порт SNMP агента",str3);  
   strcat(str1,str2);
      
//   set_submit(str3,"save_net","1","Сохранить");
//   set_table_string(str2,"Сохранить настройки сети ",str3);  
//   strcat(str1,str2);

   
 
  reset_open_block(str2,"tbody"); 
  strcat(str1,str2); 
  
  reset_open_block(str2,"table");
  strcat(str1,str2);
  
  reset_open_block(str2,"form");
  strcat(str1,str2);
  
  
  set_open_block(str2,"form action='/' method='POST'");
  strcat(str1,str2);
  
  
  set_br(str2,1);
  strcat(str1,str2);
  
  
  set_open_block(str2,"h2");
  strcat(str1,str2);
 
  set_open_block(str2,"b>НАСТРОЙКА ВСТРОЕННЫХ ЧАСОВ</b");
  strcat(str1,str2);
  
  
  reset_open_block(str2,"h2");
  strcat(str1,str2);
  
  
//  set_br(str2,1);
//  strcat(str1,str2);
  
  // Table   
   set_open_block(str2,"table border=\"1\" style=\"border-collapse: collapse; width: 80%;border: 1px solid #ffffff;margin: auto;required\"><tbody");
   strcat(str1,str2);    
   
   sprintf(str4,"%d\.%d\.%d\.%d",FW_data.V_IP_NTP1[0],FW_data.V_IP_NTP1[1],FW_data.V_IP_NTP1[2],FW_data.V_IP_NTP1[3]);  
   set_text_input(str3,"ntp1_addr",32,str4);  
   set_table_string(str2,"NTP сервер 1",str3);  
   strcat(str1,str2);
   
   sprintf(str4,"%d\.%d\.%d\.%d",FW_data.V_IP_NTP2[0],FW_data.V_IP_NTP2[1],FW_data.V_IP_NTP2[2],FW_data.V_IP_NTP2[3]);  
   set_text_input(str3,"ntp2_addr",32,str4);  
   set_table_string(str2,"NTP сервер 2",str3);  
   strcat(str1,str2);
   
   sprintf(str4,"%d",FW_data.V_NTP_CIRCL);
   set_text_input(str3,"time_circl",32,str4);  
   set_table_string(str2,"Часовой пояс UTC:-12...+12 ",str3);  
   strcat(str1,str2);
      
      GET_reple(0,&real_time);  
   
//
//   
//   time_run[0]=real_time.year
//   time_run[1]=real_time.month
//   time_run[2]=real_time.day
//   time_run[3]=real_time.reple_hours
//   time_run[4]=real_time.reple_minuts
//   time_run[5]=real_time.reple_seconds
   
   
   sprintf(str4,"%d\.%d\.%d ",real_time.year,real_time.month,real_time.day);  
   set_text_input(str3,"time_set",32,str4);  
   set_table_string(str2,"Текущая дата:",str3);  
   strcat(str1,str2);
   
   
   sprintf(str4,"%d\.%d\.%d ",real_time.reple_hours,real_time.reple_minuts,real_time.reple_seconds);  
   set_text_input(str3,"time_set",32,str4);  
   set_table_string(str2,"Текущее время: ",str3);  
   strcat(str1,str2);
   
//   set_submit(str3,"save_all","1","Сохранить");
//   set_table_string(str2,"Сохранить настройки ",str3);  
//   strcat(str1,str2);

   
 
  reset_open_block(str2,"tbody"); 
  strcat(str1,str2); 
  
  reset_open_block(str2,"table");
  strcat(str1,str2);
  
  reset_open_block(str2,"form");
  strcat(str1,str2);
  
  
  
   
  len=strlen(str1);
  return len;
};

uint32_t costr_page7(char* str1)
{
  uint32_t len;
  
  char str2[1000]={0};
  char str3[128]={0};
  char str4[128]={0};
 // len=sizeof(str1)+1;
  
  memset (str1,0, 4000);

  set_open_block(str2,"form action='/' method='POST'");
  strcat(str1,str2);
  
  
  set_br(str2,1);
  strcat(str1,str2);
  
  
  set_open_block(str2,"h2");
  strcat(str1,str2);
 
  set_open_block(str2,"b>НАСТРОЙКА УПРАВЛЕНИЯ НАГРУЗКОЙ</b");
  strcat(str1,str2);
  
  
  reset_open_block(str2,"h2");
  strcat(str1,str2);
  
  
//  set_br(str2,1);
//  strcat(str1,str2);
  
  // Table   
   set_open_block(str2,"table border=\"1\" style=\"border-collapse: collapse; width: 80%;border: 1px solid #ffffff;margin: auto;required\"><tbody");
   strcat(str1,str2);   


     
     
   set_text_input(str3,"mess_on",32,FW_data.V_ON_MESS);  
   set_table_string(str2,"Сообщение при включении нагрузки",str3);
   strcat(str1,str2);
   
   set_text_input(str3,"mess_off",32,FW_data.V_OFF_MESS);  
   set_table_string(str2,"Сообщение при выключении нагрузки",str3);  
   strcat(str1,str2);
   
//   <p><input name="dzen" type="radio" value="nedzen"> Не дзен</p>
    
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
   
   set_submit(str3,"save_all","1","Сохранить");
   set_table_string(str2,"Сохранить настройки ",str3);  
   strcat(str1,str2);
   
    set_submit(str3,"load_def","1","Загрузить");
   set_table_string(str2,"Загрузить исходные настройки ",str3);  
   strcat(str1,str2);
   
   
   set_submit(str3,"load_boot","1","Загрузка ПО");
   set_table_string(str2,"Перевести устройство в режим обновления ПО",str3);  
   strcat(str1,str2);
   
//   set_submit(str3,"load_def","1","Загрузить");
//   set_table_string(str2,"Загрузить исходные настройки",str3);  
//   strcat(str1,str2);
   

  reset_open_block(str2,"tbody"); 
  strcat(str1,str2); 
  
  reset_open_block(str2,"table");
  strcat(str1,str2);
  
  
  reset_open_block(str2,"form");
  strcat(str1,str2);
//  
//  set_open_block(str2,"dev");
//  strcat(str1,str2);
  
//  
//  set_open_block(str2,"h2");
//  strcat(str1,str2);
//  
//  set_open_block(str2,"p");
//  strcat(str1,str2);
//  
//   set_checkbox_bot(str2,"load_def","1","Загрузить исходные настройки");  
//   strcat(str1,str2);
//   
//   reset_open_block(str2,"p");
//  strcat(str1,str2);
//  
//   set_open_block(str2,"p");
//  strcat(str1,str2);
//
//    set_checkbox_bot(str2,"load_boot","1","Перевести устройство в режим обновления ПО");  
//   strcat(str1,str2);
//   
//      reset_open_block(str2,"p");
//  strcat(str1,str2);
//  
//
//  
//  reset_open_block(str2,"h2");
//  strcat(str1,str2);

//  
//   reset_open_block(str2,"dev");
//  strcat(str1,str2);
  
  
  
  len=strlen(str1);
  return len;
};


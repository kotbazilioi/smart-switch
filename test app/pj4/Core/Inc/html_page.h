#include <stdint.h>

uint8_t pass_compar(char* in_buf);
uint32_t costr_pass(char* str1);
uint32_t costr_page1(char* str1);
uint32_t costr_page2(char* str1);
uint32_t costr_page3(char* str1);
uint32_t costr_page4(char* str1);
uint32_t costr_page5(char* str1);
uint32_t costr_page6(char* str1);
uint32_t costr_page7(char* str1);
extern uint32_t ct_logoff_time;
extern uint8_t flag_logon;
extern unsigned char key_http[30];
extern char HTTP401[];
extern char HTTP401end[];
extern int key_http_len;
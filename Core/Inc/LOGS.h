#include <stdint.h>
#include "flash_if.h"


void save_reple_log (log_reple_t reple);
void GET_reple (uint8_t event,log_reple_t* reple);
void form_reple_to_save (uint8_t event);
void decode_reple (char* out,log_reple_t* reple);
extern log_reple_t reple_to_save;
extern RTC_DateTypeDef dates;
extern RTC_TimeTypeDef times;

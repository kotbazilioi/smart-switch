#ifndef __PING_H__
#define __PING_H__

/**
 * PING_USE_SOCKETS: Set to 1 to use sockets, otherwise the raw api is used
 */
#ifndef PING_USE_SOCKETS
#define PING_USE_SOCKETS    LWIP_SOCKET
#endif
typedef struct  {
  uint8_t flag_err[3];
  uint32_t timeout[3]; 
}icmp_cntrl_t ;

void ping_init(void);
extern icmp_cntrl_t ping_data;
extern uint8_t en_ping_a,en_ping_b,en_ping_c;
extern uint8_t ct_max_res;
extern uint8_t flag_delay_ping;
extern uint16_t ct_cn_a,ct_cn_b,ct_cn_c;
#if !PING_USE_SOCKETS
void ping_send_now(void);
#endif /* !PING_USE_SOCKETS */

#endif /* __PING_H__ */

#ifndef __HTTPSERVER_NETCONN_H__
#define __HTTPSERVER_NETCONN_H__

#include "lwip/api.h"

void http_server_netconn_init(void);
void DynWebPage(struct netconn *conn);

#endif /* __HTTPSERVER_NETCONN_H__ */
extern uint8_t aTxBuffer0[];
extern uint8_t aTxBuffer1[];

//extern uint8_t aRxBuffer0[RXBUFFERSIZE];
//extern uint8_t aRxBuffer1[RXBUFFERSIZE];
//extern uint8_t aRxBuffer2[RXBUFFERSIZE];
//extern uint8_t aRxBuffer3[RXBUFFERSIZE];

extern uint8_t flag_tx;
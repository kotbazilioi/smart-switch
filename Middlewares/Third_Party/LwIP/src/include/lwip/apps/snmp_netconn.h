
u8_t snmp_get_local_ip_for_dst(void* handle, const ip_addr_t *dst, ip_addr_t *result);
err_t  snmp_sendto(void *handle, struct pbuf *p, const ip_addr_t *dst, u16_t port);
void snmp_init(void);
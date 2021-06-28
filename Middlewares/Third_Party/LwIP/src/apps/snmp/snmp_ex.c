/*
 * Redistribution and use in source and binary forms, with or without modification,
 * are permitted provided that the following conditions are met:
 *
 * 1. Redistributions of source code must retain the above copyright notice,
 *    this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright notice,
 *    this list of conditions and the following disclaimer in the documentation
 *    and/or other materials provided with the distribution.
 * 3. The name of the author may not be used to endorse or promote products
 *    derived from this software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE AUTHOR ``AS IS'' AND ANY EXPRESS OR IMPLIED
 * WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF
 * MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT
 * SHALL THE AUTHOR BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL,
 * EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT
 * OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
 * INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
 * CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING
 * IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY
 * OF SUCH DAMAGE.
 *
 * This file is part of the lwIP TCP/IP stack.
 *
 * Author: Dirk Ziegelmeier <dziegel@gmx.de>
 *
 */

#include "lwip/netif.h"
#include "lwip/apps/snmp.h"
#include "lwip/apps/snmp_core.h"
#include "lwip/apps/snmp_mib2.h"
//#include "lwip/apps/snmpv3.h"
#include "lwip/apps/snmp_snmpv2_framework.h"
#include "lwip/apps/snmp_snmpv2_usm.h"
//#include "examples/snmp/snmp_v3/snmpv3_dummy.h"
//#include "private_mib.h"
#include "snmp_msg.h"
#include "snmp_ex.h"
#include "snmp_netconn.h" 
#include "flash_if.h"
ip4_addr_t tipaddr;


struct snmp_varbind vbt_tr;
  struct snmp_obj_id oid_trtest={2,{1,3}}; 



#if LWIP_SNMP
static const struct snmp_mib *mibs[] = {
  

  &mib2_1,
 // &mib_private
#if LWIP_SNMP_V3
  , &snmpframeworkmib
  , &snmpusmmib
#endif
};
#endif /* LWIP_SNMP */

void snmp_ex_init(void)
{
   IP4_ADDR(&tipaddr, FW_data.V_IP_SNMP[0], FW_data.V_IP_SNMP[1], FW_data.V_IP_SNMP[2], FW_data.V_IP_SNMP[3]);
#if LWIP_SNMP
 // lwip_privmib_init();
#if SNMP_LWIP_MIB2
#if SNMP_USE_NETCONN
  snmp_threadsync_init(&snmp_mib2_lwip_locks, snmp_mib2_lwip_synchronizer);
#endif /* SNMP_USE_NETCONN */
  snmp_mib2_set_syscontact_readonly((const u8_t*)"netping", NULL);
  snmp_mib2_set_syslocation_readonly((const u8_t*)"netping.ru", NULL);
  snmp_mib2_set_sysdescr((const u8_t*)"SmartSwich", NULL);
#endif /* SNMP_LWIP_MIB2 */

#if LWIP_SNMP_V3
  snmpv3_dummy_init();
#endif


snmp_trap_dst_ip_set (0,&tipaddr);
snmp_trap_dst_enable (0,1);

snmp_set_mibs(mibs, LWIP_ARRAYSIZE(mibs));
  snmp_init();
  snmp_coldstart_trap();
  

 
 
    
    
#endif /* LWIP_SNMP */
}
void send_mess_trap (uint32_t* OID_TR,char* mess,uint16_t lens_mess)
{
  
  vbt_tr.value=mess;
  vbt_tr.value_len=lens_mess;
  vbt_tr.type=SNMP_ASN1_TYPE_OCTET_STRING;
  vbt_tr.oid.len=4;
  memcpy((uint32_t*)vbt_tr.oid.id,(uint32_t*)OID_TR,4*4);
  snmp_send_trap(0,SNMP_GENTRAP_ENTERPRISE_SPECIFIC,SNMP_GENTRAP_ENTERPRISE_SPECIFIC,&vbt_tr);
 // snmp_send_trap_specific(6,&vbt_tr);

}
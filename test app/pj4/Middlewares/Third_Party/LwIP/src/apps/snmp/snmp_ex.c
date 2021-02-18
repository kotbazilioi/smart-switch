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
#include "lwip/apps/snmp_mib2.h"
//#include "lwip/apps/snmpv3.h"
#include "lwip/apps/snmp_snmpv2_framework.h"
#include "lwip/apps/snmp_snmpv2_usm.h"
//#include "examples/snmp/snmp_v3/snmpv3_dummy.h"
#include "private_mib.h"
#include "snmp_msg.h"
#include "snmp_ex.h"
#include "snmp_netconn.h" 

#if LWIP_SNMP
static const struct snmp_mib *mibs[] = {
//  &mib2,
//  &mib_private
////#if LWIP_SNMP_V3
////  , &snmpframeworkmib
////  , &snmpusmmib
////#endif
//};
//#endif /* LWIP_SNMP */
//
//void
//snmp_init(void)
//{
//#if LWIP_SNMP
//  s32_t req_nr;
//  lwip_privmib_init();
//#if SNMP_LWIP_MIB2
//#if SNMP_USE_NETCONN
//  snmp_threadsync_init(&snmp_mib2_lwip_locks, snmp_mib2_lwip_synchronizer);
//#endif /* SNMP_USE_NETCONN */
//  snmp_mib2_set_syscontact_readonly((const u8_t*)"root", NULL);
//  snmp_mib2_set_syslocation_readonly((const u8_t*)"lwIP development PC", NULL);
//  snmp_mib2_set_sysdescr((const u8_t*)"lwIP example", NULL);
//#endif /* SNMP_LWIP_MIB2 */
//
////#if LWIP_SNMP_V3
////  snmpv3_dummy_init();
////#endif
//
//  snmp_set_mibs(mibs, LWIP_ARRAYSIZE(mibs));
//  snmp_init();
//
//  snmp_trap_dst_ip_set(0, &netif_default->gw);
//  snmp_trap_dst_enable(0, 1);
////
////  snmp_send_inform_generic(SNMP_GENTRAP_COLDSTART, NULL, &req_nr);
////  snmp_send_trap_generic(SNMP_GENTRAP_COLDSTART);
//
//#endif /* LWIP_SNMP */
//}
  
  
 &mib2,
  &mib_private
#if LWIP_SNMP_V3
  , &snmpframeworkmib
  , &snmpusmmib
#endif
};
#endif /* LWIP_SNMP */

void
snmp_ex_init(void)
{
#if LWIP_SNMP
  lwip_privmib_init();
#if SNMP_LWIP_MIB2
#if SNMP_USE_NETCONN
  snmp_threadsync_init(&snmp_mib2_lwip_locks, snmp_mib2_lwip_synchronizer);
#endif /* SNMP_USE_NETCONN */
  snmp_mib2_set_syscontact_readonly((const u8_t*)"root", NULL);
  snmp_mib2_set_syslocation_readonly((const u8_t*)"lwIP development PC", NULL);
  snmp_mib2_set_sysdescr((const u8_t*)"simhost", NULL);
#endif /* SNMP_LWIP_MIB2 */

#if LWIP_SNMP_V3
  snmpv3_dummy_init();
#endif

  snmp_set_mibs(mibs, LWIP_ARRAYSIZE(mibs));
  snmp_init();
#endif /* LWIP_SNMP */
}
//
//
//// ?????????? ? ?????? SNMP ---------------------------------------------------
//void SnmpPrepareForWork(void)
//{
// struct ip_addr xIpAddr;
// struct snmp_obj_id SnmpObjId;
//
//
// // ????????? sysName
// snmp_set_sysname((u8_t*)Sysname, (u8_t*)&Sysname_len);
// // ????????? sysDescr
// snmp_set_sysdesr((u8_t*)Sysdescr, (u8_t*)&Sysdescr_len);
// // ????????? sysContact
// snmp_set_syscontact((u8_t*)Syscontact, (u8_t*)&Syscontact_len);
// // ????????? sysLocation
// snmp_set_syslocation((u8_t*)Syslocation, (u8_t*)&Syslocation_len);
//
// // ?????? SNMP_SYSOBJID
// SnmpObjId.len = 9;
// SnmpObjId.id[0] = 1;
// SnmpObjId.id[1] = 3;
// SnmpObjId.id[2] = 6;
// SnmpObjId.id[3] = 1;
// SnmpObjId.id[4] = 4;
// SnmpObjId.id[5] = 1;
// SnmpObjId.id[6] = ENTERPRISE_ID;
// SnmpObjId.id[7] = 2;
// SnmpObjId.id[8] = 4;
// snmp_set_sysobjid(&SnmpObjId);
//
// // ????????? ??????
// if ((gSNMP_IPaddr1[0] & gSNMP_IPaddr1[1] & gSNMP_IPaddr1[2] & gSNMP_IPaddr1[3]) != 0xFF &&
//     (gSNMP_IPaddr1[0] | gSNMP_IPaddr1[1] | gSNMP_IPaddr1[2] | gSNMP_IPaddr1[3]) != 0)
// {
//   IP4_ADDR(&xIpAddr, gSNMP_IPaddr1[0], gSNMP_IPaddr1[1], gSNMP_IPaddr1[2], gSNMP_IPaddr1[3]);
//   snmp_trap_dst_enable(0, 1);
//   snmp_trap_dst_ip_set(0, &xIpAddr);
// }
// else
// {
//   snmp_trap_dst_enable(0, 0);
// }
//// if ((gSNMP_IPaddr2[0] & gSNMP_IPaddr2[1] & gSNMP_IPaddr2[2] & gSNMP_IPaddr2[3]) != 0xFF &&
////     (gSNMP_IPaddr2[0] | gSNMP_IPaddr2[1] | gSNMP_IPaddr2[2] | gSNMP_IPaddr2[3]) != 0)
//// {
////   IP4_ADDR(&xIpAddr, gSNMP_IPaddr2[0], gSNMP_IPaddr2[1], gSNMP_IPaddr2[2], gSNMP_IPaddr2[3]);
////   snmp_trap_dst_enable(1, 1);
////   snmp_trap_dst_ip_set(1, &xIpAddr);
//// }
//// else
//// {
////   snmp_trap_dst_enable(1, 0);
//// }
//// if ((gSNMP_IPaddr3[0] & gSNMP_IPaddr3[1] & gSNMP_IPaddr3[2] & gSNMP_IPaddr3[3]) != 0xFF &&
////     (gSNMP_IPaddr3[0] | gSNMP_IPaddr3[1] | gSNMP_IPaddr3[2] | gSNMP_IPaddr3[3]) != 0)
//// {
////   IP4_ADDR(&xIpAddr, gSNMP_IPaddr3[0], gSNMP_IPaddr3[1], gSNMP_IPaddr3[2], gSNMP_IPaddr3[3]);
////   snmp_trap_dst_enable(2, 1);
////   snmp_trap_dst_ip_set(2, &xIpAddr);
//// }
//// else
//// {
////   snmp_trap_dst_enable(2, 0);
//// }
//
// // ???????? ????? ????????? ??????
// snmp_coldstart_trap();
//}
//

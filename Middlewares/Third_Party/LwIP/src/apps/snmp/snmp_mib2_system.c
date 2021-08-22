/**
 * @file
 * Management Information Base II (RFC1213) SYSTEM objects and functions.
 */

/*
 * Copyright (c) 2006 Axon Digital Design B.V., The Netherlands.
 * All rights reserved.
 *
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
 * Author: Dirk Ziegelmeier <dziegel@gmx.de>
 *         Christiaan Simons <christiaan.simons@axon.tv>
 */

#include "lwip/snmp.h"
#include "lwip/apps/snmp.h"
#include "lwip/apps/snmp_core.h"
#include "lwip/apps/snmp_mib2.h"
#include "lwip/apps/snmp_table.h"
#include "lwip/apps/snmp_scalar.h"
#include "lwip/sys.h"
#include "flash_if.h"
#include <string.h>
#include "LOGS.h"
#include "app.h"
#if LWIP_SNMP && SNMP_LWIP_MIB2

#if SNMP_USE_NETCONN
#define SYNC_NODE_NAME(node_name) node_name ## _synced
#define CREATE_LWIP_SYNC_NODE(oid, node_name) \
   static const struct snmp_threadsync_node node_name ## _synced = SNMP_CREATE_THREAD_SYNC_NODE(oid, &node_name.node, &snmp_mib2_lwip_locks);
#else
#define SYNC_NODE_NAME(node_name) node_name
#define CREATE_LWIP_SYNC_NODE(oid, node_name)
#endif

/* --- system .1.3.6.1.2.1.1 ----------------------------------------------------- */

/** mib-2.system.sysDescr */
static const u8_t   sysdescr_default[] = SNMP_LWIP_MIB2_SYSDESC;
static const u8_t*  sysdescr           = sysdescr_default;
static const u16_t* sysdescr_len       = NULL; /* use strlen for determining len */

/** mib-2.system.sysContact */
static const u8_t   syscontact_default[]     = SNMP_LWIP_MIB2_SYSCONTACT;
static const u8_t*  syscontact               = syscontact_default;
static const u16_t* syscontact_len           = NULL; /* use strlen for determining len */
static u8_t*        syscontact_wr            = NULL; /* if writable, points to the same buffer as syscontact (required for correct constness) */
static u16_t*       syscontact_wr_len        = NULL; /* if writable, points to the same buffer as syscontact_len (required for correct constness) */
static u16_t        syscontact_bufsize       = 0;    /* 0=not writable */

/** mib-2.system.sysName */
static const u8_t   sysname_default[]        = SNMP_LWIP_MIB2_SYSNAME;
static const u8_t*  sysname                  = sysname_default;
static const u16_t* sysname_len              = NULL; /* use strlen for determining len */
static u8_t*        sysname_wr               = NULL; /* if writable, points to the same buffer as sysname (required for correct constness) */
static u16_t*       sysname_wr_len           = NULL; /* if writable, points to the same buffer as sysname_len (required for correct constness) */
static u16_t        sysname_bufsize          = 0;    /* 0=not writable */

/** mib-2.system.sysLocation */
static const u8_t   syslocation_default[]    = SNMP_LWIP_MIB2_SYSLOCATION;
static const u8_t*  syslocation              = syslocation_default;
static const u16_t* syslocation_len           = NULL; /* use strlen for determining len */
static u8_t*        syslocation_wr            = NULL; /* if writable, points to the same buffer as syslocation (required for correct constness) */
static u16_t*       syslocation_wr_len        = NULL; /* if writable, points to the same buffer as syslocation_len (required for correct constness) */
static u16_t        syslocation_bufsize       = 0;    /* 0=not writable */

/**
 * @ingroup snmp_mib2
 * Initializes sysDescr pointers.
 *
 * @param str if non-NULL then copy str pointer
 * @param len points to string length, excluding zero terminator
 */
void
snmp_mib2_set_sysdescr(const u8_t *str, const u16_t *len)
{
  if (str != NULL) {
    sysdescr     = str;
    sysdescr_len = len;
  }
}

/**
 * @ingroup snmp_mib2
 * Initializes sysContact pointers
 *
 * @param ocstr if non-NULL then copy str pointer
 * @param ocstrlen points to string length, excluding zero terminator.
 *        if set to NULL it is assumed that ocstr is NULL-terminated.
 * @param bufsize size of the buffer in bytes.
 *        (this is required because the buffer can be overwritten by snmp-set)
 *        if ocstrlen is NULL buffer needs space for terminating 0 byte.
 *        otherwise complete buffer is used for string.
 *        if bufsize is set to 0, the value is regarded as read-only.
 */
void
snmp_mib2_set_syscontact(u8_t *ocstr, u16_t *ocstrlen, u16_t bufsize)
{
  if (ocstr != NULL) {
    syscontact         = ocstr;
    syscontact_wr      = ocstr;
    syscontact_len     = ocstrlen;
    syscontact_wr_len  = ocstrlen;
    syscontact_bufsize = bufsize;
  }
}

/**
 * @ingroup snmp_mib2
 * see \ref snmp_mib2_set_syscontact but set pointer to readonly memory
 */
void
snmp_mib2_set_syscontact_readonly(const u8_t *ocstr, const u16_t *ocstrlen)
{
  if (ocstr != NULL) {
    syscontact         = ocstr;
    syscontact_len     = ocstrlen;
    syscontact_wr      = NULL;
    syscontact_wr_len  = NULL;
    syscontact_bufsize = 0;
  }
}


/**
 * @ingroup snmp_mib2
 * Initializes sysName pointers
 *
 * @param ocstr if non-NULL then copy str pointer
 * @param ocstrlen points to string length, excluding zero terminator.
 *        if set to NULL it is assumed that ocstr is NULL-terminated.
 * @param bufsize size of the buffer in bytes.
 *        (this is required because the buffer can be overwritten by snmp-set)
 *        if ocstrlen is NULL buffer needs space for terminating 0 byte.
 *        otherwise complete buffer is used for string.
 *        if bufsize is set to 0, the value is regarded as read-only.
 */
void
snmp_mib2_set_sysname(u8_t *ocstr, u16_t *ocstrlen, u16_t bufsize)
{
  if (ocstr != NULL) {
    sysname         = ocstr;
    sysname_wr      = ocstr;
    sysname_len     = ocstrlen;
    sysname_wr_len  = ocstrlen;
    sysname_bufsize = bufsize;
  }
}

/**
 * @ingroup snmp_mib2
 * see \ref snmp_mib2_set_sysname but set pointer to readonly memory
 */
void
snmp_mib2_set_sysname_readonly(const u8_t *ocstr, const u16_t *ocstrlen)
{
  if (ocstr != NULL) {
    sysname         = ocstr;
    sysname_len     = ocstrlen;
    sysname_wr      = NULL;
    sysname_wr_len  = NULL;
    sysname_bufsize = 0;
  }
}

/**
 * @ingroup snmp_mib2
 * Initializes sysLocation pointers
 *
 * @param ocstr if non-NULL then copy str pointer
 * @param ocstrlen points to string length, excluding zero terminator.
 *        if set to NULL it is assumed that ocstr is NULL-terminated.
 * @param bufsize size of the buffer in bytes.
 *        (this is required because the buffer can be overwritten by snmp-set)
 *        if ocstrlen is NULL buffer needs space for terminating 0 byte.
 *        otherwise complete buffer is used for string.
 *        if bufsize is set to 0, the value is regarded as read-only.
 */
void
snmp_mib2_set_syslocation(u8_t *ocstr, u16_t *ocstrlen, u16_t bufsize)
{
  if (ocstr != NULL) {
    syslocation         = ocstr;
    syslocation_wr      = ocstr;
    syslocation_len     = ocstrlen;
    syslocation_wr_len  = ocstrlen;
    syslocation_bufsize = bufsize;
  }
}

/**
 * @ingroup snmp_mib2
 * see \ref snmp_mib2_set_syslocation but set pointer to readonly memory
 */
void
snmp_mib2_set_syslocation_readonly(const u8_t *ocstr, const u16_t *ocstrlen)
{
  if (ocstr != NULL) {
    syslocation         = ocstr;
    syslocation_len     = ocstrlen;
    syslocation_wr      = NULL;
    syslocation_wr_len  = NULL;
    syslocation_bufsize = 0;
  }
}


//static const struct snmp_scalar_array_node_def system_nodes[] = {
//  {1, SNMP_ASN1_TYPE_OCTET_STRING, SNMP_NODE_INSTANCE_READ_ONLY},  /* nemeDev */
//  {2, SNMP_ASN1_TYPE_OCTET_STRING,    SNMP_NODE_INSTANCE_READ_ONLY},  /* contact */
//  {3, SNMP_ASN1_TYPE_OCTET_STRING,    SNMP_NODE_INSTANCE_READ_ONLY},  /* geodata */
//  {4, SNMP_ASN1_TYPE_INTEGER,         SNMP_NODE_INSTANCE_READ_ONLY}, /* type_out */
//  {5, SNMP_ASN1_TYPE_INTEGER,         SNMP_NODE_INSTANCE_READ_ONLY}, /* sostout */
//  {6, SNMP_ASN1_TYPE_INTEGER,         SNMP_NODE_INSTANCE_READ_ONLY}, /* SetDefsettings */
//  {7, SNMP_ASN1_TYPE_INTEGER,      SNMP_NODE_INSTANCE_READ_ONLY},   /* StartBoots */
//  {8, SNMP_ASN1_TYPE_INTEGER,      SNMP_NODE_INSTANCE_READ_ONLY},   /* swich_to_1 */
//  {9, SNMP_ASN1_TYPE_INTEGER,      SNMP_NODE_INSTANCE_READ_ONLY},   /* swich_to_0 */
//  {10, SNMP_ASN1_TYPE_INTEGER,      SNMP_NODE_INSTANCE_READ_ONLY},   /* swich_pulse */
//  {11, SNMP_ASN1_TYPE_IPADDR,      SNMP_NODE_INSTANCE_READ_ONLY}   /* ip_addr_traps */
//
//   if (post_data->data[0]==0x30)
//            {
//               form_reple_to_save(SWICH_ON_WEB);
//               flag_global_swich_out=SWICH_ON_WEB;
//               HAL_RTCEx_BKUPWrite(&hrtc,1,0);
//            }
//            else if (post_data->data[0]==0x31)
//              {
//                form_reple_to_save(SWICH_OFF_WEB);
//                flag_global_swich_out=SWICH_OFF_WEB;
//                 HAL_RTCEx_BKUPWrite(&hrtc,1,1);
//              
//              }
//            else if (post_data->data[0]==0x32)
//              {
//                 form_reple_to_save(SWICH_TOLG_WEB);
//                 flag_global_swich_out=SWICH_TOLG_WEB;
//                 HAL_RTCEx_BKUPWrite(&hrtc,1,2);
//              }
//};
static s16_t
system_get_value(const struct snmp_scalar_array_node_def *node, void *value)
{
  const u8_t*  var = NULL;
  const s16_t* var_len;
  u16_t result;
  uint16_t lens_mes;
  

  switch (node->oid) {
  case 1: /* sysDescr */  
    var     = FW_data.V_Name_dev;
    lens_mes=strlen((char*)FW_data.V_Name_dev);
    var_len = (const s16_t*)(&lens_mes);
    break;
  case 2: /* sysObjectID */ // memset((char*)FW_data.V_CALL_DATA,0,strlen((char*)FW_data.V_CALL_DATA));
    var     = FW_data.V_CALL_DATA;
    lens_mes=strlen((char*)FW_data.V_CALL_DATA);
    var_len = (const s16_t*)(&lens_mes);
    break;
  case 3: /* sysUpTime */
    var     = FW_data.V_GEOM_NAME;
    lens_mes=strlen((char*)(FW_data.V_GEOM_NAME));
    var_len = (const s16_t*)(&lens_mes);
    break;
  case 4: /* sysContact */    
     *(s32_t*)value = FW_data.V_TYPE_OUT;
    return sizeof(s32_t);   
  case 5: /* sysName */ //= HAL_RTCEx_BKUPRead(&hrtc,1);
    *(s32_t*)value = HAL_RTCEx_BKUPRead(&hrtc,1);
    return sizeof(s32_t);   
  case 6: /* sysLocation */
    form_reple_to_save(LOAD_DEF_DATA);
    vTaskDelay(100);
    flag_global_load_def=1;
    var     = "OK";
    lens_mes=strlen((char*)("OK"));
    var_len = (const s16_t*)(&lens_mes);
    break;
   case 7: /* sysLocation */
    form_reple_to_save(UPDATE_FW);           
    
    flag_global_boot_mode=1;
    var     = "OK";
    lens_mes=strlen((char*)("OK"));
    var_len = (const s16_t*)(&lens_mes);
   break;
   case 8: /* sysLocation */ ////////////////////////////////////////////////////////////
    if (flag_runtime_out[1]==0)
      {
    form_reple_to_save(SWICH_ON_SNMP);
    flag_global_swich_out=SWICH_ON_SNMP;
    HAL_RTCEx_BKUPWrite(&hrtc,1,0);
      }
    else
      {
       form_reple_to_save(SWICH_ON_SNMP_N);
      }
    vTaskDelay(100);
    var     = FW_data.V_ON_MESS;
    lens_mes=strlen((char*)(FW_data.V_ON_MESS));
    var_len = (const s16_t*)(&lens_mes);
  break;
  case 9: /* sysLocation */
     if (flag_runtime_out[1]==1)
      {
       form_reple_to_save(SWICH_OFF_SNMP);
       flag_global_swich_out=SWICH_OFF_SNMP;
       HAL_RTCEx_BKUPWrite(&hrtc,1,1);
      }
     else
      {
       form_reple_to_save(SWICH_OFF_SNMP_N);
      }
    vTaskDelay(100);
    var     = FW_data.V_OFF_MESS;
    lens_mes=strlen((char*)(FW_data.V_OFF_MESS));
    var_len = (const s16_t*)(&lens_mes);
    break;
  case 10: /* sysLocation */
     if (flag_runtime_out[1]==1)
      {
       form_reple_to_save(SWICH_TOLG_SNMP);
       flag_global_swich_out=SWICH_TOLG_SNMP;
       HAL_RTCEx_BKUPWrite(&hrtc,1,2);
      }
     else
      {
       form_reple_to_save(SWICH_TOLG_SNMP_N);
      }
    vTaskDelay(100);
    var     = "Out_Reset_Pulse";
    lens_mes=strlen((char*)("Out_Reset_Pulse"));
    var_len = (const s16_t*)(&lens_mes);
    break;    
   case 11: /* sysLocation */
    form_reple_to_save(RESETL);
    //   flag_global_reset_mode=1;
    save_reple_log(reple_to_save);
    flag_global_save_log=0;
    vTaskDelay(100);
    jamp_to_app();
    var     = "Reset";
    lens_mes=strlen((char*)("Reset"));
    var_len = (const s16_t*)(&lens_mes);
    break;    
  case 12: /* sysServices */
    *(s32_t*)value = SNMP_SYSSERVICES;
    return sizeof(s32_t);
  default:
    LWIP_DEBUGF(SNMP_MIB_DEBUG,("system_get_value(): unknown id: %"S32_F"\n", node->oid));
    return 0;
  }

  /* handle string values (OID 1,4,5 and 6) */
  LWIP_ASSERT("", (value != NULL));
  if (var_len == NULL) {
    result = (s16_t)strlen((const char*)var);
  } else {
    result = *var_len;
  }
  MEMCPY(value, var, result);
  return result;
}

static snmp_err_t
system_set_test(const struct snmp_scalar_array_node_def *node, u16_t len, void *value)
{
  snmp_err_t ret = SNMP_ERR_WRONGVALUE;
  const u16_t* var_bufsize  = NULL;
  const u16_t* var_wr_len;

  LWIP_UNUSED_ARG(value);

  switch (node->oid) {
  case 4: /* sysContact */
//        var_bufsize  = &syscontact_bufsize;
//    var_wr_len   = syscontact_wr_len;
    var_bufsize  = value;
    var_wr_len   = &len;
    break;
  case 5: /* sysName */
    var_bufsize  = &sysname_bufsize;
    var_wr_len   = sysname_wr_len;
    break;
  case 6: /* sysLocation */
    var_bufsize  = &syslocation_bufsize;
    var_wr_len   = syslocation_wr_len;
    break;
  default:
    LWIP_DEBUGF(SNMP_MIB_DEBUG,("system_set_test(): unknown id: %"S32_F"\n", node->oid));
    return ret;
  }

  /* check if value is writable at all */
  if (*var_bufsize > 0) {
    if (var_wr_len == NULL) {
      /* we have to take the terminating 0 into account */
      if (len < *var_bufsize) {
        ret = SNMP_ERR_NOERROR;
      }
    } else {
      if (len <= *var_bufsize) {
        ret = SNMP_ERR_NOERROR;
      }
    }
  } else {
    ret = SNMP_ERR_NOTWRITABLE;
  }

  return ret;
}

static snmp_err_t
system_set_value(const struct snmp_scalar_array_node_def *node, u16_t len, void *value)
{
  u8_t*  var_wr = NULL;
  u16_t* var_wr_len;

  switch (node->oid) {
  case 4: /* sysContact */
//    var_wr     = syscontact_wr;
//    var_wr_len = syscontact_wr_len;
     var_wr  = value;
    var_wr_len   = &len;
    break;
  case 5: /* sysName */
    var_wr     = sysname_wr;
    var_wr_len = sysname_wr_len;
    break;
  case 6: /* sysLocation */
    var_wr     = syslocation_wr;
    var_wr_len = syslocation_wr_len;
    break;
  default:
    LWIP_DEBUGF(SNMP_MIB_DEBUG,("system_set_value(): unknown id: %"S32_F"\n", node->oid));
    return SNMP_ERR_GENERROR;
  }

  /* no need to check size of target buffer, this was already done in set_test method */
  LWIP_ASSERT("", var_wr != NULL);
  MEMCPY(var_wr, value, len);
  
  if (var_wr_len == NULL) {
    /* add terminating 0 */
    var_wr[len] = 0;
  } else {
    *var_wr_len = len;
  }

  return SNMP_ERR_NOERROR;
}

//static const struct snmp_scalar_array_node_def system_nodes[] = {
//  {1, SNMP_ASN1_TYPE_OCTET_STRING, SNMP_NODE_INSTANCE_READ_ONLY},  /* sysDescr */
//  {2, SNMP_ASN1_TYPE_OBJECT_ID,    SNMP_NODE_INSTANCE_READ_ONLY},  /* sysObjectID */
//  {3, SNMP_ASN1_TYPE_TIMETICKS,    SNMP_NODE_INSTANCE_READ_ONLY},  /* sysUpTime */
//  {4, SNMP_ASN1_TYPE_OCTET_STRING, SNMP_NODE_INSTANCE_READ_WRITE}, /* sysContact */
//  {5, SNMP_ASN1_TYPE_OCTET_STRING, SNMP_NODE_INSTANCE_READ_WRITE}, /* sysName */
//  {6, SNMP_ASN1_TYPE_OCTET_STRING, SNMP_NODE_INSTANCE_READ_WRITE}, /* sysLocation */
//  {7, SNMP_ASN1_TYPE_INTEGER,      SNMP_NODE_INSTANCE_READ_ONLY},   /* sysServices */
//  {8, SNMP_ASN1_TYPE_OCTET_STRING,      SNMP_NODE_INSTANCE_READ_ONLY}   /* sysServices */
//};


static const struct snmp_scalar_array_node_def system_nodes[] = {
  {1, SNMP_ASN1_TYPE_OCTET_STRING, SNMP_NODE_INSTANCE_READ_ONLY},  /* nemeDev */
  {2, SNMP_ASN1_TYPE_OCTET_STRING,    SNMP_NODE_INSTANCE_READ_ONLY},  /* contact */
  {3, SNMP_ASN1_TYPE_OCTET_STRING,    SNMP_NODE_INSTANCE_READ_ONLY},  /* geodata */
  {4, SNMP_ASN1_TYPE_INTEGER,         SNMP_NODE_INSTANCE_READ_ONLY}, /* type_out */
  {5, SNMP_ASN1_TYPE_INTEGER,         SNMP_NODE_INSTANCE_READ_ONLY}, /* sostout */
  {6, SNMP_ASN1_TYPE_OCTET_STRING,         SNMP_NODE_INSTANCE_READ_ONLY}, /* SetDefsettings */
  {7, SNMP_ASN1_TYPE_OCTET_STRING,      SNMP_NODE_INSTANCE_READ_ONLY},   /* StartBoots */
  {8, SNMP_ASN1_TYPE_OCTET_STRING,      SNMP_NODE_INSTANCE_READ_ONLY},   /* swich_to_1 */
  {9, SNMP_ASN1_TYPE_OCTET_STRING,      SNMP_NODE_INSTANCE_READ_ONLY},   /* swich_to_0 */
  {10, SNMP_ASN1_TYPE_OCTET_STRING,      SNMP_NODE_INSTANCE_READ_ONLY},   /* swich_pulse */
  {11, SNMP_ASN1_TYPE_IPADDR,      SNMP_NODE_INSTANCE_READ_ONLY}   /* ip_addr_traps */
};
const struct snmp_scalar_array_node snmp_mib2_system_node = SNMP_SCALAR_CREATE_ARRAY_NODE(1, system_nodes, system_get_value, system_set_test, system_set_value);

#endif /* LWIP_SNMP && SNMP_LWIP_MIB2 */

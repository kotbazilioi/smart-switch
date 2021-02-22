/**=============================================================================
 \file    private_mib.c
 \brief   Файл содержит функции расширение для таблицы MIB-2 SNMP
==============================================================================*/

/*------------------------------------------------------------------------------
 Включение заголовочных файлов
------------------------------------------------------------------------------*/
#include <stdio.h>
#include <string.h>
#include "opt.h"

#if LWIP_SNMP /* don't build if not configured for use in lwipopts.h */

//#include "system.h"
#include "FreeRTOS.h"
#include "task.h"
#include "semphr.h"
#include "snmp.h"
#include "netif.h"
#include "ip.h"
//#include "ip_frag.h"
#include "tcp.h"
#include "udp.h"
#include "snmp_asn1.h"
//#include "snmp_structs.h"
#include "snmp_msg.h"
#include "etharp.h"

//#include "Routines.h"
//#include "MemoryMap.h"
//#include "Fmanager.h"
//#include "SNMP_ciu.h"
//#include "Receiver/MeasureLevel.h"
//#include "Receiver/Receiver.h"
//#include "Measurement/CheckLimits.h"
//#include "Measurement/TvSystem.h"
//#include "LAN_packets.h"
//#include "ViewRSA.h"
#include "private_mib.h"

#define SNMP_SYSNAME_TYPE "SmartSwich"

#define SNMP_SYSLOCATION_TYPE "netping.ru"

#define SNMP_SYSCONTACT_TYPE "netping"
#define SNMP_SNMPENABLEAUTHTRAPS_TYPE 1
#define SNMP_TESTPOINTNAME_TYPE "test_name_discr"
/*------------------------------------------------------------------------------
 Локальные макроопределения
------------------------------------------------------------------------------*/

/*------------------------------------------------------------------------------
 Переопределения переменных и структур с локальной областью видимости
------------------------------------------------------------------------------*/
// структура канала частотного плана
//typedef __packed struct
//{
//  point_fr_map     Channel;     // канал общий
//  point_fr_map_dig DigChannel;  // канал цифровой
//} tCH_PLAN_CHANNEL;

// структура частотного плана
//typedef __packed struct
//{
//  head_fr_map      ChPlanHead;                // заголовок
//  tCH_PLAN_CHANNEL ChPlanChannel[MAX_NUM_CH]; // каналы
//} tCH_PLAN;

/*------------------------------------------------------------------------------
 Объявление глобальных переменных с локальной областью видимости
------------------------------------------------------------------------------*/
u8_t syscontact_len_ciu = 0;
u8_t syslocation_len_ciu = 0;
u8_t sysname_len_ciu = 0;
u8_t snmpenableauthentraps_ciu = 2;


/* переменные с ошибками канала */
//union
//{
//	struct
//	{
//		CHAR szSnmp_Level[12];				// 110.0(>100)
//		CHAR szSnmp_VAR[12];					// 110.0(>80)
//		CHAR szSnmp_CNR[10];					// 32.3(<43)
//		CHAR szSnmp_MER[10];					// 32.2(<35)
//		CHAR szSnmp_preBER[16];				// 1.0e-04(>1e-08)
//		CHAR szSnmp_postBER[16];			// 1.0e-04(>1e-08)
//	};
//	struct
//	{
//		DWORD dwSnmp_BandErrorType;      // код ошибки по полосам
//		CHAR szSnmp_BandErrorVal[12];    // 110.0(>20)
//	};
//};

//const CHAR* szTemperatureStatus = NULL;
//const CHAR* szHardwareStatus = NULL;
//const CHAR* szBandErrorType = NULL;

extern u8_t* syscontact_len_ptr;
extern u8_t* sysname_len_ptr;
extern u8_t* syslocation_len_ptr;
extern u8_t* snmpenableauthentraps_ptr;

//tU8 SnmpChannels;  // количество каналов канального плана записанных по SNMP

/*------------------------------------------------------------------------------
 Объявление локальных функций
------------------------------------------------------------------------------*/
void ocstrncpy(u8_t *dst, u8_t *src, u8_t n);

// Функции для узла Identification
static void Identification_get_object_def (u8_t ident_len, s32_t *ident, struct obj_def *od);
static void Identification_get_value (struct obj_def *od, u16_t len, void *value);
static u8_t Identification_set_test (struct obj_def *od, u16_t len, void *value);
static void Identification_set_value (struct obj_def *od, u16_t len, void *value);
// Функции для узла Control
static void Control_get_object_def (u8_t ident_len, s32_t *ident, struct obj_def *od);
static void Control_get_value (struct obj_def *od, u16_t len, void *value);
static u8_t Control_set_test (struct obj_def *od, u16_t len, void *value);
static void Control_set_value (struct obj_def *od, u16_t len, void *value);
//tBOOL SnmpCheckChPlanPoint (tCHAR* pChPlanPoint, tU16 Length);
//void SnmpWriteChPlanPoint (tCHAR* pChPlanPoint, tU16 Length);
void SnmpWriteChPlan (void);
// Функции для узла Measurements
  // число каналов, температура
static void Meas_var_get_object_def (u8_t ident_len, s32_t *ident, struct obj_def *od);
static void Meas_var_get_value (struct obj_def *od, u16_t len, void *value);
  // таблица канального плана
u16_t Chplan_level_length (void* addr_inf, u8_t level);
s32_t Chplan_get_ident_cmp (void* addr_inf, u8_t level, u16_t idx, s32_t sub_id);
void Chplan_get_objid (void* addr_inf, u8_t level, u16_t idx, s32_t *sub_id);
static void Chplan_get_object_def (u8_t rid, u8_t ident_len, s32_t *ident, struct obj_def *od);
static void Chplan_get_value (u8_t rid, struct obj_def *od, u16_t len, void *value);
  // таблица результатов измерений
u16_t Meastable_level_length (void* addr_inf, u8_t level);
s32_t Meastable_get_ident_cmp ( void* addr_inf, u8_t level, u16_t idx, s32_t sub_id);
void Meastable_get_objid ( void* addr_inf, u8_t level, u16_t idx, s32_t *sub_id);
static void Meastable_get_object_def ( u8_t rid, u8_t ident_len, s32_t *ident, struct obj_def *od);
static void Meastable_get_value ( u8_t rid, struct obj_def *od, u16_t len, void *value);
u32_t snmp_ConvertBER (u8_t exp, u8_t man);
  // таблица ошибок
u16_t Errtable_level_length (void* addr_inf, u8_t level);
s32_t Errtable_get_ident_cmp (void* addr_inf, u8_t level, u16_t idx, s32_t sub_id);
void Errtable_get_objid (void* addr_inf, u8_t level, u16_t idx, s32_t *sub_id);
static void Errtable_get_object_def (u8_t rid, u8_t ident_len, s32_t *ident, struct obj_def *od);
static void Errtable_get_value (u8_t rid, struct obj_def *od, u16_t len, void *value);
// Функции для узла Alarm
  // hardwareError
static void ciu_alarm_hardware_status_get_object_def (u8_t ident_len, s32_t *ident, struct obj_def *od);
static void ciu_alarm_hardware_status_get_value (struct obj_def *od, u16_t len, void *value);
  // temperature
static void ciu_alarm_temperature_status_get_object_def (u8_t ident_len, s32_t *ident, struct obj_def *od);
static void ciu_alarm_temperature_status_get_value (struct obj_def *od, u16_t len, void *value);
  // channels
static void ciu_alarm_channels_status_get_object_def (u8_t ident_len, s32_t *ident, struct obj_def *od);
static void ciu_alarm_channels_status_get_value (struct obj_def *od, u16_t len, void *value);
  // flatness
static void ciu_alarm_flatness_status_get_object_def (u8_t ident_len, s32_t *ident, struct obj_def *od);
static void ciu_alarm_flatness_status_get_value (struct obj_def *od, u16_t len, void *value);



static void system_get_object_def_ee( u8_t ident_len, s32_t *ident, struct obj_def *od );
static void system_get_value_ee(struct obj_def *od, u16_t len, void *value);
static u8_t system_set_test_ee(struct obj_def *od, u16_t len, void *value);
static void system_set_value_ee(struct obj_def *od, u16_t len, void *value);
void snmp_get_object_def(u8_t ident_len, s32_t *ident, struct obj_def *od);
void snmp_get_value(struct obj_def *od, u16_t len, void *value);
u8_t snmp_set_test(struct obj_def *od, u16_t len, void *value);
static void snmp_set_value_ee(struct obj_def *od, u16_t len, void *value);


// dummi функции
void dummy_get_object_def_q ( void* addr_inf, u8_t rid, u8_t ident_len, s32_t *ident );
void dummy_get_value_q (u8_t rid, struct obj_def *od);
void dummy_set_test_q (u8_t rid, struct obj_def *od);
void dummy_set_value_q (u8_t rid, struct obj_def *od, u16_t len, void *value);

void dummy_get_object_def_pc (u8_t rid, u8_t ident_len, s32_t *ident);
void dummy_get_value_pc (u8_t rid, struct obj_def *od);
void dummy_set_test_pc (u8_t rid, struct obj_def *od);
void dummy_set_value_pc (u8_t rid, struct obj_def *od);

u8_t noleaf_set_test_a (u8_t rid, struct obj_def *od, u16_t len, void *value);
void noleaf_set_value_a (u8_t rid, struct obj_def *od, u16_t len, void *value);


/*------------------------------------------------------------------------------
 Описание узлов 
------------------------------------------------------------------------------*/

/* узел SYSTEM (операции с EEPROM) */
/*             0 1 2 3 4 5 6 */
/* system .1.3.6.1.2.1.1 */
const mib_scalar_node sys_tem_scalar_ee = {
  &system_get_object_def_ee,
  &system_get_value_ee,
  &system_set_test_ee,
  &system_set_value_ee,
  MIB_NODE_SC,
  0
};

/* узел SNMP (операции с EEPROM) */
/* snmp .1.3.6.1.2.1.11 */
const mib_scalar_node snmp_scalar_ee = {
  &snmp_get_object_def,
  &snmp_get_value,
  &snmp_set_test,
  &snmp_set_value_ee,
  MIB_NODE_SC,
  0
};

// PRIVATE SECTION ------------------------------------------------------------

// переменные узла FLATNESS SEVERITY --------------------------------------

const mib_scalar_node ciu_alarm_flatness_status = {
  &ciu_alarm_flatness_status_get_object_def,
  &ciu_alarm_flatness_status_get_value,
  &noleafs_set_test,
  &noleafs_set_value,
  MIB_NODE_SC,
  0
};

// узел FLATNESS SEVERITY -------------------------------------------------

const s32_t ciu_alarm_flatness_ids[2] = { 2, 3 };

struct mib_node* const ciu_alarm_flatness_nodes[2] = 
{
	(struct mib_node* const)&ciu_alarm_flatness_status,
	(struct mib_node* const)&ciu_alarm_flatness_status
};

const struct mib_array_node ciu_alarm_flatness = 
{
	&noleafs_get_object_def,
	&noleafs_get_value,
	&noleafs_set_test,
	&noleafs_set_value,
	MIB_NODE_AR,
	2,
	ciu_alarm_flatness_ids,
	ciu_alarm_flatness_nodes
};

// переменные узла CHANNEL SEVERITY ---------------------------------------

const mib_scalar_node ciu_alarm_channels_status = {
  &ciu_alarm_channels_status_get_object_def,
  &ciu_alarm_channels_status_get_value,
  &noleafs_set_test,
  &noleafs_set_value,
  MIB_NODE_SC,
  0
};

// узел CHANNEL SEVERITY --------------------------------------------------

const s32_t ciu_alarm_channels_ids[7] = { 2, 3, 4, 5, 6, 7, 8 };

struct mib_node* const ciu_alarm_channels_nodes[7] = 
{
	(struct mib_node* const)&ciu_alarm_channels_status,
	(struct mib_node* const)&ciu_alarm_channels_status,
	(struct mib_node* const)&ciu_alarm_channels_status,
	(struct mib_node* const)&ciu_alarm_channels_status,
	(struct mib_node* const)&ciu_alarm_channels_status,
	(struct mib_node* const)&ciu_alarm_channels_status,
	(struct mib_node* const)&ciu_alarm_channels_status
};

const struct mib_array_node ciu_alarm_channels = 
{
	&noleafs_get_object_def,
	&noleafs_get_value,
	&noleafs_set_test,
	&noleafs_set_value,
	MIB_NODE_AR,
	7,
	ciu_alarm_channels_ids,
	ciu_alarm_channels_nodes
};

// переменные узла TEMPERATURE SEVERITY -----------------------------------

const mib_scalar_node ciu_alarm_temperature_status = {
  &ciu_alarm_temperature_status_get_object_def,
  &ciu_alarm_temperature_status_get_value,
  &noleafs_set_test,
  &noleafs_set_value,
  MIB_NODE_SC,
  0
};

// узел TEMPERATURE SEVERITY ----------------------------------------------

const s32_t ciu_alarm_temperature_ids[1] = { 2 };

struct mib_node* const ciu_alarm_temperature_nodes[1] = 
{
	(struct mib_node* const)&ciu_alarm_temperature_status,
};

const struct mib_array_node ciu_alarm_temperature = 
{
	&noleafs_get_object_def,
	&noleafs_get_value,
	&noleafs_set_test,
	&noleafs_set_value,
	MIB_NODE_AR,
	1,
	ciu_alarm_temperature_ids,
	ciu_alarm_temperature_nodes
};

// переменные узла HARDWARE ERROR -----------------------------------------

const mib_scalar_node ciu_alarm_hardware_status = {
  &ciu_alarm_hardware_status_get_object_def,
  &ciu_alarm_hardware_status_get_value,
  &noleafs_set_test,
  &noleafs_set_value,
  MIB_NODE_SC,
  0
};

// узел HARDWARE ERROR ----------------------------------------------------

const s32_t ciu_alarm_hardware_ids[1] = { 2 };

struct mib_node* const ciu_alarm_hardware_nodes[1] = 
{
	(struct mib_node* const)&ciu_alarm_hardware_status,
};

const struct mib_array_node ciu_alarm_hardware = 
{
	&noleafs_get_object_def,
	&noleafs_get_value,
	&noleafs_set_test,
	&noleafs_set_value,
	MIB_NODE_AR,
	1,
	ciu_alarm_hardware_ids,
	ciu_alarm_hardware_nodes
};

// узел ALARM -------------------------------------------------------------

const s32_t ciu_alarm_ids[4] = { 2, 4, 5, 6 };

struct mib_node* const ciu_alarm_nodes[4] = 
{
	(struct mib_node* const)&ciu_alarm_hardware,
	(struct mib_node* const)&ciu_alarm_temperature,
	(struct mib_node* const)&ciu_alarm_channels,
	(struct mib_node* const)&ciu_alarm_flatness
};

const struct mib_array_node ciu_alarm = 
{
	&noleafs_get_object_def,
	&noleafs_get_value,
	&noleafs_set_test,
	&noleafs_set_value,
	MIB_NODE_AR,
	4,
	ciu_alarm_ids,
	ciu_alarm_nodes
};

// переменные узла MEASUREMENTS -------------------------------------------

const mib_scalar_node meas_var = {
  &Meas_var_get_object_def,
  &Meas_var_get_value,
  &noleafs_set_test,
  &noleafs_set_value,
  MIB_NODE_SC,
  0
};

const struct mib_external_node meas_chplan =
{
  &noleafs_get_object_def,   // объекты доступа - узел недоступен
  &noleafs_get_value,
  &noleafs_set_test,
  &noleafs_set_value,
  
  MIB_NODE_EX,       // тип узла - внешний
  0,                 // число элементов - не используется
  NULL,              // дополнительный маркер - не используется
  3,                 // число уровней - 3 (таблица.параметр.канал)
  
  Chplan_level_length,  // функции поиска по дереву
  Chplan_get_ident_cmp,
  Chplan_get_objid,
  
  dummy_get_object_def_q,   // функции асинхронного опроса - не используем
  dummy_get_value_q,
  dummy_set_test_q,
  dummy_set_value_q,
  
  Chplan_get_object_def,   // функции асинхронного ответа
  Chplan_get_value,
  noleaf_set_test_a,
  noleaf_set_value_a,
  
  dummy_get_object_def_pc,  // функции вызываемые при ошибке - не используем
  dummy_get_value_pc,
  dummy_set_test_pc,
  dummy_set_value_pc
};

const struct mib_external_node meas_meastable =
{
  &noleafs_get_object_def,   // объекты доступа - узел недоступен
  &noleafs_get_value,
  &noleafs_set_test,
  &noleafs_set_value,
  
  MIB_NODE_EX,       // тип узла - внешний
  0,                 // число элементов - не используется
  NULL,              // дополнительный маркер - не используется
  3,                 // число уровней - 3 (таблица.параметр.канал)
  
  Meastable_level_length,  // функции поиска по дереву
  Meastable_get_ident_cmp,
  Meastable_get_objid,
  
  dummy_get_object_def_q,   // функции асинхронного опроса - не используем
  dummy_get_value_q,
  dummy_set_test_q,
  dummy_set_value_q,
  
  Meastable_get_object_def,   // функции асинхронного ответа
  Meastable_get_value,
  noleaf_set_test_a,
  noleaf_set_value_a,
  
  dummy_get_object_def_pc,  // функции вызываемые при ошибке - не используем
  dummy_get_value_pc,
  dummy_set_test_pc,
  dummy_set_value_pc
};

const struct mib_external_node meas_errtable =
{
  &noleafs_get_object_def,   // объекты доступа - узел недоступен
  &noleafs_get_value,
  &noleafs_set_test,
  &noleafs_set_value,
  
  MIB_NODE_EX,       // тип узла - внешний
  0,                 // число элементов - не используется
  NULL,              // дополнительный маркер - не используется
  3,                 // число уровней - 3 (таблица.параметр.канал)
  
  Errtable_level_length,  // функции поиска по дереву
  Errtable_get_ident_cmp,
  Errtable_get_objid,
  
  dummy_get_object_def_q,   // функции асинхронного опроса - не используем
  dummy_get_value_q,
  dummy_set_test_q,
  dummy_set_value_q,
  
  Errtable_get_object_def,   // функции асинхронного ответа
  Errtable_get_value,
  noleaf_set_test_a,
  noleaf_set_value_a,
  
  dummy_get_object_def_pc,  // функции вызываемые при ошибке - не используем
  dummy_get_value_pc,
  dummy_set_test_pc,
  dummy_set_value_pc
};

// узел MEASUREMENTS ------------------------------------------------------

const s32_t measurements_ids[6] = { 1, 2, 3, 4, 5, 6 };

struct mib_node* const measurements_nodes[6] = 
{
  (struct mib_node* const)&meas_var,
  (struct mib_node* const)&meas_chplan,
  (struct mib_node* const)&meas_meastable,
  (struct mib_node* const)&meas_errtable,
  (struct mib_node* const)&meas_var,
  (struct mib_node* const)&meas_var,
};

const struct mib_array_node measurements = 
{
  &noleafs_get_object_def,
  &noleafs_get_value,
  &noleafs_set_test,
  &noleafs_set_value,
  MIB_NODE_AR,
  6,
  measurements_ids,
  measurements_nodes
};

// переменные узла CONTROL ------------------------------------------------

const mib_scalar_node control_var = {
  &Control_get_object_def,
  &Control_get_value,
  &Control_set_test,
  &Control_set_value,
  MIB_NODE_SC,
  0
};

// узел CONTROL -----------------------------------------------------------

const s32_t control_ids[24] = { 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 
                               15, 16, 17, 18, 19, 20, 21, 22, 23, 24 };

struct mib_node* const control_nodes[24] = 
{
  (struct mib_node* const)&control_var,
  (struct mib_node* const)&control_var,
  (struct mib_node* const)&control_var,
  (struct mib_node* const)&control_var,
  (struct mib_node* const)&control_var,
  (struct mib_node* const)&control_var,
  (struct mib_node* const)&control_var,
  (struct mib_node* const)&control_var,
  (struct mib_node* const)&control_var,
  (struct mib_node* const)&control_var,
  (struct mib_node* const)&control_var,
  (struct mib_node* const)&control_var,
  (struct mib_node* const)&control_var,
  (struct mib_node* const)&control_var,
  (struct mib_node* const)&control_var,
  (struct mib_node* const)&control_var,
  (struct mib_node* const)&control_var,
  (struct mib_node* const)&control_var,
  (struct mib_node* const)&control_var,
  (struct mib_node* const)&control_var,
  (struct mib_node* const)&control_var,
  (struct mib_node* const)&control_var,
  (struct mib_node* const)&control_var,
  (struct mib_node* const)&control_var
};

const struct mib_array_node control = 
{
  &noleafs_get_object_def,
  &noleafs_get_value,
  &noleafs_set_test,
  &noleafs_set_value,
  MIB_NODE_AR,
  24,
  control_ids,
  control_nodes
};

// переменные узла IDENTIFICATION ------------------------------------------

const mib_scalar_node identification_var = 
{
  &Identification_get_object_def,
  &Identification_get_value,
  &Identification_set_test,
  &Identification_set_value,
  MIB_NODE_SC,
  0
};

// узел IDENTIFICATION -----------------------------------------------------

const s32_t identification_ids[4] = { 1, 2, 3, 4 };

struct mib_node* const identification_nodes[4] = 
{
  (struct mib_node* const)&identification_var,
  (struct mib_node* const)&identification_var,
  (struct mib_node* const)&identification_var,
  (struct mib_node* const)&identification_var
};

const struct mib_array_node identification = 
{
  &noleafs_get_object_def,
  &noleafs_get_value,
  &noleafs_set_test,
  &noleafs_set_value,
  MIB_NODE_AR,
  4,
  identification_ids,
  identification_nodes
};

// узел ITM18 --------------------------------------------------------------

const s32_t itm18_ids[4] = { 1, 2, 3, 4 };

struct mib_node* const itm18_nodes[4] = 
{
  (struct mib_node* const)&identification,
  (struct mib_node* const)&control,
  (struct mib_node* const)&measurements,
  (struct mib_node* const)&ciu_alarm,
};

const struct mib_array_node itm18 = 
{
  &noleafs_get_object_def,
  &noleafs_get_value,
  &noleafs_set_test,
  &noleafs_set_value,
  MIB_NODE_AR,
  4,
  itm18_ids,
  itm18_nodes
};

// узел TV_ANALYZERS -------------------------------------------------------

const s32_t tv_analyzers_ids[1] = { 4 };

struct mib_node* const tv_analyzers_nodes[1] = 
{
  (struct mib_node* const)&itm18,
};

const struct mib_array_node tv_analyzers = 
{
  &noleafs_get_object_def,
  &noleafs_get_value,
  &noleafs_set_test,
  &noleafs_set_value,
  MIB_NODE_AR,
  1,
  tv_analyzers_ids,
  tv_analyzers_nodes
};

// узел PLANAR ------------------------------------------------------------

const s32_t planar_ids[1] = { 2 };

struct mib_node* const planar_nodes[1] = 
{
  (struct mib_node* const)&tv_analyzers,
};

const struct mib_array_node planar = 
{
  &noleafs_get_object_def,
  &noleafs_get_value,
  &noleafs_set_test,
  &noleafs_set_value,
  MIB_NODE_AR,
  1,
  planar_ids,
  planar_nodes
};

// узел ENTERPRISES -------------------------------------------------------

//const s32_t enterprises_ids[1] = { PLANAR_ENTERPRISE_ID };
//
//struct mib_node* const enterprises_nodes[1] = 
//{
//  (struct mib_node* const)&planar
//};
//
const struct mib_array_node enterprises = 
{
  &noleafs_get_object_def,
  &noleafs_get_value,
  &noleafs_set_test,
  &noleafs_set_value,
  MIB_NODE_AR,
  1//,
//  enterprises_ids,
//  enterprises_nodes
};

// узел PRIVATE -----------------------------------------------------------

const s32_t mib_private_ids[1] = { 1 };

struct mib_node* const mib_pivate_nodes[1] = 
{
  (struct mib_node* const)&enterprises
};

const struct mib_array_node mib_private = 
{
  &noleafs_get_object_def,
  &noleafs_get_value,
  &noleafs_set_test,
  &noleafs_set_value,
  MIB_NODE_AR,
  1,
  mib_private_ids,
  mib_pivate_nodes
};


/*------------------------------------------------------------------------------
 Функции
------------------------------------------------------------------------------*/

// Функции для узла Identification

/**-----------------------------------------------------------------------------
 \fn     static void Identification_get_object_def (u8_t ident_len, s32_t *ident, struct obj_def *od)
 \brief  функция получения свойств переменных узла Identification
 \param  ident_len - the address length. длина адреса
 \param  ident - points to objectname.0 (object id trailer) 
 \param  od - points to object definition. указатель на описание переменной
 \return нет
------------------------------------------------------------------------------*/
static void Identification_get_object_def (u8_t ident_len, s32_t *ident, struct obj_def *od)
{
  u8_t id;
 // tPRIVILEGED* pPrivileged;
    
 // pPrivileged = (tPRIVILEGED*)PRIVILEGED_START_ADDRESS;
  
  /* return to object name, adding index depth (1) */
  ident_len += 1;
  ident -= 1;
  if (ident_len == 2)
  {
    od->id_inst_len = ident_len;
    od->id_inst_ptr = ident;

    id = ident[0];
    LWIP_DEBUGF(SNMP_MIB_DEBUG,("get_object_def planar.%"U16_F".0\n",(u16_t)id));
    switch (id)
    {
      case 1: /* serial number (string, RO) */
        od->instance = MIB_OBJECT_SCALAR;
        od->access = MIB_OBJECT_READ_ONLY;
        od->asn_type = (SNMP_ASN1_UNIV | SNMP_ASN1_PRIMIT | SNMP_ASN1_OC_STR);
        od->v_len = 8;//strnlen( pPrivileged->Serial, sizeof(pPrivileged->Serial) );
        break;
      case 2: /* hardware (string, RO) */
        od->instance = MIB_OBJECT_SCALAR;
        od->access = MIB_OBJECT_READ_ONLY;
        od->asn_type = (SNMP_ASN1_UNIV | SNMP_ASN1_PRIMIT | SNMP_ASN1_OC_STR);
        od->v_len = 9;
        break;
      case 3: /* software (string, RO) */
        od->instance = MIB_OBJECT_SCALAR;
        od->access = MIB_OBJECT_READ_ONLY;
        od->asn_type = (SNMP_ASN1_UNIV | SNMP_ASN1_PRIMIT | SNMP_ASN1_OC_STR);
        od->v_len = 11;
        break;
      case 4: /* testpoint name (string, RW) */
        od->instance = MIB_OBJECT_SCALAR;
        od->access = MIB_OBJECT_READ_WRITE;
        od->asn_type = (SNMP_ASN1_UNIV | SNMP_ASN1_PRIMIT | SNMP_ASN1_OC_STR);
        od->v_len = GetSnmpStringLen(SNMP_TESTPOINTNAME_TYPE);
        break;
      default:
        LWIP_DEBUGF(SNMP_MIB_DEBUG,("planar_get_object_def: no such object\n"));
        od->instance = MIB_OBJECT_NONE;
        break;
    };
  }
  else
  {
    LWIP_DEBUGF(SNMP_MIB_DEBUG,("planar_get_object_def: no scalar\n"));
    od->instance = MIB_OBJECT_NONE;
  }
}

/**-----------------------------------------------------------------------------
 \fn     static void Identification_get_value(struct obj_def *od, u16_t len, void *value)
 \brief  функция получения значения переменных узла Identification
 \param  obj_def - указатель на описание переменной
 \param  len - длина переменной в байтах 
 \param  value - указатель на значение переменной
 \return нет
------------------------------------------------------------------------------*/
static void Identification_get_value (struct obj_def *od, u16_t len, void *value)
{
  u8_t id;
  char* str;
//  tHARD_VERSION tmp;
//  tPRIVILEGED* pPrivileged;
//    
//  pPrivileged = (tPRIVILEGED*)PRIVILEGED_START_ADDRESS;
  
  id = od->id_inst_ptr[0];
  switch (id)
  {
    case 1: /* serial number */
    //  ocstrncpy(value, (u8_t*)pPrivileged->Serial, len);
      break;
    case 2: /* hardware */
//	   str = pvPortMalloc( len + 1 );
//		memcpy( &tmp, &pPrivileged->HardVersion, sizeof(tmp));
//		if( tmp.Type > 99 ) tmp.Type = 0;
//		if( tmp.Group > 99 ) tmp.Group = 0;
//		sprintf( str, TEXT("%02d.%02d.%03d"), 
//				   tmp.Group,
//					tmp.Type,
//					pPrivileged->HardVersion.Modification );
//		ocstrncpy(value, (u8_t*)str, len);
//		vPortFree( str );
      break;
    case 3: /* software */
	   str = pvPortMalloc( len + 1 );
//		sprintf( str, TEXT("%02d.%02d.%02d.%02d"), 
//		   (cSoftVersion >> 24) & 0x000000FF, (cSoftVersion >> 16) & 0x000000FF,
//		   (cSoftVersion >> 8) & 0x000000FF, (cSoftVersion >> 0) & 0x000000FF );
//		ocstrncpy(value, (u8_t*)str, len);
		vPortFree( str );
      break;
	 case 4: /* testpoint name */
	   GetSnmpString(SNMP_TESTPOINTNAME_TYPE, value, len );
		break;
  };
}

/**-----------------------------------------------------------------------------
 \fn     static u8_t Identification_set_test (struct obj_def *od, u16_t len, void *value)
 \brief  функция проверки записываемых значений переменных узла Identification
 \param  obj_def - указатель на описание переменной
 \param  len - длина переменной в байтах 
 \param  value - указатель на значение переменной
 \return 0 - некорректное значение
         1 - корректное значение
------------------------------------------------------------------------------*/
static u8_t Identification_set_test (struct obj_def *od, u16_t len, void *value)
{
  u8_t id, set_ok;

  LWIP_UNUSED_ARG(value);
  set_ok = 0;
  id = od->id_inst_ptr[0];
  switch (id)
  {
    case 4: /* testpoint name */
      if ( len <= 255 )
      {
        set_ok = 1;
      }
      break;
  };
  return set_ok;
}

/**-----------------------------------------------------------------------------
 \fn     static void Identification_set_value (struct obj_def *od, u16_t len, void *value)
 \brief  функция записи значений переменных узла Identification
 \param  obj_def - указатель на описание переменной
 \param  len - длина переменной в байтах 
 \param  value - указатель на значение переменной
 \return нет
------------------------------------------------------------------------------*/
static void Identification_set_value (struct obj_def *od, u16_t len, void *value)
{
  u8_t id;

  id = od->id_inst_ptr[0];
  switch (id)
  {
    case 4: /* testpoint name */
      SetSnmpString(SNMP_TESTPOINTNAME_TYPE, value, len );
      break;
  };
}


// Функции для узла Control

/**-----------------------------------------------------------------------------
 \fn     static void Control_get_object_def(u8_t ident_len, s32_t *ident, struct obj_def *od)
 \brief  функция получения свойств переменных узла Control (первых 5)
 \param  ident_len - the address length. длина адреса
 \param  ident - points to objectname.0 (object id trailer) 
 \param  od - points to object definition. указатель на описание переменной
 \return нет
------------------------------------------------------------------------------*/
static void Control_get_object_def (u8_t ident_len, s32_t *ident, struct obj_def *od)
{
  u8_t id;

  /* return to object name, adding index depth (1) */
  ident_len += 1;
  ident -= 1;
  if (ident_len == 2)
  {
    od->id_inst_len = ident_len;
    od->id_inst_ptr = ident;

    id = ident[0];
    LWIP_DEBUGF(SNMP_MIB_DEBUG,("get_object_def planar.%"U16_F".0\n",(u16_t)id));
    switch (id)
    {
    case 1: /* measure period (integer, RW) */
      od->instance = MIB_OBJECT_SCALAR;
      od->access = MIB_OBJECT_READ_WRITE;
      od->asn_type = (SNMP_ASN1_UNIV | SNMP_ASN1_PRIMIT | SNMP_ASN1_INTEG);
      od->v_len = sizeof( s32_t );
      break;
    case 2: /* measure launch (integer, RW) */
      od->instance = MIB_OBJECT_SCALAR;
      od->access = MIB_OBJECT_READ_WRITE;
      od->asn_type = (SNMP_ASN1_UNIV | SNMP_ASN1_PRIMIT | SNMP_ASN1_INTEG);
      od->v_len = sizeof( s32_t );
      break;
	case 3: /* time (string, RW) */
      od->instance = MIB_OBJECT_SCALAR;
      od->access = MIB_OBJECT_READ_WRITE;
      od->asn_type = (SNMP_ASN1_UNIV | SNMP_ASN1_PRIMIT | SNMP_ASN1_OC_STR);
      od->v_len = 8;
      break;
	case 4: /* date (string, RW) */
      od->instance = MIB_OBJECT_SCALAR;
      od->access = MIB_OBJECT_READ_WRITE;
      od->asn_type = (SNMP_ASN1_UNIV | SNMP_ASN1_PRIMIT | SNMP_ASN1_OC_STR);
      od->v_len = 10;
      break;
	case 5: /* device restart (integer, WO) */
      od->instance = MIB_OBJECT_SCALAR;
      od->access = MIB_OBJECT_READ_WRITE;
      od->asn_type = (SNMP_ASN1_UNIV | SNMP_ASN1_PRIMIT | SNMP_ASN1_INTEG);
      od->v_len = sizeof( s32_t );
      break;
    case 6: // trapDestination1
      od->instance = MIB_OBJECT_SCALAR;
      od->access = MIB_OBJECT_READ_WRITE;
      od->asn_type = (SNMP_ASN1_UNIV | SNMP_ASN1_PRIMIT | SNMP_ASN1_OC_STR);
      od->v_len = 15;
      break;
    case 7: // trapDestination2
      od->instance = MIB_OBJECT_SCALAR;
      od->access = MIB_OBJECT_READ_WRITE;
      od->asn_type = (SNMP_ASN1_UNIV | SNMP_ASN1_PRIMIT | SNMP_ASN1_OC_STR);
      od->v_len = 15;
      break;
    case 8: // trapDestination3
      od->instance = MIB_OBJECT_SCALAR;
      od->access = MIB_OBJECT_READ_WRITE;
      od->asn_type = (SNMP_ASN1_UNIV | SNMP_ASN1_PRIMIT | SNMP_ASN1_OC_STR);
      od->v_len = 15;
      break;
    case 9: // measParamEditMode
      od->instance = MIB_OBJECT_SCALAR;
      od->access = MIB_OBJECT_READ_WRITE;
      od->asn_type = (SNMP_ASN1_UNIV | SNMP_ASN1_PRIMIT | SNMP_ASN1_INTEG);
      od->v_len = sizeof(s32_t);
      break;
    case 10: // chPlanPointEdit
      od->instance = MIB_OBJECT_SCALAR;
      od->access = MIB_OBJECT_READ_WRITE;
      od->asn_type = (SNMP_ASN1_UNIV | SNMP_ASN1_PRIMIT | SNMP_ASN1_OC_STR);
      od->v_len = 28;
      break;
    case 11: // maxAnalogLevel
    case 12: // minAnalogLevel
    case 13: // maxDigitalLevel
    case 14: // minDigitalLevel
    case 15: // minMerQAM64
    case 16: // minMerQAM128
    case 17: // minMerQAM256
    case 18: // maxPreBER
    case 19: // maxDeltaAdj
    case 20: // maxDeltaDA
    case 21: // maxDelta300
    case 22: // maxDelta600
    case 23: // maxDelta1000
    case 24: // maxDeltaR100
      od->instance = MIB_OBJECT_SCALAR;
      od->access = MIB_OBJECT_READ_WRITE;
      od->asn_type = (SNMP_ASN1_UNIV | SNMP_ASN1_PRIMIT | SNMP_ASN1_INTEG);
      od->v_len = sizeof(s32_t);
      break;
	default:
      LWIP_DEBUGF(SNMP_MIB_DEBUG,("planar_get_object_def: no such object\n"));
      od->instance = MIB_OBJECT_NONE;
      break;
    };
  }
  else
  {
    LWIP_DEBUGF(SNMP_MIB_DEBUG,("planar_get_object_def: no scalar\n"));
    od->instance = MIB_OBJECT_NONE;
  }
}

/**-----------------------------------------------------------------------------
 \fn     static void Control_get_value (struct obj_def *od, u16_t len, void *value)
 \brief  функция получения значения переменных узла Control (первых 5)
 \param  obj_def - указатель на описание переменной
 \param  len - длина переменной в байтах 
 \param  value - указатель на значение переменной
 \return нет
------------------------------------------------------------------------------*/
static void Control_get_value (struct obj_def *od, u16_t len, void *value)
{
  u8_t id;
//  EMeasState tState;
  

  id = od->id_inst_ptr[0];
  switch (id)
  {
  case 1: /* measure period */
//	*( (s32_t*)value ) = MeasThreadGetPeriod();
    break;
  case 2: /* measure launch */
//	tState = MeasThreadGetState();
//	if( tState == e_measuring || tState == e_waitformeasure )
//	  *( (s32_t*)value ) = 1ul;
//	else
//	  *( (s32_t*)value ) = 0ul;
    break;
  case 3: /* time */
    {
//      TTime time;
//      
//      RTC_GetTime( &time );
//      sprintf(value, TEXT("%02d:%02d:%02d"), time.uHour, time.uMin, time.uSec);
      break;
    }
  case 4: /* date */
    {
//	  TDate date;
//     
//      RTC_GetDate( &date );
//      sprintf(value, TEXT("%02d.%02d.%04d"), date.uDay, date.uMonth, date.wYear);
      break;
    }
  case 5: // Reset
    {
	// *((s32_t*)value) = 0;
      break;
    }
  case 6: // trapDestination1
 //   sprintf(value, 
//            TEXT("%03d.%03d.%03d.%03d"), 
//            gSNMP_IPaddr1[0], 
//            gSNMP_IPaddr1[1], 
//            gSNMP_IPaddr1[2], 
//            gSNMP_IPaddr1[3]);
    break;
  case 7: // trapDestination2
//    sprintf(value, 
//            TEXT("%03d.%03d.%03d.%03d"), 
//            gSNMP_IPaddr2[0], 
//            gSNMP_IPaddr2[1], 
//            gSNMP_IPaddr2[2], 
//            gSNMP_IPaddr2[3]);
    break;
  case 8: // trapDestination3
//    sprintf(value, 
//            TEXT("%03d.%03d.%03d.%03d"), 
//            gSNMP_IPaddr3[0], 
//            gSNMP_IPaddr3[1], 
//            gSNMP_IPaddr3[2], 
//            gSNMP_IPaddr3[3]);
    break;
  case 9: // measParamEditMode
//    if (gITM18_Mode == ITM18_MODE_EDIT)
//    {
//      *((s32_t*)value) = 1;
//    }
//    else
//    {
//      *((s32_t*)value) = 0;
//    }
    break;
  case 10: // chPlanPointEdit
//    {
//	  *((s32_t*)value) = 0;
      break;
 //   }
  case 11: // maxAnalogLevel
 //   *((s32_t*)value) = tLimitsGeneral.MaxVideo;
    break;
  case 12: // minAnalogLevel
  //  *((s32_t*)value) = tLimitsGeneral.MinVideo;
    break;
  case 13: // maxDigitalLevel
 //   *((s32_t*)value) = tLimitsGeneral.MaxDigLevel;
    break;
  case 14: // minDigitalLevel
//    *((s32_t*)value) = tLimitsGeneral.MinDigLevel;
    break;
  case 15: // minMerQAM64
//    *((s32_t*)value) = tLimitsGeneral.MER_64;
    break;
  case 16: // minMerQAM128
 //   *((s32_t*)value) = tLimitsGeneral.MER_128;
    break;
  case 17: // minMerQAM256
  //  *((s32_t*)value) = tLimitsGeneral.MER_256;
    break;
  case 18: // maxPreBER
  //  *((s32_t*)value) = tLimitsGeneral.preBER;
    break;
  case 19: // maxDeltaAdj
  //  *((s32_t*)value) = tLimitsGeneral.MaxAdjDelta;
    break;
  case 20: // maxDeltaDA
  //  *((s32_t*)value) = tLimitsGeneral.MaxDeltaVD;
    break;
  case 21: // maxDelta300
 //   *((s32_t*)value) = tLimitsGeneral.dLevel40_300;
    break;
  case 22: // maxDelta600
  //  *((s32_t*)value) = tLimitsGeneral.dLevel40_600;
    break;
  case 23: // maxDelta1000
  //  *((s32_t*)value) = tLimitsGeneral.dLevel40_900;
    break;
  case 24: // maxDeltaR100
  //  *((s32_t*)value) = tLimitsGeneral.dLevel100;
    break;
  };
}

/**-----------------------------------------------------------------------------
 \fn     static u8_t Control_set_test (struct obj_def *od, u16_t len, void *value)
 \brief  функция проверки записываемых значений переменных узла Control (первых 5)
 \param  obj_def - указатель на описание переменной
 \param  len - длина переменной в байтах 
 \param  value - указатель на значение переменной
 \return 0 - некорректное значение
         1 - корректное значение
------------------------------------------------------------------------------*/
static u8_t Control_set_test (struct obj_def *od, u16_t len, void *value)
{
  u8_t id, set_ok;
  s32_t temp;
  char szTemp[16];


  set_ok = 0;
  id = od->id_inst_ptr[0];
  temp = *((s32_t*)value);
  switch (id)
  {
  case 1:    /* measure period */
    
	if( len == sizeof(s32_t) && temp <= 60 )
      set_ok = 1;
    break;
  case 2:    /* measure launch */
	if( len == sizeof(s32_t) && temp < 2 )
      set_ok = 1;
    break;
  case 3:    /* time */
    {
  //    TTime time;
      
//      if( len <= 8 )
//      {
//        memcpy( szTemp, value, len );
//        szTemp[len] = '\0';
//        if( StrToTime( &time, szTemp ) )
//        {
//          set_ok = 1;
//        }
//      }
      break;
    }
  case 4:    /* date */
    {
//      TDate date;
//      
//      if( len <= 10 )
//      {
//        memcpy( szTemp, value, len );
//        szTemp[len] = '\0';	
//        if( StrToDate( &date, szTemp ) )
//        {
//          set_ok = 1;
//        }
    //  }
      break;
    }
  case 5:    /* device restart */
//	 if( len == sizeof(s32_t) && temp == 1 )
//		 set_ok = 1;
     break;
  case 6: // trapDestination1
  case 7: // trapDestination2
  case 8: // trapDestination3
    {
//      tU32 ip1, ip2, ip3, ip4;
//      
//      if (len <= 15)
//      {
//        memcpy(szTemp, value, len);
//        szTemp[len] = '\0';
//        sscanf(szTemp, "%d.%d.%d.%d", &ip1, &ip2, &ip3, &ip4);
//        if (ip1 < 256 && ip2 < 256 && ip3 < 256 && ip4 < 256)
//        {
//          set_ok = 1;
//        }
//      }
    }
    break;
  case 9: // measParamEditMode
//	if (len == sizeof(s32_t))
//    {
//      EMeasState State;
//      
//      State = MeasThreadGetState();
//      
//      if (temp == 0)
//      {
//        set_ok = 1;
//      }
//      else if (temp == 1 && State == e_interrupted)
//      {
//        set_ok = 1;
//      }
//    }
	break;
  case 10: // chPlanPointEdit
    {
//      tBOOL stat;
//      
//      stat = SnmpCheckChPlanPoint((tCHAR*)value, len);
//      if (stat != FALSE)
//      {
//        set_ok = 1;
//      }
    }
    break;
  case 11: // maxAnalogLevel
//    if (gITM18_Mode == ITM18_MODE_EDIT)
//    {
//      if ((temp >= 45 && temp <= 95) || temp == 0)
//      {
//        set_ok = 1;
//      }
//    }
    break;
  case 12: // minAnalogLevel
//    if (gITM18_Mode == ITM18_MODE_EDIT)
//    {
//      if ((temp >= 45 && temp <= 95) || temp == 0)
//      {
//        set_ok = 1;
//      }
//    }
    break;
  case 13: // maxDigitalLevel
//    if (gITM18_Mode == ITM18_MODE_EDIT)
//    {
//      if ((temp >= 45 && temp <= 95) || temp == 0)
//      {
//        set_ok = 1;
//      }
//    }
    break;
  case 14: // minDigitalLevel
//    if (gITM18_Mode == ITM18_MODE_EDIT)
//    {
//      if ((temp >= 45 && temp <= 95) || temp == 0)
//      {
//        set_ok = 1;
//      }
//    }
    break;
  case 15: // minMerQAM64
//    if (gITM18_Mode == ITM18_MODE_EDIT)
//    {
//      if ((temp >= 25 && temp <= 40) || temp == 0)
//      {
//        set_ok = 1;
//      }
//    }
    break;
  case 16: // minMerQAM128
//    if (gITM18_Mode == ITM18_MODE_EDIT)
//    {
//      if ((temp >= 25 && temp <= 40) || temp == 0)
//      {
//        set_ok = 1;
//      }
//    }
    break;
  case 17: // minMerQAM256
//    if (gITM18_Mode == ITM18_MODE_EDIT)
//    {
//      if ((temp >= 25 && temp <= 40) || temp == 0)
//      {
//        set_ok = 1;
//      }
//    }
    break;
  case 18: // maxPreBER
//    if (gITM18_Mode == ITM18_MODE_EDIT)
//    {
//      if ((temp >= 1 && temp <= 5) || temp == 0)
//      {
//        set_ok = 1;
//      }
//    }
    break;
  case 19: // maxDeltaAdj
//    if (gITM18_Mode == ITM18_MODE_EDIT)
//    {
//      if ((temp >= 2 && temp <= 6) || temp == 0)
//      {
//        set_ok = 1;
//      }
//    }
    break;
  case 20: // maxDeltaDA
//    if (gITM18_Mode == ITM18_MODE_EDIT)
//    {
//      if ((temp >= 5 && temp <= 30) || temp == 0)
//      {
//        set_ok = 1;
//      }
//    }
    break;
  case 21: // maxDelta300
//    if (gITM18_Mode == ITM18_MODE_EDIT)
//    {
//      if ((temp >= 5 && temp <= 15) || temp == 0)
//      {
//        set_ok = 1;
//      }
//    }
    break;
  case 22: // maxDelta600
//    if (gITM18_Mode == ITM18_MODE_EDIT)
//    {
//      if ((temp >= 7 && temp <= 17) || temp == 0)
//      {
//        set_ok = 1;
//      }
//    }
    break;
  case 23: // maxDelta1000
//    if (gITM18_Mode == ITM18_MODE_EDIT)
//    {
//      if ((temp >= 10 && temp <= 20) || temp == 0)
//      {
//        set_ok = 1;
//      }
//    }
    break;
  case 24: // maxDeltaR100
//    if (gITM18_Mode == ITM18_MODE_EDIT)
//    {
//      if ((temp >= 5 && temp <= 12) || temp == 0)
//      {
//        set_ok = 1;
//      }
//    }
    break;
  };
  return set_ok;
}

/**-----------------------------------------------------------------------------
 \fn     static void Control_set_value (struct obj_def *od, u16_t len, void *value)
 \brief  функция записи значений переменных узла Control (первых 5)
 \param  obj_def - указатель на описание переменной
 \param  len - длина переменной в байтах 
 \param  value - указатель на значение переменной
 \return нет
------------------------------------------------------------------------------*/
static void Control_set_value (struct obj_def *od, u16_t len, void *value)
{
  u8_t id;
  uint8_t tmp;
  char szTemp[16];
  s32_t temp;
  
  
  temp = *((s32_t*)value);
  id = od->id_inst_ptr[0];
  switch (id)
  {
  case 1:    /* measure period */
	   tmp = *( (s32_t*)value );
	   MeasThreadSetPeriod( tmp );
       SaveSettingsITM18();
      break;
  case 2:    /* measure launch */
	   tmp = *( (s32_t*)value );
	   if( tmp == 0 )
		{
          StopMeasThread();
		}
		else if( tmp == 1 )
		{
       //   MeasThreadSetCommand(e_measure);
		}
	 	break;
  case 3:    /* time */
	 {
		////TTime time;
		 
		memcpy( szTemp, value, len );
		szTemp[len] = '\0';
////		if( StrToTime( &time, szTemp ) )
////		{
////			RTC_SetTime( &time );
////		}
	   break;
	 }
  case 4:    /* date */
	 {
//		TDate date;
//		 
//		memcpy( szTemp, value, len );
//		szTemp[len] = '\0';
//		if( StrToDate( &date, szTemp ) )
//		{
//			RTC_SetDate( &date );
//		}
		
	   break;
	 }
  case 5:    /* device restart */
     {
//        tmp = *( (s32_t*)value );
//        if( tmp == 1)
//        {
//          gUpdateCode = UPDATE_RESET;   // установка флага сброса без обновления
//          AIRCR = 0x5FA << 16 | 1 << 2; // перезагрузка прибора
//        }
	    break;
     }
  case 6: // trapDestination1
  case 7: // trapDestination2
  case 8: // trapDestination3
    {
//      tU32 ip1, ip2, ip3, ip4;
//      
//      if (len <= 15)
//      {
//        memcpy(szTemp, value, len);
//        szTemp[len] = '\0';
//        sscanf(szTemp, "%d.%d.%d.%d", &ip1, &ip2, &ip3, &ip4);
//        if (id == 6)
//        {
//          gSNMP_IPaddr1[0] = ip1;
//          gSNMP_IPaddr1[1] = ip2;
//          gSNMP_IPaddr1[2] = ip3;
//          gSNMP_IPaddr1[3] = ip4;
//        }
//        else if (id == 7)
//        {
//          gSNMP_IPaddr2[0] = ip1;
//          gSNMP_IPaddr2[1] = ip2;
//          gSNMP_IPaddr2[2] = ip3;
//          gSNMP_IPaddr2[3] = ip4;
//        }
//        else if (id == 8)
//        {
//          gSNMP_IPaddr3[0] = ip1;
//          gSNMP_IPaddr3[1] = ip2;
//          gSNMP_IPaddr3[2] = ip3;
//          gSNMP_IPaddr3[3] = ip4;
//        }
//        SaveSettingsITM18();
//      }
    }
    break;
  case 9: // chPlanEditMode
//    if (temp == 0 && gITM18_Mode == ITM18_MODE_EDIT)
//    {
//      if (SnmpChannels > 0)
//      {
//        SnmpWriteChPlan();  // запись канального плана из буфера
//      }
//      write_cell_mem((BYTE*)&tLimitsGeneral, 0, 0, 4); // запись данных шаблона проверки
//      MeasThreadSetState(e_changemeasparam); // установка флага изменения параметров измерения
//      gITM18_Mode = ITM18_MODE_MEASURE;
//    }
//    else if (temp == 1)
//    {
//      EMeasState State;
//      
//      State = MeasThreadGetState();
//      if (State == e_interrupted)
//      {
//        gMeasuredChBuf = 0;
//        gMeasuredCh = 0;
//        gTxViewRSACh = 0;
//        gITM18_Mode = ITM18_MODE_EDIT;
//        SnmpChannels = 0;
//      }
//    }
    break;
  case 10: // chPlanPointEdit
   // SnmpWriteChPlanPoint((tCHAR*)value, len);
    break;
  case 11: // maxAnalogLevel
 //   tLimitsGeneral.MaxVideo = (BYTE)temp;
    break;
  case 12: // minAnalogLevel
 //   tLimitsGeneral.MinVideo = (BYTE)temp;
    break;
  case 13: // maxDigitalLevel
  //  tLimitsGeneral.MaxDigLevel = (BYTE)temp;
    break;
  case 14: // minDigitalLevel
  //  tLimitsGeneral.MinDigLevel = (BYTE)temp;
    break;
  case 15: // minMerQAM64
   // tLimitsGeneral.MER_64 = (BYTE)temp;
    break;
  case 16: // minMerQAM128
  //  tLimitsGeneral.MER_128 = (BYTE)temp;
    break;
  case 17: // minMerQAM256
  //  tLimitsGeneral.MER_256 = (BYTE)temp;
    break;
  case 18: // maxPreBER
  //  tLimitsGeneral.preBER = (BYTE)temp;
    break;
  case 19: // maxDeltaAdj
  //  tLimitsGeneral.MaxAdjDelta = (BYTE)temp;
    break;
  case 20: // maxDeltaDA
   // tLimitsGeneral.MaxDeltaVD = (BYTE)temp;
    break;
  case 21: // maxDelta300
   // tLimitsGeneral.dLevel40_300 = (BYTE)temp;
    break;
  case 22: // maxDelta600
  // tLimitsGeneral.dLevel40_600 = (BYTE)temp;
    break;
  case 23: // maxDelta1000
  //  tLimitsGeneral.dLevel40_900 = (BYTE)temp;
    break;
  case 24: // maxDeltaR100
  //  tLimitsGeneral.dLevel100 = (BYTE)temp;
    break;
  };
}

/**-----------------------------------------------------------------------------
 \fn     tBOOL SnmpCheckChPlanPoint (tCHAR* pChPlanPoint, tU16 Length)
 \brief  функция проверки формата строки канального плана, полученной по SNMP
 \param  pChPlanPoint - указатель на строку канального плана
 \param  Length - длина строки 
 \return TRUE - формат строки правильный
         FALSE - формат строки не правильный
------------------------------------------------------------------------------*/
uint8_t SnmpCheckChPlanPoint (char* pChPlanPoint, uint16_t Length)
{
  char StrCh[30];
  char NameCh[7];
  uint32_t Frequency;
  uint32_t Type;
  uint32_t BandWidth;
  uint32_t Modulation;
  uint32_t SR;
  char* pStr;
  
  
  // проверка режима работы прибора
////  if (gITM18_Mode != ITM18_MODE_EDIT)
////  {
////    return FALSE;
////  }
////  // проверка количества каналов
////  if (SnmpChannels >= MAX_NUM_CH)
////  {
////    return FALSE;
////  }
////  // проверка длины строки
////  if (Length < 15 && Length > 26)
////  {
////    return FALSE;
////  }
  memcpy(StrCh, pChPlanPoint, Length);
  StrCh[Length] = '\0';
  // поиск имени канала
  pStr = strstr(StrCh, ",");
  if ((pStr - StrCh) > 0 && (pStr - StrCh) < 7)
  {
    memcpy(NameCh, pChPlanPoint, (pStr - StrCh));
    NameCh[(pStr - StrCh)] = '\0';
  }
  else
  {
    return 0;
  }
  sscanf(pStr, ",%d,%d,%d,%d,%d", &Frequency, &Type, &BandWidth, &Modulation, &SR);
  // проверка диапазона частот
  if (Frequency < 45000 || Frequency > 1000000)
  {
    return 0;
  }
  // проверка разрешения по частоте
  if ((Frequency%125) != 0)
  {
    return 0;
  }
  // проверка типа канала
  if (Type > 5)
  {
    return 0;
  }
  // проверка полосы пропускания
  if (BandWidth < 6 || BandWidth > 8)
  {
    return 0;
  }
  // проверка модуляции
  if (Modulation != 0 && Modulation != 11 && Modulation != 12 && Modulation != 13)
  {
    return 0;
  }
  // проверка символьной скорости
  if (SR != 0 && (SR < 5000 || SR > 7000))
  {
    return 0;
  }
  return 1;
}

/**-----------------------------------------------------------------------------
 \fn     void SnmpWriteChPlanPoint (tCHAR* pChPlanPoint, tU16 Length)
 \brief  функция записи ячейки канального плана, полученной по SNMP, в буфер
 \param  pChPlanPoint - указатель на строку канального плана
 \param  Length - длина строки 
 \return нет
------------------------------------------------------------------------------*/
void SnmpWriteChPlanPoint (char* pChPlanPoint, uint16_t Length)
{
  uint32_t* pChPlan = NULL; // указатель канального плана для SNMP
  char StrCh[30];
  char NameCh[7];
  uint32_t Frequency;
  uint32_t Type;
  uint32_t BandWidth;
  uint32_t Modulation;
  uint32_t SR;
  char* pStr;
  
  
//  if (gITM18_Mode != ITM18_MODE_EDIT)
//  {
//    return;
//  }
 // pChPlan = (tCH_PLAN*)gCommonBuf; // настройка указателя канального плана
  // разбор строки канального плана
  memcpy(StrCh, pChPlanPoint, Length);
  StrCh[Length] = '\0';
  // поиск имени канала
  pStr = strstr(StrCh, ",");
  if ((pStr - StrCh) > 0 && (pStr - StrCh) < 7)
  {
    memcpy(NameCh, pChPlanPoint, (pStr - StrCh));
    NameCh[(pStr - StrCh)] = '\0';
  }
  else
  {
    return;
  }
  sscanf(pStr, ",%d,%d,%d,%d,%d", &Frequency, &Type, &BandWidth, &Modulation, &SR);
  // заполение параметров канала
//  pChPlan->ChPlanChannel[SnmpChannels].Channel.freq = Frequency / 125;
//  strcpy(pChPlan->ChPlanChannel[SnmpChannels].Channel.comment, NameCh);
  if (Type == 0) // аналоговый канал
  {
//    pChPlan->ChPlanChannel[SnmpChannels].Channel.type = 0;
//    pChPlan->ChPlanChannel[SnmpChannels].Channel.delta_fr = 0;
//    pChPlan->ChPlanChannel[SnmpChannels].DigChannel.Modulation = DVBC_MOD_UNKNOWN;
//    pChPlan->ChPlanChannel[SnmpChannels].DigChannel.SR = 0;
//    pChPlan->ChPlanChannel[SnmpChannels].DigChannel.Annex = DVBC_ANNEX_A;
//    pChPlan->ChPlanChannel[SnmpChannels].DigChannel.Interleaver = 0;
  }
  else if (Type == 1) // цифровой канал с неизвестной модуляцией
  {
//    pChPlan->ChPlanChannel[SnmpChannels].Channel.type = 1;
//    pChPlan->ChPlanChannel[SnmpChannels].Channel.width_fr = BandWidth * 8;
//    pChPlan->ChPlanChannel[SnmpChannels].DigChannel.Modulation = DVBC_MOD_UNKNOWN;
//    pChPlan->ChPlanChannel[SnmpChannels].DigChannel.SR = 0;
//    pChPlan->ChPlanChannel[SnmpChannels].DigChannel.Annex = DVBC_ANNEX_A;
//    pChPlan->ChPlanChannel[SnmpChannels].DigChannel.Interleaver = 0;
  }
  else // цифровой канал DVB-C
  {
//    pChPlan->ChPlanChannel[SnmpChannels].Channel.type = 1;
//    pChPlan->ChPlanChannel[SnmpChannels].Channel.width_fr = BandWidth * 8;
//    pChPlan->ChPlanChannel[SnmpChannels].DigChannel.Modulation = Modulation;
//    pChPlan->ChPlanChannel[SnmpChannels].DigChannel.SR = SR;
    if (Type == 2)
    {
//      pChPlan->ChPlanChannel[SnmpChannels].DigChannel.Annex = DVBC_ANNEX_A;
    }
    else if (Type == 3)
    {
//      pChPlan->ChPlanChannel[SnmpChannels].DigChannel.Annex = DVBC_ANNEX_B;
    }
    else if (Type == 4)
    {
//      pChPlan->ChPlanChannel[SnmpChannels].DigChannel.Annex = DVBC_ANNEX_C;
    }
//    pChPlan->ChPlanChannel[SnmpChannels].DigChannel.Interleaver = 0;
  }
  //SnmpChannels++; // увеличение числа переданных каналов
}

/**-----------------------------------------------------------------------------
 \fn     void SnmpWriteChPlan (void)
 \brief  функция записи канального плана, в память
 \param  нет 
 \return нет
------------------------------------------------------------------------------*/
void SnmpWriteChPlan (void)
{
//////  tCH_PLAN* pChPlan = NULL; // указатель канального плана для SNMP
//////  WORD FminLast;
//////  WORD FminCur;
//////  BYTE ChFmin;
//////  BYTE i;
//////  BYTE j;
//////  
//////  
//////  if (gITM18_Mode != ITM18_MODE_EDIT)
//////  {
//////    return;
//////  }
//////  pChPlan = (tCH_PLAN*)gCommonBuf; // настройка указателя канального плана
//////  
//////  // очищаем буфер от измерений - они больше не валидны
//////  gMeasuredChBuf = 0;
//////  gMeasuredCh = 0;
//////  gTxViewRSACh = 0;
//////  
//////  // сбрасываем параметры кэш
//////  ResetChPlanCache();
//////  
//////  // сохраняем заголовок частотного плана
//////  uChPlanChannels = SnmpChannels;
//////  pChPlan->ChPlanHead.n_points = SnmpChannels;
//////  strcpy(pChPlan->ChPlanHead.comment, "SNMP");
//////  write_head_mem((BYTE*)&pChPlan->ChPlanHead, 0, 0);
//////  // сохраняем заголовок цифрового частотного плана
//////  write_head_mem((BYTE*)&pChPlan->ChPlanHead, 0, 1);
//////  
//////  // сохранение каналов с сортировкой по частоте
//////  FminLast = (45000 / 125) - 1;
//////  for (i = 0; i < SnmpChannels; i++)
//////  {
//////    ChFmin = 0;
//////    FminCur = (1000000 / 125) + 1;
//////    // выбор номера канала с минимальной частотой, но не меньшей чем FminLast
//////    for (j = 0; j < SnmpChannels; j++)
//////    {
//////      if (pChPlan->ChPlanChannel[j].Channel.freq > FminLast && 
//////          pChPlan->ChPlanChannel[j].Channel.freq < FminCur)
//////      {
//////        ChFmin = j;
//////        FminCur = pChPlan->ChPlanChannel[j].Channel.freq;
//////      }
//////    }
//////    // сохранение канала
//////    // основной частотный план
//////    write_cell_mem((BYTE*)&pChPlan->ChPlanChannel[ChFmin].Channel, i, 0, 0);
//////    // дополнительный частотный план */
//////    write_cell_mem((BYTE*)&pChPlan->ChPlanChannel[ChFmin].DigChannel, i, 0, 1);
//////    FminLast = FminCur; 
//////  }
}

/**-----------------------------------------------------------------------------
 \fn     static void Meas_var_get_object_def (u8_t ident_len, s32_t *ident, struct obj_def *od)
 \brief  функция получения свойств переменных узла Measurement (счетчики, температура)
 \param  ident_len - the address length. длина адреса
 \param  ident - points to objectname.0 (object id trailer) 
 \param  od - points to object definition. указатель на описание переменной
 \return нет
------------------------------------------------------------------------------*/
static void Meas_var_get_object_def (u8_t ident_len, s32_t *ident, struct obj_def *od)
{
  u8_t id;

  /* return to object name, adding index depth (1) */
  ident_len += 1;
  ident -= 1;
  if (ident_len == 2)
  {
    od->id_inst_len = ident_len;
    od->id_inst_ptr = ident;

    id = ident[0];
    LWIP_DEBUGF(SNMP_MIB_DEBUG,("get_object_def planar.%"U16_F".0\n",(u16_t)id));
    switch (id)
    {
      case 1: /* channels number (integer, RO) */
        od->instance = MIB_OBJECT_SCALAR;
        od->access = MIB_OBJECT_READ_ONLY;
        od->asn_type = (SNMP_ASN1_UNIV | SNMP_ASN1_PRIMIT | SNMP_ASN1_INTEG);
        od->v_len = sizeof(s32_t);
        break;
      case 5: /* measurements counter (integer, RO) */
        od->instance = MIB_OBJECT_SCALAR;
        od->access = MIB_OBJECT_READ_ONLY;
        od->asn_type = (SNMP_ASN1_UNIV | SNMP_ASN1_PRIMIT | SNMP_ASN1_INTEG);
        od->v_len = sizeof(s32_t);
        break;
      case 6: /* temperature (integer, RO) */
        od->instance = MIB_OBJECT_SCALAR;
        od->access = MIB_OBJECT_READ_ONLY;
        od->asn_type = (SNMP_ASN1_UNIV | SNMP_ASN1_PRIMIT | SNMP_ASN1_INTEG);
        od->v_len = sizeof(s32_t);
        break;
		 default:
        LWIP_DEBUGF(SNMP_MIB_DEBUG,("planar_get_object_def: no such object\n"));
        od->instance = MIB_OBJECT_NONE;
        break;
    };
  }
  else
  {
    LWIP_DEBUGF(SNMP_MIB_DEBUG,("planar_get_object_def: no scalar\n"));
    od->instance = MIB_OBJECT_NONE;
  }
}

/**-----------------------------------------------------------------------------
 \fn     static void Meas_get_value (struct obj_def *od, u16_t len, void *value)
 \brief  функция получения значения переменных узла Measurement (счетчики, температура)
 \param  obj_def - указатель на описание переменной
 \param  len - длина переменной в байтах 
 \param  value - указатель на значение переменной
 \return нет
------------------------------------------------------------------------------*/
static void Meas_var_get_value (struct obj_def *od, u16_t len, void *value)
{
  u8_t id;

  id = od->id_inst_ptr[0];
  switch (id)
  {
    case 1: /* channels number */
    ///	*( (s32_t*)value ) = (s32_t)uChPlanChannels;
      break;
    case 5: /* measurements counter */
	///   *( (s32_t*)value ) = (s32_t)gMeasCounter;
      break;
	case 6: /* temperature */
	   *( (s32_t*)value ) = (s32_t)GetTemperature();
	  break;
  };
}

/**-----------------------------------------------------------------------------
 \fn     u16_t Chplan_level_length (void* addr_inf, u8_t level)
 \brief  функция сообщения числа узлов определенного уровня
 \param  addr_inf
 \param  level - уровень 
 \return число узлов
------------------------------------------------------------------------------*/
u16_t Chplan_level_length (void* addr_inf, u8_t level)
{
	u16_t nodes_num = 0;
	
	
	(void)addr_inf;
	
	if( level == 0 )
	{
		nodes_num = 1;
	}
	else if( level == 1 )
	{
		nodes_num = 7;
	}
	else if( level == 2 )
	{
	//	nodes_num = (u16_t)uChPlanChannels;
	}

	return nodes_num;
}

/**-----------------------------------------------------------------------------
 \fn     s32_t Chplan_get_ident_cmp (void* addr_inf, u8_t level, u16_t idx, s32_t sub_id)
 \brief  функция проверки совпадения пути при поиске
 \param  addr_inf
 \param  level - уровень
 \param  idx - индекс
 \param  sub_id - уровень
 \return 0 - совпадение, не 0 - несовпадение
------------------------------------------------------------------------------*/
s32_t Chplan_get_ident_cmp (void* addr_inf, u8_t level, u16_t idx, s32_t sub_id)
{
	s32_t unsuccess = -1;
	s32_t my_sub_id;
	
    
	(void)addr_inf;
	
	if( level == 0 && idx < 1 )
	{
		my_sub_id = idx + 1;
		if( my_sub_id == sub_id )
		{
			unsuccess = 0;
		}				
	}
	else if( level == 1 && idx < 7 )
	{
		my_sub_id = idx + 1;
		if( my_sub_id == sub_id )
		{
			unsuccess = 0;
		}
	}
	else if( level == 2 && idx < (u16_t)255)//uChPlanChannels )
	{
		my_sub_id = idx + 1;
		if( my_sub_id == sub_id )
		{
			unsuccess = 0;
		}
	}
	
	return unsuccess;
}

/**-----------------------------------------------------------------------------
 \fn     void Chplan_get_objid (void* addr_inf, u8_t level, u16_t idx, s32_t *sub_id)
 \brief  функция получения OID узла
 \param  addr_inf
 \param  level - уровень
 \param  idx - индекс
 \param  sub_id - адрес OID узла
 \return нет
------------------------------------------------------------------------------*/
void Chplan_get_objid (void* addr_inf, u8_t level, u16_t idx, s32_t *sub_id)
{
	(void)addr_inf;
	
	if( level == 0 )
	{
		*sub_id = idx + 1;
	}
	else if( level == 1 )
	{
		*sub_id = idx + 1;
	}
	else if( level == 2 )
	{
		*sub_id = idx + 1;
	}
}

/**-----------------------------------------------------------------------------
 \fn     static void Chplan_get_object_def (u8_t rid, u8_t ident_len, s32_t *ident, struct obj_def *od)
 \brief  функция получения свойств переменных узла Measurements (канальный план)
 \param  rid - 
 \param  ident_len - the address length. длина адреса
 \param  ident - points to objectname.0 (object id trailer) 
 \param  od - points to object definition. указатель на описание переменной
 \return нет
------------------------------------------------------------------------------*/
static void Chplan_get_object_def (u8_t rid, u8_t ident_len, s32_t *ident, struct obj_def *od)
{
  u8_t id;
  u8_t ch_num;
 // point_fr_map* ptr;

    uint32_t* ptr;

  
  (void)rid;
  
  /* return to object name, adding index depth (1) */
  ident_len += 1;
  ident -= 1;
  if (ident_len == 2)
  {
    od->id_inst_len = ident_len;
    od->id_inst_ptr = ident;

    id = ident[0];
	ch_num = ident[1];
    LWIP_DEBUGF(SNMP_MIB_DEBUG,("get_object_def planar.%"U16_F".0\n",(u16_t)id));
    switch (id)
    {
	case 1: /* channel index (integer, RO) */
        od->instance = MIB_OBJECT_SCALAR;
        od->access = MIB_OBJECT_READ_ONLY;
        od->asn_type = (SNMP_ASN1_UNIV | SNMP_ASN1_PRIMIT | SNMP_ASN1_INTEG);
        od->v_len = sizeof(s32_t);
		break;
	case 2: /* channel name (string, RO) */
//        ptr = snmp_GetCacheChan( ch_num - 1 );
//		od->instance = MIB_OBJECT_SCALAR;
//        od->access = MIB_OBJECT_READ_ONLY;
//        od->asn_type = (SNMP_ASN1_UNIV | SNMP_ASN1_PRIMIT | SNMP_ASN1_OC_STR);
//        od->v_len = strnlen( ptr->comment, sizeof( ptr->comment ) );
        break;
    case 3: /* frequency (integer, RO) */
        od->instance = MIB_OBJECT_SCALAR;
        od->access = MIB_OBJECT_READ_ONLY;
        od->asn_type = (SNMP_ASN1_UNIV | SNMP_ASN1_PRIMIT | SNMP_ASN1_INTEG);
        od->v_len = sizeof(s32_t);
        break;
    case 4: /* channel type (integer, RO) */
        od->instance = MIB_OBJECT_SCALAR;
        od->access = MIB_OBJECT_READ_ONLY;
        od->asn_type = (SNMP_ASN1_UNIV | SNMP_ASN1_PRIMIT | SNMP_ASN1_INTEG);
        od->v_len = sizeof(s32_t);
        break;
    case 5: /* channel BandWidth (integer, RO) */
        od->instance = MIB_OBJECT_SCALAR;
        od->access = MIB_OBJECT_READ_ONLY;
        od->asn_type = (SNMP_ASN1_UNIV | SNMP_ASN1_PRIMIT | SNMP_ASN1_INTEG);
        od->v_len = sizeof(s32_t);
        break;
    case 6: /* channel Modulation (integer, RO) */
        od->instance = MIB_OBJECT_SCALAR;
        od->access = MIB_OBJECT_READ_ONLY;
        od->asn_type = (SNMP_ASN1_UNIV | SNMP_ASN1_PRIMIT | SNMP_ASN1_INTEG);
        od->v_len = sizeof(s32_t);
        break;
    case 7: /* channel Symbol Rate (integer, RO) */
        od->instance = MIB_OBJECT_SCALAR;
        od->access = MIB_OBJECT_READ_ONLY;
        od->asn_type = (SNMP_ASN1_UNIV | SNMP_ASN1_PRIMIT | SNMP_ASN1_INTEG);
        od->v_len = sizeof(s32_t);
        break;
    default:
        LWIP_DEBUGF(SNMP_MIB_DEBUG,("planar_get_object_def: no such object\n"));
        od->instance = MIB_OBJECT_NONE;
        break;
    };
  }
  else
  {
    LWIP_DEBUGF(SNMP_MIB_DEBUG,("planar_get_object_def: no scalar\n"));
    od->instance = MIB_OBJECT_NONE;
  }
}

/**-----------------------------------------------------------------------------
 \fn     static void Chplan_get_value(u8_t rid, struct obj_def *od, u16_t len, void *value)
 \brief  функция получения значения переменных узла Measurements (канальный план)
 \param  rid - 
 \param  obj_def - указатель на описание переменной
 \param  len - длина переменной в байтах 
 \param  value - указатель на значение переменной
 \return нет
------------------------------------------------------------------------------*/
static void Chplan_get_value(u8_t rid, struct obj_def *od, u16_t len, void *value)
{
  u8_t id;
  u8_t ch_num;
//  point_fr_map* ptr;
//  point_fr_map_dig* ptr_dig;
  

  id = od->id_inst_ptr[0];
  ch_num = od->id_inst_ptr[1];
//  ptr = snmp_GetCacheChan(ch_num - 1);
//  ptr_dig = snmp_GetCacheChan_dig(ch_num - 1);
  switch (id)
  {
  case 1: /* channel index */
    *((s32_t*)value) = ch_num;
    break;
  case 2: /* channel name */
	//ocstrncpy(value, (u8_t*)ptr->comment, len);
    break;
  case 3: /* frequency */
	//*((s32_t*)value) = (s32_t)ptr->freq * 125ul;
    break;
  case 4: /* channel type */
//    if (ptr->type == 0)
//    {
//      *((s32_t*)value) = 0;
//    }
//    else if (ptr_dig->Modulation == DVBC_MOD_UNKNOWN)
//    {
//      *((s32_t*)value) = 1;
//    }
//    else if (ptr_dig->Annex == DVBC_ANNEX_A)
//    {
//      *((s32_t*)value) = 2;
//    }
//    else if (ptr_dig->Annex == DVBC_ANNEX_B)
//    {
//      *((s32_t*)value) = 3;
//    }
//    else if (ptr_dig->Annex == DVBC_ANNEX_C)
//    {
//      *((s32_t*)value) = 4;
//    }
    break;
  case 5: /* channel band width */
//    if (ptr->type != 0)
//    {
//      *((s32_t*)value) = (s32_t)ptr->width_fr * 125ul;
//    }
//    else
//    {
//      *((s32_t*)value) = 0;
//    }
    break;
  case 6: /* channel modulation */
//    if (ptr->type != 0)
//    {
//    //  *((s32_t*)value) = (s32_t)ptr_dig->Modulation;
//    }
//    else
//    {
//      *((s32_t*)value) = 0;
//    }
    break;
  case 7: /* symbol rate */
//    if (ptr->type != 0)
//    {
//   //   *((s32_t*)value) = (s32_t)ptr_dig->SR;
//    }
//    else
//    {
//      *((s32_t*)value) = 0;
//    }
    break;
  };
}

/**-----------------------------------------------------------------------------
 \fn     u16_t Meastable_level_length (void* addr_inf, u8_t level)
 \brief  функция сообщения числа узлов определенного уровня
 \param  addr_inf
 \param  level - уровень 
 \return число узлов
------------------------------------------------------------------------------*/
u16_t Meastable_level_length (void* addr_inf, u8_t level)
{
	u16_t nodes_num = 0;
	
	
	(void)addr_inf;
	
	if( level == 0 )
	{
		nodes_num = 1;
	}
	else if( level == 1 )
	{
		nodes_num = 7;
	}
	else if( level == 2 )
	{
		//nodes_num = (u16_t)uChPlanChannels;
	}

	
	return nodes_num;
}

/**-----------------------------------------------------------------------------
 \fn     s32_t Meastable_get_ident_cmp (void* addr_inf, u8_t level, u16_t idx, s32_t sub_id)
 \brief  функция проверки совпадения пути при поиске
 \param  addr_inf
 \param  level - уровень
 \param  idx - индекс
 \param  sub_id - уровень
 \return 0 - совпадение, не 0 - несовпадение
------------------------------------------------------------------------------*/
s32_t Meastable_get_ident_cmp (void* addr_inf, u8_t level, u16_t idx, s32_t sub_id)
{
	s32_t unsuccess = -1;
	s32_t my_sub_id;
	
	
	(void)addr_inf;
	
	if( level == 0 && idx < 1 )
	{
		my_sub_id = idx + 1;
		if( my_sub_id == sub_id )
		{
			unsuccess = 0;
		}				
	}
	else if( level == 1 && idx < 7 )
	{
		my_sub_id = idx + 1;
		if( my_sub_id == sub_id )
		{
			unsuccess = 0;
		}
	}
	else if( level == 2 && idx < (u16_t)255)//uChPlanChannels )
	{
		my_sub_id = idx + 1;
		if( my_sub_id == sub_id )
		{
			unsuccess = 0;
		}
	}
	
	return unsuccess;
}

/**-----------------------------------------------------------------------------
 \fn     void Meastable_get_objid (void* addr_inf, u8_t level, u16_t idx, s32_t *sub_id)
 \brief  функция получения OID узла
 \param  addr_inf
 \param  level - уровень
 \param  idx - индекс
 \param  sub_id - адрес OID узла
 \return нет
------------------------------------------------------------------------------*/
void Meastable_get_objid (void* addr_inf, u8_t level, u16_t idx, s32_t *sub_id)
{
	(void)addr_inf;
	
	
	if( level == 0 )
	{
		*sub_id = idx + 1;
	}
	else if( level == 1 )
	{
		*sub_id = idx + 1;
	}
	else if( level == 2 )
	{
		*sub_id = idx + 1;
	}
}

/**-----------------------------------------------------------------------------
 \fn     static void Meastable_get_object_def (u8_t rid, u8_t ident_len, s32_t *ident, struct obj_def *od)
 \brief  функция получения свойств переменных узла Measurements (измерения)
 \param  rid - 
 \param  ident_len - the address length. длина адреса
 \param  ident - points to objectname.0 (object id trailer) 
 \param  od - points to object definition. указатель на описание переменной
 \return нет
------------------------------------------------------------------------------*/
static void Meastable_get_object_def (u8_t rid, u8_t ident_len, s32_t *ident, struct obj_def *od)
{
  u8_t id;

  
  (void)rid;
  
  /* return to object name, adding index depth (1) */
  ident_len += 1;
  ident -= 1;
  if (ident_len == 2)
  {
    od->id_inst_len = ident_len;
    od->id_inst_ptr = ident;

    id = ident[0];
    LWIP_DEBUGF(SNMP_MIB_DEBUG,("get_object_def planar.%"U16_F".0\n",(u16_t)id));
    switch (id)
    {
	case 1: /* channel index (integer, RO) */
	case 2: /* Level (integer, RO) */
	case 3: /* VAR (integer, RO) */
	case 4: /* CNR (integer, RO) */
	case 5: /* MER (integer, RO) */
      od->instance = MIB_OBJECT_SCALAR;
      od->access = MIB_OBJECT_READ_ONLY;
      od->asn_type = (SNMP_ASN1_UNIV | SNMP_ASN1_PRIMIT | SNMP_ASN1_INTEG);
      od->v_len = sizeof(s32_t);
	  break;		
	case 6: /* preBER (counter, RO) */
	case 7: /* postBER (counter, RO) */
      od->instance = MIB_OBJECT_SCALAR;
      od->access = MIB_OBJECT_READ_ONLY;
      od->asn_type = (SNMP_ASN1_APPLIC | SNMP_ASN1_PRIMIT | SNMP_ASN1_COUNTER);
      od->v_len = sizeof(s32_t);
	  break;
	default:
      LWIP_DEBUGF(SNMP_MIB_DEBUG,("planar_get_object_def: no such object\n"));
      od->instance = MIB_OBJECT_NONE;
      break;
    };
  }
  else
  {
    LWIP_DEBUGF(SNMP_MIB_DEBUG,("planar_get_object_def: no scalar\n"));
    od->instance = MIB_OBJECT_NONE;
  }
}

/**-----------------------------------------------------------------------------
 \fn     static void Meastable_get_value (u8_t rid, struct obj_def *od, u16_t len, void *value)
 \brief  функция получения значения переменных узла Measurements (измерения)
 \param  rid - 
 \param  obj_def - указатель на описание переменной
 \param  len - длина переменной в байтах 
 \param  value - указатель на значение переменной
 \return нет
------------------------------------------------------------------------------*/
static void Meastable_get_value (u8_t rid, struct obj_def *od, u16_t len, void *value)
{
  u8_t id;
  u8_t ch_num;
//  TMeasSliceCh* tMeasVal = NULL;
//  point_fr_map* pChParam;

  
  id = od->id_inst_ptr[0];
  ch_num = od->id_inst_ptr[1];
  
////  if (gITM18_Mode == 255)//ITM18_MODE_MEASURE)
////  {
////  //  tMeasVal = &(((TMeasSlice*)gCommonBuf)->tCh[ch_num - 1]);
////  //  pChParam = snmp_GetCacheChan(ch_num - 1);
////  }
  switch (id)
  {
  case 1: /* channel index */
	*((s32_t*)value) = ch_num;
	break;
  case 2: /* level */
//    if (tMeasVal == NULL || ch_num > gMeasuredChBuf)
//    {
//      *((s32_t*)value) = 0;
//    }
//    else if (pChParam->type == 0)
//    {
//      xSemaphoreTake(gMeasResultMutex, portMAX_DELAY); // захват мьютекса результатов измерений
//      *((s32_t*)value) = (s32_t)tMeasVal->tAnChan.wLevel;
//      xSemaphoreGive(gMeasResultMutex);                // освобождение мьютекса результатов измерений
//    }
//    else
//    {
//      xSemaphoreTake(gMeasResultMutex, portMAX_DELAY); // захват мьютекса результатов измерений
//      *((s32_t*)value) = (s32_t)tMeasVal->tDgChan.dwfLevel;
//      xSemaphoreGive(gMeasResultMutex);                // освобождение мьютекса результатов измерений
//    }
    break;
  case 3: /* VAR */
   // *((s32_t*)value) = (s32_t)0;
    break;
  case 4: /* CNR */
  //  *((s32_t*)value) = (s32_t)0;
	break;
  case 5: /* MER */
//    xSemaphoreTake(gMeasResultMutex, portMAX_DELAY); // захват мьютекса результатов измерений
//    if (tMeasVal == NULL || ch_num > gMeasuredChBuf)
//    {
//      *((s32_t*)value) = 0;
//    }
//    else if (pChParam->type != 0 && tMeasVal->tDgChan.dwbChanLock)
//    {
//	  *((s32_t*)value) = (s32_t)tMeasVal->tDgChan.dwfMER;
//    }
//    else
//    {
//      *((s32_t*)value) = (s32_t)0;
//    }
//    xSemaphoreGive(gMeasResultMutex);                // освобождение мьютекса результатов измерений
    break;
  case 6: /* preBER */
  case 7: /* postBER */
//    if (tMeasVal == NULL || ch_num > gMeasuredChBuf)
//    {
//      *((s32_t*)value) = 0;
//    }
//    else if (pChParam->type != 0)
//    {
//      xSemaphoreTake(gMeasResultMutex, portMAX_DELAY); // захват мьютекса результатов измерений
//      if (tMeasVal->tDgChan.dwbChanLock)
//      {
//        if (tMeasVal->tDgChan.dwbBerOff)
//        {
//          *((u32_t*)value) = (u32_t)0;
//        }
//        else if( id == 6 )
//        {
//          *((u32_t*)value) = snmp_ConvertBER(tMeasVal->tDgChan.dwfPreBER_exp, 
//                                             tMeasVal->tDgChan.uPreBER_man );
//        }
//        else
//        {
//          *((u32_t*)value) = snmp_ConvertBER(tMeasVal->tDgChan.dwfPostBER_exp, 
//                                             tMeasVal->tDgChan.uPostBER_man );
//        }
//      }
//      else
//      {
//        *((u32_t*)value) = 0xFFFFFFFF;
//      }
//      xSemaphoreGive(gMeasResultMutex);                // освобождение мьютекса результатов измерений
//    }
//    else
//    {
//      *((s32_t*)value) = (s32_t)0;
//    }
    break;
  };
}

/**-----------------------------------------------------------------------------
 \fn     u32_t snmp_ConvertBER (u8_t exp, u8_t man)
 \brief  функция преобразование BER в число ошибок на 10e10 бит
 \param  exp - экспонента
 \param  man - мантисса 
 \return число ошибок на 10e10 бит
------------------------------------------------------------------------------*/
u32_t snmp_ConvertBER (u8_t exp, u8_t man)
{
	u32_t Errors;
	u32_t i;
	
	
	if( exp <= 10 )
	{
		Errors = man;
		i = 10 - exp;
	
		while( i-- )
		{
			Errors *= 10ul;
		}
	}
	else
	{
		Errors = 1;
	}
	
	return Errors;
}

/**-----------------------------------------------------------------------------
 \fn     u16_t Errtable_level_length (void* addr_inf, u8_t level)
 \brief  функция сообщения числа узлов определенного уровня
 \param  addr_inf
 \param  level - уровень 
 \return число узлов
------------------------------------------------------------------------------*/
u16_t Errtable_level_length (void* addr_inf, u8_t level)
{
	u16_t nodes_num = 0;
	
	
	(void)addr_inf;
	
	if( level == 0 )
	{
		nodes_num = 1;
	}
	else if( level == 1 )
	{
		nodes_num = 16;
	}
	else if( level == 2 )
	{
		//nodes_num = (u16_t)uChPlanChannels;
	}

	return nodes_num;
}

/**-----------------------------------------------------------------------------
 \fn     s32_t Errtable_get_ident_cmp (void* addr_inf, u8_t level, u16_t idx, s32_t sub_id)
 \brief  функция проверки совпадения пути при поиске
 \param  addr_inf
 \param  level - уровень
 \param  idx - индекс
 \param  sub_id - уровень
 \return 0 - совпадение, 1 - несовпадение
------------------------------------------------------------------------------*/
s32_t Errtable_get_ident_cmp (void* addr_inf, u8_t level, u16_t idx, s32_t sub_id)
{
	s32_t unsuccess = -1;
	s32_t my_sub_id;
	
	
	(void)addr_inf;
	
	if( level == 0 && idx < 1 )
	{
		my_sub_id = idx + 1;
		if( my_sub_id == sub_id )
		{
			unsuccess = 0;
		}				
	}
	else if( level == 1 && idx < 16 )
	{
		my_sub_id = idx + 1;
		if( my_sub_id == sub_id )
		{
			unsuccess = 0;
		}
	}
	else if( level == 2 && idx < (u16_t)255)//uChPlanChannels )
	{
		my_sub_id = idx + 1;
		if( my_sub_id == sub_id )
		{
			unsuccess = 0;
		}
	}
	
	return unsuccess;
}

/**-----------------------------------------------------------------------------
 \fn     void Errtable_get_objid (void* addr_inf, u8_t level, u16_t idx, s32_t *sub_id)
 \brief  функция получения OID узла
 \param  addr_inf
 \param  level - уровень
 \param  idx - индекс
 \param  sub_id - адрес OID узла
 \return нет
------------------------------------------------------------------------------*/
void Errtable_get_objid (void* addr_inf, u8_t level, u16_t idx, s32_t *sub_id)
{
	(void)addr_inf;
	
	if( level == 0 )
	{
		*sub_id = idx + 1;
	}
	else if( level == 1 )
	{
		*sub_id = idx + 1;
	}
	else if( level == 2 )
	{
		*sub_id = idx + 1;
	}
}

/**-----------------------------------------------------------------------------
 \fn     static void Errtable_get_object_def (u8_t rid, u8_t ident_len, s32_t *ident, struct obj_def *od)
 \brief  функция получения свойств переменных узла Measurements (ошибки)
 \param  rid - 
 \param  ident_len - the address length. длина адреса
 \param  ident - points to objectname.0 (object id trailer) 
 \param  od - points to object definition. указатель на описание переменной
 \return нет
------------------------------------------------------------------------------*/
static void Errtable_get_object_def (u8_t rid, u8_t ident_len, s32_t *ident, struct obj_def *od)
{
  u8_t id;

  
  (void)rid;
  
  /* return to object name, adding index depth (1) */
  ident_len += 1;
  ident -= 1;
  if (ident_len == 2)
  {
    od->id_inst_len = ident_len;
    od->id_inst_ptr = ident;

    id = ident[0];
    LWIP_DEBUGF(SNMP_MIB_DEBUG,("get_object_def planar.%"U16_F".0\n",(u16_t)id));
    
	 if( id > 0 && id <= 16 )
	 {
	   od->instance = MIB_OBJECT_SCALAR;
       od->access = MIB_OBJECT_READ_ONLY;
       od->asn_type = (SNMP_ASN1_UNIV | SNMP_ASN1_PRIMIT | SNMP_ASN1_INTEG);
       od->v_len = sizeof(s32_t);
	 }
	 else
	 {
	   LWIP_DEBUGF(SNMP_MIB_DEBUG,("planar_get_object_def: no such object\n"));
       od->instance = MIB_OBJECT_NONE;
	 }
  }
  else
  {
    LWIP_DEBUGF(SNMP_MIB_DEBUG,("planar_get_object_def: no scalar\n"));
    od->instance = MIB_OBJECT_NONE;
  }
}

/**-----------------------------------------------------------------------------
 \fn     static void Errtable_get_value (u8_t rid, struct obj_def *od, u16_t len, void *value)
 \brief  функция получения значения переменных узла Measurements (ошибки)
 \param  rid - 
 \param  obj_def - указатель на описание переменной
 \param  len - длина переменной в байтах 
 \param  value - указатель на значение переменной
 \return нет
------------------------------------------------------------------------------*/
static void Errtable_get_value (u8_t rid, struct obj_def *od, u16_t len, void *value)
{
  u8_t id;
  u8_t ch_num;
  

  id = od->id_inst_ptr[0];
  ch_num = od->id_inst_ptr[1];
////
////  /* index */
////  if( id == 1 )
////  {
////     *((s32_t*)value) = ch_num;
////  }
////  /* alert */
////  else if( id == 2 )
////  {
//////    if( dwChOveralErrors[ch_num - 1] != 0 )
//////	 {
//////		 *((s32_t*)value) = 1ul;
//////	 }
//////	 else
//////	 {
//////		 *((s32_t*)value) = 0ul;
//////	 }
////  }
////  /* [3...7] lowLevel, highLevel, lowVAR, highVAR, lowCNR */
////  else if( id < 8 )
////  {
//////	 if( CHERR_GETERROR( dwChOveralErrors[ch_num - 1], id - 3 ) )
//////    {
//////		 *((s32_t*)value) = 1ul;
//////	 }
//////	 else
//////	 {
//////		 *((s32_t*)value) = 0ul;
//////	 }
////  }
////  /* [8...10] lowMER, highPreBER, highPostBER */
////  else if( id < 11 )
////  {
//////	 if( CHERR_GETERROR( dwChOveralErrors[ch_num - 1], id - 3 ) ||
//////		  CHERR_GETERROR( dwChOveralErrors[ch_num - 1], CHERR_DG_NOLOCK ) )
//////    {
//////		 *((s32_t*)value) = 1ul;
//////	 }
//////	 else
//////	 {
//////		 *((s32_t*)value) = 0ul;
//////	 }
////  }
////  /* [11] highDL_adjacent */
////  else if( id == 11 )
////  {
//////	 if( CHERR_GETERROR( dwChOveralErrors[ch_num - 1], CHERR_HIGHDLADJ_PREV ) ||
//////		  CHERR_GETERROR( dwChOveralErrors[ch_num - 1], CHERR_HIGHDLADJ_NEXT ) )
//////    {
//////		 *((s32_t*)value) = 1ul;
//////	 }
//////	 else
//////	 {
//////		 *((s32_t*)value) = 0ul;
//////	 }
////  }
////  /* [12...16] highDL_40_300, highDL_40_600, highDL_40_1000, highDL_DF_100, 
////  highDL_An_Dg */
////  else if( id < 17 )
////  {
//////	 if( CHERR_GETERROR( dwChOveralErrors[ch_num - 1], id - 2 ) )
//////    {
//////		 *((s32_t*)value) = 1ul;
//////	 }
//////	 else
//////	 {
//////		 *((s32_t*)value) = 0ul;
//////	 }
//////  }
}

// функции для узла ALARM

/**-----------------------------------------------------------------------------
 \fn     static void ciu_alarm_hardware_status_get_object_def (u8_t ident_len, s32_t *ident, struct obj_def *od)
 \brief  функция получения свойств переменных узла Alarm (hardwareError)
 \param  ident_len - the address length. длина адреса
 \param  ident - points to objectname.0 (object id trailer) 
 \param  od - points to object definition. указатель на описание переменной
 \return нет
------------------------------------------------------------------------------*/
static void ciu_alarm_hardware_status_get_object_def (u8_t ident_len, s32_t *ident, struct obj_def *od)
{
  u8_t id;

  /* return to object name, adding index depth (1) */
  ident_len += 1;
  ident -= 1;
  if (ident_len == 2)
  {
    od->id_inst_len = ident_len;
    od->id_inst_ptr = ident;

    id = ident[0];
    LWIP_DEBUGF(SNMP_MIB_DEBUG,("get_object_def planar.%"U16_F".0\n",(u16_t)id));
////    switch (id)
////    {
////      case 2: /* hardware status (string, NA) */
////        od->instance = MIB_OBJECT_SCALAR;
////        od->access = MIB_OBJECT_NOT_ACCESSIBLE;
////        od->asn_type = (SNMP_ASN1_UNIV | SNMP_ASN1_PRIMIT | SNMP_ASN1_OC_STR);
//////        if( szHardwareStatus != NULL )
//////		   od->v_len = strlen( szHardwareStatus );
//////		else
//////		   od->v_len = 0; 
////        break;
////		default:
////        LWIP_DEBUGF(SNMP_MIB_DEBUG,("planar_get_object_def: no such object\n"));
////        od->instance = MIB_OBJECT_NONE;
////        break;
////    };
  }
  else
  {
    LWIP_DEBUGF(SNMP_MIB_DEBUG,("planar_get_object_def: no scalar\n"));
    od->instance = MIB_OBJECT_NONE;
  }
}

/**-----------------------------------------------------------------------------
 \fn     static void ciu_alarm_hardware_status_get_value (struct obj_def *od, u16_t len, void *value)
 \brief  функция получения значения переменных узла Alarm (hardwareError)
 \param  obj_def - указатель на описание переменной
 \param  len - длина переменной в байтах 
 \param  value - указатель на значение переменной
 \return нет
------------------------------------------------------------------------------*/
static void ciu_alarm_hardware_status_get_value (struct obj_def *od, u16_t len, void *value)
{
  u8_t id;

  id = od->id_inst_ptr[0];
  switch (id)
  {
  case 2: /* hardware status */
  //  ocstrncpy(value, (u8_t*)szHardwareStatus, len);
    break;
  };
}

/**-----------------------------------------------------------------------------
 \fn     static void ciu_alarm_temperature_status_get_object_def (u8_t ident_len, s32_t *ident, struct obj_def *od)
 \brief  функция получения свойств переменных узла Alarm (temperature)
 \param  ident_len - the address length. длина адреса
 \param  ident - points to objectname.0 (object id trailer) 
 \param  od - points to object definition. указатель на описание переменной
 \return нет
------------------------------------------------------------------------------*/
static void ciu_alarm_temperature_status_get_object_def (u8_t ident_len, s32_t *ident, struct obj_def *od)
{
  u8_t id;

  /* return to object name, adding index depth (1) */
  ident_len += 1;
  ident -= 1;
  if (ident_len == 2)
  {
    od->id_inst_len = ident_len;
    od->id_inst_ptr = ident;

    id = ident[0];
    LWIP_DEBUGF(SNMP_MIB_DEBUG,("get_object_def planar.%"U16_F".0\n",(u16_t)id));
    switch (id)
    {
      case 2: /* temperature status (string, NA) */
        od->instance = MIB_OBJECT_SCALAR;
        od->access = MIB_OBJECT_NOT_ACCESSIBLE;
        od->asn_type = (SNMP_ASN1_UNIV | SNMP_ASN1_PRIMIT | SNMP_ASN1_OC_STR);
//        if( szTemperatureStatus != NULL )
//		    od->v_len = strlen( szTemperatureStatus );
//		else
//			od->v_len = 0;
        break;
		default:
        LWIP_DEBUGF(SNMP_MIB_DEBUG,("planar_get_object_def: no such object\n"));
        od->instance = MIB_OBJECT_NONE;
        break;
    };
  }
  else
  {
    LWIP_DEBUGF(SNMP_MIB_DEBUG,("planar_get_object_def: no scalar\n"));
    od->instance = MIB_OBJECT_NONE;
  }
}

/**-----------------------------------------------------------------------------
 \fn     static void ciu_alarm_temperature_status_get_value (struct obj_def *od, u16_t len, void *value)
 \brief  функция получения значения переменных узла Alarm (temperature)
 \param  obj_def - указатель на описание переменной
 \param  len - длина переменной в байтах 
 \param  value - указатель на значение переменной
 \return нет
------------------------------------------------------------------------------*/
static void ciu_alarm_temperature_status_get_value (struct obj_def *od, u16_t len, void *value)
{
  u8_t id;

  
  id = od->id_inst_ptr[0];
  switch (id)
  {
  case 2: /* temperature status */
  //  ocstrncpy(value, (u8_t*)szTemperatureStatus, len);
    break;
  };
}

/**-----------------------------------------------------------------------------
 \fn     static void ciu_alarm_channels_status_get_object_def (u8_t ident_len, s32_t *ident, struct obj_def *od)
 \brief  функция получения свойств переменных узла Alarm (channels)
 \param  ident_len - the address length. длина адреса
 \param  ident - points to objectname.0 (object id trailer) 
 \param  od - points to object definition. указатель на описание переменной
 \return нет
------------------------------------------------------------------------------*/
static void ciu_alarm_channels_status_get_object_def (u8_t ident_len, s32_t *ident, struct obj_def *od)
{
  u8_t id;

  /* return to object name, adding index depth (1) */
  ident_len += 1;
  ident -= 1;
  if (ident_len == 2)
  {
    od->id_inst_len = ident_len;
    od->id_inst_ptr = ident;

    id = ident[0];
    LWIP_DEBUGF(SNMP_MIB_DEBUG,("get_object_def planar.%"U16_F".0\n",(u16_t)id));
    switch (id)
    {
      case 2: /* level severity (string, NA) */
        od->instance = MIB_OBJECT_SCALAR;
        od->access = MIB_OBJECT_NOT_ACCESSIBLE;
        od->asn_type = (SNMP_ASN1_UNIV | SNMP_ASN1_PRIMIT | SNMP_ASN1_OC_STR);
	//	  od->v_len = strnlen( szSnmp_Level, sizeof(szSnmp_Level) );
        break;
      case 3: /* VAR severity (string, NA) */
        od->instance = MIB_OBJECT_SCALAR;
        od->access = MIB_OBJECT_NOT_ACCESSIBLE;
        od->asn_type = (SNMP_ASN1_UNIV | SNMP_ASN1_PRIMIT | SNMP_ASN1_OC_STR);
	//	  od->v_len = strnlen( szSnmp_VAR, sizeof(szSnmp_VAR) );
        break;
      case 4: /* CNR severity (string, NA) */
        od->instance = MIB_OBJECT_SCALAR;
        od->access = MIB_OBJECT_NOT_ACCESSIBLE;
        od->asn_type = (SNMP_ASN1_UNIV | SNMP_ASN1_PRIMIT | SNMP_ASN1_OC_STR);
	//	  od->v_len = strnlen( szSnmp_CNR, sizeof(szSnmp_CNR) );
        break;
      case 5: /* MER severity (string, NA) */
        od->instance = MIB_OBJECT_SCALAR;
        od->access = MIB_OBJECT_NOT_ACCESSIBLE;
        od->asn_type = (SNMP_ASN1_UNIV | SNMP_ASN1_PRIMIT | SNMP_ASN1_OC_STR);
	//	  od->v_len = strnlen( szSnmp_MER, sizeof(szSnmp_MER) );
        break;
      case 6: /* preBER severity (string, NA) */
        od->instance = MIB_OBJECT_SCALAR;
        od->access = MIB_OBJECT_NOT_ACCESSIBLE;
        od->asn_type = (SNMP_ASN1_UNIV | SNMP_ASN1_PRIMIT | SNMP_ASN1_OC_STR);
	//	  od->v_len = strnlen( szSnmp_preBER, sizeof(szSnmp_preBER) );
        break;
      case 7: /* postBER severity (string, NA) */
        od->instance = MIB_OBJECT_SCALAR;
        od->access = MIB_OBJECT_NOT_ACCESSIBLE;
        od->asn_type = (SNMP_ASN1_UNIV | SNMP_ASN1_PRIMIT | SNMP_ASN1_OC_STR);
	//	  od->v_len = strnlen( szSnmp_postBER, sizeof(szSnmp_postBER) );
        break;
		 default:
        LWIP_DEBUGF(SNMP_MIB_DEBUG,("planar_get_object_def: no such object\n"));
        od->instance = MIB_OBJECT_NONE;
        break;
    };
  }
  else
  {
    LWIP_DEBUGF(SNMP_MIB_DEBUG,("planar_get_object_def: no scalar\n"));
    od->instance = MIB_OBJECT_NONE;
  }
}

/**-----------------------------------------------------------------------------
 \fn     static void ciu_alarm_channels_status_get_value (struct obj_def *od, u16_t len, void *value)
 \brief  функция получения значения переменных узла Alarm (channels)
 \param  obj_def - указатель на описание переменной
 \param  len - длина переменной в байтах 
 \param  value - указатель на значение переменной
 \return нет
------------------------------------------------------------------------------*/
static void ciu_alarm_channels_status_get_value (struct obj_def *od, u16_t len, void *value)
{
  u8_t id;

  
  id = od->id_inst_ptr[0];
  switch (id)
  {
    case 2: /* level severity */
   ///   ocstrncpy(value, (u8_t*)szSnmp_Level, len);
		break;
    case 3: /* VAR severity */
   //   ocstrncpy(value, (u8_t*)szSnmp_VAR, len);
		break;
	 case 4: /* CNR severity */
   //   ocstrncpy(value, (u8_t*)szSnmp_CNR, len);
		break;
    case 5: /* MER severity */
   //   ocstrncpy(value, (u8_t*)szSnmp_MER, len);
		break;
    case 6: /* preBER severity */
   //   ocstrncpy(value, (u8_t*)szSnmp_preBER, len);
		break;
    case 7: /* postBER severity */
   //   ocstrncpy(value, (u8_t*)szSnmp_postBER, len);
      break;
  };
}

/**-----------------------------------------------------------------------------
 \fn     static void ciu_alarm_flatness_status_get_object_def (u8_t ident_len, s32_t *ident, struct obj_def *od)
 \brief  функция получения свойств переменных узла Alarm (flatness)
 \param  ident_len - the address length. длина адреса
 \param  ident - points to objectname.0 (object id trailer) 
 \param  od - points to object definition. указатель на описание переменной
 \return нет
------------------------------------------------------------------------------*/
static void ciu_alarm_flatness_status_get_object_def (u8_t ident_len, s32_t *ident, struct obj_def *od)
{
  u8_t id;

  /* return to object name, adding index depth (1) */
  ident_len += 1;
  ident -= 1;
  if (ident_len == 2)
  {
    od->id_inst_len = ident_len;
    od->id_inst_ptr = ident;

    id = ident[0];
    LWIP_DEBUGF(SNMP_MIB_DEBUG,("get_object_def planar.%"U16_F".0\n",(u16_t)id));
    switch (id)
    {
      case 2: /* flatness severity type (string, NA) */
        od->instance = MIB_OBJECT_SCALAR;
        od->access = MIB_OBJECT_NOT_ACCESSIBLE;
        od->asn_type = (SNMP_ASN1_UNIV | SNMP_ASN1_PRIMIT | SNMP_ASN1_OC_STR);
//        if( szBandErrorType != NULL )
//		    od->v_len = strlen( szBandErrorType );
//		  else
//			 od->v_len = 0;
        break;
      case 3: /* flatness severity value (string, NA) */
        od->instance = MIB_OBJECT_SCALAR;
        od->access = MIB_OBJECT_NOT_ACCESSIBLE;
        od->asn_type = (SNMP_ASN1_UNIV | SNMP_ASN1_PRIMIT | SNMP_ASN1_OC_STR);
       // od->v_len = strnlen( szSnmp_BandErrorVal, sizeof(szSnmp_BandErrorVal) );
		 default:
        LWIP_DEBUGF(SNMP_MIB_DEBUG,("planar_get_object_def: no such object\n"));
        od->instance = MIB_OBJECT_NONE;
        break;
    };
  }
  else
  {
    LWIP_DEBUGF(SNMP_MIB_DEBUG,("planar_get_object_def: no scalar\n"));
    od->instance = MIB_OBJECT_NONE;
  }
}

/**-----------------------------------------------------------------------------
 \fn     static void ciu_alarm_flatness_status_get_value (struct obj_def *od, u16_t len, void *value)
 \brief  функция получения значения переменных узла Alarm (flatness)
 \param  obj_def - указатель на описание переменной
 \param  len - длина переменной в байтах 
 \param  value - указатель на значение переменной
 \return нет
------------------------------------------------------------------------------*/
static void ciu_alarm_flatness_status_get_value (struct obj_def *od, u16_t len, void *value)
{
  u8_t id;

  
  id = od->id_inst_ptr[0];
  switch (id)
  {
    case 2: /* flatness severity type */
    //  ocstrncpy(value, (u8_t*)szBandErrorType, len);
		break;
    case 3: /* flatness severity value */
    //  ocstrncpy(value, (u8_t*)szSnmp_BandErrorVal, len);
      break;
  };
}














/**
 * Returns systems object definitions (eeprom).
 *
 * @param ident_len the address length (2)
 * @param ident points to objectname.0 (object id trailer)
 * @param od points to object definition.
 */
static void
system_get_object_def_ee(u8_t ident_len, s32_t *ident, struct obj_def *od)
{
  u8_t id;

  /* return to object name, adding index depth (1) */
  ident_len += 1;
  ident -= 1;
  if (ident_len == 2)
  {
    od->id_inst_len = ident_len;
    od->id_inst_ptr = ident;

    id = ident[0];
    LWIP_DEBUGF(SNMP_MIB_DEBUG,("get_object_def_ee system.%"U16_F".0\n",(u16_t)id));
    switch (id)
    {
		case 4: /* sysContact */
        od->instance = MIB_OBJECT_SCALAR;
        od->access = MIB_OBJECT_READ_WRITE;
        od->asn_type = (SNMP_ASN1_UNIV | SNMP_ASN1_PRIMIT | SNMP_ASN1_OC_STR);
        od->v_len = GetSnmpStringLen(SNMP_SYSCONTACT_TYPE);
        break;
      case 5: /* sysName */
        od->instance = MIB_OBJECT_SCALAR;
        od->access = MIB_OBJECT_READ_WRITE;
        od->asn_type = (SNMP_ASN1_UNIV | SNMP_ASN1_PRIMIT | SNMP_ASN1_OC_STR);
        od->v_len = GetSnmpStringLen(SNMP_SYSNAME_TYPE);
        break;
      case 6: /* sysLocation */
        od->instance = MIB_OBJECT_SCALAR;
        od->access = MIB_OBJECT_READ_WRITE;
        od->asn_type = (SNMP_ASN1_UNIV | SNMP_ASN1_PRIMIT | SNMP_ASN1_OC_STR);
        od->v_len = GetSnmpStringLen(SNMP_SYSLOCATION_TYPE);
        break;
      default:
        LWIP_DEBUGF(SNMP_MIB_DEBUG,("system_get_object_def_ee: no such object\n"));
        od->instance = MIB_OBJECT_NONE;
        break;
    };
  }
  else
  {
    LWIP_DEBUGF(SNMP_MIB_DEBUG,("system_get_object_def_ee: no scalar\n"));
    od->instance = MIB_OBJECT_NONE;
  }
}
// ----------------------------------------------------------------------------


/**
 * Returns system object value.
 *
 * @param ident_len the address length (2)
 * @param ident points to objectname.0 (object id trailer)
 * @param len return value space (in bytes)
 * @param value points to (varbind) space to copy value into.
 */
static void
system_get_value_ee(struct obj_def *od, u16_t len, void *value)
{
  u8_t id;

  id = od->id_inst_ptr[0];
  switch (id)
  {
	 case 4: /* sysContact */
      GetSnmpString(SNMP_SYSCONTACT_TYPE, value, len );
      break;
    case 5: /* sysName */
      GetSnmpString(SNMP_SYSNAME_TYPE, value, len );
      break;
    case 6: /* sysLocation */
	   GetSnmpString(SNMP_SYSLOCATION_TYPE, value, len );
      break;
  };
}
// ----------------------------------------------------------------------------


static u8_t
system_set_test_ee(struct obj_def *od, u16_t len, void *value)
{
  u8_t id, set_ok;

  LWIP_UNUSED_ARG(value);
  set_ok = 0;
  id = od->id_inst_ptr[0];
  switch (id)
  {
    case 4: /* sysContact */
      if ( len <= 255 )
      {
        set_ok = 1;
      }
      break;
    case 5: /* sysName */
      if ( len <= 255 )
      {
        set_ok = 1;
      }
      break;
    case 6: /* sysLocation */
      if ( len <= 255 )
      {
        set_ok = 1;
      }
      break;
  };
  return set_ok;
}
// ----------------------------------------------------------------------------


static void
system_set_value_ee(struct obj_def *od, u16_t len, void *value)
{
  u8_t id;

  id = od->id_inst_ptr[0];
  switch (id)
  {
    case 4: /* sysContact */
      SetSnmpString(SNMP_SYSCONTACT_TYPE, value, len );
      *syscontact_len_ptr = len;
      break;
    case 5: /* sysName */
      SetSnmpString(SNMP_SYSNAME_TYPE, value, len );
      *sysname_len_ptr = len;
      break;
    case 6: /* sysLocation */
      SetSnmpString(SNMP_SYSLOCATION_TYPE, value, len );
      *syslocation_len_ptr = len;
      break;
  };
}
// ----------------------------------------------------------------------------


static void
snmp_set_value_ee(struct obj_def *od, u16_t len, void *value)
{
  u8_t id;

  LWIP_UNUSED_ARG(len);
  id = od->id_inst_ptr[0];
  if (id == 30)
  {
    /* snmpEnableAuthenTraps */
    s32_t *sint_ptr = value;
    *snmpenableauthentraps_ptr = *sint_ptr;
	 SetSnmpString(SNMP_SNMPENABLEAUTHTRAPS_TYPE, value, len );
  }
}


/**-----------------------------------------------------------------------------
 \fn     err_t snmp_send_trap_ex (s8_t generic_trap, 
                         struct snmp_obj_id *eoid, 
                         s32_t specific_trap, 
                         struct snmp_obj_id* varbinds, 
                         u8_t varbinds_num)
 \brief  функция посылки трапа с возможностью пристыковывать переменные
 \param  generic_trap  - код трапа
 \param  eoid          - указатель на enterprise object identifier
 \param  specific_trap - used for enterprise traps when generic_trap == 6
 \param  varbinds      - указатель на переменные
 \param  varbinds_num  - число переменных
 \return код ошибки
------------------------------------------------------------------------------*/
err_t snmp_send_trap_ex (s8_t generic_trap, 
                         struct snmp_obj_id *eoid, 
                         s32_t specific_trap, 
                         struct snmp_obj_id* varbinds, 
                         u8_t varbinds_num)
{
	extern uint8_t bSnmpActive;
	
	struct snmp_varbind** vb_list;
	struct mib_node* node;
	struct obj_def obj;
	err_t tResult;
	s32_t* VarNum;
    struct snmp_name_ptr np;
	

	/* SNMP не готово - ничего не отправляем */
	if( !bSnmpActive )
	{
		return ERR_MEM;
	}
	
	/* создаем таблицу указателей переменных */
	vb_list = pvPortMalloc( varbinds_num * sizeof(struct snmp_varbind*) );
	if(vb_list == NULL)
	{
		return ERR_MEM;
	}
	
	/* создаем сами переменные */
	for(u8_t i = 0; i < varbinds_num; i++)
	{
		/* ищем узел в дереве*/
		node = snmp_search_tree( (struct mib_node*)&internet, varbinds[i].len - 4, 
										 &varbinds[i].id[4], &np );
		
		if(node != NULL)
		{
			/* получаем информацию о переменной узла */
			VarNum = &varbinds[i].id[ varbinds[i].len - 1 ];
			
			if( node->node_type == MIB_NODE_EX)
			{
				struct mib_external_node* node_ex;
				node_ex = (struct mib_external_node*)node;
				/* TODO: подставить правильный rid вместо 0 */
				node_ex->get_object_def_a(0, 1, VarNum, &obj);
			}
			else
			{
				node->get_object_def(1, VarNum, &obj);
			}
			
			/* создаем переменную */
			vb_list[i] = snmp_varbind_alloc(&varbinds[i], obj.asn_type, obj.v_len);
			if( vb_list[i] != NULL )
			{
				{
					/* получаем значение переменной */
					if( node->node_type == MIB_NODE_EX)
					{
						struct mib_external_node* node_ex;
						node_ex = (struct mib_external_node*)node;
						/* TODO: подставить правильный rid вместо 0 */
						node_ex->get_value_a(0, &obj, obj.v_len, vb_list[i]->value);
					}
					else
					{
						node->get_value(&obj, obj.v_len, vb_list[i]->value);
					}
					/* добавляем переменную в ловушку */
					snmp_varbind_tail_add(&trap_msg.outvb, vb_list[i]);
				}
			}
		}
	}

	/* посылаем ловушку */
	tResult = snmp_send_trap(generic_trap, eoid, specific_trap);
	
	/* удаляем переменные из списка */
	snmp_varbind_list_free(&trap_msg.outvb);
	
	/* удаляем указатели переменных */
	vPortFree(vb_list);
	
	return tResult;
}

/**-----------------------------------------------------------------------------
 \fn     void ocstrncpy (u8_t *dst, u8_t *src, u8_t n)
 \brief  функция копирования строки
 \param  dst - указатель куда копировать
 \param  src - указатель откуда копировать 
 \param  n - количество байт для копирования
 \return нет
------------------------------------------------------------------------------*/
void ocstrncpy (u8_t *dst, u8_t *src, u8_t n)
{
  while (n > 0)
  {
    n--;
    *dst++ = *src++;
  }
}

// DUMMY FUNCTIONS ---------------------------------------------------------

void dummy_get_object_def_q (void* addr_inf, u8_t rid, u8_t ident_len, s32_t *ident)
{
  LWIP_UNUSED_ARG(addr_inf);
  LWIP_UNUSED_ARG(ident_len);
  LWIP_UNUSED_ARG(ident);
  /* fake async quesion/answer */
  snmp_msg_event(rid);
}

void dummy_get_value_q (u8_t rid, struct obj_def *od)
{
  LWIP_UNUSED_ARG(od);
  /* fake async quesion/answer */
  snmp_msg_event(rid);
}

void dummy_set_test_q (u8_t rid, struct obj_def *od)
{
  LWIP_UNUSED_ARG(od);
  /* fake async quesion/answer */
  snmp_msg_event(rid);
}

void dummy_set_value_q (u8_t rid, struct obj_def *od, u16_t len, void *value)
{
  LWIP_UNUSED_ARG(rid);
  LWIP_UNUSED_ARG(od);
  LWIP_UNUSED_ARG(len);
  LWIP_UNUSED_ARG(value);
  /* fake async quesion/answer */
  snmp_msg_event(rid);
}

void dummy_get_object_def_pc (u8_t rid, u8_t ident_len, s32_t *ident)
{
  LWIP_UNUSED_ARG(rid);
  LWIP_UNUSED_ARG(ident_len);
  LWIP_UNUSED_ARG(ident);
}

void dummy_get_value_pc (u8_t rid, struct obj_def *od)
{
  LWIP_UNUSED_ARG(rid);
  LWIP_UNUSED_ARG(od);
}

void dummy_set_test_pc (u8_t rid, struct obj_def *od)
{
  LWIP_UNUSED_ARG(rid);
  LWIP_UNUSED_ARG(od);
}

void dummy_set_value_pc (u8_t rid, struct obj_def *od)
{
  LWIP_UNUSED_ARG(rid);
  LWIP_UNUSED_ARG(od);
}

u8_t noleaf_set_test_a (u8_t rid, struct obj_def *od, u16_t len, void *value)
{
  LWIP_UNUSED_ARG(rid);
  LWIP_UNUSED_ARG(od);
  LWIP_UNUSED_ARG(len);
  LWIP_UNUSED_ARG(value);
  /* sensors are read-only */
  return 0; /* 0 -> read only, != 0 -> read/write */
}

void noleaf_set_value_a (u8_t rid, struct obj_def *od, u16_t len, void *value)
{
  LWIP_UNUSED_ARG(rid);
  LWIP_UNUSED_ARG(od);
  LWIP_UNUSED_ARG(len);
  LWIP_UNUSED_ARG(value);
}

#endif /* LWIP_SNMP */


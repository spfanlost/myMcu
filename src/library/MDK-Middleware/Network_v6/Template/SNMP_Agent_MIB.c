/*------------------------------------------------------------------------------
 * MDK Middleware - Component ::Network:Service
 * Copyright (c) 2004-2019 Arm Limited (or its affiliates). All rights reserved.
 *------------------------------------------------------------------------------
 * Name:    SNMP_Agent_MIB.c
 * Purpose: SNMP Agent Management Information Base Module
 * Rev.:    V6.00
 *----------------------------------------------------------------------------*/
//! [code_SNMP_Agent_MIB]
#include "rl_net.h"
 
// System
extern uint32_t snmp_SysUpTime;
 
// MIB read only integer constants
static const uint8_t sysServices = 79;
 
/* Example
// snmp_agent.c
extern bool LCDupdate;
extern char lcd_text[2][16+1];
extern uint8_t get_button (void);
extern void    LED_Out (uint8_t leds);
 
// Local variables
static uint8_t  LedOut;
static uint8_t  KeyIn;
 
// MIB event callback functions
static void write_leds (int mode) {
  if (mode == MIB_WRITE) {
    LED_Out (LedOut);
  }
}
 
static void read_key (int mode) {
  if (mode == MIB_READ) {
    KeyIn = get_button();
  }
}
*/
 
// MIB data table
const SNMP_MIB snmp_mib[] = {
// ---------- System MIB ----------- 
  // SysDescr Entry
  { MIB_OCTET_STR | MIB_ATR_RO,
    8, {MIB_OID0(1,3), 6, 1, 2, 1, 1, 1, 0},
    MIB_STR("Embedded System SNMP V1.0"),
    NULL },
  // SysObjectID Entry
  { MIB_OBJECT_ID | MIB_ATR_RO,
    8, {MIB_OID0(1,3), 6, 1, 2, 1, 1, 2, 0},
    MIB_STR("\x2b\x06\x01\x02\x01\x01\x02\x00"),
    NULL },
  // SysUpTime Entry
  { MIB_TIME_TICKS | MIB_ATR_RO,
    8, {MIB_OID0(1,3), 6, 1, 2, 1, 1, 3, 0},
    4, &snmp_SysUpTime,
    NULL },
  // SysContact Entry
  { MIB_OCTET_STR | MIB_ATR_RO,
    8, {MIB_OID0(1,3), 6, 1, 2, 1, 1, 4, 0},
    MIB_STR("test@keil.com"),
    NULL },
  // SysName Entry
  { MIB_OCTET_STR | MIB_ATR_RO,
    8, {MIB_OID0(1,3), 6, 1, 2, 1, 1, 5, 0},
    MIB_STR("Evaluation board"),
    NULL },
  // SysLocation Entry
  { MIB_OCTET_STR | MIB_ATR_RO,
    8, {MIB_OID0(1,3), 6, 1, 2, 1, 1, 6, 0},
    MIB_STR("Local"),
    NULL },
  // SysServices Entry
  { MIB_INTEGER | MIB_ATR_RO,
    8, {MIB_OID0(1,3), 6, 1, 2, 1, 1, 7, 0},
    MIB_INT(sysServices),
    NULL },
// ---------- Experimental MIB -----------
  /* Example
  // AddLedOut Entry
  { MIB_INTEGER,
    6, {MIB_OID0(1,3), 6, 1, 3, 1, 0},
    MIB_INT(LedOut),
    write_leds },
  // KeyIn Entry
  { MIB_INTEGER | MIB_ATR_RO,
    6, {MIB_OID0(1,3), 6, 1, 3, 2, 0},
    MIB_INT(KeyIn),
    read_key },
     ...
  */
};
 
const int snmp_mib_size = SNMP_MIB_SIZE(snmp_mib);
//! [code_SNMP_Agent_MIB]

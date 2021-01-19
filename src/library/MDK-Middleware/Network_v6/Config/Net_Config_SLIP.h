/*------------------------------------------------------------------------------
 * MDK Middleware - Component ::Network:Interface
 * Copyright (c) 2004-2019 Arm Limited (or its affiliates). All rights reserved.
 *------------------------------------------------------------------------------
 * Name:    Net_Config_SLIP.h
 * Purpose: Network Configuration for SLIP Interface
 * Rev.:    V5.1.0
 *----------------------------------------------------------------------------*/

//-------- <<< Use Configuration Wizard in Context Menu >>> --------------------

// <h>SLIP Network Interface 
#define SLIP_ENABLE             1

//   <o>Connect to hardware via Driver_USART# <0-255>
//   <i>Select driver control block for hardware interface
#define SLIP_USART_DRIVER       0

//   <h>Modem Settings
//   <i>Serial Modem connection parameters
//     <o>Speed <300=>300
//              <600=>600
//              <1200=>1200
//              <2400=>2400
//              <4800=>4800
//              <9600=>9600
//              <14400=>14400
//              <19200=>19200
//              <28800=>28800
//              <38400=>38400
//              <57600=>57600
//              <115200=>115200
//              <128000=>128000
//              <230400=>230400
//              <256000=>256000
//              <460800=>460800
//              <921600=>921600
//     <i>Baud rate setting for USART driver
#define SLIP_MODEM_SPEED        115200

//     <o>Flow Control <0=>None
//                     <1=>RTS/CTS
//     <i>Flow control setting for USART driver
#define SLIP_MODEM_FLOW_CONTROL 0

//     <s.50>Initialization String
//     <i>Custom commands for modem initialization
//     <i>Default: none
#define SLIP_MODEM_INIT_STRING  ""
//   </h>

//   <h>IP Address
//   <i>Local Static IP Address
//     <o>Address byte 1 <0-255>
//     <i>Default: 192
#define SLIP_IP1                192

//     <o>Address byte 2 <0-255>
//     <i>Default: 168
#define SLIP_IP2                168

//     <o>Address byte 3 <0-255>
//     <i>Default: 225
#define SLIP_IP3                225

//     <o>Address byte 4 <0-255>
//     <i>Default: 1
#define SLIP_IP4                1
//   </h>

//   <h>Primary DNS Server
//   <i>Primary DNS Server IP Address
//     <o>Address byte 1 <0-255>
//     <i>Default: 194
#define SLIP_PRI_DNS1           194

//     <o>Address byte 2 <0-255>
//     <i>Default: 25
#define SLIP_PRI_DNS2           25

//     <o>Address byte 3 <0-255>
//     <i>Default: 2
#define SLIP_PRI_DNS3           2

//     <o>Address byte 4 <0-255>
//     <i>Default: 129
#define SLIP_PRI_DNS4           129
//   </h>

//   <h>Secondary DNS Server
//   <i>Secondary DNS Server IP Address
//     <o>Address byte 1 <0-255>
//     <i>Default: 194
#define SLIP_SEC_DNS1           194

//     <o>Address byte 2 <0-255>
//     <i>Default: 25
#define SLIP_SEC_DNS2           25

//     <o>Address byte 3 <0-255>
//     <i>Default: 2
#define SLIP_SEC_DNS3           2

//     <o>Address byte 4 <0-255>
//     <i>Default: 130
#define SLIP_SEC_DNS4           130
//   </h>

//   <q>Use Default Gateway on remote Network
//   <i>This option only applies when both Ethernet and SLIP Dial-up
//   <i>are used. If checked, data that cannot be sent to local LAN
//   <i>is forwarded to Dial-up network instead.
#define SLIP_DEFAULT_GW         1

// </h>

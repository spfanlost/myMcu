/*------------------------------------------------------------------------------
 * MDK Middleware - Component ::Network:Interface
 * Copyright (c) 2004-2019 Arm Limited (or its affiliates). All rights reserved.
 *------------------------------------------------------------------------------
 * Name:    Net_Config_PPP.h
 * Purpose: Network Configuration for PPP Interface
 * Rev.:    V5.1.0
 *----------------------------------------------------------------------------*/

//-------- <<< Use Configuration Wizard in Context Menu >>> --------------------

// <h>PPP Network Interface 
#define PPP_ENABLE              1

//   <o>Connect to hardware via Driver_USART# <0-255>
//   <i>Select driver control block for hardware interface
#define PPP_USART_DRIVER        0

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
#define PPP_MODEM_SPEED         115200

//     <o>Flow Control <0=>None
//                     <1=>RTS/CTS
//     <i>Flow control setting for USART driver
#define PPP_MODEM_FLOW_CONTROL  0

//     <s.50>Initialization String
//     <i>Custom commands for modem initialization
//     <i>Default: none
#define PPP_MODEM_INIT_STRING   ""
//   </h>

//   <h>IP Address
//   <i>Local Static IP Address
//     <o>Address byte 1 <0-255>
//     <i>Default: 192
#define PPP_IP1                 192

//     <o>Address byte 2 <0-255>
//     <i>Default: 168
#define PPP_IP2                 168

//     <o>Address byte 3 <0-255>
//     <i>Default: 125
#define PPP_IP3                 125

//     <o>Address byte 4 <0-255>
//     <i>Default: 1
#define PPP_IP4                 1
//   </h>

//   <h>Primary DNS Server
//   <i>Primary DNS Server IP Address
//     <o>Address byte 1 <0-255>
//     <i>Default: 194
#define PPP_PRI_DNS1            194

//     <o>Address byte 2 <0-255>
//     <i>Default: 25
#define PPP_PRI_DNS2            25

//     <o>Address byte 3 <0-255>
//     <i>Default: 2
#define PPP_PRI_DNS3            2

//     <o>Address byte 4 <0-255>
//     <i>Default: 129
#define PPP_PRI_DNS4            129
//   </h>

//   <h>Secondary DNS Server
//   <i>Secondary DNS Server IP Address
//     <o>Address byte 1 <0-255>
//     <i>Default: 194
#define PPP_SEC_DNS1            194

//     <o>Address byte 2 <0-255>
//     <i>Default: 25
#define PPP_SEC_DNS2            25

//     <o>Address byte 3 <0-255>
//     <i>Default: 2
#define PPP_SEC_DNS3            2

//     <o>Address byte 4 <0-255>
//     <i>Default: 130
#define PPP_SEC_DNS4            130
//   </h>

//   <e>Logon Authentication
//   <i>Enable or disable user authentication
#define PPP_AUTH_ENABLE         1

//     <q>Unsecured password (PAP)
//     <i>Allow or use Password Authentication Protocol.
#define PPP_PAP_ENABLE          1

//     <q>Secured password (CHAP-MD5)
//     <i>Request or use Challenge Handshake Authentication
//     <i>Protocol with MD5 digest algorithm.
#define PPP_CHAP_ENABLE         1
//   </e>

//   <q>Obtain Client IP address automatically
//   <i>This option only applies when PPP Dial-up is used to dial
//   <i>to remote PPP Server. If checked, network connection
//   <i>dynamically obtains an IP address from remote PPP Server.
#define PPP_GET_IP              1

//   <q>Use Default Gateway on remote Network
//   <i>This option only applies when both Ethernet and PPP Dial-up
//   <i>are used. If checked, data that cannot be sent to local LAN
//   <i>is forwarded to Dial-up network instead.
#define PPP_DEFAULT_GW          1

//   <o>Async Control Character Map <0x0-0xffffffff>
//   <i>A bit-map of control characters 0-31, which are
//   <i>transmitted escaped as a 2 byte sequence.
//   <i>For XON/XOFF set this value to: 0x000A 0000
//   <i>Default: 0x00000000
#define PPP_ACCM                0x00000000

//   <o>LCP Echo Interval in seconds <0-3600>
//   <i>If no frames are received within this interval, PPP sends an
//   <i>Echo Request and expects an Echo Response from the peer.
//   <i>If the response is not received, the link is terminated.
//   <i>A value of 0 disables the LCP Echo test.
//   <i>Default: 30
#define PPP_ECHO_TOUT           30

//   <o>Number of Retries <0-20>
//   <i>How many times PPP will try to retransmit data
//   <i>before giving up. Increase this value for links
//   <i>with low baud rates or high latency.
//   <i>Default: 3
#define PPP_MAX_RETRY           3

//   <o>Retry Timeout in seconds <1-10>
//   <i>If no response received within this time frame,
//   <i>PPP module will try to resend the data again.
//   <i>Default: 2
#define PPP_RETRY_TOUT          2

// </h>

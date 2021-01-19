/*------------------------------------------------------------------------------
 * MDK Middleware - Component ::Network:Service
 * Copyright (c) 2004-2019 Arm Limited (or its affiliates). All rights reserved.
 *------------------------------------------------------------------------------
 * Name:    Net_Config_TFTP_Server.h
 * Purpose: Network Configuration for TFTP Server
 * Rev.:    V5.0.0
 *----------------------------------------------------------------------------*/

//-------- <<< Use Configuration Wizard in Context Menu >>> --------------------

// <h>TFTP Server
#define TFTP_SERVER_ENABLE          1

//   <o>Number of TFTP Sessions <1-10>
//   <i>Number of simultaneously active TFTP Sessions
//   <i>Default: 1
#define TFTP_SERVER_NUM_SESSIONS    1

//   <o>Port Number <1-65535>
//   <i>Listening port number.
//   <i>Default: 69
#define TFTP_SERVER_PORT_NUM        69

//   <q>Enable Firewall Support
//   <i>Use the same Port Number to receive
//   <i>requests and send answers to clients.
//   <i>Default: Not Enabled
#define TFTP_SERVER_FIREWALL_ENABLE 0

//   <o>Inactive Session Timeout in seconds <5-120>
//   <i>When timeout expires TFTP Session is closed.
//   <i>Default: 15
#define TFTP_SERVER_TOUT            15

//   <o>Number of Retries <1-10>
//   <i>How many times TFTP Server will try to
//   <i>retransmit the data before giving up.
//   <i>Default: 4
#define TFTP_SERVER_MAX_RETRY       4

// </h>

/*------------------------------------------------------------------------------
 * MDK Middleware - Component ::Network:Service
 * Copyright (c) 2004-2019 Arm Limited (or its affiliates). All rights reserved.
 *------------------------------------------------------------------------------
 * Name:    Telnet_Server_Access.c
 * Purpose: Telnet Server Access Interface
 * Rev.:    V6.00
 *----------------------------------------------------------------------------*/
//! [code_Telnet_Server_Access]
#include "rl_net.h"
 
// Accept or deny connection from remote Telnet client.
// If this function is missing, all remote clients are accepted.
bool telnet_accept_client (const uint8_t *ip_addr, uint16_t port) {
  /* Example
  if (ip_addr[0] == 192  &&
      ip_addr[1] == 168  &&
      ip_addr[2] == 0    &&
      ip_addr[3] == 1) {
    // Accept connection.
    return (true);
  }
  // Deny connection.
  return (false);
  */
  return (true);
}
//! [code_Telnet_Server_Access]

/*------------------------------------------------------------------------------
 * MDK Middleware - Component ::Network:Service
 * Copyright (c) 2004-2019 Arm Limited (or its affiliates). All rights reserved.
 *------------------------------------------------------------------------------
 * Name:    Telnet_Server_UIF.c
 * Purpose: Telnet Server User Interface
 * Rev.:    V6.00
 *----------------------------------------------------------------------------*/
//! [code_Telnet_Server_UIF]
#include <stdio.h>
#include "rl_net.h"
 
// Request message for Telnet server session.
uint32_t telnet_server_message (telnetServerMessage msg, char *buf, uint32_t len) {
  uint32_t rlen = 0;
 
  switch (msg) {
    case telnetServerWelcome:
      // Initial welcome message
      /* Example
      rlen = sprintf (buf, "\r\n"
                           "\r\n"
                           "Telnet Server ready\r\n");
      */
      break;
    case telnetServerPrompt:
      // Prompt message
      /* Example
      rlen = sprintf (buf, "\r\n"
                           "Cmd> ");
      */
      break;
    case telnetServerLogin:
      // Login welcome message, if authentication is enabled
      /* Example
      rlen = sprintf (buf, "\r\n"
                           "Embedded Telnet Server\r\n"
                           "\r\n"
                           "Please login...");
      */
      break;
    case telnetServerUsername:
      // Username request login message
      /* Example
      rlen = sprintf (buf, "\r\n"
                           "Username: ");
      */
      break;
    case telnetServerPassword:
      // Password request login message
      /* Example
      rlen = sprintf (buf, "\r\n"
                           "Password: ");
      */
      break;
    case telnetServerLoginFailed:
      // Incorrect login error message
      /* Example
      rlen = sprintf (buf, "\r\n"
                           "Login incorrect");
      */
      break;
    case telnetServerLoginTimeout:
      // Login timeout error message
      /* Example
      rlen = sprintf (buf, "\r\n"
                           "Login timeout\r\n");
      */
      break;
    case telnetServerUnsolicitedMessage:
      // Unsolicited message (ie. from basic interpreter)
      /* Example
      rlen = sprintf (buf, "\r\n"
                           "Unsolicited message\r\n");
      */
      break;
  }
  return (rlen);
}
 
// Poll the upper-layer user application for unsolicited messages.
bool telnet_server_message_poll (int32_t session) {
  /* Example
  extern bool msg_available;
 
  if (session == 1 && msg_available == true) {
    // Allow unsolicited messages on 1st session
    msg_available = false;
    return (true);
  }
  */
  return (false);
}
 
// Process a command and generate response.
uint32_t telnet_server_process (const char *cmd, char *buf, uint32_t buflen, uint32_t *pvar) {
  uint32_t len = 0;
 
  /* Example
  extern int32_t AD_in (int32_t channel);
 
  if (telnet_check_command (cmd, "ADIN0") == true) {
    // Read analog input 0
    len = sprintf (buf, "\r\n"
                        " ADIN0 = %d", AD_in(0));
    return (len);
  }
  // ...
  if (telnet_check_command (cmd, "BYE") == true) {
    // Generate reply and disconnect
    len = sprintf (buf, "\r\n"
                        "Disconnecting\r\n");
    return (len | (1u<<30));
  }
  */
  return (len);
}
//! [code_Telnet_Server_UIF]

/*------------------------------------------------------------------------------
 * MDK Middleware - Component ::Network:Service
 * Copyright (c) 2004-2019 Arm Limited (or its affiliates). All rights reserved.
 *------------------------------------------------------------------------------
 * Name:    Telnet_Server_Multiuser.c
 * Purpose: Telnet Server Multi-user Interface
 * Rev.:    V6.00
 *----------------------------------------------------------------------------*/
//! [code_Telnet_Server_Multiuser]
#include <string.h>
#include "rl_net.h"
 
// Check if an user account exist in the user database.
uint8_t telnet_check_username (const char *username) {
  /* Example
  if (strcmp (username, "guest") == 0) {
    // Username is correct
    return (1);
  }
  */
  return (0);
}
 
// Check user account password in the user database.
bool telnet_check_password (uint8_t user_id, const char *password) {
  /* Example
  if (user_id == 1) {
    if (strcmp (password, "guest") == 0) {
      // Password is correct
      return (true);
    }
  }
  */
  return (false);
}
//! [code_Telnet_Server_Multiuser]

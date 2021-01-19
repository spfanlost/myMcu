/*------------------------------------------------------------------------------
 * MDK Middleware - Component ::Network:Service
 * Copyright (c) 2004-2019 Arm Limited (or its affiliates). All rights reserved.
 *------------------------------------------------------------------------------
 * Name:    HTTP_Server_Multiuser.c
 * Purpose: HTTP Server Multi-user Interface
 * Rev.:    V6.00
 *----------------------------------------------------------------------------*/
//! [code_HTTP_Server_Multiuser]
#include <string.h>
#include "rl_net.h"
 
// Check if an user account exist in the user database.
uint8_t http_check_account (const char *username, const char *password) {
  /* Example
  if ((strcmp (username, "guest") == 0) && (strcmp (password, "guest") == 0)) {
    // Accept guest account
    return (1);
  }
  */
  return (0);
}
 
// Check if remote user is allowed to access a file on HTTP server.
bool http_file_access (uint8_t user_id, const char *fname) {
  /* Example
  if (user_id == 1) {
    if (strcmp (fname, "system.cgi") == 0) {
      // User "guest" is not allowed to see "system.cgi"
      return (false);
    }
  }
  */
  return (true);
}
//! [code_HTTP_Server_Multiuser]

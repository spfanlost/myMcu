/*------------------------------------------------------------------------------
 * MDK Middleware - Component ::Network:Service
 * Copyright (c) 2004-2019 Arm Limited (or its affiliates). All rights reserved.
 *------------------------------------------------------------------------------
 * Name:    FTP_Server_Multiuser.c
 * Purpose: FTP Server Multi-user Interface
 * Rev.:    V6.00
 *----------------------------------------------------------------------------*/
//! [code_FTP_Server_Multiuser]
#include <string.h>
#include "rl_net.h"
 
// Check if an user account exist in the user database.
uint8_t ftp_check_username (const char *username) {
  /* Example
  if (strcmp (username, "guest") == 0) {
    // Username is correct
    return (1);
  }
  */
  return (0);
}
 
// Check user account password in the user database.
bool ftp_check_password (uint8_t user_id, const char *password) {
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
 
// Check if remote user is allowed to access a file on FTP server.
bool ftp_file_access (uint8_t user_id, const char *fname, uint8_t mode) {
  /* Example
  if (user_id == 1) {
    if ((strcmp (fname, "test.txt") == 0) && (mode == 1)) {
      // User "guest" is not allowed to modify or delete "test.txt"
      return (false);
    }
  }
  */
  return (true);
}
//! [code_FTP_Server_Multiuser]

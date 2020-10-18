/*------------------------------------------------------------------------------
 * MDK Middleware - Component ::Network:Service
 * Copyright (c) 2004-2019 Arm Limited (or its affiliates). All rights reserved.
 *------------------------------------------------------------------------------
 * Name:    FTP_Client_UIF.c
 * Purpose: FTP Client User Interface
 * Rev.:    V6.00
 *----------------------------------------------------------------------------*/
//! [code_FTP_Client_UIF]
#include <stdio.h>
#include "rl_net.h"
 
// Request parameters for FTP client session.
uint32_t ftp_client_request (ftpClientRequest request, char *buf, uint32_t len) {
  uint32_t rlen = 0;
//int32_t i;
 
  switch (request) {
    case ftpClientUsername:
      // Username to login to FTP server
      /* Example
      rlen = sprintf (buf, "anonymous");
      */
      break;

    case ftpClientPassword:
      // Password to login to FTP server
      /* Example
      rlen = sprintf (buf, "test@keil.com");
      */
      break;

    case ftpClientPath:
      // Working directory path on server
      /* Example
      rlen = sprintf (buf, "/Logs");
      */
      break;

    case ftpClientFilename:
      // Filename for PUT, GET, APPEND, DELETE or RENAME command
      /* Example
      rlen = sprintf (buf, "test.log");
      */
      break;

    case ftpClientNewName:
      // New name for a RENAME command
      /* Example
      rlen = sprintf (buf, "renamed.log");
      */
      break;

    case ftpClientDirectory:
      // Directory name for MKDIR or RMDIR command
      /* Example
      rlen = sprintf (buf, "New_Folder");
      */
      break;

    case ftpClientFilterMask:
      // File filter/mask for LIST command (wildcards allowed)
      /* Example
      rlen = sprintf (buf, "");
      */
      break;

    case ftpClientList:
      // Received data if LIST command is given
      /* Example
      for (i = 0; i < len; i++) {
        putchar (buf[i]);
      }
      */
      break;

    case ftpClientLocalFile:
      // Local filename
      /* Example
      rlen = sprintf (buf, "test_log.txt");
      */
      break;
  }
  return (rlen);
}
 
// Notify the user application when FTP client operation ends.
void ftp_client_notify (ftpClientEvent event) {
  switch (event) {
    case ftpClientSuccess:
      // File operation successful
      break;
    case ftpClientTimeout:
      // Timeout on file operation
      break;
    case ftpClientLoginFailed:
      // Login error, username/password invalid
      break;
    case ftpClientAccessDenied:
      // File access not allowed
      break;
    case ftpClientFileNotFound:
      // File not found
      break;
    case ftpClientInvalidPath:
      // Working directory path not found
      break;
    case ftpClientLocalFileError:
      // Local file open/write error
      break;
    case ftpClientError:
      // Generic FTP client error
      break;
  }
}
//! [code_FTP_Client_UIF]

/*------------------------------------------------------------------------------
 * MDK Middleware - Component ::Network:Service
 * Copyright (c) 2004-2019 Arm Limited (or its affiliates). All rights reserved.
 *------------------------------------------------------------------------------
 * Name:    FTP_Server_Event.c
 * Purpose: FTP Server Notification events
 * Rev.:    V6.00
 *----------------------------------------------------------------------------*/
//! [code_FTP_Server_Event]
#include "rl_net.h"
 
// Notify the user application about events in FTP server service.
void ftp_server_notify (ftpServerEvent event) {

  switch (event) {
    case ftpServerLogin:
      // User logged in, session is busy
      break;

    case ftpServerLogout:
      // User logged out, session is idle
      break;

    case ftpServerLoginFailed:
      // User login failed (invalid credentials)
      break;

    case ftpServerDownload:
      // File download ended
      break;

    case ftpServerUpload:
      // File upload ended
      break;

    case ftpServerDelete:
      // File deleted
      break;

    case ftpServerRename:
      // File or directory renamed
      break;

    case ftpServerMakeDirectory:
      // Directory created
      break;

    case ftpServerRemoveDirectory:
      // Directory removed
      break;

    case ftpServerOperationDenied:
      // Requested file operation denied
      break;

    case ftpServerLocalFileError:
      // Local file operation error
      break;

    case ftpServerFileError:
      // Generic file operation error

    case ftpServerError:
      // Generic FTP server error
      break;
  }
}
//! [code_FTP_Server_Event]

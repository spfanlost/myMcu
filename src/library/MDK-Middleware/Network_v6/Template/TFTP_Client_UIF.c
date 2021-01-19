/*------------------------------------------------------------------------------
 * MDK Middleware - Component ::Network:Service
 * Copyright (c) 2004-2019 Arm Limited (or its affiliates). All rights reserved.
 *------------------------------------------------------------------------------
 * Name:    TFTP_Client_UIF.c
 * Purpose: TFTP Client User Interface
 * Rev.:    V6.00
 *----------------------------------------------------------------------------*/
//! [code_TFTP_Client_UIF]
#include <stdio.h>
#include "rl_net.h"
 
// Notify the user application when TFTP client operation ends.
void tftp_client_notify (tftpClientEvent event) {
  switch (event) {
    case tftpClientSuccess:
      // File operation successful
      break;
    case tftpClientTimeout:
      // Timeout on file operation
      break;
    case tftpClientAccessDenied:
      // File access not allowed
      break;
    case tftpClientFileNotFound:
      // File not found
      break;
    case tftpClientDiskFull:
      // Disk full (local or remote)
      break;
    case tftpClientError:
      // Generic TFTP client error
      break;
  }
}
//! [code_TFTP_Client_UIF]

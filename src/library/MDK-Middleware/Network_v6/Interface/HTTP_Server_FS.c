/*------------------------------------------------------------------------------
 * MDK Middleware - Component ::Network:Service
 * Copyright (c) 2004-2019 Arm Limited (or its affiliates). All rights reserved.
 *------------------------------------------------------------------------------
 * Name:    HTTP_Server_FS.c
 * Purpose: HTTP Server File System Interface
 * Rev.:    V6.3.1
 *----------------------------------------------------------------------------*/
//! [code_HTTP_Server_FS]
#include <stdio.h>
#include "rl_net_lib.h"
#include "rl_fs.h"

#ifndef __CC_ARM
  #pragma clang diagnostic ignored "-Wcast-align"
#endif

// Open a file for reading in HTTP server.
__weak void *http_server_fopen (const char *fname) {
  return (fopen (fname, "r"));
}

// Close a file previously open in HTTP server.
__weak void http_server_fclose (void *file) {
  fclose (file);
}

// Read block of data from a file in HTTP server.
__weak uint32_t http_server_fread (void *file, uint8_t *buf, uint32_t len) {
  return (fread (buf, 1, len, file));
}

// Read a string from a file in HTTP server.
__weak char *http_server_fgets (void *file, char *buf, uint32_t size) {
  return (fgets (buf, (int32_t)size, file));
}

// Retrieve last modification time of a file.
__weak uint32_t http_server_ftime (const char *fname) {
  fsFileInfo *info;
  uint32_t utc;

  info = (fsFileInfo *)mem_alloc (sizeof (fsFileInfo));
  info->fileID = 0;
  utc = 0;
  if (ffind (fname, info) == fsOK) {
    // File found, convert creation time to UTC format.
    utc = http_utc_time (info->time.hr, info->time.min, info->time.sec,
                         info->time.day, info->time.mon, info->time.year);
  }
  mem_free ((NET_FRAME *)info);
  return (utc);
}
//! [code_HTTP_Server_FS]

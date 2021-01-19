/*------------------------------------------------------------------------------
 * MDK Middleware - Component ::Network:Service
 * Copyright (c) 2004-2019 Arm Limited (or its affiliates). All rights reserved.
 *------------------------------------------------------------------------------
 * Name:    FTP_Server_FS.c
 * Purpose: FTP Server File System Interface
 * Rev.:    V6.5.2
 *----------------------------------------------------------------------------*/
//! [code_FTP_Server_FS]
#include <stdio.h>
#include "rl_net.h"
#include "rl_fs.h"

#ifdef MW_FILESYSTEM_FDELETE_LEGACY
  #define FDELETE(fn)   fdelete(fn)
#else
  #define FDELETE(fn)   fdelete(fn,NULL)
#endif

#ifndef NET_FTP_SERVER_VBUF_SIZE
#define NET_FTP_SERVER_VBUF_SIZE 0
#endif
 
// Open a file for reading or writing in FTP server.
__weak void *ftp_server_fopen (const char *fname, const char *mode) {
#if (NET_FTP_SERVER_VBUF_SIZE > 512)
  FILE *f;

  f = fopen (fname, mode);

  if (f != NULL) {
    setvbuf (f, NULL, _IOFBF, NET_FTP_SERVER_VBUF_SIZE);
  }

  return (f);
#else
  return (fopen (fname, mode));
#endif
}

// Close a file previously open in FTP server.
__weak void ftp_server_fclose (void *file) {
  fclose (file);
}

// Read block of data from a file in FTP server.
__weak uint32_t ftp_server_fread (void *file, uint8_t *buf, uint32_t len) {
  return (fread (buf, 1, len, file));
}

// Write block of data to a file in FTP server.
__weak uint32_t ftp_server_fwrite (void *file, const uint8_t *buf, uint32_t len) {
  return (fwrite (buf, 1, len, file));
}

// Delete a file in FTP server.
__weak bool ftp_server_fdelete (const char *fname) {
  if (FDELETE (fname) == fsOK) {
    return (true);
  }
  return (false);
}

// Rename a file or directory in FTP server.
__weak bool ftp_server_frename (const char *fname, const char *newname) {
  if (frename (fname, newname) == fsOK) {
    return (true);
  }
  return (false);
}

// Make a new directory in FTP server.
__weak bool ftp_server_mkdir (const char *path) {
  if (fmkdir (path) == fsOK) {
    return (true);
  }
  return (false);
}

// Remove an empty directory in FTP server.
__weak bool ftp_server_rmdir (const char *path) {
  if (frmdir (path, NULL) == fsOK) {
    return (true);
  }
  return (false);
}

// Search the file system directory for matching files.
// Request type code: - 0 = Retrieve file size.
//                    - 1 = Retrieve last-modified time of a file.
//                    - 2 = List files by name, first call.
//                    - 3 = List files in extended format, first call.
//                    - 4 = List files by name, subsequent call.
//                    - 5 = List files in extended format, subsequent call.
__weak uint32_t ftp_server_ffind (uint8_t code, char *buf, uint32_t buflen, const char *mask) {
  static fsFileInfo info;
  int32_t rlen,v;
  const char *tp;

  (void)buflen;
  if (code < 4) {
    /* First call to ffind, initialize the info. */
    info.fileID = 0;
  }

  rlen = 0;
next:
  if (ffind (mask, &info) == fsOK) {
    // File found, print file information.
    if (info.name[0] == '.') {
      if ((info.name[1] == 0) || (info.name[1] == '.' && info.name[2] == 0)) {
        // Ignore the '.' and '..' folders.
        goto next;
      }
    }
    switch (code) {
      case 0:
        // Return file size as decimal number.
        rlen = sprintf (buf,"%d\r\n", info.size);
        break;

      case 1:
        // Return last-modified time in format "YYYYMMDDhhmmss".
        rlen  = sprintf (buf,"%04d%02d%02d",
                         info.time.year, info.time.mon, info.time.day);
        rlen += sprintf (&buf[rlen],"%02d%02d%02d\r\n",
                         info.time.hr, info.time.min, info.time.sec);
        break;

      case 2:
      case 4:
        // List file names only.
        rlen = sprintf (buf,"%s\r\n", info.name);
        break;

      case 3:
      case 5:
        // List directory in extended format.
        rlen  = sprintf (buf,"%02d-%02d-%02d", 
                         info.time.mon, info.time.day, info.time.year%100);
        // Convert time to "AM/PM" format.
        v = info.time.hr % 12;
        if (v == 0) v = 12;
        if (info.time.hr < 12) tp = "AM";
        else                   tp = "PM";
        rlen += sprintf (&buf[rlen],"  %02d:%02d%s",v,info.time.min,tp);
        if (info.attrib & FS_FAT_ATTR_DIRECTORY) {
          rlen += sprintf (&buf[rlen],"%-21s","       <DIR>");
        }
        else {
          rlen += sprintf (&buf[rlen],"%21u", info.size);
        }
        rlen += sprintf (&buf[rlen]," %s\r\n", info.name);
        break;
    }
  }
  return ((uint32_t)rlen);
}
//! [code_FTP_Server_FS]

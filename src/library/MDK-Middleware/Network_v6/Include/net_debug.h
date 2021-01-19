/*------------------------------------------------------------------------------
 * MDK Middleware - Component ::Network
 * Copyright (c) 2004-2019 Arm Limited (or its affiliates). All rights reserved.
 *------------------------------------------------------------------------------
 * Name:    net_debug.h
 * Purpose: Network Library Debug Configuration
 * Rev.:    V6.7.0
 *----------------------------------------------------------------------------*/

#include <stdio.h>
#include <stdarg.h>
#include "rl_net_lib.h"

#if   defined(__CC_ARM)
  #pragma diag_suppress 111
#elif defined(__clang__)
  #pragma clang diagnostic ignored "-Wformat-nonliteral"
#endif

#if (DBG_TIME)
  #define __DBG_TIME    dbg_time()
#else
  #define __DBG_TIME    ""
#endif

/*--------------------------- dbg_info --------------------------------------*/

void dbg_info (int proc, const char *fmt, ...) {
  /* Print debug information message. */
  __va_list args;

  switch (proc) {
#if (DBG_MEM > 1)
    case NET_DYN_MEMORY:    break;
#endif
#if (DBG_ETH > 1)
    case NET_ETH_LINK:      break;
#endif
#if (DBG_PPP > 1)
    case NET_PPP_LINK:      break;
#endif
#if (DBG_SLIP > 1)
    case NET_SLIP_LINK:     break;
#endif
#if (DBG_ARP > 1)
    case NET_ARP_CACHE:     break;
#endif
#if (DBG_IP > 1)
    case NET_IP4_LHOST:     break;
#endif
#if (DBG_ICMP > 1)
    case NET_ICMP_CONTROL:  break;
#endif
#if (DBG_IGMP > 1)
    case NET_IGMP_HOST:     break;
#endif
#if (DBG_UDP > 1)
    case NET_UDP_SOCKET:    break;
#endif
#if (DBG_TCP > 1)
    case NET_TCP_SOCKET:    break;
#endif
#if (DBG_NBNS > 1)
    case NET_NBNS_CLIENT:   break;
#endif
#if (DBG_DHCP > 1)
    case NET_DHCP_CLIENT:   break;
#endif
#if (DBG_DNS > 1)
    case NET_DNS_CLIENT:    break;
#endif
#if (DBG_SNMP > 1)
    case NET_SNMP_AGENT:    break;
#endif
#if (DBG_SNTP > 1)
    case NET_SNTP_CLIENT:   break;
#endif
#if (DBG_BSD > 1)
    case NET_BSD_SOCKET:    break;
#endif
#if (DBG_HTTP_SERVER > 1)
    case NET_HTTP_SERVER:   break;
#endif
#if (DBG_FTP_SERVER > 1)
    case NET_FTP_SERVER:    break;
#endif
#if (DBG_FTP_CLIENT > 1)
    case NET_FTP_CLIENT:    break;
#endif
#if (DBG_TELNET_SERVER > 1)
    case NET_TELNET_SERVER: break;
#endif
#if (DBG_TFTP_SERVER > 1)
    case NET_TFTP_SERVER:   break;
#endif
#if (DBG_TFTP_CLIENT > 1)
    case NET_TFTP_CLIENT:   break;
#endif
#if (DBG_SMTP_CLIENT > 1)
    case NET_SMTP_CLIENT:   break;
#endif
    default: return;
  }
  printf ("%s%s:",__DBG_TIME,dbg_proc(proc));

  va_start (args,fmt);
  vprintf (fmt,args);
  va_end (args);
}

/*--------------------------- dbg_error -------------------------------------*/

void dbg_error (int proc, const char *fmt, ...) {
  /* Pring debug error message. */
  __va_list args;

  switch (proc) {
#if (DBG_MEM > 0)
    case NET_DYN_MEMORY:    break;
#endif
#if (DBG_ETH > 0)
    case NET_ETH_LINK:      break;
#endif
#if (DBG_PPP > 0)
    case NET_PPP_LINK:      break;
#endif
#if (DBG_SLIP > 0)
    case NET_SLIP_LINK:     break;
#endif
#if (DBG_ARP > 0)
    case NET_ARP_CACHE:     break;
#endif
#if (DBG_IP > 0)
    case NET_IP4_LHOST:     break;
#endif
#if (DBG_ICMP > 0)
    case NET_ICMP_CONTROL:  break;
#endif
#if (DBG_IGMP > 0)
    case NET_IGMP_HOST:     break;
#endif
#if (DBG_UDP > 0)
    case NET_UDP_SOCKET:    break;
#endif
#if (DBG_TCP > 0)
    case NET_TCP_SOCKET:    break;
#endif
#if (DBG_NBNS > 0)
    case NET_NBNS_CLIENT:   break;
#endif
#if (DBG_DHCP > 0)
    case NET_DHCP_CLIENT:   break;
#endif
#if (DBG_DNS > 0)
    case NET_DNS_CLIENT:    break;
#endif
#if (DBG_SNMP > 0)
    case NET_SNMP_AGENT:    break;
#endif
#if (DBG_SNTP > 0)
    case NET_SNTP_CLIENT:   break;
#endif
#if (DBG_BSD > 0)
    case NET_BSD_SOCKET:    break;
#endif
#if (DBG_HTTP_SERVER > 0)
    case NET_HTTP_SERVER:   break;
#endif
#if (DBG_FTP_SERVER > 0)
    case NET_FTP_SERVER:    break;
#endif
#if (DBG_FTP_CLIENT > 0)
    case NET_FTP_CLIENT:    break;
#endif
#if (DBG_TELNET_SERVER > 0)
    case NET_TELNET_SERVER: break;
#endif
#if (DBG_TFTP_SERVER > 0)
    case NET_TFTP_SERVER:   break;
#endif
#if (DBG_TFTP_CLIENT > 0)
    case NET_TFTP_CLIENT:   break;
#endif
#if (DBG_SMTP_CLIENT > 0)
    case NET_SMTP_CLIENT:   break;
#endif
    default: return;
  }
  printf ("%s%s-ERR:",__DBG_TIME,dbg_proc(proc));

  va_start (args,fmt);
  vprintf (fmt,args);
  va_end (args);
}

/*------------------------------------------------------------------------------
 * MDK Middleware - Component ::Network:Service
 * Copyright (c) 2004-2019 Arm Limited (or its affiliates). All rights reserved.
 *------------------------------------------------------------------------------
 * Name:    DNS_Client.c
 * Purpose: DNS Client Code Template
 * Rev.:    V6.00
 *----------------------------------------------------------------------------*/
//! [code_DNS_Client]
#include <rl_net.h>
 
// Notify the user application about DNS client events.
void dns_cbfunc (dnsClientEvent event, const uint8_t *ip_addr) {

  switch (event) {
    case dnsClientSuccess:
      // Host Address successfully resolved
      /* Example
      printf("IP Address: %d.%d.%d.%d\n", ip_addr[0], ip_addr[1],
                                          ip_addr[2], ip_addr[3]);
      */
      break;

    case dnsClientNotResolved:
      // Error, host name does not exist in DNS record database
      break;

    case dnsClientTimeout:
      // Error, DNS resolver timeout expired
      break;

    case dnsClientError:
      // Error, DNS protocol error occurred
      break;
  }
}
 
// Start DNS resolving process.
void resolve_host (void) {
  netStatus res;
 
  res = get_host_by_name ("www.arm.com", dns_cbfunc);
  switch (res) {
    case netOK:
      // Started, will complete on callback notification
      break;

    case netBusy:
      // Busy, resolving in progress
      break;

    case netInvalidParameter:
      // Error, undefined parameters or invalid host name 
      break;

    case netServerError:
      // Error, DNS Server IP address unknown
      break;

    default:
      break;
  }
}
//! [code_DNS_Client]

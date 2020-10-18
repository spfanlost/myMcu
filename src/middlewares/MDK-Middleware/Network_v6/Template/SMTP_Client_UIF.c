/*------------------------------------------------------------------------------
 * MDK Middleware - Component ::Network:Service
 * Copyright (c) 2004-2019 Arm Limited (or its affiliates). All rights reserved.
 *------------------------------------------------------------------------------
 * Name:    SMTP_Client_UIF.c
 * Purpose: SMTP Client User Interface
 * Rev.:    V6.00
 *----------------------------------------------------------------------------*/
//! [code_SMTP_Client_UIF]
#include <stdio.h>
#include "rl_net.h"
 
// Request parameters for SMTP client session.
uint32_t smtp_client_request (smtpClientRequest request, char *buf, uint32_t buflen, uint32_t *pvar) {
  uint32_t len = 0;
 
  switch (request) {
    case smtpClientUsername:
      // Username to login to SMTP server
      /* Example
      len = sprintf (buf, "guest");
      */
      break;

    case smtpClientPassword:
      // Password to login to SMTP server
      /* Example
      len = sprintf (buf, "guest");
      */
      break;

    case smtpClientSender:
      // Email address of the sender
      /* Example
      len = sprintf (buf, "me@domain.com");
      */
      break;

    case smtpClientRecipient:
      // Email address of the recipient
      /* Example
      len = sprintf (buf, "you@domain.com");
      */
      break;

    case smtpClientSubject:
      // Subject of email
      /* Example
      len = sprintf (buf, "Hello");
      */
      break;

    case smtpClientBody:
      // Email body in plain ascii format
      /* Example
      len = sprintf (buf, "Hello, how are you?\r\n");
      */
      break;
  }
  return (len);
}
 
// Notify the user application when SMTP client operation ends.
void smtp_client_notify (smtpClientEvent event) {
  switch (event) {
    case smtpClientSuccess:
      // Email successfully sent
      break;
    case smtpClientTimeout:
      // Timeout sending email
      break;
    case smtpClientError:
      // Error when sending email
      break;
  }
}
 
// Accept or deny authentication requested by SMTP server.
bool smtp_client_accept_authentication (const uint8_t *ip_addr) {
  /* Example
  if (ip_addr[0] == 192  &&
      ip_addr[1] == 168  &&
      ip_addr[2] == 0    &&
      ip_addr[3] == 1) {
    // Deny authentication from local smtp server
    return (false);
  }
  */
  return (true);
}
//! [code_SMTP_Client_UIF]

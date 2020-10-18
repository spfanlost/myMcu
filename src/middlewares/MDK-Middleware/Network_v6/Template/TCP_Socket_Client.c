/*------------------------------------------------------------------------------
 * MDK Middleware - Component ::Network:Service
 * Copyright (c) 2004-2019 Arm Limited (or its affiliates). All rights reserved.
 *------------------------------------------------------------------------------
 * Name:    TCP_Socket_Client.c
 * Purpose: TCP Socket Client Code Template
 * Rev.:    V6.00
 *----------------------------------------------------------------------------*/
//! [code_TCP_Socket_Client]
#include "rl_net.h"
 
int32_t tcp_sock;                       // TCP socket handle number
 
// Notify the user application about TCP socket events.
uint32_t tcp_cb_client (int32_t socket, tcpEvent event, const uint8_t *buf, uint32_t len) {

  switch (event) {
    case tcpEventConnect:
      // Connect request received
      break;

    case tcpEventEstablished:
      // Connection established
      break;

    case tcpEventClosed:
      // Connection was properly closed
      break;

    case tcpEventAbort:
      // Connection is for some reason aborted
      break;

    case tcpEventACK:
      // Previously sent data acknowledged
      break;

    case tcpEventData:
      // Data received
      break;
  }
  return (0);
}
 
// Connect to TCP server and send data.
void send_data (void) {
  uint8_t dest_ip [IP4_ADDR_LEN];
 
  if (tcp_sock > 0) {
    switch (tcp_get_state (tcp_sock)) {
      case tcpStateUNUSED:
      case tcpStateCLOSED:
        // Connect to server at 192.168.0.100, port 2000
        dest_ip [0] = 192;
        dest_ip [1] = 168;
        dest_ip [2] = 0;
        dest_ip [3] = 100;

        tcp_connect (tcp_sock, dest_ip, 2000, 0);
        break;

      case tcpStateESTABLISHED:
        // Connected, send the data
        if (tcp_check_send (tcp_sock) == true) {
          /* Example
          uint8_t *sendbuf;

          sendbuf = tcp_get_buf (2);
          sendbuf[0] = 0x01;
          sendbuf[1] = 0x55;

          tcp_send (tcp_sock, sendbuf, 2);
          */
        }
        break;

      default:
        break;
    }
  }
}
 
// Allocate and initialize the socket.
/* Example
int main (void) {

  net_initialize ();

  // Allocate a free TCP socket.
  tcp_sock = tcp_get_socket (TCP_TYPE_CLIENT, 0, 10, tcp_cb_client);

  while(1) {
    net_main ();
    osThreadYield ();
  }
}
*/
//! [code_TCP_Socket_Client]

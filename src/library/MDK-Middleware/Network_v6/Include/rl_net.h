/*------------------------------------------------------------------------------
 * MDK Middleware - Component ::Network
 * Copyright (c) 2004-2019 Arm Limited (or its affiliates). All rights reserved.
 *------------------------------------------------------------------------------
 * Name:    rl_net.h
 * Purpose: Network API
 * Rev.:    V6.7.2
 *----------------------------------------------------------------------------*/

#ifndef __RL_NET_H
#define __RL_NET_H

#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>
#include "RTE_Components.h"

#ifdef __cplusplus
extern "C"  {
#endif

#ifdef __clang__
  #define __weak                __attribute__((weak))
#endif

/// Network to host byte order conversion.
#if defined(__BIG_ENDIAN) || defined(__ARM_BIG_ENDIAN)
  #define ntohl(v)              (uint32_t)(v)
  #define ntohs(v)              (uint16_t)(v)
#elif defined (__CC_ARM)
  /* ARM Compiler 4/5 */
  #define ntohl(v)              (uint32_t)(__rev(v))
  #define ntohs(v)              (uint16_t)(__rev(v) >> 16)
#else
  /* ARM Compiler 6 */
  #define ntohl(v)              __builtin_bswap32((uint32_t)(v))
  #define ntohs(v)              __builtin_bswap16((uint16_t)(v))
#endif

/// Host to network byte order conversion.
#define htons(v)                ntohs(v)
#define htonl(v)                ntohl(v)

/// General definitions.
#define ETH_ADDR_LEN            6       ///< Ethernet Address Length in bytes
#define IP4_ADDR_LEN            4       ///< IPv4 Address Length in bytes
#define IP6_ADDR_LEN            16      ///< IPv6 Address Length in bytes

/// Network Interfaces.
#define NETIF_ETH               0       ///< Network interface: Ethernet
#define NETIF_PPP               1       ///< Network interface: PPP
#define NETIF_SLIP              2       ///< Network interface: SLIP
#define NETIF_LOCAL             3       ///< Network interface: Localhost (loopback)
#define NETIF_NULL              4       ///< Network interface: Null (none)

/// UDP Options.
#define UDP_OPT_SEND_CHECKSUM   0x01    ///< Calculate Checksum for UDP send frames
#define UDP_OPT_VERIFY_CHECKSUM 0x02    ///< Verify Checksum for received UDP frames

/// TCP Socket Types.
#define TCP_TYPE_SERVER         0x01    ///< Socket Type: Server (open for listening)
#define TCP_TYPE_CLIENT         0x02    ///< Socket Type: Client (initiate connect)
#define TCP_TYPE_CLIENT_SERVER  0x03    ///< Socket Type: Client and Server
#define TCP_TYPE_DELAY_ACK      0x04    ///< Socket Type: Delayed Acknowledge
#define TCP_TYPE_FLOW_CTRL      0x08    ///< Socket Type: Flow Control
#define TCP_TYPE_KEEP_ALIVE     0x10    ///< Socket Type: Keep Alive

/// TCP Callback Events.
typedef enum {
  tcpEventConnect             = 0,      ///< Connect request received event
  tcpEventEstablished,                  ///< Connection established event
  tcpEventClosed,                       ///< Connection was properly closed
  tcpEventAbort,                        ///< Connection is for some reason aborted
  tcpEventACK,                          ///< Previously send data acknowledged
  tcpEventData                          ///< Data received event
} tcpEvent;

/// TCP States.
typedef enum {
  tcpStateUNUSED              = 0,      ///< Entry is free and unused
  tcpStateCLOSED,                       ///< Entry allocated, socket still closed
  tcpStateLISTEN,                       ///< Socket waiting for incoming connection
  tcpStateSYN_RECEIVED,                 ///< SYN frame received
  tcpStateSYN_SENT,                     ///< SYN packet sent to establish a connection
  tcpStateFIN_WAIT_1,                   ///< Tcp_close started FIN packet was sent
  tcpStateFIN_WAIT_2,                   ///< Our FIN ack-ed, waiting for remote FIN
  tcpStateCLOSING,                      ///< Received FIN independently of our FIN
  tcpStateLAST_ACK,                     ///< Waiting for last ACK for our FIN
  tcpStateTIME_WAIT,                    ///< Timed waiting for 2MSL
  tcpStateESTABLISHED,                  ///< TCP Connection established
} tcpState;

/// UDP Event callback function.
typedef uint32_t (*net_udp_cb_t)(int32_t socket, const uint8_t *ip_addr, uint16_t port, const uint8_t *buf, uint32_t len);

/// TCP Event callback function.
typedef uint32_t (*net_tcp_cb_t)(int32_t socket, tcpEvent event, const uint8_t *buf, uint32_t len);

//  ==== BSD Socket definitions ====
#ifdef RTE_Network_Socket_BSD

/// BSD Socket Address Family
#define AF_UNSPEC               0       ///< Unspecified
#define AF_INET                 1       ///< Internet Address Family (UDP, TCP)
#define AF_NETBIOS              2       ///< NetBios-style addresses

/// BSD Protocol families (same as address families)
#define PF_UNSPEC               0       ///< Unspecified
#define PF_INET                 1       ///< Internet Address Family (UDP, TCP)
#define PF_NETBIOS              2       ///< NetBios-style addresses

/// BSD Socket Type
#define SOCK_STREAM             1       ///< Stream Socket (Connection oriented)
#define SOCK_DGRAM              2       ///< Datagram Socket (Connectionless)

/// BSD Socket Protocol
#define IPPROTO_TCP             1       ///< TCP Protocol
#define IPPROTO_UDP             2       ///< UDP Protocol

/// BSD Internet Addresses
#define INADDR_ANY          0x00000000  ///< All IP addresses accepted
#define INADDR_NONE         0xffffffff  ///< No IP address accepted

/// BSD Socket flags parameter
#define MSG_DONTWAIT            0x01    ///< Enables non-blocking operation
#define MSG_PEEK                0x02    ///< Peeks at the incoming data

/// BSD Socket ioctl commands
#define FIONBIO                 1       ///< Set mode (blocking/non-blocking)
#define FIO_DELAY_ACK           2       ///< Set DELAY_ACK mode for stream socket
#define FIO_KEEP_ALIVE          3       ///< Set KEEP_ALIVE mode for stream socket
#define FIO_FLOW_CTRL           4       ///< Set FLOW_CTRL mode for stream socket

/// BSD Socket Return values
#define BSD_SUCCESS             0       ///< Success
#define BSD_ERROR               (-1)    ///< General Error
#define BSD_ERROR_SOCKET        (-2)    ///< Invalid socket descriptor
#define BSD_ERROR_PARAMETER     (-3)    ///< Invalid parameter
#define BSD_ERROR_WOULDBLOCK    (-4)    ///< It would have blocked
#define BSD_ERROR_NOMEMORY      (-5)    ///< Not enough memory in memory pool
#define BSD_ERROR_CLOSED        (-6)    ///< Connection is closed or aborted
#define BSD_ERROR_LOCKED        (-7)    ///< Socket is locked by another thread
#define BSD_ERROR_TIMEOUT       (-8)    ///< Socket, Host Resolver timeout
#define BSD_ERROR_INPROGRESS    (-9)    ///< Host Name resolving in progress
#define BSD_ERROR_NONAME        (-10)   ///< Host Name not existing

#endif /* RTE_Network_Socket_BSD */

/// Ethernet Link Callback Events.
typedef enum {
  ethLinkDown                 = 0,      ///< Link down
  ethLinkUp_10MHalfDuplex,              ///< Link up (10 MBit half duplex)
  ethLinkUp_10MFullDuplex,              ///< Link up (10 MBit full duplex)
  ethLinkUp_100MHalfDuplex,             ///< Link up (100 MBit half duplex)
  ethLinkUp_100MFullDuplex,             ///< Link up (100 MBit full duplex)
  ethLinkUp_1GHalfDuplex,               ///< Link up (1 GBit half duplex)
  ethLinkUp_1GFullDuplex                ///< Link up (1 GBit full duplex)
} ethLinkEvent;

/// ARP Cache Entry types.
typedef enum {
  arpCacheFixedIP,                      ///< Fixed IP address is refreshed after timeout
  arpCacheTemporaryIP                   ///< Temporary IP address is removed after timeout
} arpCacheType;

/// ICMP (ping) Callback Events.
typedef enum {
  icmpEventSuccess            = 0,      ///< Pinged Host responded
  icmpEventTimeout                      ///< Timeout, no ping response received
} icmpEvent;

/// DHCP Client Callback Events.
typedef enum {
  dhcpClientIPaddress         = 0,      ///< IP address change event
  dhcpClientNTPservers        = 42,     ///< NTP Servers option
  dhcpClientBootfileName      = 67,     ///< Bootfile name option
} dhcpClientOption;

/// DNS Client Callback Events.
typedef enum {
  dnsClientSuccess            = 0,      ///< Host name successfully resolved
  dnsClientTimeout,                     ///< Timeout resolving host
  dnsClientNotResolved,                 ///< DNS Error, no such name
  dnsClientError                        ///< Erroneous response packet
} dnsClientEvent;

/// FTP Commands.
typedef enum {
  ftpPUT,                               ///< Puts a file on FTP server
  ftpGET,                               ///< Retrieves a file from FTP server
  ftpAPPEND,                            ///< Append file on FTP server (with create)
  ftpDELETE,                            ///< Deletes a file on FTP server
  ftpLIST,                              ///< Lists files stored on FTP server
  ftpRENAME,                            ///< Renames a file on FTP server
  ftpMKDIR,                             ///< Makes a directory on FTP server
  ftpRMDIR,                             ///< Removes an empty directory on FTP server
  ftpNLIST                              ///< Lists file names only (short format)
} ftpCommand;

/// FTP Server Events.
typedef enum {
  ftpServerLogin,                       ///< User logged in, session is busy
  ftpServerLogout,                      ///< User logged out, session is idle
  ftpServerLoginFailed,                 ///< User login failed (invalid credentials)
  ftpServerDownload,                    ///< File download ended
  ftpServerUpload,                      ///< File upload ended
  ftpServerDelete,                      ///< File deleted
  ftpServerRename,                      ///< File or directory renamed
  ftpServerMakeDirectory,               ///< Directory created
  ftpServerRemoveDirectory,             ///< Directory removed
  ftpServerOperationDenied,             ///< Requested file operation denied
  ftpServerLocalFileError,              ///< Local file operation error
  ftpServerFileError,                   ///< Generic file operation error
  ftpServerError                        ///< Generic FTP server error
} ftpServerEvent;

/// FTP Client Requests.
typedef enum {
  ftpClientUsername,                    ///< Username to login to FTP server
  ftpClientPassword,                    ///< Password to login to FTP server
  ftpClientPath,                        ///< Working directory path on server
  ftpClientFilename,                    ///< Filename for PUT, GET, APPEND, DELETE or RENAME command
  ftpClientNewName,                     ///< New name for a RENAME command
  ftpClientDirectory,                   ///< Directory name for MKDIR or RMDIR command
  ftpClientFilterMask,                  ///< File filter/mask for LIST command (wildcards allowed)
  ftpClientList,                        ///< Received data if LIST command is given
  ftpClientLocalFile                    ///< Local filename
} ftpClientRequest;

/// FTP Client Events.
typedef enum {
  ftpClientSuccess           = 0,       ///< File operation successful
  ftpClientTimeout,                     ///< Timeout on file operation
  ftpClientLoginFailed,                 ///< Login error, username/password invalid
  ftpClientAccessDenied,                ///< File access not allowed
  ftpClientFileNotFound,                ///< File not found
  ftpClientInvalidPath,                 ///< Working directory path not found
  ftpClientLocalFileError,              ///< Local file open/write error
  ftpClientError                        ///< Generic FTP client error 
} ftpClientEvent;            

/// TFTP Client Events.
typedef enum {
  tftpClientSuccess          = 0,       ///< File operation successful
  tftpClientTimeout,                    ///< Timeout on file operation
  tftpClientAccessDenied,               ///< File access not allowed
  tftpClientFileNotFound,               ///< File not found
  tftpClientDiskFull,                   ///< Disk full (local or remote)
  tftpClientError                       ///< Generic TFTP client error
} tftpClientEvent;          

/// Telnet Server Messages.
typedef enum {
  telnetServerWelcome,                  ///< Initial welcome message
  telnetServerPrompt,                   ///< Prompt message
  telnetServerLogin,                    ///< Login welcome message, if authentication is enabled
  telnetServerUsername,                 ///< Username request login message
  telnetServerPassword,                 ///< Password request login message
  telnetServerLoginFailed,              ///< Incorrect login error message
  telnetServerLoginTimeout,             ///< Login timeout error message
  telnetServerUnsolicitedMessage        ///< Unsolicited message (ie. from basic interpreter)
} telnetServerMessage;

/// SMTP Client Request.
typedef enum {
  smtpClientUsername,                   ///< Username to login to SMTP server
  smtpClientPassword,                   ///< Password to login to SMTP server
  smtpClientSender,                     ///< Email address of the sender
  smtpClientRecipient,                  ///< Email address of the recipient
  smtpClientSubject,                    ///< Subject of email
  smtpClientBody,                       ///< Email body in plain ASCII format
} smtpClientRequest;

/// SMTP Client Events.
typedef enum {
  smtpClientSuccess           = 0,      ///< Email successfully sent
  smtpClientTimeout,                    ///< Timeout sending email
  smtpClientError                       ///< Error when sending email
} smtpClientEvent;

/// Network return codes.
typedef enum {
  netOK                       =  0,     ///< Operation succeeded
  netBusy,                              ///< Process is busy
  netError,                             ///< Unspecified error
  netInvalidParameter,                  ///< Invalid parameter specified
  netWrongState,                        ///< Wrong state error
  netDriverError,                       ///< Driver error
  netServerError,                       ///< DNS/SNTP server error
} netStatus;

/// ICMP Event callback function.
typedef void (*net_icmp_cb_t)(icmpEvent event);

/// DNS Client Event callback function.
typedef void (*net_dns_client_cb_t)(dnsClientEvent event, const uint8_t *ip_addr);

/// SNTP Client callback function.
typedef void (*net_sntp_client_cb_t)(uint32_t utc_time);

//  ==== BSD Socket structures ====
#ifdef RTE_Network_Socket_BSD

/// Generic Socket Address structure
typedef struct sockaddr {
  uint16_t sa_family;                   ///< Address family
  int8_t   sa_data[14];                 ///< Direct address (up to 14 bytes)
} SOCKADDR;

#if   defined(__CC_ARM)
  #pragma push
  #pragma anon_unions
#elif defined(__clang__)
  #pragma clang diagnostic push
  #pragma clang diagnostic ignored "-Wc11-extensions"
#endif

/// Generic IPv4 Address structure
typedef struct in_addr {
  union {
    struct {
      uint8_t s_b1,s_b2,s_b3,s_b4;      ///< IP address, byte access
    };
    struct {
      uint16_t s_w1,s_w2;               ///< IP address, short int access
    };
    uint32_t s_addr;                    ///< IP address in network byte order
  };
} IN_ADDR;

#if   defined(__CC_ARM)
  #pragma pop
#elif defined(__clang__)
  #pragma clang diagnostic pop
#endif

/// IPv4 Socket Address structure
typedef struct sockaddr_in {
  int16_t  sin_family;                  ///< Socket domain
  uint16_t sin_port;                    ///< Port
  IN_ADDR  sin_addr;                    ///< IP address
  int8_t   sin_zero[8];                 ///< reserved
} SOCKADDR_IN;

/// BSD Host Entry structure
typedef struct hostent {
  char   *h_name;                       ///< Official name of host
  char  **h_aliases;                    ///< Pointer to an array of alias names
  int16_t h_addrtype;                   ///< Address Type: AF_INET, AF_NETBIOS
  int16_t h_length;                     ///< Length of address in bytes
  char  **h_addr_list;                  ///< Pointer to an array of IPv4 addresses
} HOSTENT;

#endif /* RTE_Network_Socket_BSD */

/// SNMP-MIB definitions
#define MIB_INTEGER             0x02    ///< MIB entry type INTEGER
#define MIB_OCTET_STR           0x04    ///< MIB entry type OCTET_STRING
#define MIB_OBJECT_ID           0x06    ///< MIB entry type OBJECT_IDENTIFIER
#define MIB_IP_ADDR             0x40    ///< MIB entry type IP ADDRESS (uint8_t[4])
#define MIB_COUNTER             0x41    ///< MIB entry type COUNTER (uint32_t)
#define MIB_GAUGE               0x42    ///< MIB entry type GAUGE (uint32_t)
#define MIB_TIME_TICKS          0x43    ///< MIB entry type TIME_TICKS
#define MIB_ATR_RO              0x80    ///< MIB entry attribute READ_ONLY
#define MIB_OID_SIZE            17      ///< Max.size of Object ID value
#define MIB_STR_SIZE            110     ///< Max.size of Octet String variable
#define MIB_READ                0       ///< MIB entry Read access
#define MIB_WRITE               1       ///< MIB entry Write access

/// SNMP-MIB macros
#define MIB_STR(s)              sizeof(s)-1, s
#define MIB_INT(o)              sizeof(o), (void *)&o
#define MIB_IP(ip)              4, (void *)&ip
#define MIB_OID0(f,s)           (f*40 + s)
#define SNMP_MIB_SIZE(mib)      (sizeof(mib) / sizeof(SNMP_MIB))

/// SNMP-MIB Entry info
typedef struct snmp_mib {
  uint8_t  Type;                        ///< Object Type
  uint8_t  OidLen;                      ///< Object ID length
  uint8_t  Oid[MIB_OID_SIZE];           ///< Object ID value
  uint8_t  ValSz;                       ///< Size of a variable
  void     *Val;                        ///< Pointer to a variable
  void     (*cb_func)(int mode);        ///< Write/Read event callback function
} const SNMP_MIB;

//  ==== Network native API ====

/// \brief Initialize Network Component, resources and interfaces.
/// \return      status code that indicates the execution status of the function as defined with \ref netStatus.
extern netStatus net_initialize (void);

/// \brief Run Network Component main function.
/// \return      system status information:
///              - 1           = Another call to this function needed.
///              - 0           = System is idle.
extern int net_main (void);

/// \brief Set default network interface used for internet access.
/// \param[in]     netif         Network interface number:
///                              - NETIF_ETH  = Ethernet interface.
///                              - NETIF_PPP  = PPP interface.
///                              - NETIF_SLIP = SLIP interface.
/// \return      status code that indicates the execution status of the function as defined with \ref netStatus.
extern netStatus netif_set_default (uint8_t netif);

//  ==== UDP Socket API ====

/// \brief Allocate a free UDP socket.
/// \param[in]     tos           IP type of service:
///                              - 0 = default value.
/// \param[in]     opt           Checksum calculation option: 
///                              - UDP_OPT_SEND_CHECKSUM   = Calculate transmit checksum.
///                              - UDP_OPT_VERIFY_CHECKSUM = Verify receive checksum.
///                              - 0                       = No checksum calculation.
/// \param[in]     cb_func       event listening callback function.
/// \return      socket handle number or execution status:
///              - value >= 0:   socket handle number
///              - value < 0:    error occurred, -value is execution status as defined with \ref netStatus 
extern int32_t udp_get_socket (uint8_t tos, uint8_t opt, net_udp_cb_t cb_func);

/// \brief Release UDP socket and free resources.
/// \param[in]     socket        socket handle obtained with \ref udp_get_socket.
/// \return      status code that indicates the execution status of the function as defined with \ref netStatus.
extern netStatus udp_release_socket (int32_t socket);

/// \brief Open UDP socket for communication.
/// \param[in]     socket        socket handle obtained with \ref udp_get_socket.
/// \param[in]     port          local port number or
///                                0 for system assigned local port.
/// \return      status code that indicates the execution status of the function as defined with \ref netStatus.
extern netStatus udp_open (int32_t socket, uint16_t port);

/// \brief Stop UDP communication and close socket.
/// \param[in]     socket        socket handle obtained with \ref udp_get_socket.
/// \return      status code that indicates the execution status of the function as defined with \ref netStatus.
extern netStatus udp_close (int32_t socket);

/// \brief Set Time To Live value for multicast packets.
/// \param[in]     socket        socket handle obtained with \ref udp_get_socket.
/// \param[in]     ttl           number of routers the packet can cross before being expired.
/// \return      status code that indicates the execution status of the function as defined with \ref netStatus.
extern netStatus udp_multicast_ttl (int32_t socket, uint8_t ttl);

/// \brief Allocate memory for UDP send buffer.
/// \param[in]     size          number of bytes to allocate.
/// \return        pointer to the allocated memory.
extern uint8_t *udp_get_buf (uint32_t size);

/// \brief Send data to a remote node.
/// \param[in]     socket        socket handle obtained with \ref udp_get_socket.
/// \param[in]     ip_addr       IP address of the remote node.
/// \param[in]     port          port number of the remote node to send data to.
/// \param[in]     buf           buffer containing the data.
/// \param[in]     len           length of data in bytes.
/// \return      status code that indicates the execution status of the function as defined with \ref netStatus.
extern netStatus udp_send (int32_t socket, const uint8_t *ip_addr, uint16_t port, uint8_t *buf, uint32_t len);

//  ==== TCP Socket API ====

/// \brief Allocate a free TCP socket.
/// \param[in]     type          socket type:
///                              - TCP_TYPE_SERVER     = Able to listen.
///                              - TCP_TYPE_CLIENT     = Able to initiate a connection.
///                              - TCP_TYPE_DELAY_ACK  = Send delayed ack.
///                              - TCP_TYPE_FLOW_CTRL  = Control data flow.
///                              - TCP_TYPE_KEEP_ALIVE = Keep a connection alive.
/// \param[in]     tos           IP type of service:
///                              - 0 = default value.
/// \param[in]     tout          idle timeout in seconds.
/// \param[in]     cb_func       event listening callback function.
/// \return      socket handle number or execution status:
///              - value >= 0:   socket handle number
///              - value < 0:    error occurred, -value is execution status as defined with \ref netStatus 
extern int32_t tcp_get_socket (uint8_t type, uint8_t tos, uint32_t tout, net_tcp_cb_t cb_func);

/// \brief Release TCP socket and free resources.
/// \param[in]     socket        socket handle obtained with \ref tcp_get_socket.
/// \return      status code that indicates the execution status of the function as defined with \ref netStatus.
extern netStatus tcp_release_socket (int32_t socket);

/// \brief Open TCP socket for incoming connection.
/// \param[in]     socket        socket handle obtained with \ref tcp_get_socket.
/// \param[in]     port          local port number.
/// \return      status code that indicates the execution status of the function as defined with \ref netStatus.
extern netStatus tcp_listen (int32_t socket, uint16_t port);

/// \brief Initiate a TCP connection to a remote node.
/// \param[in]     socket        socket handle obtained with \ref tcp_get_socket.
/// \param[in]     ip_addr       IP address of the remote node.
/// \param[in]     port          port number of the remote node.
/// \param[in]     local_port    local port number or
///                                0 for system assigned local port.
/// \return      status code that indicates the execution status of the function as defined with \ref netStatus.
extern netStatus tcp_connect (int32_t socket, const uint8_t *ip_addr, uint16_t port, uint16_t local_port);

/// \brief Allocate memory for TCP send buffer.
/// \param[in]     size          number of bytes to allocate.
/// \return        pointer to the allocated memory.
extern uint8_t *tcp_get_buf (uint32_t size);

/// \brief Determine maximum number of data bytes that can be sent in TCP packet.
/// \param[in]     socket        socket handle obtained with \ref tcp_get_socket.
/// \return        maximum segment size in bytes.
extern uint32_t tcp_max_data_size (int32_t socket);

/// \brief Check if TCP socket can send data.
/// \param[in]     socket        socket handle obtained with \ref tcp_get_socket.
/// \return      send status:
///              - true        = Ready to send data.
///              - false       = Not ready.
extern bool tcp_check_send (int32_t socket);

/// \brief Determine current state of a TCP socket.
/// \param[in]     socket        socket handle obtained with \ref tcp_get_socket.
/// \return      status information as defined with \ref tcpState.
extern tcpState tcp_get_state (int32_t socket);

/// \brief Convert TCP socket state into an ASCII string.
/// \param[in]     state         socket state as defined with \ref tcpState
/// \return      pointer to a buffer containing the text representation of a socket state.
extern const char *tcp_ntoa (tcpState state);

/// \brief Send a data packet to remote node.
/// \param[in]     socket        socket handle obtained with \ref tcp_get_socket.
/// \param[in]     buf           buffer containing the data.
/// \param[in]     len           length of data in bytes.
/// \return      status code that indicates the execution status of the function as defined with \ref netStatus.
extern netStatus tcp_send (int32_t socket, uint8_t *buf, uint32_t len);

/// \brief Stop TCP communication and start closing procedure.
/// \param[in]     socket        socket handle obtained with \ref tcp_get_socket.
/// \return      status code that indicates the execution status of the function as defined with \ref netStatus.
extern netStatus tcp_close (int32_t socket);

/// \brief Instantly stop TCP communication.
/// \param[in]     socket        socket handle obtained with \ref tcp_get_socket.
/// \return      status code that indicates the execution status of the function as defined with \ref netStatus.
extern netStatus tcp_abort (int32_t socket);

/// \brief Reset TCP window size to a default value from the configuration.
/// \param[in]     socket        socket handle obtained with \ref tcp_get_socket.
/// \return      status code that indicates the execution status of the function as defined with \ref netStatus.
extern netStatus tcp_reset_window (int32_t socket);

//  ==== BSD Socket API ====
#ifdef RTE_Network_Socket_BSD

/// \brief Create a communication endpoint called socket.
/// \param[in]     family        address family:
///                              - AF_INET        = address family internet.
/// \param[in]     type          connection type of a socket:
///                              - SOCK_STREAM    = connection based type,
///                              - SOCK_DGRAM     = datagram connectionless type.
/// \param[in]     protocol      protocol type:
///                              - IPPROTO_TCP    = must be used with SOCK_STREAM type
///                              - IPPROTO_UDP    = must be used with SOCK_DGRAM TYPE,
///                              - 0              = for system auto-select.
/// \return      status information:
///              - Socket descriptor identification number or
///              - BSD_ERROR_PARAMETER = Invalid or not supported parameters.
///              - BSD_ERROR           = Allocate socket failed.
extern int socket (int family, int type, int protocol);

/// \brief Assign a local address and port to a socket.
/// \param[in]     sock          socket descriptor obtained with \ref socket.
/// \param[in]     addr          structure containing local IP address and port.
/// \param[in]     addrlen       length of \ref SOCKADDR structure.
/// \return      status information:
///              - BSD_SUCCESS         = Operation successful.
///              - BSD_ERROR_SOCKET    = Socket invalid.
///              - BSD_ERROR_PARAMETER = Invalid or not supported parameters.
extern int bind (int sock, const SOCKADDR *addr, int addrlen);

/// \brief Set a socket in a listening mode.
/// \param[in]     sock          socket descriptor obtained with \ref socket.
/// \param[in]     backlog       number of connection requests that can be queued.
/// \return      status information:
///              - BSD_SUCCESS         = Operation successful.
///              - BSD_ERROR_SOCKET    = Socket invalid.
///              - BSD_ERROR_PARAMETER = Invalid or not supported parameters.
///              - BSD_ERROR           = Not enough sockets available for backlog.
extern int listen (int sock, int backlog);

/// \brief Accept connect request for a listening socket.
/// \param[in]     sock          socket descriptor obtained with \ref socket.
/// \param[out]    addr          structure that will receive IP address and port number,
///                              or NULL for none
/// \param[in]     addrlen       length of \ref SOCKADDR structure.
/// \return      status information:
///              - Socket descriptor identification number or
///              - BSD_ERROR_SOCKET     = Socket invalid.
///              - BSD_ERROR_WOULDBLOCK = Function would block in non-blocking mode.
///              - BSD_ERROR_CLOSED     = Remote node has closed the connection.
extern int accept (int sock, SOCKADDR *addr, int *addrlen);

/// \brief Connect a socket to a remote host.
/// \param[in]     sock          socket descriptor obtained with \ref socket.
/// \param[in]     addr          structure containing remote IP address and port.
/// \param[in]     addrlen       length of \ref SOCKADDR structure.
/// \return      status information:
///              - BSD_SUCCESS          = Operation successful.
///              - BSD_ERROR_SOCKET     = Socket invalid or in invalid state.
///              - BSD_ERROR_PARAMETER  = Invalid or not supported parameters.
///              - BSD_ERROR_WOULDBLOCK = Function would block in non-blocking mode.
///              - BSD_ERROR_CLOSED     = Remote node has closed the connection.
///              - BSD_ERROR            = Error in underlying native socket.
extern int connect (int sock, SOCKADDR *addr, int addrlen);

/// \brief Send data on already connected socket.
/// \param[in]     sock          socket descriptor obtained with \ref socket.
/// \param[in]     buf           pointer to application data buffer to transmit.
/// \param[in]     len           length of data (in bytes).
/// \param[in]     flags         message flags:
///                              - MSG_DONTWAIT   = don't wait to send data,
///                              - 0              = for none.
/// \return      status information:
///              - Number of bytes sent or
///              - BSD_ERROR_SOCKET     = Socket invalid or in invalid state.
///              - BSD_ERROR_PARAMETER  = Invalid or not supported parameters.
///              - BSD_ERROR_WOULDBLOCK = Function would block in non-blocking mode.
///              - BSD_ERROR_CLOSED     = Remote node has closed the connection.
///              - BSD_ERROR            = Error in underlying native socket.
///              - BSD_ERROR_LOCKED     = Socket is locked by another task.
extern int send (int sock, const char *buf, int len, int flags);

/// \brief Send data to endpoint node.
/// \param[in]     sock          socket descriptor obtained with \ref socket.
/// \param[in]     buf           pointer to application data buffer to transmit.
/// \param[in]     len           length of data (in bytes).
/// \param[in]     flags         message flags:
///                              - MSG_DONTWAIT   = don't wait to send data,
///                              - 0              = for none.
/// \param[in]     to            structure containing remote IP address and port.
/// \param[in]     tolen         length of \ref SOCKADDR structure.
/// \return      status information:
///              - Number of bytes sent or
///              - BSD_ERROR_SOCKET     = Socket invalid or in invalid state.
///              - BSD_ERROR_PARAMETER  = Invalid or not supported parameters.
///              - BSD_ERROR_WOULDBLOCK = Function would block in non-blocking mode.
///              - BSD_ERROR_CLOSED     = Remote node has closed the connection.
///              - BSD_ERROR            = Error in underlying native socket.
///              - BSD_ERROR_LOCKED     = Socket is locked by another task.
/// \note Parameters \b to and \b tolen are ignored for SOCK_STREAM socket type.
extern int sendto (int sock, const char *buf, int len, int flags, SOCKADDR *to, int tolen);

/// \brief Receive data on already connected socket.
/// \param[in]     sock          socket descriptor obtained with \ref socket.
/// \param[out]    buf           pointer to application data buffer to store the data to.
/// \param[in]     len           size of application data buffer (in bytes).
/// \param[in]     flags         message flags: 
///                              - MSG_DONTWAIT   = don't wait for data,
///                              - MSG_PEEK       = peek at incoming data,
///                              - 0              = for none.
/// \return      status information:
///              - Number of bytes received or
///              - BSD_ERROR_SOCKET     = Socket invalid or in invalid state.
///              - BSD_ERROR_CLOSED     = Remote node has closed the connection.
///              - BSD_ERROR_WOULDBLOCK = Function would block in non-blocking mode.
///              - BSD_ERROR_TIMEOUT    = Wait for data timeout in blocking mode.
///              - BSD_ERROR_LOCKED     = Socket is locked by another task.
extern int recv (int sock, char *buf, int len, int flags);

/// \brief Receive data from endpoint node.
/// \param[in]     sock          socket descriptor obtained with \ref socket.
/// \param[out]    buf           pointer to application data buffer to store the data to.
/// \param[in]     len           size of application data buffer (in bytes).
/// \param[in]     flags         message flags: 
///                              - MSG_DONTWAIT   = don't wait for data,
///                              - MSG_PEEK       = peek at incoming data,
///                              - 0              = for none.
/// \param[out]    from          structure that will receive IP address and port number,
///                              or NULL for none
/// \param[out]    fromlen       length of \ref SOCKADDR structure.
/// \return      status information:
///              - Number of bytes received or
///              - BSD_ERROR_SOCKET     = Socket invalid or in invalid state.
///              - BSD_ERROR_CLOSED     = Remote node has closed the connection.
///              - BSD_ERROR_WOULDBLOCK = Function would block in non-blocking mode.
///              - BSD_ERROR_TIMEOUT    = Wait for data timeout in blocking mode.
///              - BSD_ERROR_LOCKED     = Socket is locked by another task.
/// \note Parameters \b from and \b fromlen are ignored for SOCK_STREAM socket type.
extern int recvfrom (int sock, char *buf, int len, int flags, SOCKADDR *from, int *fromlen);

/// \brief Close socket and release socket descriptor.
/// \param[in]     sock          socket descriptor obtained with \ref socket.
/// \return      status information:
///              - BSD_SUCCESS          = Operation successful.
///              - BSD_ERROR_SOCKET     = Socket invalid.
///              - BSD_ERROR_WOULDBLOCK = Function would block in non-blocking mode.
///              - BSD_ERROR_LOCKED     = Socket is locked by another task.
extern int closesocket (int sock);

/// \brief Retrieve IP address and port number of the endpoint node.
/// \param[in]     sock          socket descriptor obtained with \ref socket.
/// \param[out]    name          structure that will receive IP address and port number.
/// \param[out]    namelen       length of \ref SOCKADDR structure.
/// \return      status information:
///              - BSD_SUCCESS        = Operation successful.
///              - BSD_ERROR_SOCKET   = Socket invalid.
///              - BSD_ERROR          = Socket is not created.
extern int getpeername (int sock, SOCKADDR *name, int *namelen);

/// \brief Retrieve local IP address and port number.
/// \param[in]     sock          socket descriptor obtained with \ref socket.
/// \param[out]    name          structure that will receive IP address and port number.
/// \param[out]    namelen       length of \ref SOCKADDR structure.
/// \return      status information:
///              - BSD_SUCCESS        = Operation successful.
///              - BSD_ERROR_SOCKET   = Socket invalid.
///              - BSD_ERROR          = Socket is not created.
extern int getsockname (int sock, SOCKADDR *name, int *namelen);

/// \brief Control IO mode of a socket.
/// \param[in]     sock          socket descriptor obtained with \ref socket.
/// \param[in]     cmd           command to perform:
///                              - FIONBIO          = enable non-blocking mode.
///                              - FIO_DELAY_ACK    = enable delay-ack.
///                              - FIO_KEEP_ALIVE   = enable keep-alive mode.
///                              - FIO_FLOW_CTRL    = enable flow-control mode.
/// \param[in]     argp          command's parameter
/// \return      status information:
///              - BSD_SUCCESS         = Operation successful.
///              - BSD_ERROR_SOCKET    = Socket invalid.
///              - BSD_ERROR_PARAMETER = Invalid or not supported parameters.
///              - BSD_ERROR           = Socket is not created.
extern int ioctlsocket (int sock, long cmd, unsigned long *argp);

/// \brief Retrieve host IP address from host name.
/// \param[in]     name          host name.
/// \param[out]    err           returned error code:
///                              - BSD_SUCCESS          = Operation successful.
///                              - BSD_ERROR_LOCKED     = Function is locked by another task.
///                              - BSD_ERROR_WOULDBLOCK = Function would block in non-blocking mode.
///                              - BSD_ERROR_PARAMETER  = Invalid parameter.
///                              - BSD_ERROR_NONAME     = Host name does not exist.
///                              - BSD_ERROR_TIMEOUT    = DNS server response timeout.
///                              - BSD_ERROR            = Generic error.
/// \return      status information:
///              - \ref HOSTENT result structure or
///              - NULL in case of error.
extern HOSTENT *gethostbyname (const char *name, int *err);
#endif /* RTE_Network_Socket_BSD */

//  ==== Ethernet Interface User API ====

/// \brief Determine whether the ARP table has MAC address resolved for requested IP address.
/// \param[in]     if_num        interface number.
/// \param[in]     ip_addr       requested IP address.
/// \param[in]     type          address cache type
/// \return      status code that indicates the execution status of the function as defined with \ref netStatus.
extern netStatus arp_cache_ip (uint32_t if_num, const uint8_t *ip_addr, arpCacheType type);

/// \brief Determine whether the ARP table has IP address resolved for requested MAC address.
/// \param[in]     if_num        interface number.
/// \param[in]     mac_addr      requested MAC address.
/// \return      status code that indicates the execution status of the function as defined with \ref netStatus.
extern netStatus arp_cache_mac (uint32_t if_num, const uint8_t *mac_addr);

/// \brief Get IP address from the ARP cache.
/// \param[in]     mac_addr      requested MAC address.
/// \param[out]    ip_addr       resolved IP address.
/// \return      status code that indicates the execution status of the function as defined with \ref netStatus.
extern netStatus arp_get_ip (const uint8_t *mac_addr, uint8_t *ip_addr);

/// \brief Get MAC address from the ARP cache.
/// \param[in]     ip_addr       requested IP address.
/// \param[out]    mac_addr      resolved MAC address.
/// \return      status code that indicates the execution status of the function as defined with \ref netStatus.
extern netStatus arp_get_mac (const uint8_t *ip_addr, uint8_t *mac_addr);

/// \brief Disable Dynamic Host Configuration at runtime.
/// \param[in]     if_num        interface number.
/// \return      status code that indicates the execution status of the function as defined with \ref netStatus.
extern netStatus dhcp_disable (uint32_t if_num);

/// \brief Join this host to a host group specified with IP address.
/// \param[in]     if_num        interface number.
/// \param[in]     ip_addr       group IP address.
/// \return      status code that indicates the execution status of the function as defined with \ref netStatus.
extern netStatus igmp_join (uint32_t if_num, const uint8_t *ip_addr);

/// \brief Leave a host group specified with IP address.
/// \param[in]     if_num        interface number.
/// \param[in]     ip_addr       group IP address.
/// \return      status code that indicates the execution status of the function as defined with \ref netStatus.
extern netStatus igmp_leave (uint32_t if_num, const uint8_t *ip_addr);

//  ==== Ethernet Interface Callbacks ====

/// \brief Notify the user of Ethernet link state change event.
/// \param[in]     if_num        interface number.
/// \param[in]     event         Ethernet link state event as defined in \ref ethLinkEvent.
/// \return        none.
extern void eth_link_notify (uint32_t if_num, ethLinkEvent event);

/// \brief Notify the user of DHCP event or extended DHCP option.
/// \param[in]     if_num        interface number.
/// \param[in]     opt           DHCP option as defined in \ref dhcpClientOption.
/// \param[in]     val           pointer to the option value.
/// \param[in]     len           length of the option value in bytes.
/// \return        none.
extern void dhcp_client_notify (uint32_t if_num, dhcpClientOption opt, const uint8_t *val, uint32_t len);

//  ==== PPP Interface User API ====

/// \brief Start PPP interface to accept incoming PPP connection.
/// \param[in]     username      remote username for authentication.
/// \param[in]     password      remote password for authentication.
/// \return      status code that indicates the execution status of the function as defined with \ref netStatus.
/// \note Both parameters are null-terminated strings.
extern netStatus ppp_listen (const char *username, const char *password);

/// \brief Start a dial-up connection to remote PPP server.
/// \param[in]     dial_num      phone number of remote PPP server.
/// \param[in]     username      username for authentication.
/// \param[in]     password      password for authentication.
/// \return      status code that indicates the execution status of the function as defined with \ref netStatus.
/// \note All parameters are null-terminated strings.
extern netStatus ppp_connect (const char *dial_num, const char *username, const char *password);

/// \brief Disconnect PPP link between two modems.
/// \return      status code that indicates the execution status of the function as defined with \ref netStatus.
extern netStatus ppp_close (void);

/// \brief Determine the state of PPP link.
/// \return      link state:
///              - true        = Link is up, IP frames can be exchanged.
///              - false       = Link is down.
extern bool ppp_is_up (void);

//  ==== SLIP Interface User API ====

/// \brief Start SLIP interface to accept incoming SLIP connections.
/// \return      status code that indicates the execution status of the function as defined with \ref netStatus.
extern netStatus slip_listen (void);

/// \brief Start a dial-up connection to remote SLIP server.
/// \param[in]     dial_num      phone number of remote SLIP server.
/// \return      status code that indicates the execution status of the function as defined with \ref netStatus.
/// \note Parameter is null-terminated string.
extern netStatus slip_connect (const char *dial_num);

/// \brief Disconnect SLIP link between two modems.
/// \return      status code that indicates the execution status of the function as defined with \ref netStatus.
extern netStatus slip_close (void);

/// \brief Determine the state of SLIP link.
/// \return      link state:
///              - true        = Link is up, IP frames can be exchanged.
///              - false       = Link is down.
extern bool slip_is_up (void);

//  ==== ICMP User API ====

/// \brief Start ICMP ping process.
/// \param[in]     ip_addr       IP address of the remote host.
/// \param[in]     cb_func       callback function to call, when ping session ends.
/// \return      status code that indicates the execution status of the function as defined with \ref netStatus.
extern netStatus icmp_ping (const uint8_t *ip_addr, net_icmp_cb_t cb_func);

//  ==== DNS Client User API ====

/// \brief Resolve IP address of a host from a hostname.
/// \param[in]     name          hostname, a null-terminated string.
/// \param[in]     cb_func       callback function to call, when DNS session ends.
/// \return      status code that indicates the execution status of the function as defined with \ref netStatus.
extern netStatus get_host_by_name (const char *name, net_dns_client_cb_t cb_func);

//  ==== FTP Server Access Interface ====

/// \brief Accept or deny connection from remote FTP client.
/// \param[in]     ip_addr       IP address of the remote FTP client.
/// \param[in]     port          port number of the remote FTP client.
/// \return
///              - true        = Connection from the remote client is allowed.
///              - false       = Connection is denied.
extern bool ftp_accept_client (const uint8_t *ip_addr, uint16_t port);

//  ==== FTP Server Multi-User Interface ====

/// \brief Check if an user account exist in the user database.
/// \param[in]     username      pointer to username.
/// \return      status information:
///              - User identification number or
///              - 0 if the user is not existing.
extern uint8_t ftp_check_username (const char *username);

/// \brief Check user account password in the user database.
/// \param[in]     user_id       user identification number.
/// \param[in]     password      pointer to password.
/// \return
///              - true        = password accepted.
///              - false       = invalid password.
extern bool ftp_check_password (uint8_t user_id, const char *password);

/// \brief Check if remote user is allowed to access a file on FTP server.
/// \param[in]     user_id       user identification number.
/// \param[in]     fname         name of a file to access.
/// \param[in]     mode          access mode:
///                              - 0 = Read a file.
///                              - 1 = Write a file.
///                              - 2 = Create or remove a directory.
///                              - 3 = List file directory.
/// \return
///              - true        = File access is allowed.
///              - false       = File access is denied.
extern bool ftp_file_access (uint8_t user_id, const char *fname, uint8_t mode);

/// \brief Retrieve the user identification.
/// \return        user identification number (0 = system administrator).
extern uint8_t ftp_get_user_id (void);

//  ==== FTP Server User Callbacks ====

/// \brief Notify the user application about events in FTP server service.
/// \param[in]     event         FTP Server notification event
/// \return        none.
/// \note This function is called by the system to inform about events.
extern void ftp_server_notify (ftpServerEvent event);

//  ==== FTP Server File System Interface ====

/// \brief Open a file for reading or writing in FTP server.
/// \param[in]     fname         name of the file to open.
/// \param[in]     mode          type of access:
///                              - "rb" = opens a file for reading.
///                              - "wb" = opens a file for writing.
/// \return      status information:
///              - Pointer to an open file or
///              - NULL in case of an error.
extern void *ftp_server_fopen (const char *fname, const char *mode);

/// \brief Close a file previously open in FTP server.
/// \param[in]     file          pointer to the file to close.
/// \return        none.
extern void ftp_server_fclose (void *file);

/// \brief Read block of data from a file in FTP server.
/// \param[in]     file          pointer to the file to read from.
/// \param[out]    buf           block of memory to write data to.
/// \param[in]     len           length of data to read in bytes.
/// \return        number of bytes successfully read.
extern uint32_t ftp_server_fread (void *file, uint8_t *buf, uint32_t len);

/// \brief Write block of data to a file in FTP server.
/// \param[in]     file          pointer to the file to write to.
/// \param[in]     buf           block of memory to be written.
/// \param[in]     len           length of data to write in bytes.
/// \return        number of bytes successfully written.
extern uint32_t ftp_server_fwrite (void *file, const uint8_t *buf, uint32_t len);

/// \brief Delete a file in FTP server.
/// \param[in]     fname         name of the file to delete.
/// \return
///              - true        = File successfully deleted.
///              - false       = Failed to delete a file.
extern bool ftp_server_fdelete (const char *fname);

/// \brief Rename a file or directory in FTP server.
/// \param[in]     fname         old name to rename from.
/// \param[in]     newname       new name to rename to.
/// \return
///              - true        = File or directory successfully renamed.
///              - false       = Failed to rename a file or directory.
extern bool ftp_server_frename (const char *fname, const char *newname);

/// \brief Make a new directory in FTP server.
/// \param[in]     path          directory path to create.
/// \return
///              - true        = Directory successfully created.
///              - false       = Failed to create a directory.
extern bool ftp_server_mkdir (const char *path);

/// \brief Remove an empty directory in FTP server.
/// \param[in]     path          directory path to remove.
/// \return
///              - true        = Directory successfully removed.
///              - false       = Failed to remove a directory.
extern bool ftp_server_rmdir (const char *path);

/// \brief Search the file system directory for matching files.
/// \param[in]     code          request type code:
///                              - 0 = Retrieve file size.
///                              - 1 = Retrieve last-modified time of a file.
///                              - 2 = List files by name, first call.
///                              - 3 = List files in extended format, first call.
///                              - 4 = List files by name, subsequent call.
///                              - 5 = List files in extended format, subsequent call.
/// \param[out]    buf           output buffer to write the listing to.
/// \param[in]     buflen        length of the output buffer in bytes.
/// \param[in]     mask          file mask filter.
/// \return        number of bytes written to output buffer.
extern uint32_t ftp_server_ffind (uint8_t code, char *buf, uint32_t buflen, const char *mask);

//  ==== FTP Client User API ====

/// \brief Start FTP client file operation session.
/// \param[in]     ip_addr       IP address of remote FTP server.
/// \param[in]     port          port number of FTP server.
/// \param[in]     command       FTP command to perform.
/// \return      status code that indicates the execution status of the function as defined with \ref netStatus.
extern netStatus ftp_client_connect (const uint8_t *ip_addr, uint16_t port, ftpCommand command);

//  ==== FTP Client User Callbacks ====

/// \brief Request parameters for FTP client session.
/// \param[in]     request       request code.
/// \param[out]    buf           output buffer to write the data to.
/// \param[in]     len           length of the output buffer in bytes.
/// \return        number of bytes written to output buffer.
extern uint32_t ftp_client_request (ftpClientRequest request, char *buf, uint32_t len);

/// \brief Notify the user application when FTP client operation ends.
/// \param[in]     event         FTP client notification event
/// \return        none.
/// \note This function is called by the system to inform about events.
extern void ftp_client_notify (ftpClientEvent event);

//  ==== FTP Client File System Interface ====

/// \brief Open local file for reading or writing in FTP client.
/// \param[in]     fname         name of the file to open.
/// \param[in]     mode          type of access:
///                              - "rb" = opens a file for reading.
///                              - "wb" = opens a file for writing.
/// \return      status information:
///              - Pointer to an open file or
///              - NULL in case of an error.
extern void *ftp_client_fopen (const char *fname, const char *mode);

/// \brief Close local file previously open in FTP client.
/// \param[in]     file          pointer to the file to close.
/// \return        none.
extern void ftp_client_fclose (void *file);

/// \brief Read block of data from local file in FTP client.
/// \param[in]     file          pointer to the file to read from.
/// \param[out]    buf           block of memory to write data to.
/// \param[in]     len           length of data to read in bytes.
/// \return        number of bytes successfully read.
extern uint32_t ftp_client_fread (void *file, uint8_t *buf, uint32_t len);

/// \brief Write block of data to local file in FTP client.
/// \param[in]     file          pointer to the file to write to.
/// \param[in]     buf           block of memory to be written.
/// \param[in]     len           length of data to write in bytes.
/// \return        number of bytes successfully written.
/// \note FTP client stops writing and closes the file, if the return value is less than len bytes.
extern uint32_t ftp_client_fwrite (void *file, const uint8_t *buf, uint32_t len);

//  ==== TFTP Server Access Interface ====

/// \brief Accept or deny connection from remote TFTP client.
/// \param[in]     ip_addr       IP address of the remote TFTP client.
/// \param[in]     port          port number of the remote TFTP client.
/// \return
///              - true        = Connection from the remote client is allowed.
///              - false       = Connection is denied.
extern bool tftp_accept_client (const uint8_t *ip_addr, uint16_t port);

//  ==== TFTP Server File System Interface ====

/// \brief Open a file for reading or writing in TFTP server.
/// \param[in]     fname         name of the file to open.
/// \param[in]     mode          type of access:
///                              - "rb" = opens a file for reading.
///                              - "wb" = opens a file for writing.
/// \return      status information:
///              - Pointer to an open file or
///              - NULL in case of an error.
extern void *tftp_server_fopen (const char *fname, const char *mode);

/// \brief Close a file previously open in TFTP server.
/// \param[in]     file          pointer to the file to close.
/// \return        none.
extern void tftp_server_fclose (void *file);

/// \brief Read block of data from a file in TFTP server.
/// \param[in]     file          pointer to the file to read from.
/// \param[out]    buf           block of memory to write data to.
/// \param[in]     len           length of data to read in bytes.
/// \return        number of bytes successfully read.
extern uint32_t tftp_server_fread (void *file, uint8_t *buf, uint32_t len);

/// \brief Write block of data to a file in TFTP server.
/// \param[in]     file          pointer to the file to write to.
/// \param[in]     buf           block of memory to be written.
/// \param[in]     len           length of data to write in bytes.
/// \return        number of bytes successfully written.
extern uint32_t tftp_server_fwrite (void *file, const uint8_t *buf, uint32_t len);

//  ==== TFTP Client User API ====

/// \brief Put a file to remote TFTP server.
/// \param[in]     ip_addr       IP address of remote TFTP server.
/// \param[in]     port          port number of TFTP server.
/// \param[in]     src           local file name, a null-terminated string.
/// \param[in]     dst           remote file name, a null-terminated string.
/// \return      status code that indicates the execution status of the function as defined with \ref netStatus.
extern netStatus tftp_client_put (const uint8_t *ip_addr, uint16_t port, const char *src, const char *dst);

/// \brief Retrieve a file from remote TFTP server.
/// \param[in]     ip_addr       IP address of remote TFTP server.
/// \param[in]     port          port number of TFTP server.
/// \param[in]     src           remote file name, a null-terminated string.
/// \param[in]     dst           local file name, a null-terminated string.
/// \return      status code that indicates the execution status of the function as defined with \ref netStatus.
extern netStatus tftp_client_get (const uint8_t *ip_addr, uint16_t port, const char *src, const char *dst);

//  ==== TFTP Client User Callbacks ====

/// \brief Notify the user application when TFTP client operation ends.
/// \param[in]     event         TFTP client notification event.
/// \return        none.
/// \note This function is called by the system to inform about events.
extern void tftp_client_notify (tftpClientEvent event);

//  ==== TFTP Client File System Interface ====

/// \brief Open local file for reading or writing in TFTP client.
/// \param[in]     fname         name of the file to open.
/// \param[in]     mode          type of access:
///                              - "rb" = opens a file for reading.
///                              - "wb" = opens a file for writing.
/// \return      status information:
///              - Pointer to an open file or
///              - NULL in case of an error.
extern void *tftp_client_fopen (const char *fname, const char *mode);

/// \brief Close local file previously open in TFTP client.
/// \param[in]     file          pointer to the file to close.
/// \return        none.
extern void tftp_client_fclose (void *file);

/// \brief Read block of data from local file in TFTP client.
/// \param[in]     file          pointer to the file to read from.
/// \param[out]    buf           block of memory to write data to.
/// \param[in]     len           length of data to read in bytes.
/// \return        number of bytes successfully read.
extern uint32_t tftp_client_fread (void *file, uint8_t *buf, uint32_t len);

/// \brief Write block of data to local file in TFTP client.
/// \param[in]     file          pointer to the file to write to.
/// \param[in]     buf           block of memory to be written.
/// \param[in]     len           length of data to write in bytes.
/// \return        number of bytes successfully written.
extern uint32_t tftp_client_fwrite (void *file, const uint8_t *buf, uint32_t len);

//  ==== Telnet Server User API ====

/// \brief Set a delay between two consecutive calls to telnet_server_process function.
/// \param[in]     delay         time period to wait in number of 100ms ticks.
/// \return      status code that indicates the execution status of the function as defined with \ref netStatus.
extern netStatus telnet_server_set_delay (uint32_t delay);

/// \brief Get IP and MAC address of connected remote machine.
/// \param[out]    ip_addr       pointer to IP address.
/// \param[out]    mac_addr      pointer to MAC address.
/// \return      status code that indicates the execution status of the function as defined with \ref netStatus.
extern netStatus telnet_server_get_client (uint8_t *ip_addr, uint8_t *mac_addr);

/// \brief Get current session number of Telnet server.
/// \return        current session number.
extern int32_t telnet_server_get_session (void);

/// \brief Check command string for a command.
/// \param[in]     cmd           pointer to command string from Telnet client.
/// \param[in]     user_cmd      user command to check for (in upper case).
/// \return
///              - true        = Command found in command string.
///              - false       = Command not found.
extern bool telnet_check_command (const char *cmd, const char *user_cmd);

//  ==== Telnet Server User Callbacks ====

/// \brief Request message for Telnet server session.
/// \param[in]     msg           code of requested message.
/// \param[out]    buf           output buffer to write the message to.
/// \param[in]     len           length of the output buffer in bytes.
/// \return        number of bytes written to output buffer.
extern uint32_t telnet_server_message (telnetServerMessage msg, char *buf, uint32_t len);

/// \brief Process and execute a command requested by the Telnet client.
/// \param[in]     cmd           pointer to command string from Telnet client.
/// \param[out]    buf           output buffer to write the return message to.
/// \param[in]     buflen        length of the output buffer in bytes.
/// \param[in,out] pvar          pointer to a session's local buffer of 4 bytes.
///                              - 1st call = set to 0,
///                              - 2nd call = not altered by the system,
///                              - 3rd call = not altered by the system, etc.
/// \return        number of bytes written to output buffer.
///                - result len | (1u<<31) = repeat flag, the system calls this function
///                                          again for the same command string.
///                - result len | (1u<<30) = disconnect flag, the system disconnects
///                                          the Telnet client.
extern uint32_t telnet_server_process (const char *cmd, char *buf, uint32_t buflen, uint32_t *pvar);

/// \brief Poll the upper-layer user application for unsolicited messages.
/// \param[in]     session       session identification, when multiple connections are active.
/// \return
///              - true        = User application has a pending message to send.
///              - false       = No messages pending to send exist.
extern bool telnet_server_message_poll (int32_t session);

//  ==== Telnet Server Access Interface ====

/// \brief Accept or deny connection from remote Telnet client.
/// \param[in]     ip_addr       IP address of the remote Telnet client.
/// \param[in]     port          port number of the remote Telnet client.
/// \return
///              - true        = Connection from the remote client is allowed.
///              - false       = Connection is denied.
extern bool telnet_accept_client (const uint8_t *ip_addr, uint16_t port);

//  ==== Telnet Server Multi-User Interface ====

/// \brief Check if an user account exist in the user database.
/// \param[in]     username      pointer to username.
/// \return      status information:
///              - User identification number or
///              - 0 if the user is not existing.
extern uint8_t telnet_check_username (const char *username);

/// \brief Check user account password in the user database.
/// \param[in]     user_id       user identification number.
/// \param[in]     password      pointer to password.
/// \return
///              - true        = password accepted.
///              - false       = invalid password.
extern bool telnet_check_password (uint8_t user_id, const char *password);

/// \brief Retrieve the user identification.
/// \return        user identification number (0 = system administrator).
extern uint8_t telnet_get_user_id (void);

//  ==== HTTP Utility functions ====

/// \brief Process environment variables and convert to ANSI format.
/// \param[in]     env           pointer to environment variables.
/// \param[out]    ansi          output buffer to write converted variable to.
/// \param[in]     maxlen        maximum length of environment variable.
/// \return      status information:
///              - pointer to the remaining environment variables to process or
///              - NULL if there are no more environment variables to process.
extern const char *http_get_env_var (const char *env, char *ansi, uint32_t maxlen);

/// \brief Convert generic time to UTC time format.
/// \param[in]     hr            hours   [0..23].
/// \param[in]     min           minutes [0..59].
/// \param[in]     sec           seconds [0..59].
/// \param[in]     day           day     [1..31].
/// \param[in]     mon           month   [1..12].
/// \param[in]     year          year    [1980..2107].
/// \return        time converted to UTC format.
extern uint32_t http_utc_time (uint8_t hr, uint8_t min, uint8_t sec, 
                               uint8_t day, uint8_t mon, uint16_t year);

//  ==== HTTP Server User API ====

/// \brief Get IP and MAC address of connected remote machine.
/// \param[out]    ip_addr       pointer to IP address.
/// \param[out]    mac_addr      pointer to MAC address.
/// \return      status code that indicates the execution status of the function as defined with \ref netStatus.
extern netStatus http_server_get_client (uint8_t *ip_addr, uint8_t *mac_addr);

/// \brief Get current session number of HTTP server.
/// \return        current session number.
extern int32_t http_server_get_session (void);

/// \brief Retrieve the preferred language setting from the browser.
/// \return        pointer to the language code, a null-terminated string.
extern const char *http_server_get_lang (void);

/// \brief Get Content-Type HTML header, received in XML post request.
/// \return        pointer to content type header, a null-terminated string.
extern const char *http_server_get_content_type (void);

//  ==== HTTP Server CGI ====

/// \brief Process query string received by GET request.
/// \param[in]     qstr          pointer to the query string.
/// \return        none.
extern void cgi_process_query (const char *qstr);

/// \brief Process data received by POST request.
/// \param[in]     code          callback context:
///                              - 0 = www-url-encoded form data,
///                              - 1 = filename for file upload (null-terminated string),
///                              - 2 = file upload raw data,
///                              - 3 = end of file upload (file close requested),
///                              - 4 = any other type of POST data (single or last stream),
///                              - 5 = the same as 4, but with more data to follow.
/// \param[in]     data          pointer to POST data.
/// \param[in]     len           length of POST data.
/// \return        none.
extern void cgi_process_data (uint8_t code, const char *data, uint32_t len);

/// \brief Generate dynamic web data from a script line.
/// \param[in]     env           environment string.
/// \param[out]    buf           output data buffer.
/// \param[in]     buflen        size of output buffer (from 536 to 1460 bytes).
/// \param[in,out] pcgi          pointer to a session's local buffer of 4 bytes.
///                              - 1st call = set to 0,
///                              - 2nd call = not altered by the system,
///                              - 3rd call = not altered by the system, etc.
/// \return        number of bytes written to output buffer.
///                - result len | (1U<<31) = repeat flag, the system calls this function
///                                          again for the same script line.
///                - result len | (1U<<30) = force transmit flag, the system transmits
///                                          current packet immediately.
extern uint32_t cgi_script (const char *env, char *buf, uint32_t buflen, uint32_t *pcgi);

/// \brief Add custom MIME type for unsupported file types.
/// \param[in]     file_ext      filename extension, a null-terminated string.
/// \return        pointer to user defined Content-Type
///                or NULL for unknown type.
extern const char *cgi_content_type (const char *file_ext);

/// \brief Override default Content-Type for CGX script files.
/// \return        pointer to user defined Content-Type.
extern const char *cgx_content_type (void);

/// \brief Override default character encoding in html documents.
/// \return        pointer to user defined character set type.
extern const char *http_encoding (void);

//  ==== HTTP Server Access Interface ====

/// \brief Accept or deny connection from remote HTTP client.
/// \param[in]     ip_addr       IP address of the remote HTTP client.
/// \param[in]     port          port number of the remote HTTP client.
/// \return
///              - true        = Connection from the remote client is allowed.
///              - false       = Connection is denied.
extern bool http_accept_client (const uint8_t *ip_addr, uint16_t port);

//  ==== HTTP Server Multi-User Interface ====

/// \brief Check if an user account exist in the user database.
/// \param[in]     username      pointer to username.
/// \param[in]     password      pointer to password.
/// \return      status information:
///              - User identification number or
///              - 0 if the user is not existing.
extern uint8_t http_check_account (const char *username, const char *password);

/// \brief Check if remote user is allowed to access a file on HTTP server.
/// \param[in]     user_id       user identification number.
/// \param[in]     fname         name of a file to access.
/// \return
///              - true        = File access is allowed.
///              - false       = File access is denied.
extern bool http_file_access (uint8_t user_id, const char *fname);

/// \brief Retrieve the user identification.
/// \return        user identification number (0 = system administrator).
extern uint8_t http_get_user_id (void);

//  ==== HTTP Server File System Interface ====

/// \brief Open a file for reading in HTTP server.
/// \param[in]     fname         name of the file to open.
/// \return      status information:
///              - Pointer to an open file or
///              - NULL in case of an error.
extern void *http_server_fopen (const char *fname);

/// \brief Close a file previously open in HTTP server.
/// \param[in]     file          pointer to the file to close.
/// \return        none.
extern void http_server_fclose (void *file);

/// \brief Read block of data from a file in HTTP server.
/// \param[in]     file          pointer to the file to read from.
/// \param[out]    buf           block of memory to write data to.
/// \param[in]     len           length of data to read in bytes.
/// \return        number of bytes successfully read.
extern uint32_t http_server_fread (void *file, uint8_t *buf, uint32_t len);

/// \brief Read a string from a file in HTTP server.
/// \param[in]     file          pointer to the file to read from.
/// \param[out]    buf           output buffer to write data to.
/// \param[in]     size          size of output buffer.
/// \return      status information:
///              - Pointer to string on success
///              - NULL in case of an error.
extern char *http_server_fgets (void *file, char *buf, uint32_t size);

/// \brief Retrieve last modification time of a file.
/// \param[in]     fname         name of the file.
/// \return        last modification time in UTC format.
extern uint32_t http_server_ftime (const char *fname);

//  ==== SMTP Client User API ====

/// \brief Start SMTP client to send an email.
/// \param[in]     ip_addr       IP address of the SMTP server.
/// \param[in]     port          port number of SMTP server.
/// \return      status code that indicates the execution status of the function as defined with \ref netStatus.
extern netStatus smtp_client_connect (const uint8_t *ip_addr, uint16_t port);

//  ==== SMTP Client User Callbacks ====

/// \brief Request parameters for SMTP client session.
/// \param[in]     request       request code.
/// \param[out]    buf           output buffer to write the data to.
/// \param[in]     buflen        length of the output buffer in bytes.
/// \param[in,out] pvar          pointer to a session's local buffer of 4 bytes.
///                              - 1st call = set to 0,
///                              - 2nd call = not altered by the system,
///                              - 3rd call = not altered by the system, etc.
/// \return        number of bytes written to output buffer.
extern uint32_t smtp_client_request (smtpClientRequest request, char *buf, uint32_t buflen, uint32_t *pvar);

/// \brief Notify the user application when SMTP client operation ends.
/// \param[in]     event         SMTP client notification event
/// \return        none.
/// \note This function is called by the system to inform about events.
extern void smtp_client_notify (smtpClientEvent event);

/// \brief Accept or deny authentication requested by SMTP server.
/// \param[in]     ip_addr       IP address of the remote SMTP server.
/// \return
///              - true        = Authentication is accepted.
///              - false       = Authentication is denied.
extern bool smtp_client_accept_authentication (const uint8_t *ip_addr);

//  ==== SNTP Client User API ====

/// \brief Determine current time from NTP or SNTP time server.
/// \param[in]     ip_addr       IP address of NTP or SNTP server.
/// \param[in]     cb_func       callback function to call, when the session ends.
/// \return      status code that indicates the execution status of the function as defined with \ref netStatus.
extern netStatus sntp_get_time (const uint8_t *ip_addr, net_sntp_client_cb_t cb_func);

//  ==== SNMP Agent User API ====

/// \brief Send a trap message to the Trap Manager.
/// \param[in]     ip_addr       IP address of the Trap server or
///                                NULL to use Trap server IP address from system configuration.
/// \param[in]     generic       generic trap type:
///                              - 0  = ColdStart trap
///                              - 1  = WarmStart trap
///                              - 2  = LinkDown trap
///                              - 3  = LinkUp trap
///                              - 4  = AuthenticationFailure trap
///                              - 5  = EgpNeighborLoss trap
///                              - 6  = EnterpriseSpecific trap
/// \param[in]     specific      specific trap type for generic enterpriseSpecific trap:
///                              - must be set to 0 for generic traps 0 ... 5
/// \param[in]     obj_list      object list included in trap message.
/// \return      status code that indicates the execution status of the function as defined with \ref netStatus.
extern netStatus snmp_trap (const uint8_t *ip_addr, uint8_t generic, uint8_t specific, const uint16_t *obj_list);

/// \brief Change SNMP community to a new community.
/// \param[in]     community     new community, a null-terminated string.
/// \return      status code that indicates the execution status of the function as defined with \ref netStatus.
extern netStatus snmp_set_community (const char *community);

// ==== Network Address Conversion ====

/// \brief Convert an IPv4 Network address into an ASCII string.
/// \param[in]     ip4_addr      IPv4 address.
/// \return      pointer to a static buffer containing address string in dotted-decimal notation.
extern const char *ip4_ntoa (const uint8_t *ip4_addr);

/// \brief Convert a string containing an IPv4 address into a Network address.
/// \param[in]     cp            address string in dotted-decimal notation.
/// \param[out]    ip4_addr      IPv4 address.
/// \return
///              - true        = Conversion successful.
///              - false       = Conversion failed.
extern bool ip4_aton (const char *cp, uint8_t *ip4_addr);

/// \brief Convert a MAC address into an ASCII string.
/// \param[in]     mac_addr      MAC address.
/// \return      pointer to a static buffer containing address string in hyphen MAC-address notation.
extern const char *mac_ntoa (const uint8_t *mac_addr);

/// \brief Convert a string containing a MAC address into a binary MAC address.
/// \param[in]     cp            address string in hyphen MAC-address notation.
/// \param[out]    mac_addr      MAC address.
/// \return
///              - true        = Conversion successful.
///              - false       = Conversion failed.
extern bool mac_aton (const char *cp, uint8_t *mac_addr);

#ifdef __cplusplus
}
#endif

#endif /* __RL_NET_H */

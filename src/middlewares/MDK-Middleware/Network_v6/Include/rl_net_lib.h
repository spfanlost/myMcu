/*------------------------------------------------------------------------------
 * MDK Middleware - Component ::Network
 * Copyright (c) 2004-2019 Arm Limited (or its affiliates). All rights reserved.
 *------------------------------------------------------------------------------
 * Name:    rl_net_lib.h
 * Purpose: Network Library Definitions
 * Rev.:    V6.7.0
 *----------------------------------------------------------------------------*/

#ifndef __RL_NET_LIB_H
#define __RL_NET_LIB_H

#include "Driver_USART.h"
#include "Driver_ETH_MAC.h"
#include "Driver_ETH_PHY.h"
#include "net_modem.h"
#include "rl_net.h"

#ifdef __clang__
  #pragma clang diagnostic ignored "-Wpadded"
  #pragma clang diagnostic ignored "-Wmissing-variable-declarations"
#endif

/// General definitions
#define NET_HEADER_LEN      4           ///< Network frame header length
#define PHY_HEADER_LEN      14          ///< Physical/ethernet header length
#define ETH_FRAME_MTU       1514        ///< Ethernet frame max. transfer unit
#define NET_PASSWORD_SIZE   20          ///< Authentication password buffer size

/// Debug process definitions
#ifdef RTE_Network_Debug
#define NET_DYN_MEMORY      0           ///< Dynamic memory module
#define NET_ETH_LINK        1           ///< Ethernet interface
#define NET_PPP_LINK        2           ///< PPP interface
#define NET_SLIP_LINK       3           ///< SLIP interface
#define NET_ARP_CACHE       4           ///< Ethernet ARP cache module
#define NET_IP4_LHOST       5           ///< IPv4 localhost module
#define NET_ICMP_CONTROL    6           ///< ICMP control service
#define NET_IGMP_HOST       7           ///< IGMP host module
#define NET_UDP_SOCKET      8           ///< UDP socket module
#define NET_TCP_SOCKET      9           ///< TCP socket module
#define NET_NBNS_CLIENT     10          ///< NBNS client service
#define NET_DHCP_CLIENT     11          ///< DHCP client service
#define NET_DNS_CLIENT      12          ///< DNS client service
#define NET_SNMP_AGENT      13          ///< SNMP agent module
#define NET_BSD_SOCKET      14          ///< BSD socket module
#define NET_HTTP_SERVER     15          ///< HTTP server service
#define NET_FTP_SERVER      16          ///< FTP server service
#define NET_FTP_CLIENT      17          ///< FTP client service
#define NET_TELNET_SERVER   18          ///< Telnet server service
#define NET_TFTP_SERVER     19          ///< TFTP server service
#define NET_TFTP_CLIENT     20          ///< TFTP client service
#define NET_SMTP_CLIENT     21          ///< SMTP client service
#define NET_SNTP_CLIENT     22          ///< SNTP client service
#endif /* RTE_Network_Debug */

/// Telnet definitions
#define TNET_LBUFSZ         96          ///< Command Line buffer size in bytes
#define TNET_HISTSZ         128         ///< Command History buffer size in bytes
#define TNET_FIFOSZ         128         ///< Input character Fifo buffer in bytes

#ifdef __cplusplus
extern "C"  {
#endif

/// System Error codes
typedef enum error_code {
  ERR_MEM_ALLOC,                        ///< Alloc memory failed, out of memory
  ERR_MEM_FREE,                         ///< Free memory failed, memory slot invalid
  ERR_MEM_CORRUPT,                      ///< Memory corruption detected
  ERR_MEM_LOCK,                         ///< Locked memory function re-entered error
  ERR_UDP_ALLOC,                        ///< No free UDP sockets available
  ERR_TCP_ALLOC,                        ///< No free TCP sockets available
  ERR_TCP_STATE                         ///< TCP socket in undefined state
} ERROR_CODE;

/// Local Machine info
typedef struct localm {
  uint8_t IpAddr[IP4_ADDR_LEN];         ///< Local IP Address
  uint8_t DefGW[IP4_ADDR_LEN];          ///< Default GateWay
  uint8_t NetMask[IP4_ADDR_LEN];        ///< Net Mask
  uint8_t PriDNS[IP4_ADDR_LEN];         ///< Primary DNS Server
  uint8_t SecDNS[IP4_ADDR_LEN];         ///< Secondary DNS Server
} LOCALM;

/// System frame buffer structure
typedef struct net_frame {
  uint16_t length;                      ///< Total Length of data in frame
  uint16_t index;                       ///< Buffer Position Index
  uint8_t  data[1];                     ///< Buffer data (protocol headers + data)
} NET_FRAME;

/// ARP Cache Entry info
typedef struct arp_info {
  uint8_t State;                        ///< ARP Cache entry current state
  uint8_t Type;                         ///< Cache Entry type
  uint8_t Retries;                      ///< Number of Retries left
  uint8_t Tout;                         ///< Cache Entry Timeout
  uint8_t HwAdr[ETH_ADDR_LEN];          ///< Ethernet Hardware Address
  uint8_t IpAdr[IP4_ADDR_LEN];          ///< Ethernet IP Address
} ARP_INFO;

/// IGMP Group info
typedef struct igmp_info {
  uint8_t State;                        ///< Group membership current state
  uint8_t Tout;                         ///< Timeout Timer for sending reports
  uint8_t Flags;                        ///< State machine flags
  uint8_t GrpIpAdr[IP4_ADDR_LEN];       ///< Group IP address
} IGMP_INFO;

/// UDP Socket info
typedef struct udp_info {
  uint8_t  State;                       ///< Socket entry current state
  uint8_t  Opt;                         ///< Socket options
  uint8_t  Flags;                       ///< State machine flags
  uint8_t  Tos;                         ///< Type of Service allocated
  uint16_t LocPort;                     ///< Local port of socket
  uint8_t  McastTtl;                    ///< MultiCast Time To Live
  net_udp_cb_t cb_func;                 ///< Application event CallBack function
} UDP_INFO;

/// TCP Socket info
typedef struct tcp_info {
  tcpState State;                       ///< Socket entry current state
  uint8_t  Type;                        ///< Socket type
  uint8_t  Flags;                       ///< State machine flags
  uint8_t  Tos;                         ///< Type of service allocated
  uint8_t  RemAddr[IP4_ADDR_LEN];       ///< Remote IPv4 address
  uint16_t RemPort;                     ///< Remote TCP port
  uint16_t LocPort;                     ///< Local TCP port
  uint16_t MaxSegSize;                  ///< Transmit Max. Segment Size
  uint16_t Tout;                        ///< Socket idle timeout (in seconds)
  uint16_t AliveTimer;                  ///< Keep Alive timer
  uint16_t RetryTimer;                  ///< Retransmission timer
  uint8_t  AckTimer;                    ///< Receive Delay-ack timer
  uint8_t  Id;                          ///< Socket identification number
  uint8_t  Retries;                     ///< Number of retries left before aborting
  uint8_t  DupAcks;                     ///< Number of duplicate acks (fast recovery)
  uint32_t SendUna;                     ///< Send Sequence Number unacknowledged
  uint32_t SendNext;                    ///< Next Send Sequence Number
  uint32_t SendChk;                     ///< Check Sequence Number for dupacks
  uint32_t SendWl1;                     ///< Sequence Number of last Window update
  uint32_t SendWl2;                     ///< Acknowledge Number of last Window update
  uint16_t SendWin;                     ///< Current Send Window
  int16_t  RttSa;                       ///< Scaled Average for RTT estimator
  int16_t  RttSv;                       ///< Scaled deViation for RTT estimator
  uint16_t CWnd;                        ///< Congestion Window
  uint16_t SsThresh;                    ///< Slow Start Treshold
  uint16_t RecWin;                      ///< Current Receive Window
  uint32_t RecNext;                     ///< Next Receive Sequence Number
  NET_FRAME *unack_list;                ///< Unacked queue list
  net_tcp_cb_t cb_func;                 ///< Application Event-CallBack function
} TCP_INFO;

/// BSD Socket info
typedef struct bsd_info {
  uint8_t  State;                       ///< Socket entry current state
  uint8_t  Socket;                      ///< Binded TCP/UDP Socket
  uint8_t  Flags;                       ///< Flags for Socket Option
  uint8_t  Type;                        ///< Type of Socket (SOCK_DGRAM/SOCK_STREAM)
  uint16_t LocPort;                     ///< Bind local Port
  uint16_t RemPort;                     ///< Remote TCP/UDP Port
  uint32_t LocIP;                       ///< Bind local IP address (net byte order)
  uint32_t RemIP;                       ///< Remote IP address (net byte order)
  uint8_t  AcceptSock;                  ///< Accept Socket list
  uint8_t  ParentSock;                  ///< Parent (server) socket
  uint16_t Tout;                        ///< Receive timer value
  uint8_t  Event;                       ///< Owner Wait-for Events
  uint8_t  xEvent;                      ///< Sender Wait-for Events
  void    *Thread;                      ///< Socket Owner thread
  void    *xThread;                     ///< Socket Sender thread
  void    *buf_list;                    ///< Rx Data buffer chain
} BSD_INFO;

/// DNS Cache Entry info
typedef struct dns_cache {
  uint32_t HostId;                      ///< Host Id (CRC32 value of host name)
  uint32_t Ttl;                         ///< Cache Entry Time To Live
  uint8_t  IpAdr[IP4_ADDR_LEN];         ///< Ethernet IP Address
} DNS_CACHE;

/// TFTP Server Session info
typedef struct tftp_info {
  uint8_t  State;                       ///< Session entry current state
  uint8_t  Socket;                      ///< binded UDP Data Socket
  uint8_t  Flags;                       ///< State machine Flags
  uint8_t  Retries;                     ///< Retry counter
  uint8_t  RemAddr[IP4_ADDR_LEN];       ///< Remote IP address
  uint16_t RemPort;                     ///< Remote UDP port (TID)
  uint16_t BlockSz;                     ///< Transfer Block size
  uint16_t BlockNr;                     ///< Block Number
  uint8_t  Timer;                       ///< Timeout Timer value
  uint16_t BufLen;                      ///< Length of retransmit buffer
  uint8_t  *Buf;                        ///< Transmit/Retransmit buffer
  void     *File;                       ///< File Handle pointer
} TFTP_INFO;

/// Telnet Session info
typedef struct tnet_info {
  uint8_t  State;                       ///< Session current state
  uint8_t  Socket;                      ///< binded TCP Socket
  uint8_t  Flags;                       ///< State machine Flags
  uint8_t  BCnt;                        ///< Received Data byte count
  uint16_t Tout;                        ///< Timeout delay counter
  uint8_t  UserId;                      ///< User Id (authentication enabled)
  uint8_t  Widx;                        ///< Fifo buffer Write index
  uint8_t  Ridx;                        ///< Fifo buffer Read index
  uint8_t  hNext;                       ///< History command next position
  uint8_t  hCurr;                       ///< History command current position
  uint32_t SVar;                        ///< Session private variable
  char     LBuf[TNET_LBUFSZ];           ///< Data Line Buffer
  char     Fifo[TNET_FIFOSZ];           ///< Fifo Buffer for received line/keycodes
  char     Hist[TNET_HISTSZ];           ///< Command History buffer
} TNET_INFO;

/// FTP Session info
typedef struct ftp_info {
  uint8_t  State;                       ///< FTP Session current state
  uint8_t  Socket;                      ///< binded TCP Control Socket
  uint16_t Flags;                       ///< State machine Flags
  uint8_t  RemIpAdr[IP4_ADDR_LEN];      ///< Client IP address
  uint16_t DPort;                       ///< TCP Data port (Server or Client)
  uint8_t  DSocket;                     ///< TCP Data Socket
  uint8_t  UserId;                      ///< User Id (authentication enabled)
  uint8_t  Resp;                        ///< FTP Server Response Code
  uint8_t  PathLen;                     ///< Size of the Path string
  char    *Path;                        ///< Current Working Directory
  char    *Name;                        ///< Absolute File/Folder Path Name
  void    *File;                        ///< File Handle pointer
} FTP_INFO;

/// HTTP Session info
typedef struct http_info {
  uint8_t  State;                       ///< HTTP Session current state
  uint8_t  Socket;                      ///< binded TCP Socket
  uint16_t Flags;                       ///< State machine Flags
  uint8_t  FType;                       ///< File Type: html, jpeg, gif,...
  uint8_t  PostSt;                      ///< POST method Processing Status
  uint16_t DelimSz;                     ///< Boundary delimiter size
  uint8_t  UserId;                      ///< User Id (authentication enabled)
  uint32_t CGIvar;                      ///< CGI session private variable
  uint32_t DLen;                        ///< Data length (number of bytes to send)
  uint32_t Count;                       ///< Total number of bytes sent
  uint16_t BCnt;                        ///< Number of bytes sent in last packet
  char     Lang[6];                     ///< Selected Web Language: en-us, de, fr...
  uint32_t LMDate;                      ///< Last Modified Date (UTC format)
  char    *Script;                      ///< Script Buffer pointer
  char    *pDelim;                      ///< Multipart Boundary delimiter value
  void    *sFile;                       ///< Script Interpreter File pointer
  void    *dFile;                       ///< Data File pointer
} HTTP_INFO;

/// HTTP Error page info
typedef struct http_error {
  const char *header;                   ///< Common page header
  const char *footer;                   ///< Common page footer
  const char *e401;                     ///< 401 Unauthorized
  const char *e403;                     ///< 403 Forbidden
  const char *e404;                     ///< 404 Not Found
  const char *e501;                     ///< 501 Not Implemented
} const HTTP_ERROR;

/// SYS Configuration info
typedef struct sys_cfg {
  uint32_t *MemPool;                    ///< Dynamic Memory Pool buffer
  uint32_t MemSize;                     ///< Memory Pool size in bytes
  uint8_t  TickRate;                    ///< Tick Rate in ticks per second
  uint8_t  TickItv;                     ///< Tick Interval in ms
  uint8_t  T200ms;                      ///< Delay 200 ms in ticks
  uint8_t  NetCfg;                      ///< Network Interface Configuration flags
  char    *HostName;                    ///< Local Host Name
} const SYS_CFG;

/// ARP Configuration info
typedef struct arp_cfg {
  ARP_INFO *Table;                      ///< Cache Table array
  uint8_t TabSize;                      ///< Cache Table size
  uint8_t TimeOut;                      ///< Table Entry expiration time in seconds
  uint8_t MaxRetry;                     ///< Number of Retries to resolve MAC address
  uint8_t Resend;                       ///< Resend Timeout in seconds
  bool    Notify;                       ///< Notify on IP address changes
} const ARP_CFG;

/// IGMP Configuration info
typedef struct igmp_cfg {
  IGMP_INFO *Table;                     ///< Group Table array
  uint16_t TabSize;                     ///< Group Table size
} const IGMP_CFG;

/// DHCP Client Configuration info
typedef struct dhcp_cfg {
  const char *Vcid;                     ///< Vendor Class Identifier
  uint8_t    Opt;                       ///< Extra DHCP option flags
} const DHCP_CFG;

/// Ethernet Interface Configuration info
typedef struct eth_cfg {
  ARM_DRIVER_ETH_MAC *DrvMac;           ///< Registered MAC driver
  ARM_DRIVER_ETH_PHY *DrvPhy;           ///< Registered PHY driver
  uint8_t *MacAddr;                     ///< MAC address
  ARP_CFG  ArpCfg;                      ///< ARP configuration
  IGMP_CFG IgmpCfg;                     ///< IGMP configuration
  DHCP_CFG DhcpCfg;                     ///< DHCP configuraton
} const ETH_CFG;

/// Serial Device Configuration info
typedef struct com_dev {
  ARM_DRIVER_USART *DrvUsart;           ///< Registered USART driver
  DRIVER_MODEM     *DrvModem;           ///< Registered MODEM driver
  const char       *InitString;         ///< Modem initialization string
  uint32_t Speed;                       ///< Connection Speed
  uint8_t  FlowCtrl;                    ///< Flow control
} const COM_DEV;

/// PPP Interface Configuration info
typedef struct ppp_cfg {
  COM_DEV  Dev;                         ///< Serial Device Configuration
  uint32_t ACCmap;                      ///< Async Control Character map
  uint16_t SerTout;                     ///< Serial Frame Receive Timeout
  uint16_t RetryTout;                   ///< Retry Timeout in ticks
  uint16_t EchoTout;                    ///< LCP Echo Timeout in seconds
  uint8_t  MaxRetry;                    ///< Number of Retries
  uint8_t  EnAuth;                      ///< Enabled Authentication protocols
} const PPP_CFG;

/// SLIP Interface Configuration info
typedef struct slip_cfg {
  COM_DEV  Dev;                         ///< Serial Device configuration
  uint16_t SerTout;                     ///< Serial Frame Receive Timeout
} const SLIP_CFG;

/// UDP Socket Configuration info
typedef struct udp_cfg {
  UDP_INFO *Scb;                        ///< Socket Control Block array
  uint8_t  NumSocks;                    ///< Number of UDP Sockets
} const UDP_CFG;

/// TCP Socket Configuration info
typedef struct tcp_cfg {
  TCP_INFO *Scb;                        ///< Socket Control Block array
  uint8_t  NumSocks;                    ///< Number of TCP Sockets
  uint8_t  MaxRetry;                    ///< Number of Retries
  uint16_t RetryTout;                   ///< Retry Timeout in ticks
  uint16_t T2MSLTout;                   ///< Maximum Segment Life Time
  uint16_t SynRetryTout;                ///< SYN Retry Timeout in ticks
  uint16_t InitRetryTout;               ///< Initial Retransmit timeout in ticks
  uint16_t DefTout;                     ///< Default Connect Timeout in seconds
  uint16_t MaxSegSize;                  ///< Maximum Segment Size value
  uint16_t RecWinSize;                  ///< Receiving Window Size in bytes
  uint8_t  ConRetry;                    ///< Number of Retries to Connect
} const TCP_CFG;

/// BSD Socket Configuration info
typedef struct bsd_cfg {
  BSD_INFO *Scb;                        ///< Socket Control Block array
  uint8_t  NumSocks;                    ///< Number of BSD Sockets
  uint16_t RcvTout;                     ///< Blocking recv timeout in ticks
} const BSD_CFG;

/// DNS Client Configuration info
typedef struct dns_cfg {
  DNS_CACHE *Table;                     ///< Cache Table array
  uint8_t   TabSize;                    ///< Cache Table size
} const DNS_CFG;

/// SNMP Agent Configuration info
typedef struct snmp_cfg {
  uint16_t PortNum;                     ///< Listening Port number
  uint16_t TrapPort;                    ///< Trap Port number
  uint8_t  TrapIp[IP4_ADDR_LEN];        ///< Trap Server IP Address
  const char *Community;                ///< Community Name
  uint8_t  TickItv;                     ///< Tick Interval in ticks
} const SNMP_CFG;

/// SNTP Client Configuration info
typedef struct sntp_cfg {
  uint8_t  SrvIp[IP4_ADDR_LEN];         ///< NTP/SNTP Server IP Address
  bool     Bcast;                       ///< Broadcast client mode (for LAN only)
} const SNTP_CFG;

/// TFTP Server Configuration info
typedef struct tftp_cfg {
  TFTP_INFO *Scb;                       ///< Session Control Block array
  uint8_t  NumSess;                     ///< Max. Number of Active Sessions
  uint8_t  MaxRetry;                    ///< Number of Retries
  uint16_t PortNum;                     ///< Listening Port number
  uint8_t  DefTout;                     ///< Inactive Session Timeout in seconds
  bool     EnFwall;                     ///< Enable Firewall Support
} const TFTP_CFG;

/// TFTP Client Configuration info
typedef struct tftpc_cfg {
  uint16_t BlockSize;                   ///< Transfer Block size
  uint16_t RetryTout;                   ///< Retry Timeout in ticks
  uint8_t  MaxRetry;                    ///< Number of Retries
} const TFTPC_CFG;

/// Telnet Server Configuration info
typedef struct tnet_cfg {
  TNET_INFO *Scb;                       ///< Session Control Block array
  uint8_t  NumSess;                     ///< Max. Number of Active Sessions
  uint8_t  EnAuth;                      ///< Enable User Authentication
  uint8_t  NoEcho;                      ///< Disable Server Echo mode
  uint16_t PortNum;                     ///< Listening Port number
  uint16_t IdleTout;                    ///< Idle Connection timeout in ticks
  const char *User;                     ///< Authentication User Name
  char     *Passw;                      ///< Authentication Password
} const TNET_CFG;

/// FTP Server Configuration info
typedef struct ftp_cfg {
  FTP_INFO *Scb;                        ///< Session Control Block array
  uint8_t  NumSess;                     ///< Max. Number of Active Sessions
  uint8_t  EnAuth;                      ///< Enable User Authentication
  uint16_t PortNum;                     ///< Listening Port number
  uint16_t IdleTout;                    ///< Idle Connection timeout in ticks
  uint8_t  MsgLen;                      ///< Length of welcome message
  const char *Msg;                      ///< Server Welcome message
  const char *User;                     ///< Authentication User Name
  char       *Passw;                    ///< Authentication Password
} const FTP_CFG;

/// FTP Client Configuration info
typedef struct ftpc_cfg {
  uint8_t DefTout;                      ///< Default inactivity timeout
  bool    PasvMode;                     ///< FTP Passive Mode used
} const FTPC_CFG;

/// HTTP Server Configuration info
typedef struct http_cfg {
  HTTP_INFO *Scb;                       ///< Session Control Block array
  uint8_t   NumSess;                    ///< Max. Number of Active Sessions
  uint8_t   EnAuth;                     ///< Enable User Authentication
  uint16_t  PortNum;                    ///< Listening Port number
  const char *SrvId;                    ///< Server-Id http header
  const char *Realm;                    ///< Authentication Realm
  const char *User;                     ///< Authentication User name
  char       *Passw;                    ///< Authentication Password
} const HTTP_CFG;

//  ==== Network system ====

/// \brief Initialize Network Component, resources and interfaces.
/// \return None
extern void net_sys_init (void);

/// \brief Run Network Component main function.
/// \return None
extern void net_sys_run (void);

/// \brief Signal Network Component error.
/// \param[in]     error         System error code.
/// \return None
extern void net_sys_error (ERROR_CODE error);

//  ==== Memory management ====

/// \brief Allocate memory for the network frame.
/// \param[in]     byte_size     buffer size in bytes.
/// \return        Pointer to the allocated memory.
extern NET_FRAME *mem_alloc (uint32_t byte_size);

/// \brief Release allocated memory.
/// \param[in]     mem_ptr       pointer to the allocated memory.
/// \return None
extern void mem_free (NET_FRAME *mem_ptr);

//  ==== Ethernet interface ====

/// \brief Initialize Ethernet interface.
/// \return None
extern void eth_init_link (void);

/// \brief Run ethernet interface main function.
/// \return None
extern void eth_run_link (void);

/// \brief Construct ethernet header for the frame and send it.
/// \param[in]     frame         frame to be sent.
/// \return
///              - true        = Frame successfuly sent.
///              - false       = Failed to send a frame.
extern bool eth_send_frame (NET_FRAME *frame);

/// \brief Check if ethernet destination MAC address is local MAC address.
/// \param[in]     frame         received frame to be checked.
/// \return
///              - true        = Destination MAC is local MAC address.
///              - false       = Destination MAC is not local.
extern bool eth_chk_addr (NET_FRAME *frame);

/// \brief Get MAC address for the given IP address.
/// \param[in]     ip_addr     IP address of a host.
/// \return        Pointer to the MAC address in ARP cache.
extern const uint8_t *eth_get_addr (const uint8_t *ip_addr);

/// \brief Send a notification of local IP address change (gratuitous ARP).
/// \return None
extern void arp_notify (void);

/// \brief Initialize IGMP group management.
/// \return None
extern void igmp_host_init (void);

/// \brief Run IGMP group management main function.
/// \return None
extern void igmp_host_run (void);

/// \brief Process IGMP group management frame.
/// \param[in]     frame         received IGMP group management frame.
/// \return None
extern void igmp_process (NET_FRAME *frame);

/// \brief Check if local host is a member of provided group.
/// \param[in]     ipadr         multicast IP address to be checked.
/// \return
///              - true        = Local host is member of IGMP group ipadr.
///              - false       = Local host is not member.
extern bool igmp_is_member (const uint8_t *ipadr);

/// \brief Initialize NBNS client.
/// \return None
extern void nbns_client_init (void);

/// \brief Initialize DHCP client.
/// \return None
extern void dhcp_client_init (void);

/// \brief Run DHCP client main function.
/// \return None
extern void dhcp_client_run (void);

//  ==== PPP interface ====

/// \brief Initialize PPP interface.
/// \return None
extern void ppp_init_link (void);

/// \brief Run PPP interface main function.
/// \return None
extern void ppp_run_link (void);

/// \brief Construct PPP header for the frame and send it.
/// \param[in]     frame         frame to be sent.
/// \param[in]     prot          PPP protocol type of the frame.
/// \return
///              - true        = Frame successfuly sent.
///              - false       = Failed to send a frame.
extern bool ppp_send_frame (NET_FRAME *frame, uint16_t prot);

/// \brief Initialize PAP authentication.
/// \return None
extern void pap_init (void);

/// \brief Run PAP authentication main function.
/// \return None
extern void pap_run (void);

/// \brief Process PAP authentication frame.
/// \param[in]     frame         received PAP authentication frame.
/// \return None
extern void pap_process (NET_FRAME *frame);

/// \brief Initialize CHAP authentication.
/// \return None
extern void chap_init (void);

/// \brief Run CHAP authentication main function.
/// \return None
extern void chap_run (void);

/// \brief Process CHAP authentication frame.
/// \param[in]     frame         received CHAP authentication frame.
/// \return None
extern void chap_process (NET_FRAME *frame);

//  ==== SLIP interface ====

/// \brief Initialize SLIP interface.
/// \return None
extern void slip_init_link (void);

/// \brief Run SLIP interface main function.
/// \return None
extern void slip_run_link (void);

/// \brief Send a SLIP frame.
/// \param[in]     frame         frame to be sent.
/// \return
///              - true        = Frame successfuly sent.
///              - false       = Failed to send a frame.
extern bool slip_send_frame (NET_FRAME *frame);

//  ==== Network core ====

/// \brief Initialize IP localhost.
/// \return None
extern void ip_localhost_init (void);

/// \brief Run IP localhost main function.
/// \return None
extern void ip_localhost_run (void);

/// \brief Initialize ICMP service.
/// \return None
extern void icmp_service_init (void);

/// \brief Run ICMP service main function.
/// \return None
extern void icmp_service_run (void);

/// \brief Initialize UDP sockets.
/// \return None
extern void udp_socket_init (void);

/// \brief Process UDP frame.
/// \param[in]     frame         received UDP frame.
/// \return None
extern void udp_process (NET_FRAME *frame);

/// \brief Initialize TCP sockets.
/// \return None
extern void tcp_socket_init (void);

/// \brief Run TCP socket main function.
/// \return None
extern void tcp_socket_poll (void);

/// \brief Process TCP frame.
/// \param[in]     frame         received TCP frame.
/// \return None
extern void tcp_process (NET_FRAME *frame);

/// \brief Initialize DNS client.
/// \return None
extern void dns_client_init (void);

/// \brief Run DNS client main function.
/// \return None
extern void dns_client_run (void);

/// \brief Initialize BSD sockets.
/// \return None
extern void bsd_socket_init (void);

/// \brief Run BSD socket main function.
/// \return None
extern void bsd_socket_poll (void);

/// \brief Lock BSD socket functions (acquire mutex).
/// \return None
extern void bsd_lock (void);

/// \brief Unlock BSD socket functions (release mutex).
/// \return None
extern void bsd_unlock (void);

/// \brief Initialize BSD host resolver.
/// \return None
extern void bsd_host_init (void);

//  ==== FTP server service ====

/// \brief Initialize FTP server.
/// \return None
extern void ftp_server_init (void);

/// \brief Run FTP server main function.
/// \return None
extern void ftp_server_run (void);

//  ==== FTP client application ====

/// \brief Initialize FTP client.
/// \return None
extern void ftp_client_init (void);

/// \brief Run FTP client main function.
/// \return None
extern void ftp_client_run (void);

//  ==== TFTP server service ====

/// \brief Initialize TFTP server.
/// \return None
extern void tftp_server_init (void);

/// \brief Run TFTP server main function.
/// \return None
extern void tftp_server_run (void);

//  ==== TFTP client application ====

/// \brief Initialize TFTP client.
/// \return None
extern void tftp_client_init (void);

/// \brief Run TFTP client main function.
/// \return None
extern void tftp_client_run (void);

//  ==== SMTP client application ====

/// \brief Initialize SMTP client.
/// \return None
extern void smtp_client_init (void);

/// \brief Run SMTP client main function.
/// \return None
extern void smtp_client_run (void);

//  ==== SNMP agent application ====

/// \brief Initialize SNMP agent.
/// \return None
extern void snmp_agent_init (void);

/// \brief Run SNMP agent main function.
/// \return None
extern void snmp_agent_run (void);

//  ==== SNTP client application ====

/// \brief Initialize SNTP client.
/// \return None
extern void sntp_client_init (void);

/// \brief Run SNTP client main function.
/// \return None
extern void sntp_client_run (void);

//  ==== Telnet server service ====

/// \brief Initialize Telnet server.
/// \return None
extern void telnet_server_init (void);

/// \brief Run Telnet server main function.
/// \return None
extern void telnet_server_run (void);

//  ==== HTTP server service ====

/// \brief Initialize HTTP server.
/// \return None
extern void http_server_init (void);

/// \brief Run HTTP server main function.
/// \return None
extern void http_server_run (void);

//  ==== Network debug ====

/// \brief Initialize debug interface.
/// \return None
extern void net_debug_init (void);

/// \brief Debug print information message.
/// \param[in]     proc          network process id.
/// \param[in]     fmt           printf format string.
/// \return        None
extern void dbg_info (int32_t proc, const char *fmt, ...);

/// \brief Debug print error message.
/// \param[in]     proc          network process id.
/// \param[in]     fmt           printf format string.
/// \return        None.
extern void dbg_error (int32_t proc, const char *fmt, ...);

/// \brief Convert process id into a string.
/// \param[in]     proc          network process id.
/// \return        pointer to process id string.
extern const char *dbg_proc (int32_t proc);

/// \brief Get current network time for debug.
/// \return        pointer to current time string.
extern const char *dbg_time (void);

#ifdef __cplusplus
}
#endif

#endif /* __RL_NET_LIB_H */

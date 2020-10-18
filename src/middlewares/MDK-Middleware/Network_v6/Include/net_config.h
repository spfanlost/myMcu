/*------------------------------------------------------------------------------
 * MDK Middleware - Component ::Network
 * Copyright (c) 2004-2019 Arm Limited (or its affiliates). All rights reserved.
 *------------------------------------------------------------------------------
 * Name:    net_config.h
 * Purpose: Network Library Configuration
 * Rev.:    V6.7.0
 *----------------------------------------------------------------------------*/

#if   defined(__CC_ARM)
  #pragma O3
#elif defined(__clang__)
  #pragma clang diagnostic ignored "-Wundef"
  #pragma clang diagnostic ignored "-Wpadded"
  #pragma clang diagnostic ignored "-Wunused-parameter"
  #pragma clang diagnostic ignored "-Wmissing-noreturn"
#endif

#include "cmsis_os.h"
#include "rl_net_lib.h"

/* Tick interval fixed to 100 ms */
#define NET_TICK_INTERVAL   100
#define NET_TICK_RATE       (1000/ NET_TICK_INTERVAL)

/* Current limitation */
#if (ETH0_ENABLE && ETH1_ENABLE)
  #error "::Network:Interface: Only one instance of same interface allowed"
#endif

/* Check enabled interfaces */
#if (!ETH0_ENABLE && !ETH1_ENABLE && !PPP_ENABLE && !SLIP_ENABLE)
  #error "::Network:Interface: No interface enabled in configuration"
#endif

#if (SLIP_ENABLE && PPP_ENABLE)
  #error "::Network:Interface:PPP and ::Network:Interface:SLIP cannot be used simultaneously"
#endif

/* Check ETH port consistency */
#if ((ETH0_ENABLE && ETH1_ENABLE) && (ETH0_DRIVER == ETH1_DRIVER))
  #error "::Network:Interface:ETH: ETH0 and ETH1 driver numbers must be different"
#endif

/* Check TCP Socket enable */
#if (!TCP_ENABLE && (BSD_ENABLE           || \
                     HTTP_SERVER_ENABLE   || \
                     TELNET_SERVER_ENABLE || \
                     FTP_SERVER_ENABLE    || \
                     FTP_CLIENT_ENABLE    || \
                     SMTP_CLIENT_ENABLE))
  #error "::Network:Socket:TCP component required"
#endif

/* Check UDP Socket enable */
#if (!UDP_ENABLE && (BSD_ENABLE                       || \
                     TFTP_SERVER_ENABLE               || \
                     DNS_CLIENT_ENABLE                || \
                     SNMP_AGENT_ENABLE                || \
                     SNTP_CLIENT_ENABLE               || \
                     (ETH0_DHCP_ENABLE & ETH0_ENABLE) || \
                     (ETH0_NBNS_ENABLE & ETH0_ENABLE)))
  #error "::Network:Socket:UDP component required"
#endif

/* Check number of TCP sockets available */
#define __TCPNS    ((BSD_ENABLE           * BSD_NUM_SOCKS)               + \
                    (HTTP_SERVER_ENABLE   * HTTP_SERVER_NUM_SESSIONS)    + \
                    (TELNET_SERVER_ENABLE * TELNET_SERVER_NUM_SESSISONS) + \
                    (FTP_SERVER_ENABLE    * FTP_SERVER_NUM_SESSIONS * 2) + \
                    (FTP_CLIENT_ENABLE    * 2)                           + \
                    (SMTP_CLIENT_ENABLE   * 1))

#if (__TCPNS > TCP_NUM_SOCKS)
  #error "::Network:Socket:TCP: Number of TCP Sockets too small"
#endif

/* Calculate number of UDP sockets needed for TFTP server */
#if (TFTP_SERVER_FIREWALL_ENABLE)
  #define __TFTP_NSOCKS  1
#else
  #define __TFTP_NSOCKS  (1 + TFTP_SERVER_NUM_SESSIONS)
#endif

/* Check number of UDP sockets available */
#define __UDPNS    ((BSD_ENABLE         * BSD_NUM_SOCKS)  + \
                    (TFTP_SERVER_ENABLE * __TFTP_NSOCKS)  + \
                    (TFTP_CLIENT_ENABLE * 1)              + \
                    (DNS_CLIENT_ENABLE  * 1)              + \
                    (SNMP_AGENT_ENABLE  * 1)              + \
                    (SNTP_CLIENT_ENABLE * 1)              + \
                    (ETH0_DHCP_ENABLE   * ETH0_ENABLE)    + \
                    (ETH0_NBNS_ENABLE   * ETH0_ENABLE))

#if (__UDPNS > UDP_NUM_SOCKS)
  #error "::Network:Socket:UDP: Number of UDP Sockets too small"
#endif

/* Check Maximum Segment Size of TCP Socket */
#if (TCP_ENABLE && ((TCP_MAX_SEG_SIZE < 536) || (TCP_MAX_SEG_SIZE > 1460)))
  #error "::Network:Socket:TCP: Maximum Segment Size out of range"
#endif

/* Check Receive Window Size of TCP Socket */
#if (TCP_ENABLE && (TCP_RECEIVE_WIN_SIZE < TCP_MAX_SEG_SIZE))
  #error "::Network:Socket:TCP: Receive Window Size too small"
#endif

/* Check BSD Server sockets */
#if (BSD_ENABLE && (BSD_SERVER_SOCKS > BSD_NUM_SOCKS))
  #error "::Network:Socket:BSD: Number of BSD Server sockets too large"
#endif

/* Check BSD Hostname resolver */
#if (BSD_ENABLE && BSD_HOSTNAME_ENABLE && !DNS_CLIENT_ENABLE)
  #error "::Network:Socket:BSD: Hostname resolver needs DNS enabled"
#endif

/* Check PPP Authentication protocols */
#if (PPP_ENABLE && PPP_AUTH_ENABLE && !(PPP_PAP_ENABLE || PPP_CHAP_ENABLE))
  #error "::Network:Interface:PPP: Authentication needs PAP or/and CHAP enabled"
#endif

/* Check Block Size of TFTP Client */
#if (TFTP_CLIENT_ENABLE && ((TFTP_CLIENT_BLOCK_SIZE < 128) || (TFTP_CLIENT_BLOCK_SIZE > 1428)))
  #error "::Network:Service:TFTP Client: Block Size out of range"
#endif

#define EXPAND_SYMBOL(name, port) name##port
#define CREATE_SYMBOL(name, port) EXPAND_SYMBOL(name, port)

/* Local Machine info */
LOCALM const nlocalm[3] = {
#if (ETH0_ENABLE)
  /* Local IP address */
 {{ETH0_IP1, ETH0_IP2, ETH0_IP3, ETH0_IP4},
  /* Default Gateway */
  {ETH0_GW1, ETH0_GW2, ETH0_GW3, ETH0_GW4},
  /* Local Subnet Mask */
  {ETH0_MASK1, ETH0_MASK2, ETH0_MASK3, ETH0_MASK4},
  /* Primary DNS Server */
  {ETH0_PRI_DNS1, ETH0_PRI_DNS2, ETH0_PRI_DNS3, ETH0_PRI_DNS4},
  /* Secondary DNS Server */
  {ETH0_SEC_DNS1, ETH0_SEC_DNS2, ETH0_SEC_DNS3, ETH0_SEC_DNS4}},
#else
 {{0, 0, 0, 0},
  {0, 0, 0, 0},
  {0, 0, 0, 0},
  {0, 0, 0, 0},
  {0, 0, 0, 0}},
#endif
#if (PPP_ENABLE)
  /* Local IP address */
 {{PPP_IP1, PPP_IP2, PPP_IP3, PPP_IP4},
  /* Default Gateway none */
  {0, 0, 0, 0},
  /* Local Subnet Mask */
  {255, 255, 255, 240},
  /* Primary DNS Server */
  {PPP_PRI_DNS1, PPP_PRI_DNS2, PPP_PRI_DNS3, PPP_PRI_DNS4},
  /* Secondary DNS Server */
  {PPP_SEC_DNS1, PPP_SEC_DNS2, PPP_SEC_DNS3, PPP_SEC_DNS4}},
#else
 {{0, 0, 0, 0},
  {0, 0, 0, 0},
  {0, 0, 0, 0},
  {0, 0, 0, 0},
  {0, 0, 0, 0}},
#endif
#if (SLIP_ENABLE)
  /* Local IP address */
 {{SLIP_IP1, SLIP_IP2, SLIP_IP3, SLIP_IP4},
  /* Default Gateway none */
  {0, 0, 0, 0},
  /* Subnet IP Mask */
  {255, 255, 255, 240},
  /* Primary DNS Server */
  {SLIP_PRI_DNS1, SLIP_PRI_DNS2, SLIP_PRI_DNS3, SLIP_PRI_DNS4},
  /* Secondary DNS Server */
  {SLIP_SEC_DNS1, SLIP_SEC_DNS2, SLIP_SEC_DNS3, SLIP_SEC_DNS4}}
#else
 {{0, 0, 0, 0},
  {0, 0, 0, 0},
  {0, 0, 0, 0},
  {0, 0, 0, 0},
  {0, 0, 0, 0}},
#endif
};

/* Temporary solution */
#if (ETH0_ENABLE)
  #define __ETH0_CFG    (ETH0_ENABLE)
#else
  #define __ETH0_CFG    0x00
#endif
#if (PPP_ENABLE)
  #define __PPP_CFG     (PPP_GET_IP << 5 | PPP_DEFAULT_GW << 3 | PPP_ENABLE << 1)
#else
  #define __PPP_CFG     0x00
#endif
#if (SLIP_ENABLE)
  #define __SLIP_CFG    (SLIP_DEFAULT_GW << 4 | SLIP_ENABLE << 2)
#else
  #define __SLIP_CFG    0x00
#endif

/* System configuration */
static uint32_t mem_pool[NET_MEM_SIZE];
char lhost_name[16] = NET_HOST_NAME;
SYS_CFG sys_config = {
  mem_pool,
  sizeof (mem_pool),
  NET_TICK_RATE,
  NET_TICK_INTERVAL,
  NET_TICK_RATE / 5,
  (__ETH0_CFG | __PPP_CFG | __SLIP_CFG),
  lhost_name
};

#if (ETH0_ENABLE)
  /* ETH0 device MAC and PHY drivers */
  extern ARM_DRIVER_ETH_MAC ARM_Driver_ETH_MAC_(ETH0_DRIVER);
  extern ARM_DRIVER_ETH_PHY ARM_Driver_ETH_PHY_(ETH0_DRIVER);

  /* ETH0 device MAC address */
  static uint8_t eth0_mac_addr[ETH_ADDR_LEN] = {
    ETH0_MAC1, ETH0_MAC2, ETH0_MAC3, ETH0_MAC4, ETH0_MAC5, ETH0_MAC6
  };
  static ARP_INFO eth0_arp_table[ETH0_ARP_TAB_SIZE];
  #if (ETH0_IGMP_ENABLE)
    static IGMP_INFO eth0_igmp_table[ETH0_IGMP_TAB_SIZE];
  #endif

  /* ETH0 device configuration */
  ETH_CFG eth0_config = {
    &ARM_Driver_ETH_MAC_(ETH0_DRIVER),
    &ARM_Driver_ETH_PHY_(ETH0_DRIVER),
    eth0_mac_addr,
    /* ARP configuration */
    { eth0_arp_table,
      ETH0_ARP_TAB_SIZE,
      ETH0_ARP_CACHE_TOUT,
      ETH0_ARP_MAX_RETRY,
      ETH0_ARP_RESEND_TOUT,
      ETH0_ARP_NOTIFY
    },
  #if (ETH0_IGMP_ENABLE)
    /* IGMP configuration */
    { eth0_igmp_table,
      ETH0_IGMP_TAB_SIZE
    },
  #else
    { NULL, 0 },
  #endif
  #if (ETH0_DHCP_ENABLE)
    /* DHCP configuration */
    { ETH0_DHCP_VCID,
     (ETH0_DHCP_NTP_SERVERS << 1) | ETH0_DHCP_BOOTFILE
    }
  #else
    { NULL, 0 }
  #endif
  };
#endif

#if (PPP_ENABLE)
  /* PPP device USART and MODEM drivers */
  extern ARM_DRIVER_USART CREATE_SYMBOL (Driver_USART, PPP_USART_DRIVER);
  extern DRIVER_MODEM     Driver_MODEM;

  /* PPP device configuration */
  PPP_CFG ppp_config = {
    /* Modem device */
    { &CREATE_SYMBOL (Driver_USART, PPP_USART_DRIVER),
      &Driver_MODEM,
      PPP_MODEM_INIT_STRING,
      PPP_MODEM_SPEED,
      PPP_MODEM_FLOW_CONTROL
    },
    PPP_ACCM,
    5 * NET_TICK_RATE,
    PPP_RETRY_TOUT * NET_TICK_RATE,
    PPP_ECHO_TOUT,
    PPP_MAX_RETRY,
    ((PPP_CHAP_ENABLE << 1) | PPP_PAP_ENABLE) * PPP_AUTH_ENABLE
  };
#endif

#if (SLIP_ENABLE)
  /* SLIP device USART and MODEM drivers */
  extern ARM_DRIVER_USART CREATE_SYMBOL (Driver_USART, SLIP_USART_DRIVER);
  extern DRIVER_MODEM     Driver_MODEM;

  /* SLIP device configuration */
  SLIP_CFG slip_config = {
    /* Modem device */
    { &CREATE_SYMBOL (Driver_USART, SLIP_USART_DRIVER),
      &Driver_MODEM,
      SLIP_MODEM_INIT_STRING,
      SLIP_MODEM_SPEED,
      SLIP_MODEM_FLOW_CONTROL
    },
    5 * NET_TICK_RATE
  };
#endif

#if (UDP_ENABLE)
  static UDP_INFO udp_scb[UDP_NUM_SOCKS];
  UDP_CFG udp_config = {
    udp_scb,
    UDP_NUM_SOCKS
  };
#endif

#if (TCP_ENABLE)
  static TCP_INFO tcp_scb[TCP_NUM_SOCKS];
  TCP_CFG tcp_config = {
    tcp_scb,
    TCP_NUM_SOCKS,
    TCP_MAX_RETRY,
    TCP_RETRY_TOUT * NET_TICK_RATE,
    (200 + NET_TICK_INTERVAL - 1) / NET_TICK_INTERVAL,
    TCP_SYN_RETRY_TOUT * NET_TICK_RATE,
    TCP_INITIAL_RETRY_TOUT * NET_TICK_RATE,
    TCP_DEFAULT_TOUT,
    TCP_MAX_SEG_SIZE,
    TCP_RECEIVE_WIN_SIZE,
    TCP_CONNECT_RETRY
  };
#endif

#if (BSD_ENABLE)
  static BSD_INFO bsd_scb[BSD_NUM_SOCKS + BSD_SERVER_SOCKS];
  BSD_CFG bsd_config = {
    bsd_scb,
    BSD_NUM_SOCKS + BSD_SERVER_SOCKS,
    BSD_RECEIVE_TOUT * NET_TICK_RATE
  };
#endif

#if (HTTP_SERVER_ENABLE)
  static HTTP_INFO http_scb[HTTP_SERVER_NUM_SESSIONS];
  static char http_passw[NET_PASSWORD_SIZE] = HTTP_SERVER_AUTH_PASS;
  HTTP_CFG  http_config = {
    http_scb,
    HTTP_SERVER_NUM_SESSIONS,
    HTTP_SERVER_AUTH_ENABLE,
    HTTP_SERVER_PORT_NUM,
    HTTP_SERVER_ID,
  #if (HTTP_SERVER_AUTH_ENABLE)
    HTTP_SERVER_AUTH_REALM,
    HTTP_SERVER_AUTH_USER,
    http_passw
  #else
    "","",""
  #endif
  };
#endif

#if (TELNET_SERVER_ENABLE)
  static TNET_INFO tnet_scb[TELNET_SERVER_NUM_SESSISONS];
  static char tnet_passw[NET_PASSWORD_SIZE] = TELNET_SERVER_AUTH_PASS;
  TNET_CFG  tnet_config = {
    tnet_scb,
    TELNET_SERVER_NUM_SESSISONS,
    TELNET_SERVER_AUTH_ENABLE,
    TELNET_SERVER_NO_ECHO,
    TELNET_SERVER_PORT_NUM,
    TELNET_SERVER_TOUT,
  #if (TELNET_SERVER_AUTH_ENABLE)
    TELNET_SERVER_AUTH_USER,
    tnet_passw
  #else
    "",""
  #endif
  };
#endif

#if (TFTP_SERVER_ENABLE)
  static TFTP_INFO tftp_scb[TFTP_SERVER_NUM_SESSIONS];
  TFTP_CFG tftp_config = {
    tftp_scb,
    TFTP_SERVER_NUM_SESSIONS,
    TFTP_SERVER_MAX_RETRY,
    TFTP_SERVER_PORT_NUM,
    TFTP_SERVER_TOUT,
    TFTP_SERVER_FIREWALL_ENABLE
  };
#endif

#if (TFTP_CLIENT_ENABLE)
  TFTPC_CFG tftpc_config = {
    TFTP_CLIENT_BLOCK_SIZE,
    (TFTP_CLIENT_RETRY_TOUT * NET_TICK_RATE + 5) / 10,
    TFTP_CLIENT_MAX_RETRY
  };
#endif

#if (FTP_SERVER_ENABLE)
  static FTP_INFO ftp_scb[FTP_SERVER_NUM_SESSIONS];
  static char ftp_passw[NET_PASSWORD_SIZE] = FTP_SERVER_AUTH_PASS;
  #define _LS(s)  sizeof(s)-1,s
  FTP_CFG ftp_config = {
    ftp_scb,
    FTP_SERVER_NUM_SESSIONS,
    FTP_SERVER_AUTH_ENABLE,
    FTP_SERVER_PORT_NUM,
    FTP_SERVER_TOUT,
   _LS(FTP_SERVER_MESSAGE),
  #if (FTP_SERVER_AUTH_ENABLE)
    FTP_SERVER_AUTH_USER,
    ftp_passw
  #else
    "",""
  #endif
  };
#endif

#if (FTP_CLIENT_ENABLE)
  FTPC_CFG ftpc_config = {
    FTP_CLIENT_TOUT,
    FTP_CLIENT_PASSIVE_MODE
  };
#endif

#if (DNS_CLIENT_ENABLE)
  static DNS_CACHE dns_table[DNS_CLIENT_TAB_SIZE];
  DNS_CFG dns_config = {
    dns_table,
    DNS_CLIENT_TAB_SIZE
  };
#endif

#if (SMTP_CLIENT_ENABLE)
  uint8_t const smtp_DefTout = SMTP_CLIENT_TOUT;
#endif

#if (SNMP_AGENT_ENABLE)
  SNMP_CFG snmp_config = {
    SNMP_AGENT_PORT_NUM,
    SNMP_AGENT_TRAP_PORT,
    { SNMP_AGENT_TRAP_IP1, SNMP_AGENT_TRAP_IP2, SNMP_AGENT_TRAP_IP3, SNMP_AGENT_TRAP_IP4 },
    SNMP_AGENT_COMMUNITY,
    NET_TICK_INTERVAL / 5
  };
#endif

#if (SNTP_CLIENT_ENABLE)
  SNTP_CFG sntp_config = {
    { SNTP_CLIENT_SERVER_IP1, SNTP_CLIENT_SERVER_IP2, SNTP_CLIENT_SERVER_IP3, SNTP_CLIENT_SERVER_IP4 },
    SNTP_CLIENT_BCAST_MODE
  };
#endif

/*-----------------------------------------------------------------------------
 *  Initialize network interfaces and services
 *----------------------------------------------------------------------------*/

void net_sys_init (void) {
#if (ETH0_ENABLE)
  eth_init_link ();
#endif
#if (PPP_ENABLE)
  ppp_init_link ();
#endif
#if (SLIP_ENABLE)
  slip_init_link ();
#endif
  ip_localhost_init ();
  icmp_service_init ();
#if (ETH0_ENABLE && ETH0_IGMP_ENABLE)
  igmp_host_init ();
#endif
#if (UDP_ENABLE)
  udp_socket_init ();
#endif
#if (TCP_ENABLE)
  tcp_socket_init ();
#endif
#if (BSD_ENABLE)
  bsd_socket_init ();
 #if (BSD_HOSTNAME_ENABLE)
  bsd_host_init ();
 #endif
#endif
#if (HTTP_SERVER_ENABLE)
  http_server_init ();
#endif
#if (TELNET_SERVER_ENABLE)
  telnet_server_init ();
#endif
#if (TFTP_SERVER_ENABLE)
  tftp_server_init ();
#endif
#if (TFTP_CLIENT_ENABLE)
  tftp_client_init ();
#endif
#if (FTP_SERVER_ENABLE)
  ftp_server_init ();
#endif
#if (FTP_CLIENT_ENABLE)
  ftp_client_init ();
#endif
#if (ETH0_ENABLE && ETH0_NBNS_ENABLE)
  nbns_client_init ();
#endif
#if (ETH0_ENABLE && ETH0_DHCP_ENABLE)
  dhcp_client_init ();
#elif (ETH0_ENABLE)
  arp_notify ();
#endif
#if (DNS_CLIENT_ENABLE)
  dns_client_init ();
#endif
#if (SMTP_CLIENT_ENABLE)
  smtp_client_init ();
#endif
#if (SNMP_AGENT_ENABLE)
  snmp_agent_init ();
#endif
#if (SNTP_CLIENT_ENABLE)
  sntp_client_init ();
#endif
}

/*-----------------------------------------------------------------------------
 *  Run network interfaces and services
 *----------------------------------------------------------------------------*/

void net_sys_run (void) {
#if (BSD_ENABLE)
  bsd_lock ();
#endif
#if (ETH0_ENABLE)
  eth_run_link ();
#endif
#if (PPP_ENABLE)
  ppp_run_link ();
#endif
#if (SLIP_ENABLE)
  slip_run_link ();
#endif
  ip_localhost_run ();
  icmp_service_run ();
#if (ETH0_ENABLE && ETH0_IGMP_ENABLE)
  igmp_host_run ();
#endif
#if (TCP_ENABLE)
  tcp_socket_poll ();
#endif
#if (BSD_ENABLE)
  bsd_socket_poll ();
#endif
#if (HTTP_SERVER_ENABLE)
  http_server_run ();
#endif
#if (TELNET_SERVER_ENABLE)
  telnet_server_run ();
#endif
#if (TFTP_SERVER_ENABLE)
  tftp_server_run ();
#endif
#if (TFTP_CLIENT_ENABLE)
  tftp_client_run ();
#endif
#if (FTP_SERVER_ENABLE)
  ftp_server_run ();
#endif
#if (FTP_CLIENT_ENABLE)
  ftp_client_run ();
#endif
#if (ETH0_ENABLE && ETH0_DHCP_ENABLE)
  dhcp_client_run ();
#endif
#if (DNS_CLIENT_ENABLE)
  dns_client_run ();
#endif
#if (SMTP_CLIENT_ENABLE)
  smtp_client_run ();
#endif
#if (SNMP_AGENT_ENABLE)
  snmp_agent_run ();
#endif
#if (SNTP_CLIENT_ENABLE)
  sntp_client_run ();
#endif
#if (BSD_ENABLE)
  bsd_unlock ();
#endif
}

/*-----------------------------------------------------------------------------
 *  Image size optimization
 *----------------------------------------------------------------------------*/

#if !(ETH0_ENABLE)
/* Empty functions when Ethernet Interface is disabled */
bool eth_chk_addr (NET_FRAME *frame)                    { return (false); }
const uint8_t *eth_get_addr (const uint8_t *ip_adr)     { return (NULL);  }
bool eth_send_frame (NET_FRAME *frame)                  { return (false); }
#endif

#if !(ETH0_ENABLE && ETH0_IGMP_ENABLE)
/* Empty functions when IP Multicasting is not enabled */
bool igmp_is_member (const uint8_t *ipadr)              { return (false); }
void igmp_process (NET_FRAME *frame)                    { ; }
#endif

#if (ETH0_ENABLE && !ETH0_DHCP_ENABLE)
/* Empty function when DHCP not enabled */
netStatus dhcp_disable (uint32_t if_num)                { return (netError); }
#endif

#if !(PPP_ENABLE)
/* Empty function when PPP Interface is disabled */
bool ppp_send_frame (NET_FRAME *frame, uint16_t prot)   { return (false); }
#endif

#if (PPP_ENABLE && !(PPP_AUTH_ENABLE && PPP_PAP_ENABLE))
/* Empty functions when PAP authentication not enabled */
void pap_init (void)                                    { ; }
void pap_run (void)                                     { ; }
void pap_process (NET_FRAME *frame)                     { ; }
#endif

#if (PPP_ENABLE && !(PPP_AUTH_ENABLE && PPP_CHAP_ENABLE))
/* Empty functions when CHAP authentication not enabled */
void chap_init (void)                                   { ; }
void chap_run (void)                                    { ; }
void chap_process (NET_FRAME *frame)                    { ; }
#endif

#if !(SLIP_ENABLE)
/* Empty function when SLIP Interface is disabled */
bool slip_send_frame (NET_FRAME *frame)                 { return (false); }
#endif

#if !(UDP_ENABLE)
/* Empty function when UDP socket not enabled */
void udp_process (NET_FRAME *frame)                     { ; }
#endif

#if !(TCP_ENABLE)
/* Empty function when TCP socket not enabled */
void tcp_process (NET_FRAME *frame)                     { ; }
#endif

#if !(SNTP_CLIENT_ENABLE)
/* Empty function when SNTP not enabled */
netStatus sntp_get_time (const uint8_t *ip_addr,
                         net_sntp_client_cb_t cb_func)  { return (netError); }
#endif

#if (HTTP_SERVER_ENABLE && defined(RTE_Network_Web_Server_RO))
/* Empty interface functions for Compact Web server */
__weak void *http_server_fopen (const char *fname)      { return (NULL);  }
__weak void http_server_fclose (void *file)             { ;               }
__weak uint32_t http_server_fread (void *file, uint8_t *buf,
                                   uint32_t len)        { return (0);     }
__weak char *http_server_fgets (void *file, char *buf,
                                uint32_t size)          { return (NULL);  }
__weak uint32_t http_server_ftime (const char *fname)   { return (0);     }
#endif

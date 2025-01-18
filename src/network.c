/**
 * @file network.c
 * @brief Network thread and related utilities for Ethernet communication.
 * 
 * This file provides functions to manage the network thread, handle Ethernet
 * events, perform UDP communication, and display data as a hexdump. It uses
 * the CMSIS-RTOS2 and RL-NET libraries.
 * 
 * @author S54MTB, Marko Pavlin
 * @date January 2025
 * @version 1.0
 */

#include "RTE_Components.h"
#include CMSIS_device_header
#include "cmsis_os2.h"
#include "main.h"
#include "rl_net.h"
#include "stdio.h" 
#include "network.h"
#include "MY_net_config.h"
#include "rotator.h"
#include <string.h>
#include <stdlib.h>
#include "n1mm_xml_parser.h"
#include "config.h"


int32_t udp_sock;
static uint32_t link_status = NET_FL_NOTIFY_LINK_DOWN;
static uint32_t link_speed = 0;
static NET_ETH_LINK_INFO eth_info;
void Network_IP_Assignment( void );
osEventFlagsId_t network_evt_id;                        // event flags id
static N1MMRotor LastRotatorData = {0};

/** rotator thread */
extern osEventFlagsId_t rotator_evt_id;                        // event flags id


osThreadId_t tid_Network_Thread; // Thread ID

/**
 * @brief Network thread function.
 * 
 * This function initializes the network stack, waits for a link to be established,
 * sets up a UDP socket, and performs periodic tasks within an infinite loop.
 * 
 * @param[in] argument Unused parameter (reserved for future use).
 */
void Network_Thread(void *argument);

/**
 * @brief Print a buffer as a hexdump.
 * 
 * This function displays the content of a buffer in both hexadecimal and ASCII format.
 * 
 * @param[in] buf Pointer to the buffer.
 * @param[in] len Length of the buffer.
 */
void print_hexdump(uint8_t *buf, uint32_t len);


/**
 * @brief UDP callback function.
 * 
 * This function is called when a UDP packet is received. It logs the packet
 * information and displays the data as a hexdump.
 * 
 * @param[in] socket UDP socket handle.
 * @param[in] addr Pointer to the source address.
 * @param[in] buf Pointer to the received data.
 * @param[in] len Length of the received data.
 * @return 1 if parsing is successful, 0 otherwise.
 */
uint32_t udp_cb_func(int32_t socket, const NET_ADDR *addr, const uint8_t *buf, uint32_t len) {
	int pr = 0; // parsing result
	
  printf("Socket %d, port %x, len %d\n", socket, addr->port, len);
  printf("Received buffer: -> \n");
  print_hexdump((uint8_t *)buf, len);
	
  if (strstr((char *)buf, "<N1MMRotor>")) {
    if (strstr((char *)buf, "<stop>")) {
      pr = parse_special_xml((char *)buf, &LastRotatorData);
			if (pr) {
			  LastRotatorData.stop = true;
			  printf("Parsed rotor (stop): %s\n", LastRotatorData.rotor);
			  printf("Freqband: %.1f\n", LastRotatorData.freqband);
			} // if (pr)
    } else {  // not "stop", parse normal "goazi..."
      pr = parse_n1mmrotor_xml((char *)buf, &LastRotatorData);
			if (pr) {
				LastRotatorData.stop = false;
			  printf("Parsed rotor: %s\n", LastRotatorData.rotor);
			  printf("Azimuth: %.1f, Offset: %.1f, Bidirectional: %d, Freqband: %.1f\n", LastRotatorData.goazi, LastRotatorData.offset, LastRotatorData.bidirectional, LastRotatorData.freqband);
			}
    }
  }

	if (pr) {
		// notify network task about new command received via UDP
		osEventFlagsSet(network_evt_id, NET_FL_NOTIFY_UDP_RECEIVED);
	}
  return (pr);
}

/**
 * @brief Send a ping message to a predefined host.
 * 
 * This function sends a small UDP packet to a predefined IP address
 * to signal activity.
 * 
 * @param[in] NET_ADDR4 Destination address.
 */
void ping_host(NET_ADDR4 addr) {
  //
  uint8_t *sendbuf;

  sendbuf = netUDP_GetBuffer(2);
  sendbuf[0] = 0x01;
  sendbuf[1] = 0xAA;

  netUDP_Send(udp_sock, (NET_ADDR *)&addr, sendbuf, 2);
}

/**
 * @brief Ethernet event notification handler.
 * 
 * This function processes Ethernet events such as link status changes
 * and wakeup signals.
 * 
 * @param[in] if_num Interface number.
 * @param[in] event Event type.
 * @param[in] val Additional event data.
 */
void netETH_Notify(uint32_t if_num, netETH_Event event, uint32_t val) {
  NET_ETH_LINK_INFO *info;

  printf("ETH IF #%d event: ", if_num);
  switch (event) {
    case netETH_LinkDown:
      printf("Link is down\n");
      link_status = NET_FL_NOTIFY_LINK_DOWN;
      link_speed = 0;
      break;
    case netETH_LinkUp:
      printf("Link is up\n");
      link_status = NET_FL_NOTIFY_LINK_UP;
      info = (NET_ETH_LINK_INFO *)&val;
		  eth_info = *info;
		  osEventFlagsSet(network_evt_id, NET_FL_NOTIFY_LINK_UP);
      switch (info->speed) {
        case 0:
          printf("10 MBit\n");
          link_speed = 10;
          break;
        case 1:
          printf("100 MBit\n");
          link_speed = 100;
          break;
        case 2:
          printf("1 GBit\n");
          link_speed = 1000;
          break;
      }
      switch (info->duplex) {
        case 0:
          printf("Half duplex\n");
          break;
        case 1:
          printf("Full duplex\n");
          break;
      }
      break;
    case netETH_Wakeup:
      printf("Wakeup frame received\n");
      link_status = NET_FL_NOTIFY_WAKEUP;
      break;
    case netETH_TimerAlarm:
      printf("Timer alarm\n");
      link_status = NET_FL_NOTIFY_TIMER_ALARM;
      break;
  }
} 

/**
 * @brief Display a buffer as a hexdump.
 * 
 * This function prints the content of a buffer in hexadecimal and ASCII formats.
 * Each line includes an address offset, the hex representation, and the ASCII characters.
 * 
 * @param[in] buf Pointer to the buffer.
 * @param[in] len Length of the buffer.
 */
void print_hexdump(uint8_t *buf, uint32_t len) {
  const uint32_t bytes_per_line = 16; // Number of bytes to display per line
  for (uint32_t i = 0; i < len; i += bytes_per_line) {
    // Print the address offset
    printf("%08X  ", i);

    // Print the hex representation
    for (uint32_t j = 0; j < bytes_per_line; ++j) {
      if (i + j < len) {
        printf("%02X ", buf[i + j]);
      } else {
        printf("   "); // Padding for shorter lines
      }
    }

    // Add a gap between hex and ASCII
    printf(" ");

    // Print the ASCII representation
    for (uint32_t j = 0; j < bytes_per_line; ++j) {
      if (i + j < len) {
        uint8_t c = buf[i + j];
        // Print printable characters, otherwise use '.'
        printf("%c", (c >= 32 && c <= 126) ? c : '.');
      } else {
        printf(" ");
      }
    }

    // End the line
    printf("\n");
  }
}

/**
 * @brief Initialize the network thread.
 * 
 * This function creates the network thread and starts it.
 * 
 * @return 0 on success, or -1 on error.
 */
int Init_Network_Thread(void) {
  tid_Network_Thread = osThreadNew(Network_Thread, NULL, NULL);
  if (tid_Network_Thread == NULL) {
    return (-1);
  }
	
	network_evt_id = osEventFlagsNew(NULL);
  if (network_evt_id == NULL) {
    return (-1); // Event Flags object not created
  }

  return (0);
}





void Network_IP_Assignment( void ) {

  /* Change host name */
  netSYS_SetHostName (config.host_name);

  /* Change MAC address */
  netIF_SetOption (NET_IF_CLASS_ETH | 0, netIF_OptionMAC_Address, config.mac_addr, NET_ADDR_ETH_LEN);
 
  if (config.DHCP_enabled == false) {
    /* Static configuration mode */ 
    netDHCP_Disable (NET_IF_CLASS_ETH | 0);
 
    /* Change IP address */
    netIF_SetOption (NET_IF_CLASS_ETH | 0, netIF_OptionIP4_Address, config.ip_addr, NET_ADDR_IP4_LEN);
 
    /* Change Network mask */
    netIF_SetOption (NET_IF_CLASS_ETH | 0, netIF_OptionIP4_SubnetMask, config.net_mask, NET_ADDR_IP4_LEN);
 
    /* Change Default Gateway address */
    netIF_SetOption (NET_IF_CLASS_ETH | 0, netIF_OptionIP4_DefaultGateway, config.def_gw, NET_ADDR_IP4_LEN);
 
    /* Change Primary DNS Server address */
    netIF_SetOption (NET_IF_CLASS_ETH | 0, netIF_OptionIP4_PrimaryDNS, config.pri_dns, NET_ADDR_IP4_LEN);
 
    /* Change Secondary DNS Server address */
    netIF_SetOption (NET_IF_CLASS_ETH | 0, netIF_OptionIP4_SecondaryDNS, config.sec_dns, NET_ADDR_IP4_LEN);
  }

}

/**
 * @brief Main function for the network thread.
 * 
 * This function initializes the network stack, waits for a link to be established,
 * sets up a UDP socket, and enters an infinite loop for periodic tasks.
 * 
 * @param[in] argument Unused parameter (reserved for future use).
 */
void Network_Thread(void *argument) {
  (void)argument;
	
	uint32_t flags;

	NET_ADDR4 keepalive_addr = NET_SEND_UDP_ADDRESS;
	
  netInitialize();
	
	Network_IP_Assignment();

  // Waiting for link
	osEventFlagsWait(network_evt_id, NET_FL_NOTIFY_LINK_UP, osFlagsWaitAny, osWaitForever);

  // Initialize UDP socket
  udp_sock = netUDP_GetSocket(udp_cb_func);
  if (udp_sock >= 0) {
    netUDP_Open(udp_sock, 2000);
  }

	// Send one test UDP 
  ping_host(keepalive_addr); // Send something

  while (1) {
		flags = osEventFlagsWait(network_evt_id, NET_FL_NOTIFY_UDP_RECEIVED, osFlagsWaitAny, 100);
		
		if (flags == NET_FL_NOTIFY_UDP_RECEIVED) {
		  Rotator_Set(&LastRotatorData);
			osEventFlagsSet(rotator_evt_id, ROT_NOTIFY_NEW_POSITION);
		}
		
		// do other network stuff here
		
    osThreadYield(); // Suspend thread
  }
}

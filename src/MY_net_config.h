/**
 * @file net_config.h
 * @brief Network configuration settings.
 *
 * This header file defines the default configuration settings for the network.
 *
 * @author S54MTB, Marko Pavlin
 * @date January 2025
 */

#ifndef NETWORK_CONFIG_H
#define NETWORK_CONFIG_H

#ifdef __cplusplus
extern "C" {
#endif

// Network configuration
/** Default UDP send address */
#define NET_SEND_UDP_ADDRESS { NET_ADDR_IP4, 2000, {44, 150, 168, 1} }

#define NET_MAC_ADDR  "00-01-02-32-3c-46" 
#define NET_IP_ADDR   "44.150.168.199"    
#define NET_GW        "44.150.168.1"      
#define NET_MASK      "255.255.255.0"     
#define NET_PRI_DNS   "44.150.168.1"      
#define NET_SEC_DNS   "4.4.4.4"           
#define NET_HOSTNAME  "S54MTB_ROTATOR"    
#define NET_DHCP      false
#define NET_N1MM_IP   "44.150.168.1"
#define NET_N1MMPORT  12345
#define NET_USER  "admin"
#define NET_PASS  "admin"


#ifdef __cplusplus
}
#endif

#endif // NETWORK_CONFIG_H

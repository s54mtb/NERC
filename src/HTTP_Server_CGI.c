/*------------------------------------------------------------------------------
 * MDK Middleware - Component ::Network:Service
 * Copyright (c) 2004-2019 Arm Limited (or its affiliates). All rights reserved.
 *------------------------------------------------------------------------------
 * Name:    HTTP_Server_CGI.c
 * Purpose: HTTP Server CGI Module
 * Rev.:    V7.0.0
 *----------------------------------------------------------------------------*/
//! [code_HTTP_Server_CGI]
#include <stdio.h>
#include <string.h>
#include "rl_net.h"
#include "config.h"
#include "HTTP_Server_CGI.h"


static uint8_t ip_addr[NET_ADDR_IP6_LEN];
static char    ip_string[40];

static char username[20];
static char password1[20];


const struct cmd_st cmd_tbl [] =
{    
    { "mac", 	 CMD_MAC,  }, 
    { "ip", 	 CMD_IP,  }, 
    { "dhcp", 	 CMD_DHCP,  }, 
    { "mask", 	 CMD_MASK,  }, 
    { "gateway", 	 CMD_GATEWAY,  }, 
    { "dns1", 	 CMD_DNS1,  }, 
    { "dns2", 	 CMD_DNS2,  }, 
    { "hostname", 	 CMD_HOSTNAME,  }, 
    { "n1mm_ip", 	 CMD_N1MM_IP,  }, 
    { "n1mm_port", 	 CMD_N1MM_PORT,  }, 
    { "username", 	 CMD_USERNAME,  }, 
    { "password", 	 CMD_PASSWORD,  }, 
    { "password2", 	 CMD_PASSWORD2,  }, 
    { "angle_0", 	 CMD_ANGLE_0,  }, 
    { "angle_1", 	 CMD_ANGLE_1,  }, 
    { "angle_2", 	 CMD_ANGLE_2,  }, 
    { "adc_0", 	 CMD_ADC_0,  }, 
    { "adc_1", 	 CMD_ADC_1,  }, 
    { "adc_2", 	 CMD_ADC_2,  }, 
};

#define CMD_TBL_LEN (sizeof (cmd_tbl) / sizeof (cmd_tbl [0]))


static int cmdid_search (char *cmdstr) {
  const struct cmd_st *ctp;

  for (ctp = cmd_tbl; ctp < &cmd_tbl [CMD_TBL_LEN]; ctp++) {
    if (strcmp (ctp->cmdstr, cmdstr) == 0) return (ctp->id);
  }

  return (CMD_LAST);
}


void cmd_MAC(char *value) {
  uint8_t mac_addr[6];
	if (netMAC_aton (value, mac_addr)) {
	  netIF_SetOption (NET_IF_CLASS_ETH | 0, netIF_OptionMAC_Address, mac_addr, NET_ADDR_ETH_LEN);
		memcpy(config.mac_addr, mac_addr, NET_ADDR_ETH_LEN);
	}
}

void cmd_IP(char *value, target_ip_t target_ip) {
	
	uint8_t ip_addr[NET_ADDR_IP4_LEN];

	if (netIP_aton (value, NET_ADDR_IP4, ip_addr)) {	
		switch (target_ip) {
			case IP_OWN: 
				netIF_SetOption (NET_IF_CLASS_ETH | 0, netIF_OptionIP4_Address, ip_addr, NET_ADDR_IP4_LEN);
			  memcpy(config.ip_addr, ip_addr, NET_ADDR_IP4_LEN);
			break;
			
			case IP_MASK:
				netIF_SetOption (NET_IF_CLASS_ETH | 0, netIF_OptionIP4_SubnetMask, ip_addr, NET_ADDR_IP4_LEN);
			  memcpy(config.net_mask, ip_addr, NET_ADDR_IP4_LEN);
			break;
			
			case IP_GATEWAY: 
			  netIF_SetOption (NET_IF_CLASS_ETH | 0, netIF_OptionIP4_DefaultGateway, ip_addr, NET_ADDR_IP4_LEN);
			  memcpy(config.def_gw, ip_addr, NET_ADDR_IP4_LEN);
			break;
				
			case IP_DNS1:
				netIF_SetOption (NET_IF_CLASS_ETH | 0, netIF_OptionIP4_PrimaryDNS, ip_addr, NET_ADDR_IP4_LEN);
			  memcpy(config.pri_dns, ip_addr, NET_ADDR_IP4_LEN);
			break;
				
			case IP_DNS2:
				netIF_SetOption (NET_IF_CLASS_ETH | 0, netIF_OptionIP4_SecondaryDNS, ip_addr, NET_ADDR_IP4_LEN);
			  memcpy(config.sec_dns, ip_addr, NET_ADDR_IP4_LEN);
			break;
				
			case IP_N1MM:
				memcpy(config.N1MM_IP, ip_addr, NET_ADDR_IP4_LEN);
			break;
				
		}
	} // if...
}



void cmd_PWD(char *value, pwd_seetings_t type) {
	switch (type) {
		case PWD_USER  : strcpy(username, value); break;
		case PWD_PASS1 : strcpy(password1, value); break;
		case PWD_PASS2 : 
			if (strcmp(password1, value) == 0) { // passwords match
				strcpy(config.username, username);
				strcpy(config.password, password1);
			}
		break;
		
		
	}
	
}


static void cmd_proc (uint16_t cmd_id, char *value)
{
  
  switch (cmd_id)
  {    case CMD_MAC:
      cmd_MAC(value);
    break;

    case CMD_IP:
      cmd_IP(value, IP_OWN);
    break;

//    case CMD_DHCP:
//      cmd_DHCP(value);
//    break;

    case CMD_MASK:
      cmd_IP(value, IP_MASK);
    break;

    case CMD_GATEWAY:
      cmd_IP(value, IP_GATEWAY);
    break;

    case CMD_DNS1:
      cmd_IP(value, IP_DNS1);
    break;

    case CMD_DNS2:
      cmd_IP(value, IP_DNS2);
    break;

//    case CMD_HOSTNAME:
//      cmd_HOSTNAME(value);
//    break;

    case CMD_N1MM_IP:
      cmd_IP(value, IP_N1MM);
    break;

//    case CMD_N1MM_PORT:
//      cmd_N1MM_PORT(value);
//    break;

    case CMD_USERNAME:
      cmd_PWD(value, PWD_USER);
    break;

    case CMD_PASSWORD:
      cmd_PWD(value, PWD_PASS1);
    break;

    case CMD_PASSWORD2:
      cmd_PWD(value, PWD_PASS2);
    break;

//    case CMD_ANGLE_0:
//      cmd_ANGLE_0(value);
//    break;

//    case CMD_ANGLE_1:
//      cmd_ANGLE_1(value);
//    break;

//    case CMD_ANGLE_2:
//      cmd_ANGLE_2(value);
//    break;

//    case CMD_ADC_0:
//      cmd_ADC_0(value);
//    break;

//    case CMD_ADC_1:
//      cmd_ADC_1(value);
//    break;

//    case CMD_ADC_2:
//      cmd_ADC_2(value);
//    break;

  }
}




 
// \brief Process query string received by GET request.
// \param[in]     qstr          pointer to the query string.
void netCGI_ProcessQuery (const char *qstr) {

  char var[40];
	char *param;
	char *value;
	
	int id;
	
  do {
    // Loop through all the parameters
    qstr = netCGI_GetEnvVar (qstr, var, sizeof (var));
    // Check return string, 'qstr' now points to the next parameter

		value = strchr(var, '=');
		
		if (value != NULL) {
			// Null terminate the parameter part
      *value = '\0';
			param = var;
			// Move the value pointer past the '=' character
      value++;
			// Now find the parameter from the list
			id = cmdid_search (param);
			// process the param=value
			cmd_proc(id, value);
		}
		
  } while (qstr);
}
 
// \brief Process data received by POST request.
// \param[in]     code          callback context:
//                              - 0 = www-url-encoded form data,
//                              - 1 = filename for file upload (null-terminated string),
//                              - 2 = file upload raw data,
//                              - 3 = end of file upload (file close requested),
//                              - 4 = any other type of POST data (single or last stream),
//                              - 5 = the same as 4, but with more data to follow.
// \param[in]     data          pointer to POST data.
// \param[in]     len           length of POST data.
void netCGI_ProcessData (uint8_t code, const char *data, uint32_t len) {
	
	(void) len;
	
  char var[40];
	
	if (code != 0) {
    // Ignore all other codes
    return;
  }
 
	char *param;
	char *value;
	
	int id;
	
  do {
    // Loop through all the parameters
    data = netCGI_GetEnvVar (data, var, sizeof (var));
    // Check return string, 'data' now points to the next parameter

		value = strchr(var, '=');
		
		if (value != NULL) {
			// Null terminate the parameter part
      *value = '\0';
			param = var;
			// Move the value pointer past the '=' character
      value++;
			// Now find the parameter from the list
			id = cmdid_search (param);
			// process the param=value
			cmd_proc(id, value);
		}
		
  } while (data);
}
 
// \brief Generate dynamic web data based on a CGI script.
// \param[in]     env           environment string.
// \param[out]    buf           output data buffer.
// \param[in]     buf_len       size of output buffer (from 536 to 1440 bytes).
// \param[in,out] pcgi          pointer to a session's local buffer of 4 bytes.
//                              - 1st call = cleared to 0,
//                              - 2nd call = not altered by the system,
//                              - 3rd call = not altered by the system, etc.
// \return        number of bytes written to output buffer.
//                - return len | (1U<<31) = repeat flag, the system calls this function
//                                          again for the same script line.
//                - return len | (1U<<30) = force transmit flag, the system transmits
//                                          current packet immediately.
uint32_t netCGI_Script (const char *env, char *buf, uint32_t buf_len, uint32_t *pcgi) {
  (void) buf_len;
	(void) pcgi;

//  int32_t socket;
//  netTCP_State state;
//  NET_ADDR r_client;
//  const char *lang;
  uint32_t len = 0U;
//  uint8_t id;
//  static uint32_t adv;
  netIF_Option opt = netIF_OptionMAC_Address;
  int16_t      typ = NET_ADDR_IP4;
		
  switch (env[0]) {
    // Analyze a 'c' script line starting position 2
    case 'a' : // ----------------------------------------- NETWORK SETTINGS
      
      switch (env[2]) {

        case 'i':
          // Write local IP address (IPv4 or IPv6)
          opt = netIF_OptionIP4_Address;
          break;

        case 'm':
          // Write local network mask
          opt = netIF_OptionIP4_SubnetMask;
          break;

        case 'g':
          // Write default gateway IP address
          opt = netIF_OptionIP4_DefaultGateway;
          break;

        case 'p':
          // Write primary DNS server IP address
          opt = netIF_OptionIP4_PrimaryDNS;
          break;

        case 's':
          // Write secondary DNS server IP address
          opt = netIF_OptionIP4_SecondaryDNS;
          break;
				
				  // Write host name
				case 'h': 
					len = (uint32_t)sprintf (buf, &env[4], netSYS_GetHostName ());
				  return (len);
				  break;
				
				  // Write N1MM IP address
				case 'n': 
					netIP_ntoa (typ, config.N1MM_IP , ip_string, sizeof(ip_string));
					len = (uint32_t)sprintf (buf, &env[4], ip_string);
				  return (len);
				  break;				
				
				  // Write N1MM port
				case 'r': 
					len = (uint32_t)sprintf (buf, &env[4], config.N1MM_Port);
				  return (len);
				  break;				

				  // Write MAC address
				case 'v': 					
				  netMAC_ntoa (config.mac_addr, ip_string, sizeof(ip_string));
					len = (uint32_t)sprintf (buf, &env[4], ip_string);
				  return (len);
				  break;				

				  // Write MAC address
				case 'j': 					
				  if (config.DHCP_enabled) {
						sprintf(ip_string,"checked");
					} else {
						sprintf(ip_string,"unchecked");						
					}
					len = (uint32_t)sprintf (buf, &env[4], ip_string);
				  return (len);
				  break;				

      }

      netIF_GetOption (NET_IF_CLASS_ETH, opt, ip_addr, sizeof(ip_addr));
      netIP_ntoa (typ, ip_addr, ip_string, sizeof(ip_string));
      len = (uint32_t)sprintf (buf, &env[4], ip_string);
      break;
			
    case 'b' : // ----------------------------------------- SYSTEM SETTINGS
      
      switch (env[2]) {

        case 'u':
          // Username
          len = (uint32_t)sprintf (buf, &env[4],config.username);
				  return len;
          break;
				
				case 'p' :
				case 'q' :
          len = (uint32_t)sprintf (buf, &env[4],config.password);
				  return len;
				  break;
				
			}

			
	}

  return (len);
}
 
// \brief Override default Content-Type for CGX script files.
// \return        pointer to user defined Content-Type.
const char *netCGX_ContentType (void) {
  /* Example
  return ("text/xml; charset=utf-8");
  */
  return (NULL);
}
 
// \brief Override default character encoding in html documents.
// \return        pointer to user defined character set type.
const char *netCGI_Charset (void) {
  /* Example
  return ("utf-8");
  */
  return (NULL);
}
//! [code_HTTP_Server_CGI]

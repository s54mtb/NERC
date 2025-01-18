#include <string.h>
#include "config.h"
#include "MY_net_config.h"

ConfigStruct config = {0}; 

#define CRC32_POLY 0xEDB88320  // CRC32 Polynomial (Ethernet standard)


// CRC32 Table Generation (Static Array)
static uint32_t crc32_table[256];
static int crc32_initialized = 0;

static void config_copy(ConfigStruct *config, ConfigStruct *new_config);
static void generate_crc32_table(void);
static uint32_t calculate_crc32(const uint8_t *data, size_t length);	
void config_reset(ConfigStruct *config);
void config_init(ConfigStruct *config);


static void generate_crc32_table(void) {
  for (uint32_t i = 0; i < 256; i++) {
    uint32_t crc = i;
    for (uint32_t j = 8; j > 0; j--) {
      if (crc & 1) {
        crc = (crc >> 1) ^ CRC32_POLY;
      } else {
        crc = crc >> 1;
      }
    }
    crc32_table[i] = crc;
  }
	
	crc32_initialized = 1;
}


static uint32_t calculate_crc32(const uint8_t *data, size_t length) {
  uint32_t crc = 0xFFFFFFFF;
  for (size_t i = 0; i < length; i++) {
    uint8_t byte = data[i];
    uint32_t lookup_index = (crc ^ byte) & 0xFF;
    crc = (crc >> 8) ^ crc32_table[lookup_index];
  }
  return ~crc;
}



uint32_t config_CalculateChecksum(ConfigStruct *config) {

	ConfigStruct c;
	config_copy(config, &c);  // local copy
	c.checksum = 0;	          // clear checksum, allways calculate with checksum=0
	
  if (!crc32_initialized) {
    generate_crc32_table();
  }

  // Convert the entire ConfigStruct to byte array and calculate CRC32
  uint8_t *byte_ptr = (uint8_t *)&c;
  size_t struct_size = sizeof(ConfigStruct);

  return calculate_crc32(byte_ptr, struct_size);
}


static void config_copy(ConfigStruct *config, ConfigStruct *new_config) {
    // Use memcpy to copy the entire structure
    memcpy(new_config, config, sizeof(ConfigStruct));
}



void config_init(ConfigStruct *config) {
	generate_crc32_table();
	
	if (config_CalculateChecksum(config) != config->checksum) {
		config_reset(config);
	}
}


void config_reset(ConfigStruct *config) {
  // Set default values for all fields
	
	const char mac_addr[]  = { NET_MAC_ADDR };
	const char ip_addr[]   = { NET_IP_ADDR };
	const char def_gw[]    = { NET_GW };
	const char net_mask[]  = { NET_MASK };
	const char pri_dns[]   = { NET_PRI_DNS };
	const char sec_dns[]   = { NET_SEC_DNS };
	const char host_name[] = { NET_HOSTNAME };
	const char n1mm[]      = { NET_N1MM_IP };
	bool DHCP_enabled      = { NET_DHCP };	
	const char username[]  = { NET_USER };
	const char password[]  = { NET_PASS };
	

  config->PID_MaxSteps = 1000;  // Default  value
	config->PID_pid.integral = 0;
	config->PID_pid.kd = 0;
	config->PID_pid.ki = 0;
	config->PID_pid.kp = 1;
	
  config->MOT_SpeedMin = -1.0f;   // Default value
  config->MOT_SpeedMax = +1.0f;  // Default value
  config->MOT_FinalPositionError = 0.005f;  // Default value

  config->Rot_NumCalibPoints = 3; // Default value
	
	netMAC_aton (mac_addr, config->mac_addr);  // Set MAC address
	netIP_aton (ip_addr, NET_ADDR_IP4, config->ip_addr);  // Set IP address

	netIP_aton (def_gw, NET_ADDR_IP4, config->def_gw);  // Set IP address
	netIP_aton (net_mask, NET_ADDR_IP4, config->net_mask);  // Set mask
	netIP_aton (pri_dns, NET_ADDR_IP4, config->pri_dns);  // Set primary DNS IP address
	netIP_aton (sec_dns, NET_ADDR_IP4, config->sec_dns);  // Set secondary DNS IP address
	netIP_aton (n1mm, NET_ADDR_IP4, config->N1MM_IP);  // Set N1MM IP address

  // Set default host name
  strncpy(config->host_name, host_name, sizeof(config->host_name) - 1);
  config->host_name[sizeof(config->host_name) - 1] = '\0'; // Ensure null termination

  config->N1MM_Port = NET_N1MMPORT;  // Set default port for N1MM

  config->DHCP_enabled = DHCP_enabled;  // Default to DHCP enabled

  // Set default user name
  strncpy(config->username, username, sizeof(config->username) - 1);
  config->username[sizeof(config->username) - 1] = '\0'; // Ensure null termination
	
  // Set default password
  strncpy(config->password, password, sizeof(config->password) - 1);
  config->password[sizeof(config->password) - 1] = '\0'; // Ensure null termination

  config->checksum = 0;  // Initially set checksum to 0
}



//// Flash memory address for storing the structure (choose a proper address within available flash)
//#define FLASH_USER_START_ADDR 0x08020000  // Example start address
//#define FLASH_USER_END_ADDR   0x080207FF  // Example end address


//// Function prototypes
//HAL_StatusTypeDef Flash_Write_Config(const ConfigStruct *config);
//HAL_StatusTypeDef Flash_Read_Config(ConfigStruct *config);

//HAL_StatusTypeDef Flash_Write_Config(const ConfigStruct *config) {
//    HAL_StatusTypeDef status;
//    uint32_t address = FLASH_USER_START_ADDR;

//    // Unlock the Flash to enable write access
//    HAL_FLASH_Unlock();

//    // Erase the Flash sector
//    FLASH_EraseInitTypeDef eraseInitStruct = {0};
//    uint32_t sectorError = 0;

//    eraseInitStruct.TypeErase = FLASH_TYPEERASE_SECTORS;
//    eraseInitStruct.Banks = FLASH_BANK_1;
//    eraseInitStruct.Sector = FLASH_SECTOR_5; // Adjust based on the address
//    eraseInitStruct.NbSectors = 1;
////    eraseInitStruct.VoltageRange = FLASH_VOLTAGE_RANGE_3;

//    status = HAL_FLASHEx_Erase(&eraseInitStruct, &sectorError);
//    if (status != HAL_OK) {
//        HAL_FLASH_Lock();
//        return status;
//    }

//    // Write the structure to flash memory
//    uint64_t *data = (uint64_t *)config;
//    size_t size = sizeof(ConfigStruct);

//    for (size_t i = 0; i < size; i += 8) {
// //       status = HAL_FLASH_Program(FLASH_TYPEPROGRAM_DOUBLEWORD, address, *(data++));
//        if (status != HAL_OK) {
//            HAL_FLASH_Lock();
//            return status;
//        }
//        address += 8;
//    }

//    // Lock the Flash to disable the write access
//    HAL_FLASH_Lock();

//    return HAL_OK;
//}

//HAL_StatusTypeDef Flash_Read_Config(ConfigStruct *config) {
//    // Copy the configuration from flash memory
//    memcpy(config, (void *)FLASH_USER_START_ADDR, sizeof(ConfigStruct));
//    return HAL_OK;
//}


/**
 * @file config.h
 * @brief This header file defines the configuration structure for storing configuration data in flash memory.
 *
 * The structure holds all configuration parameters needed for system initialization and operation,
 * including settings for the PID controller, motor speeds, calibration points, network settings,
 * and N1MM (a logging software) parameters. This data is saved to flash memory to persist across resets.
 */

#ifndef CONFIG__H
#define CONFIG__H

#ifdef __cplusplus
extern "C" {
#endif

#include "main.h"
#include "rotator.h"
#include <stdint.h>  // For uint32_t, uint8_t
#include "rl_net.h"
/** 
 * @brief Configuration structure to store settings in flash.
 * 
 * This structure holds various parameters such as PID settings, motor speed limits, calibration points,
 * network configuration, and N1MM settings. The structure will be saved and loaded from flash memory.
 */
typedef struct {
	uint32_t PID_MaxSteps;              /**< Maximum number of steps for PID control */
	PIDController PID_pid;              /**< PID controller configuration */
	float MOT_SpeedMin;                 /**< Minimum motor speed */
	float MOT_SpeedMax;                 /**< Maximum motor speed */
	float MOT_FinalPositionError;       /**< Final position error threshold for motor */
	uint8_t Rot_NumCalibPoints;         /**< Number of calibration points for rotation */
	rot_point_t calib_points[ROT_MAX_CALIB_POINTS]; /**< Array of calibration points for rotation */
	uint8_t mac_addr[6];                 /**< MAC address  */
	uint8_t  ip_addr[4];                   /**< IP address  */
	uint8_t  def_gw[4];                    /**< Default gateway IP  */
	uint8_t  net_mask[4];                  /**< Network mask  */
	uint8_t  pri_dns[4];                   /**< Primary DNS server IP  */
	uint8_t  sec_dns[4];                   /**< Secondary DNS server IP  */
	char host_name[32];                 /**< Hostname for the system */
	uint8_t  N1MM_IP[4];                   /**< IP address for N1MM software communication */
	uint16_t N1MM_Port;                 /**< Port number for N1MM software communication */
	uint8_t DHCP_enabled;               /**< Flag to indicate if DHCP is enabled (0 = disabled, 1 = enabled) */
	char   password[NET_PASSWORD_LEN];
	char   username[NET_USERNAME_LEN];
	uint32_t checksum;                  /**< Checksum for configuration validation */
} ConfigStruct;

extern ConfigStruct config;

void config_reset(ConfigStruct *config);
void config_init(ConfigStruct *config);


#ifdef __cplusplus
}
#endif

#endif // CONFIG__H

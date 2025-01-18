/**
 * @file rtx_init.c
 * @brief Implementation of the RTX initialization and application init thread.
 * 
 * This file provides functions for initializing the CMSIS-RTOS kernel and
 * creating an application initialization thread. It also includes the 
 * implementation of the application initialization logic.
 * 
 * @author S54MTB, Marko Pavlin
 * @date January 2025
 * @version 1.0
 */

#include "cmsis_os2.h"
#include "main.h"
#include "config.h"

// External references network initialization
extern int Init_Network_Thread(void);
extern int Init_rotator_Thread (void);

// Thread ID for the initialization thread
osThreadId_t tid_Init_Thread;

/**
 * @brief Application initialization thread.
 * 
 * This function performs application-specific initialization,
 * including starting all application threads. Once the initialization is complete,
 * it terminates itself.
 * 
 * @param[in] argument Unused parameter (reserved for future use).
 * @return This function does not return.
 */
 
__NO_RETURN static void app_init(void *argument) {
  (void)argument; // Suppress unused parameter warning

  osStatus_t status;
	
	config_init(&config);


  // Initialize the network thread
  Init_Network_Thread();
	
	// Initialize the rotator thread
	Init_rotator_Thread();

  // Terminate this initialization thread
  status = osThreadTerminate(tid_Init_Thread);
  if (status != osOK) {
    Error_Handler(); // Handle error if thread termination fails
  }

  for (;;) {} // Prevent compiler warnings for non-returning function
}



/**
 * @brief Initialize CMSIS-RTOS and start the application.
 * 
 * This function initializes the CMSIS-RTOS kernel, creates the application
 * initialization thread, and starts thread execution.
 */
void rtx_init(void) {
  osKernelInitialize(); // Initialize CMSIS-RTOS kernel

  // Create the application initialization thread
  tid_Init_Thread = osThreadNew(app_init, NULL, NULL);

  // Start thread execution
  osKernelStart();

}

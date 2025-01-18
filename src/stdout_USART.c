/**
 * @file main_stdout.c
 * @brief Implementation of standard output functions for UART communication.
 * 
 * This file contains functions to initialize and use standard output
 * for UART communication, specifically configured for USART3. It uses
 * the CMSIS-RTOS2 and HAL libraries.
 * 
 * @author S54MTB, Marko Pavlin
 * @date January 2025
 * @version 1.0
 */

#include "main.h"
#include "cmsis_os2.h"          // CMSIS:RTOS2

// External UART handle for USART3, defined elsewhere (in main.c)
extern UART_HandleTypeDef huart3;

/**
 * @brief Initialize standard output.
 * 
 * This function initializes the standard output mechanism. In this case,
 * the initialization is handled by the HAL configuration generated in
 * CubeMX, so this function simply returns success.
 * 
 * @return 0 on success, or -1 on error.
 */
int stdout_init (void) {
  // UART initialization is managed by CubeMX in the MX_USART3_UART_Init function.
  return (0);
}

/**
 * @brief Write a character to the standard output.
 * 
 * This function transmits a single character via USART3 using the HAL_UART_Transmit function.
 * 
 * @param[in] ch The character to output.
 * @return The character written on success, or -1 on write error.
 */
int stdout_putchar (int ch) {
  uint8_t buf[1];

  buf[0] = ch; // Store the character in the buffer

  // Transmit the character via UART and check for errors
  if (HAL_UART_Transmit(&huart3, (uint8_t *)&ch, 1, 0xFFFF) != HAL_OK) {
    return (-1); // Return -1 if transmission fails
  }

  return (ch); // Return the transmitted character on success
}

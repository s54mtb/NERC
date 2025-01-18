/**
 * @file adc.h
 * @brief Header file for ADC utility functions.
 * 
 * This header file declares functions to configure and read ADC channels
 * and compute derived values such as potentiometer position and motor current.
 */

#ifndef ADC_FUNCTIONS_H
#define ADC_FUNCTIONS_H

#include "main.h"

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @brief Reads the ADC value of a specified channel.
 * 
 * Configures the ADC to read the specified channel, starts the conversion,
 * and retrieves the converted value.
 * 
 * @param hadc Pointer to the ADC handle.
 * @param Channel ADC channel to be read.
 * @return The ADC reading (uint32_t) on success, or -1 on failure.
 */
uint32_t ReadADC_ch(ADC_HandleTypeDef *hadc, uint32_t Channel);

/**
 * @brief Calculates the current position of a potentiometer.
 * 
 * Determines the ratiometric position of a potentiometer based on its ADC
 * reading and a reference ADC channel. The result is a normalized value between
 * 0.0f and 1.0f, or -1.0f in case of an error.
 * 
 * @param hadc Pointer to the ADC handle.
 * @param pot_ch ADC channel connected to the potentiometer.
 * @param ref_ch ADC channel used as the reference.
 * @param minref Minimum acceptable reference value.
 * @return Normalized potentiometer position (0.0f to 1.0f) or -1.0f on error.
 */
float current_position(ADC_HandleTypeDef *hadc, uint32_t pot_ch, uint32_t ref_ch, uint32_t minref);

/**
 * @brief Calculates the motor current from ADC readings.
 * 
 * Computes the motor current in amperes based on the ADC reading of the
 * current sensor, an offset value, and a scaling factor.
 * 
 * @param hadc Pointer to the ADC handle.
 * @param current_ch ADC channel connected to the current sensor.
 * @param adc_offset Offset to subtract from the ADC reading.
 * @param scale Scaling factor to convert ADC value to amperes.
 * @return Motor current in amperes.
 */
float motor_current(ADC_HandleTypeDef *hadc, uint32_t current_ch, uint32_t adc_offset, float scale);

#ifdef __cplusplus
}
#endif

#endif // ADC_FUNCTIONS_H

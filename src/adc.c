/**
 * @file adc.c
 * @brief ADC reading and utility functions for current measurement and position calculation.
 * 
 * This file contains functions to configure and read ADC channels and compute derived
 * values such as potentiometer position and motor current. These utilities are designed
 * for use with STM32 HAL.
 */

#include "main.h"
#include "adc.h"

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
uint32_t ReadADC_ch(ADC_HandleTypeDef *hadc, uint32_t Channel) {

  ADC_ChannelConfTypeDef sConfig = {0};
  uint32_t adc_reading = -1;

  // Configure the ADC channel
  sConfig.Channel = Channel;
  sConfig.Rank = ADC_REGULAR_RANK_1;
  sConfig.SamplingTime = ADC_SAMPLETIME_247CYCLES_5;
  if (HAL_ADC_ConfigChannel(hadc, &sConfig) != HAL_OK) {
    return -1;
  }

  // Start ADC conversion
  HAL_ADC_Start(hadc);
  if (HAL_ADC_PollForConversion(hadc, HAL_MAX_DELAY) == HAL_OK) {
    adc_reading = HAL_ADC_GetValue(hadc);
  }
  HAL_ADC_Stop(hadc);

  return adc_reading;
}

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
float current_position(ADC_HandleTypeDef *hadc, uint32_t pot_ch, uint32_t ref_ch, uint32_t minref) {
  // Position is between 0.0f and 1.0f (ratiometric value of potentiometer)
  float position = -1.0f; 
  uint32_t pot_adc_reading = ReadADC_ch(hadc, pot_ch);
  uint32_t ref_adc_reading = ReadADC_ch(hadc, ref_ch);

  if (ref_adc_reading < minref) return -1.0f;

  if (ref_adc_reading != 0) {
    position = (float)pot_adc_reading / (float)ref_adc_reading;
  } else {
    return -1.0f;
  }

  return position;
}

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
float motor_current(ADC_HandleTypeDef *hadc, uint32_t current_ch, uint32_t adc_offset, float scale) {
  uint32_t motor_current_adc_reading = ReadADC_ch(hadc, current_ch);

  // Calculate current: subtract offset and multiply by scale (Amperes per ADC digit)
  return (float)(motor_current_adc_reading - adc_offset) * scale;
}

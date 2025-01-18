/**
 * @file drv8701.h
 * @brief Header file for DRV8701 motor driver.
 *
 * This header provides function prototypes and includes required for controlling
 * the DRV8701 motor driver using PWM signals.
 */

#ifndef DRV8701_H
#define DRV8701_H

#ifdef __cplusplus
extern "C" {
#endif

#include "main.h"

typedef struct 
{
	uint8_t    initialized;
  uint16_t 	 max_pwm;
} motor_status_t;


/**
 * @brief Set the PWM duty cycle for a specific channel.
 * @param htim Pointer to the TIM handle.
 * @param channel Timer channel to update.
 * @param pulse Pulse width value (duty cycle) to set.
 */
void SetPWMDutyCycle(TIM_HandleTypeDef *htim, uint32_t channel, uint32_t pulse);

/**
 * @brief Initialize the motor driver and start PWM channels.
 * @param timer Pointer to the TIM handle.
 * @param ch1 Timer channel 1 for the motor.
 * @param ch2 Timer channel 2 for the motor.
 */
void motor_init(TIM_HandleTypeDef *timer, uint32_t ch1, uint32_t ch2);

/**
 * @brief Deinitialize the motor driver and stop PWM channels.
 * @param timer Pointer to the TIM handle.
 * @param ch1 Timer channel 1 for the motor.
 * @param ch2 Timer channel 2 for the motor.
 */
void motor_deinit(TIM_HandleTypeDef *timer, uint32_t ch1, uint32_t ch2);

/**
 * @brief Set the motor to coast mode (0% duty cycle).
 * @param timer Pointer to the TIM handle.
 * @param ch1 Timer channel 1 for the motor.
 * @param ch2 Timer channel 2 for the motor.
 */
void motor_coast(TIM_HandleTypeDef *timer, uint32_t ch1, uint32_t ch2);

/**
 * @brief Set the motor to brake mode (100% duty cycle).
 * @param timer Pointer to the TIM handle.
 * @param ch1 Timer channel 1 for the motor.
 * @param ch2 Timer channel 2 for the motor.
 */
void motor_brake(TIM_HandleTypeDef *timer, uint32_t ch1, uint32_t ch2);

/**
 * @brief Set the motor speed with a range of -10000 to 10000.
 *
 * This function adjusts the PWM duty cycle to control motor speed and direction.
 * A negative speed value reverses the motor direction, while a positive value
 * drives it forward. The function limits the speed to the range [-10000, 10000].
 *
 * @param timer Pointer to the TIM handle.
 * @param ch1 Timer channel 1 for the motor.
 * @param ch2 Timer channel 2 for the motor.
 * @param speed Speed value ranging from -10000 to 10000.
 */
void motor_speed(TIM_HandleTypeDef *timer, uint32_t ch1, uint32_t ch2, int speed);


/**
 * @brief Checks if the motor is initialized.
 *
 * This function returns the initialization status of the motor.
 *
 * @return uint8_t Returns 1 if the motor is initialized, 0 otherwise.
 */
uint8_t is_motor_initialized(void);

#ifdef __cplusplus
}
#endif

#endif // DRV8701_H

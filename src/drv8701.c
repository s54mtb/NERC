/**
 * @file drv8701.c
 * @brief Motor control driver for DRV8701 using STM32 HAL.
 *
 * This file provides functions to initialize, control, and deinitialize the motor
 * driver DRV8701 using PWM signals.
 */

#include "main.h"
#include "drv8701.h"

static motor_status_t motor_status;

/**
 * @brief Set the PWM duty cycle for a specific channel.
 * @param htim Pointer to the TIM handle.
 * @param channel Timer channel to update.
 * @param pulse Pulse width value (duty cycle) to set.
 */
void SetPWMDutyCycle(TIM_HandleTypeDef *htim, uint32_t channel, uint32_t pulse) {
  // Change the pulse value for the specified channel
  __HAL_TIM_SET_COMPARE(htim, channel, pulse);
}

/**
 * @brief Initialize the motor driver and start PWM channels.
 * @param timer Pointer to the TIM handle.
 * @param ch1 Timer channel 1 for the motor.
 * @param ch2 Timer channel 2 for the motor.
 */
void motor_init(TIM_HandleTypeDef *timer, uint32_t ch1, uint32_t ch2) {
	motor_status.initialized = 1;	 
  // Start PWM channels
  HAL_TIM_PWM_Start(timer, ch1);
  HAL_TIM_PWM_Start(timer, ch2);

  motor_brake(timer, ch1, ch2);

  // Disable sleep
  HAL_GPIO_WritePin(BDC_SLEEP_GPIO_Port, BDC_SLEEP_Pin, GPIO_PIN_SET);
}

/**
 * @brief Deinitialize the motor driver and stop PWM channels.
 * @param timer Pointer to the TIM handle.
 * @param ch1 Timer channel 1 for the motor.
 * @param ch2 Timer channel 2 for the motor.
 */
void motor_deinit(TIM_HandleTypeDef *timer, uint32_t ch1, uint32_t ch2) {
	motor_status.initialized = 0;	 
  // Enable sleep
  HAL_GPIO_WritePin(BDC_SLEEP_GPIO_Port, BDC_SLEEP_Pin, GPIO_PIN_RESET);

  // Stop PWM channels
  HAL_TIM_PWM_Stop(timer, ch1);
  HAL_TIM_PWM_Stop(timer, ch2);

  // Set 0% duty cycle - coasting
  SetPWMDutyCycle(timer, ch1, 0);
  SetPWMDutyCycle(timer, ch2, 0);
}

/**
 * @brief Set the motor to coast mode (0% duty cycle).
 * @param timer Pointer to the TIM handle.
 * @param ch1 Timer channel 1 for the motor.
 * @param ch2 Timer channel 2 for the motor.
 */
void motor_coast(TIM_HandleTypeDef *timer, uint32_t ch1, uint32_t ch2) {
  // Set 0% duty cycle - coasting
  SetPWMDutyCycle(timer, ch1, 0);
  SetPWMDutyCycle(timer, ch2, 0);
}

/**
 * @brief Set the motor to brake mode (100% duty cycle).
 * @param timer Pointer to the TIM handle.
 * @param ch1 Timer channel 1 for the motor.
 * @param ch2 Timer channel 2 for the motor.
 */
void motor_brake(TIM_HandleTypeDef *timer, uint32_t ch1, uint32_t ch2) {
  // Set 100% duty cycle - braking
  SetPWMDutyCycle(timer, ch1, 10000);
  SetPWMDutyCycle(timer, ch2, 10000);
}



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
void motor_speed(TIM_HandleTypeDef *timer, uint32_t ch1, uint32_t ch2, int speed) {
  // Limit the speed to the range -10000 to 10000
  if (speed < -10000) speed = -10000;
  if (speed > 10000) speed = 10000;

  if (speed < 0) {
    // Reverse direction: Full duty on ch1, reduced duty on ch2
    speed = -speed;
    SetPWMDutyCycle(timer, ch1, 10000);
    SetPWMDutyCycle(timer, ch2, 10000 - speed);
  } else {
    // Forward direction: Reduced duty on ch1, full duty on ch2
    SetPWMDutyCycle(timer, ch1, 10000 - speed);
    SetPWMDutyCycle(timer, ch2, 10000);
  }
}



/**
 * @brief Checks if the motor is initialized.
 *
 * This function returns the initialization status of the motor.
 *
 * @return uint8_t Returns 1 if the motor is initialized, 0 otherwise.
 */
uint8_t is_motor_initialized(void) {
	return motor_status.initialized;
}

/**
 * @file rotator_thread.c
 * @brief Implementation of the rotator control system.
 *
 * This file contains the implementation of the rotator thread, calibration
 * point handling, and functions to calculate voltage from an angle using
 * piecewise linear interpolation.
 * 
 * @author S54MTB, Marko Pavlin
 * @date January 2025
 * @version 1.0
 */

#include "cmsis_os2.h"              // CMSIS RTOS header file
#include "rotator.h"
#include "main.h"
#include "adc.h"
#include "drv8701.h"
#include <string.h>
#include <stdio.h>

/*** configuration settings for rotator PWM ***/
extern TIM_HandleTypeDef htim3;            // timer handler for PWM
TIM_HandleTypeDef *rotator_pwm = &htim3;   // pointer to timer structure
uint32_t pwm_ch1 = TIM_CHANNEL_3;          // PWM channel connected to motor driver IN1
uint32_t pwm_ch2 = TIM_CHANNEL_4;          // PWM channel connected to motor driver IN2

#define PID_MAX_STEPS   10000

/*** configuration settings for rotator ADC ***/
extern ADC_HandleTypeDef hadc1;
ADC_HandleTypeDef *rot_adc = &hadc1;
uint32_t pos_pot_ch = ADC_CHANNEL_12;
uint32_t pos_ref_ch = ADC_CHANNEL_13;
uint32_t mot_current_ch = ADC_CHANNEL_15;


void Rotator_stop(void);


/********************** PID functions **************************/
// Initialize PID controller
void PID_Init(PIDController *pid, float kp, float ki, float kd) {
    pid->kp = kp;
    pid->ki = ki;
    pid->kd = kd;
    pid->integral = 0.0;
    pid->prev_error = 0.0;
}

// Update the PID controller and calculate motor speed
float PID_Update(PIDController *pid, float target, float current, float dt) {
    float error = target - current;
    pid->integral += error * dt;
    float derivative = (error - pid->prev_error) / dt;
    pid->prev_error = error;

    // PID formula
    float output = pid->kp * error + pid->ki * pid->integral + pid->kd * derivative;
    return output;
}


// Clamp motor speed to allowable range
float clamp(float value, float min, float max) {
    if (value > max) return max;
    if (value < min) return min;
    return value;
}


int MoveToNewPosition_PID(float target_position) { 
	PIDController pid;
	PID_Init(&pid, 90000, 8000, -1000);
	motor_init(rotator_pwm, pwm_ch1, pwm_ch2);
	
	int i;
	float motor_sp = 0.0; // Motor speed (-10000 to 10000)
	float dt = 0.01; // Time step
	float curr_position = current_position(rot_adc, pos_pot_ch, pos_ref_ch, 100);
		
	for (i=0; i < PID_MAX_STEPS; i++) {
		// Update motor speed using PID controller
		motor_sp = PID_Update(&pid, target_position, curr_position, dt);
		// Clamp motor speed to range -10000 to 10000
		motor_sp = clamp(motor_sp, -10000, 10000);
		motor_speed(rotator_pwm, pwm_ch1, pwm_ch2, motor_sp);
		// Update current position
		curr_position = current_position(rot_adc, pos_pot_ch, pos_ref_ch, 100);
		
		printf("Step %05d: position: %5.3f speed: %6.2f \n", i, curr_position, motor_sp);
		
		if (fabs(curr_position-target_position) < 0.001) break;
		osDelay(1);
		
	}
	Rotator_stop();
	
	return (i<PID_MAX_STEPS) ? 0 : -1;
	
}


/**
 * @brief Structure to store rotator data.
 */
static N1MMRotor RotatorData = {0};

/**
 * @brief Array to store calibration points for the rotator.
 */
static rot_point_t calib_points[ROT_MAX_CALIB_POINTS] = {0};

/**
 * @brief Thread ID for the rotator thread.
 */
osThreadId_t tid_rotator_Thread;

/**
 * @brief Event flags ID for the rotator.
 */
osEventFlagsId_t rotator_evt_id;

/**
 * @brief Rotator thread function.
 *
 * @param argument Pointer to thread argument (unused).
 */
void Rotator_Thread (void *argument);

/**
 * @brief Set the rotator data.
 *
 * This function copies the rotator settings from the provided structure
 * to the internal RotatorData structure.
 *
 * @param rot Pointer to an N1MMRotor structure containing rotator settings.
 */
void Rotator_Set(N1MMRotor *rot) {
  RotatorData.bidirectional = rot->bidirectional;
  RotatorData.freqband = rot->freqband;
  RotatorData.goazi = rot->goazi;
  RotatorData.offset = rot->offset;
  strcpy(RotatorData.rotor, rot->rotor);
  RotatorData.stop = rot->stop;
}

/**
 * @brief Calculate voltage using piecewise linear (PWL) interpolation.
 *
 * This function computes the voltage corresponding to a given angle
 * based on predefined calibration points.
 *
 * @param angle The angle for which voltage is to be calculated.
 * @return The calculated voltage, or -1 on error.
 */
double rot_get_voltage_from_angle(double angle) {
  if (ROT_MAX_CALIB_POINTS < 2) {
    printf("Error: Not enough calibration points for interpolation.\n");
    return -1; // Error code or handle appropriately
  }

  if (angle <= calib_points[0].angle) {
    return calib_points[0].voltage;
  }
  if (angle >= calib_points[ROT_MAX_CALIB_POINTS - 1].angle) {
    return calib_points[ROT_MAX_CALIB_POINTS - 1].voltage;
  }

  for (int i = 0; i < ROT_MAX_CALIB_POINTS - 1; i++) {
    if (angle >= calib_points[i].angle && angle <= calib_points[i + 1].angle) {
      double t = (angle - calib_points[i].angle) /
             (calib_points[i + 1].angle - calib_points[i].angle);
      return calib_points[i].voltage +
           t * (calib_points[i + 1].voltage - calib_points[i].voltage);
    }
  }

  printf("Error: Angle not within calibration range.\n");
  return -1; // Error code or handle appropriately
}

/**
 * @brief Initialize the rotator thread.
 *
 * This function creates the rotator thread and initializes the event flags.
 *
 * @return 0 on success, -1 on failure.
 */
int Init_rotator_Thread (void) {
  tid_rotator_Thread = osThreadNew(Rotator_Thread, NULL, NULL);
  if (tid_rotator_Thread == NULL) {
    return -1;
  }

  rotator_evt_id = osEventFlagsNew(NULL);
  if (rotator_evt_id == NULL) {
    return -1; // Event Flags object not created
  }

  return 0;
}


/**
 * @brief Stops the rotator motor.
 *
 * This function ensures the motor is initialized before applying a brake to stop the rotator.
 * If the motor is not initialized, it initializes the motor first.
 */
void Rotator_stop(void) {
	if (!is_motor_initialized()) {
		motor_init(rotator_pwm, pwm_ch1, pwm_ch2);
	}
	motor_brake(rotator_pwm, pwm_ch1, pwm_ch2);
}





/**
 * @brief Rotator thread function.
 *
 * This function handles rotator events and manages the calibration points
 * for voltage calculation based on angle and moves the rotator to new position.
 *
 * @param argument Pointer to thread argument (unused).
 */
void Rotator_Thread (void *argument) {
  (void)argument;

  uint32_t flags;
	
	float target_position = 0;
//	float test_pos;

	// testing only
  calib_points[0] = (rot_point_t){-45.0f ,0.16f };
  calib_points[1] = (rot_point_t){30.0f ,0.194166666666667f };
  calib_points[2] = (rot_point_t){105.0f ,0.228333333333333f };
  calib_points[3] = (rot_point_t){180.0f ,0.2625f };
  calib_points[4] = (rot_point_t){255.0f ,0.296666666666667f };
  calib_points[5] = (rot_point_t){330.0f ,0.330833333333333f };
  calib_points[6] = (rot_point_t){405.0f ,0.365f };

  while (1) {
    flags = osEventFlagsWait(rotator_evt_id, ROT_NOTIFY_NEW_POSITION, osFlagsWaitAny, 100);

    if (flags == ROT_NOTIFY_NEW_POSITION) {
      if (RotatorData.stop) {
        printf("\nStop rotator!!!\n");
      } else {
				target_position = rot_get_voltage_from_angle(RotatorData.goazi);
        printf("\nMoving to new position %3.1f deg, target voltage: %3.2f\n",
             RotatorData.goazi, target_position );
				MoveToNewPosition_PID(target_position);
      }
    }

//		test_pos = current_position(rot_adc, pos_pot_ch, pos_ref_ch, 100);
//		printf("Pos = %5.3f\n",test_pos);
    osThreadYield(); // Suspend thread
  }
}











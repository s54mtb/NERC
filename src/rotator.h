/**
 * @file rotator.h
 * @brief Header file for the Rotor data structures and functions.
 * 
 * This file defines the N1MMRotor structure, which is used to represent
 * rotor settings and configurations for a rotator system and other 
 * rotor related functions.
 * 
 * @author S54MTB, Marko Pavlin
 * @date January 2025
 * @version 1.0
 */

#ifndef ROTATOR_H
#define ROTATOR_H


/** Rotator configuration */
#define ROT_MIN_POSITION    -44.99f
#define ROT_MAX_POSITION    404.99f
#define ROT_REAL_NORTH      165.3f
#define ROT_OVERLAP         true
#define ROT_NAME            "main"


#define ROT_MAX_CALIB_POINTS 7


typedef struct {
	double angle;
	double voltage;
} rot_point_t;


// PID controller parameters
typedef struct {
  float kp;   // Proportional gain
  float ki;   // Integral gain
  float kd;   // Derivative gain
  float integral; // Integral term
  float prev_error; // Previous error for derivative calculation
} PIDController;



/** Thread flags */
#define ROT_NOTIFY_NEW_POSITION  (1U)<<1

/**
 * @brief Data structure representing a rotator configuration.
 * 
 * This structure contains parameters for controlling and configuring a rotator.
 * It includes information about the rotor name, current azimuth, offset values,
 * directional capabilities, and the frequency band.
 */
typedef struct {
  char rotor[100];   /**< Name or identifier of the rotor. */
  double goazi;      /**< Current azimuth position in degrees. */
  double offset;     /**< Offset value to apply to the azimuth. */
  int bidirectional; /**< Flag indicating if the rotor is bidirectional (1 for true, 0 for false). */
  double freqband;   /**< Frequency band associated with the rotor in MHz. */
	int stop;          /**< Rotator stop command. */
} N1MMRotor;


void Rotator_Set(N1MMRotor *rot);


#endif // ROTATOR_H

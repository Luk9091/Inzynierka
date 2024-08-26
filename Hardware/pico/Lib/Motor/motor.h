#ifndef __MOTOR__H
#define __MOTOR__H

#include <pico/stdlib.h>
#include <hardware/gpio.h>
#include <hardware/pwm.h>
#include <hardware/timer.h>


#include "my_gpio.h"

#define MOTOR_WHEEL_DIAMETER        66.45f
#define MOTOR_ENCODER_RESOLUTION    1920

#define MOTOR_FULL_SPEED            65535
#define MOTOR_THREE_QUARTER_SPEED   49151
#define MOTOR_HALF_SPEED            32767
#define MOTOR_QUARTER_SPEED         16383
#define MOTOR_EIGHTH_SPEED          8191
#define MOTOR_ZERO_SPEED            0


#define MOTOR_RIGHT_PIN_FORWARD     GPIO_MOTOR_RIGHT_PIN_FORWARD
#define MOTOR_RIGHT_PIN_BACKWARD    GPIO_MOTOR_RIGHT_PIN_BACKWARD
#define MOTOR_ENCODER_RIGHT_UP      GPIO_MOTOR_ENCODER_RIGHT_UP
#define MOTOR_ENCODER_RIGHT_DOWN    GPIO_MOTOR_ENCODER_RIGHT_DOWN

#define MOTOR_LEFT_PIN_FORWARD      GPIO_MOTOR_LEFT_PIN_FORWARD
#define MOTOR_LEFT_PIN_BACKWARD     GPIO_MOTOR_LEFT_PIN_BACKWARD
#define MOTOR_ENCODER_LEFT_UP       GPIO_MOTOR_ENCODER_LEFT_UP
#define MOTOR_ENCODER_LEFT_DOWN     GPIO_MOTOR_ENCODER_LEFT_DOWN

#define MOTOR_RIGHT_PWM             GPIO_MOTOR_RIGHT_PWM
#define MOTOR_LEFT_PWM              GPIO_MOTOR_LEFT_PWM

#define METER                       1000
#define CM                          10
#define MM                          1

typedef struct {
    uint up;
    uint down;
} Encoder_t;

extern Encoder_t right, left;


void Motor_init(uint16_t default_speed);

void Motor_forward();
void Motor_backward();
void Motor_stop();

/*
 * Set distance in mm,
 * with precision `0.11mm`
*/
void Motor_forwardDistance(float distance);
/*
 * Set distance in mm
 * with precision `0.11mm`
*/
void Motor_backwardDistance(float distance);

void Motor_setSpeed(uint16_t speed);
void Motor_setSpeedRight(uint16_t speed);
void Motor_setSpeedLeft(uint16_t speed);


inline float pulseToDistance(uint pulse){
    return pulse * (MOTOR_WHEEL_DIAMETER * 3.1415f / MOTOR_ENCODER_RESOLUTION);
}

/*
 * Set distance in mm
 * with precision `0.11mm`
*/
inline uint16_t distanceToPulse(float distance){
    return distance * (MOTOR_ENCODER_RESOLUTION / (MOTOR_WHEEL_DIAMETER * 3.1415f));
}

/*
 * Set distance in mm
 * with precision `0.11mm`
*/
void Motor_set_leftDistance(float distance);

/*
 * Set distance in mm
 * with precision `0.11mm`
*/
void Motor_set_rightDistance(float distance);

static inline float Motor_get_leftDistance(){
    return pulseToDistance(pwm_get_counter(pwm_gpio_to_slice_num(MOTOR_ENCODER_LEFT_UP)));
}

static inline float Motor_get_rightDistance(){
    return pulseToDistance(pwm_get_counter(pwm_gpio_to_slice_num(MOTOR_ENCODER_RIGHT_UP)));
}

#endif
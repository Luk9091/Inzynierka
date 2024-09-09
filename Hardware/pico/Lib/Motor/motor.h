#ifndef __MOTOR__H
#define __MOTOR__H

#include <pico/stdlib.h>
#include <hardware/gpio.h>
#include <hardware/pwm.h>
#include <hardware/timer.h>

#include "pid.h"
#include "my_gpio.h"

#define MOTOR_WHEEL_DIAMETER        66.45f
#define MOTOR_ENCODER_RESOLUTION    1920
#define MOTOR_PID_DT                20

#define MOTOR_FULL_SPEED            (65535)
#define MOTOR_THREE_QUARTER_SPEED   (49151)
#define MOTOR_HALF_SPEED            (32767)
#define MOTOR_QUARTER_SPEED         (16383)
#define MOTOR_EIGHTH_SPEED          (8191)
#define MOTOR_ZERO_SPEED            (0)


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


void Motor_init();
void Motor_pid_run();
void Motor_pid_stop();

void Motor_forward();
void Motor_backward();
void Motor_stop();
void Motor_stopLeft();
void Motor_stopRight();

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

static inline void Motor_setSpeed(uint16_t speed){
    pwm_set_gpio_level(MOTOR_LEFT_PWM,  speed * 1.3f);
    pwm_set_gpio_level(MOTOR_RIGHT_PWM, speed * 0.7);
}

static inline void Motor_setSpeedRight(uint16_t speed){
    pwm_set_gpio_level(MOTOR_RIGHT_PWM, speed);
}

static inline void Motor_setSpeedLeft(uint16_t speed){
    pwm_set_gpio_level(MOTOR_LEFT_PWM, speed);
}

static inline uint16_t Motor_getSpeed(uint pin){
    int slice = pwm_gpio_to_slice_num(pin);
    volatile int top = pwm_hw->slice[slice].cc;
    if (pwm_gpio_to_channel(pin)){
        top = top >> PWM_CH0_CC_B_LSB;
    }
    return top & 0xFFFF;
};


/*
 * \return distance in `mm`
 * with precision `0.11mm`
*/
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
#ifndef __MOTOR__H__
#define __MOTOR__H__

#include <pico/stdlib.h>
#include <hardware/gpio.h>
#include <hardware/pwm.h>
#include <hardware/timer.h>
#include <math.h>

#include "pid.h"
#include "my_gpio.h"

#include "utilites.h"

#define MOTOR_DIFFERENTIAL          true


#define MOTOR_WHEEL_DIAMETER        50.f
#define MOTOR_ENCODER_RESOLUTION    1920
#define MOTOR_PID_DT                20

#define MOTOR_RUN                   true
#define MOTOR_STOP                  false

#define MOTOR_SPEED_DIFFERENCE      (0.3f)
#define MOTOR_FULL_SPEED            (65535)
#define MOTOR_THREE_QUARTER_SPEED   (49151)
#define MOTOR_HALF_SPEED            (32767)
#define MOTOR_QUARTER_SPEED         (16383)
#define MOTOR_EIGHTH_SPEED          (8191)
#define MOTOR_ZERO_SPEED            (0)

#define MOTOR_MINIMUM_SPEED         ((int)(MOTOR_FULL_SPEED * 0.05))


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

#define METER                       (1000)
#define CM                          (10)
#define MM                          (1)

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

void Motor_forwardLeft();
void Motor_forwardRight();
void Motor_backwardLeft();
void Motor_backwardRight();


void Motor_stop();
void Motor_stopLeft();
void Motor_stopRight();

void Motor_setDifferential(int angle);



/*
 * Set distance in mm,
 * with precision `0.1mm`
*/
void Motor_setDistance(float distance);

/*
 * Set distance in mm,
 * with precision `0.1mm`
*/
void Motor_forwardDistance(float distance);
/*
 * Set distance in mm
 * with precision `0.1mm`
*/
void Motor_backwardDistance(float distance);

void Motor_setSpeed(uint16_t speed);

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
 * with precision `0.1mm`
*/
inline float pulseToDistance(uint pulse){
    return pulse * (MOTOR_WHEEL_DIAMETER * 2 * 3.1415f / MOTOR_ENCODER_RESOLUTION);
}

/*
 * Set distance in mm
 * with precision `0.1mm`
*/
inline uint16_t distanceToPulse(float distance){
    return distance * (MOTOR_ENCODER_RESOLUTION / (MOTOR_WHEEL_DIAMETER * 2 * 3.1415f));
}

/*
 * Set distance in mm
 * with precision `0.1mm`
*/
void Motor_set_leftDistance(float distance);

/*
 * Set distance in mm
 * with precision `0.1mm`
*/
void Motor_set_rightDistance(float distance);

static inline float Motor_get_leftDistance(){
    return pulseToDistance(pwm_get_counter(pwm_gpio_to_slice_num(MOTOR_ENCODER_LEFT_UP)));
}

static inline float Motor_get_rightDistance(){
    return pulseToDistance(pwm_get_counter(pwm_gpio_to_slice_num(MOTOR_ENCODER_RIGHT_UP)));
}

const PID_t *Motor_getPID();

bool Motor_state();
#endif // __MOTOR__H__
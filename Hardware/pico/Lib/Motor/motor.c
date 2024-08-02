#include "motor.h"
#include <stdio.h>
#include <pico/stdlib.h>
#include <pico/stdio.h>
#include "counter.h"

// Motor frequency 50Hz, f_sys = 125MHz
#define PRESCALER  100
#define TOP        24999


static uint MOTOR_MASK = (1 << MOTOR_RIGHT_PIN_FORWARD) | (1 << MOTOR_RIGHT_PIN_BACKWARD) | (1 << MOTOR_LEFT_PIN_FORWARD) | (1 << MOTOR_LEFT_PIN_BACKWARD);
static uint MOTOR_MASK_FORWARD = (1 << MOTOR_RIGHT_PIN_FORWARD) | (1 << MOTOR_LEFT_PIN_FORWARD);
static uint MOTOR_MASK_BACKWARD = (1 << MOTOR_RIGHT_PIN_BACKWARD) | (1 << MOTOR_LEFT_PIN_BACKWARD);

Encoder_t right, left;




void Motor_init(uint16_t default_speed) {
    gpio_init_mask(MOTOR_MASK);

    gpio_set_dir(MOTOR_RIGHT_PIN_FORWARD, GPIO_OUT);
    gpio_set_dir(MOTOR_RIGHT_PIN_BACKWARD, GPIO_OUT);
    gpio_set_dir(MOTOR_LEFT_PIN_FORWARD, GPIO_OUT);
    gpio_set_dir(MOTOR_LEFT_PIN_BACKWARD, GPIO_OUT);

    Counter_init(MOTOR_ENCODER_RIGHT_UP);
    Counter_init(MOTOR_ENCODER_LEFT_UP);

    gpio_set_function(MOTOR_RIGHT_PWM, GPIO_FUNC_PWM);
    gpio_set_function(MOTOR_LEFT_PWM, GPIO_FUNC_PWM);

    uint slice_left, slice_right;
    slice_left = pwm_gpio_to_slice_num(MOTOR_LEFT_PWM);
    slice_right = pwm_gpio_to_slice_num(MOTOR_RIGHT_PWM);

    pwm_config config = pwm_get_default_config();
    config.div = PRESCALER << PWM_CH0_DIV_INT_LSB;
    config.top = TOP;

    pwm_init(slice_left, &config, true);
    pwm_init(slice_right, &config, true);

    Motor_setSpeed(default_speed);
}


void Motor_forward(){
    gpio_clr_mask(MOTOR_MASK_BACKWARD);
    Counter_clear(MOTOR_ENCODER_RIGHT_UP);
    Counter_clear(MOTOR_ENCODER_LEFT_UP);
    gpio_set_mask(MOTOR_MASK_FORWARD);
}

void Motor_backward(){
    gpio_clr_mask(MOTOR_MASK_FORWARD);
    Counter_clear(MOTOR_ENCODER_RIGHT_UP);
    Counter_clear(MOTOR_ENCODER_LEFT_UP);
    gpio_set_mask(MOTOR_MASK_BACKWARD);
}

void Motor_stop(){
    gpio_clr_mask(MOTOR_MASK);
}


// Set distance in mm
void Motor_forwardDistance(float distance){
    uint pulse = distanceToPulse(distance);
    Counter_setCountTo(MOTOR_ENCODER_RIGHT_UP, pulse);
    Counter_setCountTo(MOTOR_ENCODER_LEFT_UP,  pulse);
    Motor_forward();
}


// Set distance in mm
void Motor_backwardDistance(float distance){
    uint pulse = distanceToPulse(distance);
    Counter_setCountTo(MOTOR_ENCODER_RIGHT_UP, pulse);
    Counter_setCountTo(MOTOR_ENCODER_LEFT_UP,  pulse);
    Motor_backward();
}


void Motor_setSpeed(uint16_t speed){
    // if (speed < MOTOR_THREE_QUARTER_SPEED){
    //     speed = 0;
    // }
    pwm_set_gpio_level(MOTOR_LEFT_PWM, speed);
    pwm_set_gpio_level(MOTOR_RIGHT_PWM, speed);
}

void Motor_setSpeedRight(uint16_t speed){
    // if (speed > MOTOR_THREE_QUARTER_SPEED)
    //     speed = 0;
    pwm_set_gpio_level(MOTOR_RIGHT_PWM, speed);
}

void Motor_setSpeedLeft(uint16_t speed){
    // if (speed > MOTOR_THREE_QUARTER_SPEED)
    //     speed = 0;
    pwm_set_gpio_level(MOTOR_LEFT_PWM, speed);
}
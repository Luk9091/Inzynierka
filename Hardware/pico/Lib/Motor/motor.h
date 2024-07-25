#ifndef __MOTOR__H
#define __MOTOR__H

#include <pico/stdlib.h>
#include <hardware/gpio.h>


#define MOTOR_RIGHT_PIN_FORWARD 19
#define MOTOR_RIGHT_PIN_BACKWARD 18
#define MOTOR_ENCODER_RIGHT_UP 10
#define MOTOR_ENCODER_RIGHT_DOWN 11

#define MOTOR_LEFT_PIN_FORWARD  17
#define MOTOR_LEFT_PIN_BACKWARD 16
#define MOTOR_ENCODER_LEFT_UP 12
#define MOTOR_ENCODER_LEFT_DOWN 13


void Motor_init();

void Motor_forward();
void Motor_backward();
void Motor_stop();

#endif
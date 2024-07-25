#include "motor.h"
static uint MOTOR_MASK = (1 << MOTOR_RIGHT_PIN_FORWARD) | (1 << MOTOR_RIGHT_PIN_BACKWARD) | (1 << MOTOR_LEFT_PIN_FORWARD) | (1 << MOTOR_LEFT_PIN_BACKWARD);
static uint MOTOR_MASK_FORWARD = (1 << MOTOR_RIGHT_PIN_FORWARD) | (1 << MOTOR_LEFT_PIN_FORWARD);
static uint MOTOR_MASK_BACKWARD = (1 << MOTOR_RIGHT_PIN_BACKWARD) | (1 << MOTOR_LEFT_PIN_BACKWARD);

void Motor_init() {
    uint mask = (1 << MOTOR_ENCODER_RIGHT_UP) | (1 << MOTOR_ENCODER_RIGHT_DOWN) | (1 << MOTOR_ENCODER_LEFT_UP) | (1 << MOTOR_ENCODER_LEFT_DOWN) | MOTOR_MASK;
    gpio_init_mask(mask);

    gpio_set_dir(MOTOR_RIGHT_PIN_FORWARD, GPIO_OUT);
    gpio_set_dir(MOTOR_RIGHT_PIN_BACKWARD, GPIO_OUT);
    gpio_set_dir(MOTOR_LEFT_PIN_FORWARD, GPIO_OUT);
    gpio_set_dir(MOTOR_LEFT_PIN_BACKWARD, GPIO_OUT);

    gpio_set_dir(MOTOR_ENCODER_RIGHT_UP, GPIO_IN);
    gpio_set_dir(MOTOR_ENCODER_RIGHT_DOWN, GPIO_IN);
    gpio_set_dir(MOTOR_ENCODER_LEFT_UP, GPIO_IN);
    gpio_set_dir(MOTOR_ENCODER_LEFT_DOWN, GPIO_IN);
}


void Motor_forward(){
    gpio_clr_mask(MOTOR_MASK_BACKWARD);
    gpio_set_mask(MOTOR_MASK_FORWARD);
}

void Motor_backward(){
    gpio_clr_mask(MOTOR_MASK_FORWARD);
    gpio_set_mask(MOTOR_MASK_BACKWARD);
}

void Motor_stop(){
    gpio_clr_mask(MOTOR_MASK);
}
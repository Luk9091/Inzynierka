#include "motor.h"
#include <stdio.h>
#include <stdlib.h>
#include <pico/stdlib.h>
#include <pico/stdio.h>
#include "counter.h"
#include "udp.h"

// Motor frequency 25kHz, f_sys = 125MHz
#define PRESCALER  1
#define TOP        4999

#define FORWARD    1
#define BACKWARD   0


static uint MOTOR_MASK = (1 << MOTOR_RIGHT_PIN_FORWARD) | (1 << MOTOR_RIGHT_PIN_BACKWARD) | (1 << MOTOR_LEFT_PIN_FORWARD) | (1 << MOTOR_LEFT_PIN_BACKWARD);
static uint MOTOR_MASK_FORWARD = (1 << MOTOR_RIGHT_PIN_FORWARD) | (1 << MOTOR_LEFT_PIN_FORWARD);
static uint MOTOR_MASK_BACKWARD = (1 << MOTOR_RIGHT_PIN_BACKWARD) | (1 << MOTOR_LEFT_PIN_BACKWARD);

Encoder_t right, left;
static repeating_timer_t pid_timer;
static bool pid_running = false;
PID_t pid;
static bool direction = FORWARD;

#if MOTOR_DIFFERENTIAL
static float differential = 0;
#endif





bool Motor_pid_distanceCorrection(repeating_timer_t *timer){
    const float dt = MOTOR_PID_DT / (1000.f);
    
    int countLeft = Counter_getCount(MOTOR_ENCODER_LEFT_UP);
    int countRight= Counter_getCount(MOTOR_ENCODER_RIGHT_UP);
#if MOTOR_DIFFERENTIAL
    int diff = countLeft * (1 + differential) - countRight * (1 - differential);
#else
    int diff = countLeft - countRight;
#endif
    int ans = PID_update(&pid, diff, dt);

    int speedLeft  = Motor_getSpeed(MOTOR_LEFT_PWM)  - ans;
    int speedRight = Motor_getSpeed(MOTOR_RIGHT_PWM) + ans;

    if (speedLeft > MOTOR_FULL_SPEED){
        speedLeft = MOTOR_FULL_SPEED;
    } else if (speedLeft < 0){
        speedLeft = 0;
    }

    if (speedRight > MOTOR_FULL_SPEED){
        speedRight = MOTOR_FULL_SPEED;
    } else if (speedRight < 0){
        speedRight = 0;
    }

    // printf("Impulse: %6u, %6u, speed: %6u, %6u\n", countLeft, countRight, speedLeft, speedRight);
    Motor_setSpeedLeft(speedLeft);
    Motor_setSpeedRight(speedRight);
    return true;
}

void Motor_pid_run(){
    add_repeating_timer_ms(MOTOR_PID_DT, Motor_pid_distanceCorrection, NULL, &pid_timer);
    PID_reset(&pid);
    pid_running = true;
    Counter_clear(MOTOR_ENCODER_RIGHT_UP);
    Counter_clear(MOTOR_ENCODER_LEFT_UP);
}

void Motor_pid_stop(){
    cancel_repeating_timer(&pid_timer);
    pid_running = false;
}


void Motor_init() {
    gpio_init_mask(MOTOR_MASK);
    gpio_init_mask(1 << MOTOR_ENCODER_LEFT_UP  | 1 << MOTOR_ENCODER_RIGHT_UP);

    gpio_set_dir_out_masked(MOTOR_MASK);

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

    // Motor_setSpeed(0);
    PID_init(&pid, 1.75f, 0.5f, -3.00f);
}


void Motor_forward(){
#if MOTOR_DIFFERENTIAL
    if (direction != FORWARD){
        differential = -differential;
    }
#endif
    direction = FORWARD;
    gpio_clr_mask(MOTOR_MASK);
    Counter_clear(MOTOR_ENCODER_RIGHT_UP);
    Counter_clear(MOTOR_ENCODER_LEFT_UP);
    gpio_set_mask(MOTOR_MASK_FORWARD);
}

void Motor_backward(){
#if MOTOR_DIFFERENTIAL
    if (direction != BACKWARD){
        differential = -differential;
    }
#endif
    direction = BACKWARD;
    gpio_clr_mask(MOTOR_MASK);
    Counter_clear(MOTOR_ENCODER_RIGHT_UP);
    Counter_clear(MOTOR_ENCODER_LEFT_UP);
    gpio_set_mask(MOTOR_MASK_BACKWARD);
}


void Motor_forwardLeft(){
    gpio_clr_mask(1 << MOTOR_LEFT_PIN_BACKWARD);
    gpio_set_mask(1 << MOTOR_LEFT_PIN_FORWARD);
}

void Motor_forwardRight(){
    gpio_clr_mask(1 << MOTOR_RIGHT_PIN_BACKWARD);
    gpio_set_mask(1 << MOTOR_RIGHT_PIN_FORWARD);
}

void Motor_backwardLeft(){
    gpio_clr_mask(1 << MOTOR_LEFT_PIN_FORWARD);
    gpio_set_mask(1 << MOTOR_LEFT_PIN_BACKWARD);
}

void Motor_backwardRight(){
    gpio_clr_mask(1 << MOTOR_RIGHT_PIN_FORWARD);
    gpio_set_mask(1 << MOTOR_RIGHT_PIN_BACKWARD);
}


void Motor_stopLeft(){
    gpio_clr_mask(1 << MOTOR_LEFT_PIN_BACKWARD | 1 << MOTOR_LEFT_PIN_FORWARD);
    Motor_pid_stop();
}

void Motor_stopRight(){
    gpio_clr_mask(1 << MOTOR_RIGHT_PIN_BACKWARD | 1 << MOTOR_RIGHT_PIN_FORWARD);
    Motor_pid_stop();
}

void Motor_stop(){
    gpio_clr_mask(MOTOR_MASK);
    Motor_pid_stop();
}

void Motor_setDistance(float distance){
    uint pulse = distanceToPulse(distance);
    Counter_clear(MOTOR_ENCODER_RIGHT_UP);
    Counter_clear(MOTOR_ENCODER_LEFT_UP);
    Counter_setCountTo(MOTOR_ENCODER_RIGHT_UP, pulse);
    Counter_setCountTo(MOTOR_ENCODER_LEFT_UP,  pulse);
}

void Motor_forwardDistance(float distance){
    Motor_setDistance(distance);
    Motor_forward();
}


void Motor_backwardDistance(float distance){
    Motor_setDistance(distance);
    Motor_backward();
}


void Motor_set_leftDistance(float distance){
    Counter_setCountTo(MOTOR_ENCODER_LEFT_UP, (uint16_t)distanceToPulse(distance));
}

void Motor_set_rightDistance(float distance){
    Counter_setCountTo(MOTOR_ENCODER_RIGHT_UP, (uint16_t)distanceToPulse(distance));
}

void Motor_setSpeed(uint16_t speed){
#if MOTOR_DIFFERENTIAL
    pwm_set_gpio_level(MOTOR_LEFT_PWM,  speed * (1.f + MOTOR_SPEED_DIFFERENCE) * (1 - differential));
    pwm_set_gpio_level(MOTOR_RIGHT_PWM, speed * (1.f - MOTOR_SPEED_DIFFERENCE) * (1 + differential));
#else
    pwm_set_gpio_level(MOTOR_LEFT_PWM,  speed * (1.f + MOTOR_SPEED_DIFFERENCE));
    pwm_set_gpio_level(MOTOR_RIGHT_PWM, speed * (1.f - MOTOR_SPEED_DIFFERENCE));
#endif
}

#if MOTOR_DIFFERENTIAL
void Motor_setDifferential(int angle){
    direction = FORWARD;
    differential = tanf((angle - 90) * DEG2RAD) * (CAR_WIDTH / (2.f*CAR_LENGTH));
}
#endif

const PID_t *Motor_getPID(){
    return &pid;
}

bool Motor_state(){
    return sio_hw->gpio_out & MOTOR_MASK;
}
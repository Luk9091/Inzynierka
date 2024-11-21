#ifndef __SERVO__H
#define __SERVO__H

#include <stdlib.h>
#include <pico/stdlib.h>

#include <hardware/gpio.h>
#include <hardware/pwm.h>
#include <hardware/adc.h>

#define SERVO_READ_ADC false

/*
 * Servo config struct
 * if not use check servo position change only GPIO
 * On default:
 * ADC_PIN = 0         -- not use
 * enableMux = 0       -- not use
 * enableState = false -- use only if enabled mux is used
*/
typedef struct SERVO {
    bool run;
    uint8_t GPIO;
#if SERVO_READ_ADC
    uint ADC_PIN;
    bool flipRead;
#endif
    int8_t angle;
    int8_t current_angle;
    uint step;
    int8_t min;
    int8_t max;
    int8_t offset;
} Servo_t;

// Servo initialization function, enable PWM
void Servo_init(Servo_t *servo, bool enable);
void Servo_start(Servo_t *servo);
void Servo_stop(Servo_t *servo);



void Servo_setAngle(Servo_t *servo, uint angle);
#if SERVO_READ_ADC
int Servo_readAngle(Servo_t *servo);
#endif

void Servo_reachAngle(Servo_t *servo);
void Servo_goto(Servo_t *servo);
void Servo_overshoot(Servo_t *servo, uint angle);


#endif
#ifndef __STEP_MOTOR_H__
#define __STEP_MOTOR_H__

#include <pico/stdio.h>
#include <pico/stdlib.h>
#include <hardware/pio.h>

#include <hardware/gpio.h>

#define FORWARD 1
#define BACKWARD 0

typedef struct  {
    uint stepsToTake;
    bool direction; // true = forward, false = backward
    uint FULL_SETP;
    uint pins[4];
    uint maskedPin;

    uint *moving;
    uint substep;

    PIO stm_pio;
    uint stm_sm;
} StepperMotor_t;

void StepperMotor_init(StepperMotor_t *motor, uint full_steps, uint pins[4], PIO pio);
void StepperMotor_step(StepperMotor_t *motor, uint steps, bool direction);

void StepperMotor_nextStep(StepperMotor_t *motor);



#endif
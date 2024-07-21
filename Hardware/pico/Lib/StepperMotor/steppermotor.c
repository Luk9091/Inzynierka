#include "steppermotor.h"

static uint StepperMotor_FORWARD[8]  = {0x9, 0x8, 0xC, 0x4, 0x6, 0x2, 0x3, 0x1};
static uint StepperMotor_BACKWARD[8] = {0x1, 0x3, 0x2, 0x6, 0x4, 0xC, 0x8, 0x9};
static uint StepperMotor_STATIONARY[8] = {0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0};

void StepperMotor_init(StepperMotor_t *motor, uint full_steps, uint pins[4], PIO pio) {
    motor->FULL_SETP = full_steps;
    motor->stepsToTake = 0;
    motor->direction = true;
    for (int i = 0; i < 4; i++) {
        motor->pins[i] = pins[i];
        motor->maskedPin |= 1 << pins[i];
        gpio_init(motor->pins[i]);
        gpio_set_dir(motor->pins[i], GPIO_OUT);
    }
    motor->stm_pio = pio;
    motor->stm_sm = pio_claim_unused_sm(motor->stm_pio, true);
    motor->moving = StepperMotor_STATIONARY;
    motor->substep = 0;


    // pio_gpio_init(motor->stm_pio, motor->pins[0]);
    // pio_gpio_init(motor->stm_pio, motor->pins[1]);
    // pio_gpio_init(motor->stm_pio, motor->pins[2]);
    // pio_gpio_init(motor->stm_pio, motor->pins[3]);
    // pio_sm_set_consecutive_pindirs(motor->stm_pio, motor->stm_sm, motor->pins[0], 4, true);
}


void StepperMotor_step(StepperMotor_t *motor, uint steps, bool direction) {
    motor->stepsToTake = steps;
    motor->direction = direction;

    if (motor->direction) {
        motor->moving = StepperMotor_FORWARD;
    } else {
        motor->moving = StepperMotor_BACKWARD;
    }
}

void StepperMotor_nextStep(StepperMotor_t *motor) {
    gpio_put_masked(motor->maskedPin, motor->moving[motor->substep] << motor->pins[0]);

    motor->substep++;
    if (motor->substep >= 8){
        motor->substep = 0;
        motor->stepsToTake--;
        if (motor->stepsToTake == 0) {
            motor->moving = StepperMotor_STATIONARY;
            gpio_put_masked(motor->maskedPin, motor->moving[motor->substep] << motor->pins[0]);
        } 
    }
}
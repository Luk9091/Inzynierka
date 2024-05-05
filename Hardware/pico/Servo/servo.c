#include "servo.h"
#include <pico/stdlib.h>

#include "servo.pio"

#define SERVO_PIN_1 2
#define SERVO_PIN_2 3
#define SERVO_PIN_3 4
#define SERVO_PIN_4 5

static PIO ENGINE;
static uint stateMachine;


void Servo_Init(){
    uint pins = 1 << SERVO_PIN_1 | 1 << SERVO_PIN_2 | 1 << SERVO_PIN_3 | 1 << SERVO_PIN_4;
    gpio_init_mask(pins);
    gpio_set_dir_out_masked(pins);

    ENGINE = pio0;
    uint offset = pio_add_program(ENGINE, &servo_program);
    stateMachine = pio_claim_sm_mask(ENGINE, true);
    servo_program_init(ENGINE, stateMachine, offset)

}

void Servo_step(uint number){
    pio_sm_put(LEFT_ENGINE,  stateMachine)
}
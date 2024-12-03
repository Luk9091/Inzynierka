#include "counter.h"
#include <pico/stdlib.h>
#include <stdio.h>
#include "motor.h"
#include "udp.h"

#define COUNTER_FINISH_MEASURE_ANGLE 250

// bool stop = false;

void Counter_irq(){
    uint status = pwm_get_irq_status_mask();
    // printf("Irq status: %u\n", status);

    if (status & (1u << 2)){
        UDP_send("Right encoder up\n");
        // Motor_setSpeedRight(0);
        // Motor_stopRight();
    }
    
    if (status & (1u << 1)){
        UDP_send("Left encoder up\n");
        // Motor_setSpeedLeft(0);
        // Motor_stopLeft();
    }
    Motor_stop();
    Servo_stop(&servo);
    Motor_pid_stop();

    pwm_hw->intr |= status;
}


void Counter_init(uint8_t pin){
    assert(pwm_gpio_to_channel(pin) == PWM_CHAN_B);

    gpio_set_function(pin, GPIO_FUNC_PWM);
    uint slice = pwm_gpio_to_slice_num(pin);

    pwm_config config = pwm_get_default_config();
    pwm_config_set_clkdiv_mode(&config, PWM_DIV_B_RISING);
    pwm_config_set_clkdiv_int_frac(&config, 1, 0);
    // pwm_config_set_wrap(&config, 10);

    pwm_set_irq_enabled(slice, true);
    Counter_enabledIRQ(pin);
    irq_set_exclusive_handler(PWM_IRQ_WRAP, Counter_irq);
    irq_set_enabled(PWM_IRQ_WRAP, true);

    
    pwm_init(slice, &config, true);
    Counter_clear(pin);
}

void Counter_enabledIRQ(uint8_t pin){
    uint slice = pwm_gpio_to_slice_num(pin);
    pwm_set_enabled(slice, false);
    pwm_set_irq_enabled(slice, true);
    pwm_set_enabled(slice, true);
}

void Counter_disabledIRQ(uint8_t pin){
    uint slice = pwm_gpio_to_slice_num(pin);
    pwm_set_enabled(slice, false);
    pwm_set_irq_enabled(slice, false);
    pwm_set_enabled(slice, true);
}

void Counter_setCountTo(uint8_t pin, uint16_t countTo){
    uint slice = pwm_gpio_to_slice_num(pin);

    pwm_set_enabled(slice, false);
    pwm_set_counter(slice, 0);
    pwm_set_wrap(slice, countTo);
    pwm_set_enabled(slice, true);
}

void Counter_clear(uint8_t pin){
    uint slice = pwm_gpio_to_slice_num(pin);
    pwm_set_enabled(slice, false);
    pwm_set_counter(slice, 0);
    pwm_set_enabled(slice, true);
}
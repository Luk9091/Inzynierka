#include "ir.h"
#include "motor.h"
#include "udp.h"

static const uint mask = 1 << (IR_LEFT) | 1 << (IR_RIGHT);


void _IR_check(uint gpio, uint event){
    UDP_send("IR: %d\n", gpio);
    IR_disable();
    Motor_stop();
}

void IR_init(){
    gpio_init_mask(mask);
    gpio_pull_up(IR_LEFT);
    gpio_pull_up(IR_RIGHT);
    gpio_set_dir_in_masked(mask);
}

void IR_enabled(){
    gpio_set_irq_enabled_with_callback(IR_LEFT,  GPIO_IRQ_EDGE_FALL, true, (gpio_irq_callback_t)(&_IR_check));
    gpio_set_irq_enabled_with_callback(IR_RIGHT, GPIO_IRQ_EDGE_FALL, true, (gpio_irq_callback_t)(&_IR_check));
}

void IR_disable(){
    gpio_set_irq_enabled(IR_LEFT,  GPIO_IRQ_EDGE_FALL, false);
    gpio_set_irq_enabled(IR_RIGHT, GPIO_IRQ_EDGE_FALL, false);
}
#ifndef __IR_DISTANCE_H__
#define __IR_DISTANCE_H__

#include <pico/stdlib.h>
#include <hardware/gpio.h>

#include "my_gpio.h"
#include "utilites.h"

#define IR_LEFT GPIO_LEFT_BACK_SENSOR
#define IR_RIGHT GPIO_RIGHT_BACK_SENSOR

void IR_init();
void IR_enabled();
void IR_disable();


#endif
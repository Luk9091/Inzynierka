#ifndef __SERVO__H
#define __SERVO__H

#include <stdio.h>
#include <pico/stdlib.h>
#include <hardware/gpio.h>
#include <hardware/pio.h>

void Servo_Init();
void Servo_step(uint number);

#endif
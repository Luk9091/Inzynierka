#ifndef __UNIT_TEST_H__
#define __UNIT_TEST_H__


#include <pico/stdlib.h>
#include <pico/stdio.h>
#include <pico/multicore.h>
#include <pico/util/queue.h>
#include <hardware/timer.h>
#include <math.h>


#include "my_gpio.h"
#include "utilites.h"
#include "i2c.h"

#include "udp.h"
#include "tof.h"
#include "mpu6050.h"
#include "compass.h"
#include "servo.h"
#include "motor.h"
#include "counter.h"
#include "ir.h"
#include "eeprom.h"
#include "flash.h"

static void TEST_WAIT_ON_INPUT(){
    char c = 0;
    printf("Press 1 to continue\n");
    while(c != '1'){
        c = getchar();
    }
    printf("Continue\n");
}

static int TEST_INIT(){
    stdio_init_all();
    UDP_initQueue();
    I2C_Init();
    TEST_WAIT_ON_INPUT();
    return 0;
}



int COMPASS_TEST__main();
int SPEED_TEST__main();


#endif
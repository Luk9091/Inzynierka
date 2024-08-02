#ifndef __VL53L0X__MODULE__H
#define __VL53L0X__MODULE__H

#include <stdio.h>
#include <pico/stdio.h>
#include <hardware/gpio.h>

#include <pico/error.h>

#include "i2c.h"
#include "vl53l0x_api.h"
#include "vl53l0x_rp2040.h"

// Deny address
// 0x30 <- Compass
// 0x76 <- MBE
// 

#define ToF_CENTER_ADDRESS  0x31
#define ToF_LEFT_ADDRESS    0x32
#define ToF_RIGHT_ADDRESS   0x33

#define ToF_DEFAULT_ADDRESS 0x29

#define ToF_LEFT_PIN        GPIO_ToF_LEFT_CONTROL_PIN
#define ToF_CENTER_PIN      GPIO_ToF_CENTER_CONTROL_PIN
#define ToF_RIGHT_PIN       GPIO_ToF_RIGHT_CONTROL_PIN

typedef VL53L0X_Dev_t ToF_t;



void ToF_Init(VL53L0X_Dev_t *left, VL53L0X_Dev_t *center, VL53L0X_Dev_t *right);
VL53L0X_Error ToF_sensorInit(VL53L0X_Dev_t *tof, uint8_t address, uint32_t pin);
void ToF_startContinuesMeasuring(VL53L0X_Dev_t *tof);


int16_t ToF_oneShotMeasure(VL53L0X_Dev_t *tof);
int16_t ToF_continuesMeasure(VL53L0X_Dev_t *tof);


#endif
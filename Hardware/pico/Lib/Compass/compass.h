#ifndef __COMPASS_H__
#define __COMPASS_H__

#include <pico/stdio.h>
#include <pico/stdlib.h>

#include "i2c.h"
#include "utilites.h"

#define I2C_COMPASS_ADDRESS 0x0D
#define COMPASS_CALIBRATION true

void COMPASS_Init();
void COMPASS_rawRead(axis_t *axis);
void COMPASS_read(axis_t *axis);
float COMPASS_getAzimuth(float x, float y);

axis_t COMPASS_calibrateRaw(axis_t raw);

// void COMPASS_Calibrate();

#endif
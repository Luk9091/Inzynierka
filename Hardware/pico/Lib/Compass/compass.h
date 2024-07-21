#ifndef __COMPASS_H__
#define __COMPASS_H__

#include <pico/stdio.h>
#include <pico/stdlib.h>

#include "i2c.h"
#include "utilites.h"

#define I2C_COMPASS_ADDRESS 0x0D
#define COMPASS_CALIBRATION true

// static const axis_t COMPASS_USER_OFFSET = {0, 0, 0};
// static const axis_ft_t COMPASS_USER_SCALE  = {1, 1, 1};
static const axis_t COMPASS_USER_OFFSET = {-12738, -5215, 8031};
static const axis_ft_t COMPASS_USER_SCALE  = {1.158998, 0.990322, 0.886986};

void COMPASS_Init();
void COMPASS_rawRead(axis_t *axis);
void COMPASS_read(axis_ft_t *axis);
float COMPASS_getAzimuth(float x, float y);

axis_ft_t COMPASS_calibrateRaw(axis_t raw);

void COMPASS_Calibrate();

#endif
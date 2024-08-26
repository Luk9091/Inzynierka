#ifndef __COMPASS_H__
#define __COMPASS_H__

#include <pico/stdio.h>
#include <pico/stdlib.h>

#include "i2c.h"
#include "utilites.h"

#define I2C_COMPASS_ADDRESS 0x0D
#define LUBLIN_MAGNETIC_DECLINATION 7.7f
#define KRAKOW_MAGNETIC_DECLINATION 6.2f
#define MAGNETIC_DECLINATION LUBLIN_MAGNETIC_DECLINATION

/*!
 * \brief Select if compass should auto add data to calibrate process
 * \ingroup Compass
 * 
 * \exception Its does not set calibration data, only store data!
 * To update calibration data use COMPASS_applyCalibration fuction
*/
#define COMPASS_AUTO_CALIBRATION_PROCESS false

void COMPASS_Init();
void COMPASS_rawRead(axis_t *axis);
void COMPASS_read(axis_t *axis);
float COMPASS_getAzimuth(float x, float y);

axis_t COMPASS_calibrateRaw(axis_t raw);

void COMPASS_addToCalibrate(axis_t *raw);
void COMPASS_applyCalibration();

axis_t COMPASS_getMin();
axis_t COMPASS_getMax();

#endif
#ifndef __COMPASS_H__
#define __COMPASS_H__

#include <pico/stdio.h>
#include <pico/stdlib.h>
#include <hardware/gpio.h>

#include "i2c.h"
#include "utilites.h"
#include "my_gpio.h"

#define I2C_COMPASS_ADDRESS 0x0D
#define COMPASS_INT_GPIO     GPIO_COMPASS_INT_PIN
#define LUBLIN_MAGNETIC_DECLINATION 7.7f
#define KRAKOW_MAGNETIC_DECLINATION 6.2f
#define MAGNETIC_DECLINATION KRAKOW_MAGNETIC_DECLINATION

/*!
 * \brief Select if compass should auto add data to calibrate process
 * \ingroup Compass
 * 
 * \exception Its does not set calibration data, only store data!
 * To update calibration data use COMPASS_applyCalibration fuction
*/
#define COMPASS_AUTO_CALIBRATION_PROCESS false

void COMPASS_Init();
int COMPASS_rawRead(axis_t *axis);
int COMPASS_read(axis_t *axis);
float COMPASS_getAzimuth(float x, float y);

axis_t COMPASS_calibrateRaw(axis_t raw);

#if COMPASS_AUTO_CALIBRATION_PROCESS
void COMPASS_addToCalibrate(axis_t *raw);
axis_t COMPASS_applyCalibration();

axis_t COMPASS_getMin();
axis_t COMPASS_getMax();
#endif

#endif
#include "compass.h"
#include "compass_regMap.h"
#include <math.h>

#define COMPASS_CALIBRATION_TIME 10e6

static axis_t offset = {-3907, 2370, 668};
static axis_t min = {32767, 32767, 32767};
static axis_t max = {-32768,-32768,-32768};

void COMPASS_Init(){
    I2C_writeReg(I2C_COMPASS_ADDRESS, COMPASS_CONTROL_1_REG, CONTINUE_MODE | FREQ_10HZ | RANGE_2G | SAMPLE_512);
    I2C_writeReg(I2C_COMPASS_ADDRESS, COMPASS_PERIOD_REG, 0x01);

}


void COMPASS_rawRead(axis_t *axis){
    uint8_t data[6];
    I2C_readNReg(I2C_COMPASS_ADDRESS, COMPASS_OUT_X_LSB_REG, data, 6);
    axis->x = ((int16_t)(data[1] << 8) | data[0]);
    axis->y = ((int16_t)(data[3] << 8) | data[2]);
    axis->z = ((int16_t)(data[5] << 8) | data[4]);
}

void COMPASS_read(axis_t *axis){
    axis_t raw;
    COMPASS_rawRead(&raw);
#if COMPASS_AUTO_CALIBRATION_PROCESS == true
    COMPASS_addToCalibrate(&raw);
#endif
    *axis = COMPASS_calibrateRaw(raw);
}

axis_t COMPASS_calibrateRaw(axis_t raw){
    axis_t calibrated;
    calibrated.x = (raw.x - offset.x);
    calibrated.y = (raw.y - offset.y);
    calibrated.z = (raw.z - offset.z);
    return calibrated;
}


float COMPASS_getAzimuth(float x, float y){
    float angle = atan2f(y, x);
    if (angle < 0) angle += 2 * π;
    else if (angle > 2 * π) angle -= 2 * π;

    angle = angle * 180 / π + MAGNETIC_DECLINATION;
    return angle;
}


void COMPASS_addToCalibrate(axis_t *raw){
    if (raw->x < min.x) min.x = raw->x;
    if (raw->x > max.x) max.x = raw->x;

    if (raw->y < min.y) min.y = raw->y;
    if (raw->y > max.y) max.y = raw->y;

    if (raw->z < min.z) min.z = raw->z;
    if (raw->z > max.z) max.z = raw->z;
}

void COMPASS_applyCalibration(){
    offset.x = (max.x + min.x) / 2;
    offset.y = (max.y + min.y) / 2;
    offset.z = (max.z + min.z) / 2;
}


axis_t COMPASS_getMin(){
    return min;
}

axis_t COMPASS_getMax(){
    return max;
}
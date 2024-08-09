#include "compass.h"
#include "compass_regMap.h"
#include <math.h>

#define COMPASS_CALIBRATION_TIME 10e6

axis_t COMPASS_calibrationOffset = {340, -2020, -1061};

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
    *axis = COMPASS_calibrateRaw(raw);
}

axis_t COMPASS_calibrateRaw(axis_t raw){
    axis_t calibrated;
    calibrated.x = (raw.x - COMPASS_calibrationOffset.x);
    calibrated.y = (raw.y - COMPASS_calibrationOffset.y);
    calibrated.z = (raw.z - COMPASS_calibrationOffset.z);
    return calibrated;
}


float COMPASS_getAzimuth(float x, float y){
    float angle = atan2f(y, x);
    if (angle < 0) angle += 2 * π;
    else if (angle > 2 * π) angle -= 2 * π;

    angle = angle * 180 / π;
    return angle;
}
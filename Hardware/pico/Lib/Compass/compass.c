#include "compass.h"
#include "compass_regMap.h"
#include <math.h>

#define COMPASS_CALIBRATION_TIME 10e6

static  struct{
    axis_t offset;
    axis_ft_t scale;
} COMPASS_calibrationData;


void COMPASS_Init(){
    I2C_writeReg(I2C_COMPASS_ADDRESS, COMPASS_CONTROL_1_REG, CONTINUE_MODE | FREQ_10HZ | RANGE_2G | SAMPLE_512);
    I2C_writeReg(I2C_COMPASS_ADDRESS, COMPASS_PERIOD_REG, 0x01);

    COMPASS_calibrationData.offset = COMPASS_USER_OFFSET;
    COMPASS_calibrationData.scale  = COMPASS_USER_SCALE;
}


void COMPASS_rawRead(axis_t *axis){
    uint8_t data[6];
    I2C_readNReg(I2C_COMPASS_ADDRESS, COMPASS_OUT_X_LSB_REG, data, 6);
    axis->x = ((int16_t)(data[1] << 8) | data[0]);
    axis->y = ((int16_t)(data[3] << 8) | data[2]);
    axis->z = ((int16_t)(data[5] << 8) | data[4]);
}

void COMPASS_read(axis_ft_t *axis){
    axis_t raw;
    COMPASS_rawRead(&raw);
    *axis = COMPASS_calibrateRaw(raw);
    // axis->x = (raw.x - COMPASS_calibrationData.offset.x) * COMPASS_calibrationData.scale.x;
    // axis->y = (raw.y - COMPASS_calibrationData.offset.x) * COMPASS_calibrationData.scale.y;
    // axis->z = (raw.z - COMPASS_calibrationData.offset.z) * COMPASS_calibrationData.scale.z;
}

axis_ft_t COMPASS_calibrateRaw(axis_t raw){
    axis_ft_t calibrated;
    calibrated.x = (raw.x - COMPASS_calibrationData.offset.x) * COMPASS_calibrationData.scale.x;
    calibrated.y = (raw.y - COMPASS_calibrationData.offset.y) * COMPASS_calibrationData.scale.y;
    calibrated.z = (raw.z - COMPASS_calibrationData.offset.z) * COMPASS_calibrationData.scale.z;
    return calibrated;
}


float COMPASS_getAzimuth(float x, float y){
    float angle = atan2f(y, x);
    if (angle < 0) angle += 2 * π;
    else if (angle > 2 * π) angle -= 2 * π;

    angle = angle * 180 / π;
    return angle;
}


void COMPASS_Calibrate(){
    axis_t min = { 32767,  32767,  32767};
    axis_t max = {-32768, -32768, -32768};
    axis_t data;

    uint start = time_us_32();
    int i = 0;

    for(i = 0; i < 1000; i++){
        COMPASS_rawRead(&data);

        if (data.x < min.x) min.x = data.x;
        else if (data.x > max.x) max.x = data.x;
        
        if (data.y < min.y) min.y = data.y;
        else if (data.y > max.y) max.y = data.y;

        if (data.z < min.z) min.z = data.z;
        else if (data.z > max.z) max.z = data.z;

        sleep_ms(100);
    }


    COMPASS_calibrationData.offset = (axis_t){(max.x + min.x) / 2, (max.y + min.y) / 2, (max.z + min.z) / 2};
    axis_ft_t avg_delta = {(max.x - min.x) / 2, (max.y - min.y) / 2, (max.z - min.z) / 2};
    float avg = (avg_delta.x + avg_delta.y + avg_delta.z) / 3;

    COMPASS_calibrationData.scale = (axis_ft_t){avg / avg_delta.x, avg / avg_delta.y, avg / avg_delta.z};
    
    printf("Calibration data: \n");
    printf("Offset: X: %d, Y: %d, Z: %d\n", COMPASS_calibrationData.offset.x, COMPASS_calibrationData.offset.y, COMPASS_calibrationData.offset.z);
    printf("Scale: X: %f, Y: %f, Z: %f\n", COMPASS_calibrationData.scale.x, COMPASS_calibrationData.scale.y, COMPASS_calibrationData.scale.z);
    printf("Calibration cycle: %d\n", i);
}
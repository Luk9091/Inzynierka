#include "compass.h"
#include "compass_regMap.h"
#include <math.h>

#define COMPASS_CALIBRATION_TIME 10e6

static axis_t offset = {-3907, 2370, 668};
// static axis_t offset = {-3057, -10677, -4979};

#if COMPASS_AUTO_CALIBRATION_PROCESS
static axis_t min = {32767, 32767, 32767};
static axis_t max = {-32768,-32768,-32768};
#endif

void COMPASS_Init(){
    gpio_init(COMPASS_INT_GPIO);
    gpio_set_dir(COMPASS_INT_GPIO, GPIO_IN);

    I2C_writeReg(I2C_COMPASS_ADDRESS, COMPASS_CONTROL_1_REG, CONTINUE_MODE | FREQ_10HZ | RANGE_2G | SAMPLE_512);
    I2C_writeReg(I2C_COMPASS_ADDRESS, COMPASS_PERIOD_REG, 0x01);
}


int COMPASS_rawRead(axis_t *axis){
    uint8_t data[6];
    int status = I2C_readNReg(I2C_COMPASS_ADDRESS, COMPASS_OUT_X_LSB_REG, data, 6);
    if (status < 0) return status;

    axis->x = ((int16_t)(data[1] << 8) | data[0]);
    axis->y = ((int16_t)(data[3] << 8) | data[2]);
    axis->z = ((int16_t)(data[5] << 8) | data[4]);
    return 0;
}

int COMPASS_read(axis_t *axis){
    if (!gpio_get(COMPASS_INT_GPIO)) return 1;

    
    axis_t raw;
    static axis_t old = {0, 0, 0};
    int status = COMPASS_rawRead(&raw);

    if (status < 0) return status;

#if COMPASS_AUTO_CALIBRATION_PROCESS == true
    COMPASS_addToCalibrate(&raw);
#endif
    const float ALPHA = 0.1f;
    old = LPF_ai(ALPHA, old, COMPASS_calibrateRaw(raw));
    *axis = old;

    return 0;
}

axis_t COMPASS_calibrateRaw(axis_t raw){
    axis_t calibrated;
    calibrated.x = (raw.x - offset.x);
    calibrated.y = (raw.y - offset.y);
    calibrated.z = (raw.z - offset.z);

    return calibrated;
}


float COMPASS_getAzimuth(float x, float y){
    float angle = atan2f(y, x) * RAD2DEG + MAGNETIC_DECLINATION;
    if (angle < -0) angle += 360.f;
    else if (angle > 360) angle -= 360.f;

    return angle;
}


#if COMPASS_AUTO_CALIBRATION_PROCESS
void COMPASS_addToCalibrate(axis_t *raw){
    if (raw->x < min.x) min.x = raw->x;
    if (raw->x > max.x) max.x = raw->x;

    if (raw->y < min.y) min.y = raw->y;
    if (raw->y > max.y) max.y = raw->y;

    if (raw->z < min.z) min.z = raw->z;
    if (raw->z > max.z) max.z = raw->z;
}

axis_t COMPASS_applyCalibration(){
    offset.x = (max.x + min.x) / 2;
    offset.y = (max.y + min.y) / 2;
    offset.z = (max.z + min.z) / 2;
    return offset;
}


axis_t COMPASS_getMin(){
    return min;
}

axis_t COMPASS_getMax(){
    return max;
}
#endif
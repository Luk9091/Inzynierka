#include "mpu6050.h"
#include "mpu6050_regMap.h"

#include <stdlib.h>
#include <math.h>

static struct {
    struct {
        axis_t linear;
        axis_t offset;
        float range_per_digit;
    } acc;

    struct {
        axis_t offset;
        axis_t threshold;
        float dps_per_digit;
    } gyro;
} mpu6050_calibrationData;

#define RANGE_MULTIPLY 1000
#define GYRO_CALIBRATION_TIME 1e6
#define ACC_CALIBRATION_TIME 10e6



int mpu6050_acc_setRange(MPU6050_ACC_RANGE_t range){
    int status = I2C_writeReg(I2C_MPU6050_ADDRESS, MPU6050_ACCEL_CONFIG, range<<3);
    if(status >= 0) {
        switch(range){
             case RANGE_2G:
                mpu6050_calibrationData.acc.range_per_digit = 0.000061f * GRAVITY_CONTAIN * RANGE_MULTIPLY;
                break;
            case RANGE_4G:
                mpu6050_calibrationData.acc.range_per_digit = 0.000122f * GRAVITY_CONTAIN * RANGE_MULTIPLY;
                break;
            case RANGE_8G:
                mpu6050_calibrationData.acc.range_per_digit = 0.000244f * GRAVITY_CONTAIN * RANGE_MULTIPLY;
                break;
            case RANGE_16G:
                mpu6050_calibrationData.acc.range_per_digit = 0.0004882f * GRAVITY_CONTAIN * RANGE_MULTIPLY;
                break;

            default:
                mpu6050_calibrationData.acc.range_per_digit = 1;
                break;
        }
    }
    return status;
}

int mpu6050_gyro_setRange(MPU6050_GYRO_RANGE_t range){
    int status = I2C_writeReg(I2C_MPU6050_ADDRESS, MPU6050_GYRO_CONFIG, range << 3 );
    if (status >= 0) {
        switch (range){
        case RANGE_250DPS:
            mpu6050_calibrationData.gyro.dps_per_digit = 0.007633f;// * RANGE_MULTIPLY;
            break;
        case RANGE_500DPS:
            mpu6050_calibrationData.gyro.dps_per_digit = 0.015267f;// * RANGE_MULTIPLY;
            break;
        case RANGE_1000DPS:
            mpu6050_calibrationData.gyro.dps_per_digit = 0.030487f;
            break;
        case RANGE_2000DPS:
            mpu6050_calibrationData.gyro.dps_per_digit = 0.060975f;
            break;

        default:
            mpu6050_calibrationData.gyro.dps_per_digit = 1;
            break;
        }
    }


    return status;
}


void mpu6050_Init(){
    I2C_writeReg(I2C_MPU6050_ADDRESS, MPU6050_POW_MANAGEMENT_1_REG, 0b00000000);

    mpu6050_acc_setRange(RANGE_2G);
    mpu6050_gyro_setRange(RANGE_500DPS);
    
    #if MPU6050_CALIBRATE_GYRO
    mpu6050_gyro_calibration();
    #else
    mpu6050_calibrationData.gyro.offset    = MPU6050_CALIBRATION_DATA_GYRO_OFFSET;
    mpu6050_calibrationData.gyro.threshold = MPU6050_CALIBRATION_DATA_GYRO_THRESHOLD;
    #endif

    // #if MPU6050_CALIBRATION_ACC
    // mpu6050_acc_calibration();
    // #else
    // mpu6050_calibrationData.acc.offset = (axis_t){MPU6050_CALIBRATION_DATA_ACC_OFFSET_X, MPU6050_CALIBRATION_DATA_ACC_OFFSET_Y, MPU6050_CALIBRATION_DATA_ACC_OFFSET_Z};
    // #endif

}

void mpu6050_readRawAcc(axis_t *acc){
    uint8_t buffer[6];

    I2C_readNReg(I2C_MPU6050_ADDRESS, MPU6050_ACCEL_XH, buffer, 6);
    acc->x = (int16_t)((buffer[0]) << 8) | buffer[1];
    acc->y = (int16_t)((buffer[2]) << 8) | buffer[3];
    acc->z = (int16_t)((buffer[4]) << 8) | buffer[5];
}

void mpu6050_readAcc(axis_t *acc){
    mpu6050_readRawAcc(acc);
    acc->x = (acc->x) * mpu6050_calibrationData.acc.range_per_digit;
    acc->y = (acc->y) * mpu6050_calibrationData.acc.range_per_digit;
    acc->z = (acc->z) * mpu6050_calibrationData.acc.range_per_digit;
}


void mpu6050_readRawGyro(axis_t *gyro){
    uint8_t buffer[6];

    I2C_readNReg(I2C_MPU6050_ADDRESS, MPU6050_GYRO_XH, buffer, 6);
    gyro->x = (int16_t)((buffer[0]) << 8) | buffer[1];
    gyro->y = (int16_t)((buffer[2]) << 8) | buffer[3];
    gyro->z = (int16_t)((buffer[4]) << 8) | buffer[5];
}

void mpu6050_readGyro(axis_t *gyro){
    mpu6050_readRawGyro(gyro);
    gyro->x = (gyro->x - mpu6050_calibrationData.gyro.offset.x) * mpu6050_calibrationData.gyro.dps_per_digit;
    gyro->y = (gyro->y - mpu6050_calibrationData.gyro.offset.y) * mpu6050_calibrationData.gyro.dps_per_digit;
    gyro->z = (gyro->z - mpu6050_calibrationData.gyro.offset.z) * mpu6050_calibrationData.gyro.dps_per_digit;

    if (abs(gyro->x) < mpu6050_calibrationData.gyro.threshold.x) gyro->x = 0;
    if (abs(gyro->y) < mpu6050_calibrationData.gyro.threshold.y) gyro->y = 0;
    if (abs(gyro->z) < mpu6050_calibrationData.gyro.threshold.z) gyro->z = 0;
}

void mpu6050_readRawTemp(float *temp){
    int16_t data;
    uint8_t buffer[2];
    I2C_readNReg(I2C_MPU6050_ADDRESS, MPU6050_TEMP_H, buffer, 2);
    data = (int16_t)((buffer[0]) << 8) | buffer[1];

    *temp = (float)data/340+36.53f;
}



void mpu6050_readData(axis_t *acc, axis_t *gyro, float *temp){
    mpu6050_readAcc(acc);
    mpu6050_readGyro(gyro);
    mpu6050_readRawTemp(temp);
}


void mpu6050_gyro_calibration(){
    axis_t measure;
    float sumX = 0, sumY = 0, sumZ = 0;
    float sigmaX = 0, sigmaY = 0, sigmaZ = 0;
    uint sample = 0;

    printf("Start GYROSCOPE calibration process, dont move\n");
    for(int i = 3; i >= 0; i--){
        printf("In: %i", i);
    }

    uint start = time_us_32();
    while((time_us_32() - start) < (GYRO_CALIBRATION_TIME)){
        mpu6050_readRawGyro(&measure);

        sumX += measure.x;
        sumY += measure.y;
        sumZ += measure.z;

        sigmaX += measure.x*measure.x;
        sigmaY += measure.y*measure.y;
        sigmaZ += measure.z*measure.z;

        sample++;
    }

    sumX = sumX/sample;
    sumY = sumY/sample;
    sumZ = sumZ/sample;

    mpu6050_calibrationData.gyro.offset.x = sumX;
    mpu6050_calibrationData.gyro.offset.y = sumY;
    mpu6050_calibrationData.gyro.offset.z = sumZ;

    mpu6050_calibrationData.gyro.threshold.x = sqrtf((float)((sigmaX/sample) - (sumX*sumX)));
    mpu6050_calibrationData.gyro.threshold.y = sqrtf((float)((sigmaY/sample) - (sumY*sumY)));
    mpu6050_calibrationData.gyro.threshold.z = sqrtf((float)((sigmaZ/sample) - (sumZ*sumZ)));


    printf("Gyroscope calibration value\n");
    printf("Offset:    %4hi, %4hi, %4hi\n", mpu6050_calibrationData.gyro.offset.x, mpu6050_calibrationData.gyro.offset.y,mpu6050_calibrationData.gyro.offset.z);
    printf("Threshold: %4hi, %4hi, %4hi\n", mpu6050_calibrationData.gyro.threshold.x, mpu6050_calibrationData.gyro.threshold.y, mpu6050_calibrationData.gyro.threshold.z);
    printf("\n\n");
}
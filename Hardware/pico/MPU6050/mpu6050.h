#ifndef __MPU6050__H
#define __MPU6050__H

#include <pico/stdio.h>
#include <pico/stdlib.h>
#include <hardware/i2c.h>

#include "i2c.h"
#include "utilites.h"

// LOW address of MPU6050
#define I2C_MPU6050_ADDRESS 0x68
#define GRAVITY_CONTAIN 9.80665f


/*
 * If true then make calibration rotate acceleration.
 * Under calibration process MPU has not to move! 
 * 
 * If false then use default parameter
*/
#define MPU6050_CALIBRATE_GYRO false

#define MPU6050_CALIBRATION_DATA_GYRO_OFFSET_X  -630
#define MPU6050_CALIBRATION_DATA_GYRO_OFFSET_Y  -311
#define MPU6050_CALIBRATION_DATA_GYRO_OFFSET_Z    65

#define MPU6050_CALIBRATION_DATA_GYRO_THRESHOLD_X 10
#define MPU6050_CALIBRATION_DATA_GYRO_THRESHOLD_Y 12
#define MPU6050_CALIBRATION_DATA_GYRO_THRESHOLD_Z 10

// #define MPU6050_CALIBRATION_ACC false

// #define MPU6050_CALIBRATION_DATA_ACC_OFFSET_X  1000
// #define MPU6050_CALIBRATION_DATA_ACC_OFFSET_Y  -400
// #define MPU6050_CALIBRATION_DATA_ACC_OFFSET_Z  -880


typedef enum {
    MPU6050_OK  = 0,
} MPU6050_ERROR_t;


void mpu6050_Init();
void mpu6050_reset();

/*
 * Read linear acceleration in mm/s²
*/
void mpu6050_readAcc(axis_t *acc);
void mpu6050_readGyro(axis_t *gyro);
void mpu6050_readTemp(float *temp);


/*
 * Linear acceleration in mm/s² ax int16
 * Rotate acceleration in deg/s² ax int16
 * Temperature in float
*/
void mpu6050_readData(axis_t *acc, axis_t *gyro, float *temp);


void mpu6050_gyro_calibration();
void mpu6050_acc_calibration();

#endif
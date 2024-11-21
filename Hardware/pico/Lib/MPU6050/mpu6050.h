#ifndef __MPU6050_H__
#define __MPU6050_H__

#include <pico/stdio.h>
#include <pico/stdlib.h>
#include <hardware/i2c.h>

#include "i2c.h"
#include "utilites.h"

#define MPU6050_INT_GPIO GPIO_MPU6050_INT_PIN

// LOW address of MPU6050
#define I2C_MPU6050_ADDRESS 0x68
#define GRAVITY_CONTAIN 9.80665f
#define MPU6050_RANGE_MULTIPLY 1000


/*
 * If true then make calibration rotate acceleration.
 * Under calibration process MPU has not to move! 
 * 
 * If false then use default parameter
*/
#define MPU6050_CALIBRATE_GYRO true
// #define MPU6050_CALIBRATION_ACC false

#define MPU6050_USE_ADDITIONAL_LPF_GYRO false


typedef enum{
    RANGE_2G = 1,
    RANGE_4G = 2,
    RANGE_8G = 3,
    RANGE_16G= 4,
} MPU6050_ACC_RANGE_t;

// DPS = degree per second
typedef enum{
    RANGE_250DPS  = 0,
    RANGE_500DPS  = 1,
    RANGE_1000DPS = 2,
    RANGE_2000DPS = 3,
} MPU6050_GYRO_RANGE_t;



void MPU6050_Init();
void MPU6050_reset();

uint8_t MPU6050_getIrqStatus();
/*
 * FIFO buffer init
 * Max size of FIFO is 1024BYTES!
*/
void MPU6050_fifoInit();
void MPU6050_fifoRun();
int  MPU6050_fifoGetCount();
int MPU6050_fifoRead(int16_t *data, int deep);

int MPU6050_readRawAcc(axis_t *acc);
int MPU6050_readRawGyro(axis_t *gyro);

/*
 * Read linear acceleration in `MPU6050_RANGE_MULTIPLY` * mm/s²
*/
int MPU6050_readAcc(axis_ft_t *acc);
/*
 * Rotate acceleration in `MPU6050_RANGE_MULTIPLY` deg/s² ax int16
*/
int MPU6050_readGyro(axis_ft_t *gyro);
/*
 * Temperature in C as float with 0.1C precision
*/
int MPU6050_readTemp(float *temp);


/*
 * Linear acceleration in `MPU6050_RANGE_MULTIPLY` mm/s² ax int16
 * Rotate acceleration in `MPU6050_RANGE_MULTIPLY` deg/s² ax int16
 * Temperature in C as float with 0.1C precision
*/
int MPU6050_readData(axis_ft_t *acc, axis_ft_t *gyro);
int MPU6050_readDataWithTemp(axis_ft_t *acc, axis_ft_t *gyro, float *temp);

/*
 * Remove gravity force from acceleration
 * \param accel - acceleration with gravity force
 * \return gravity force
*/
axis_ft_t MPU6050_removeGravity(axis_ft_t *accel);

/*
 * Get delta rotation acceleration,
 * \param angles - return pointer
 * \param gyro - flot read of gyroscope
 * \param noGravityAccel - acceleration without gravity force
 * \param deltaT - time between next samples in `s`
*/
void MPU6050_angleWithGyro(axis_ft_t *angles, axis_ft_t *gyro, axis_ft_t *noGravityAccel, float deltaT);

#if MPU6050_CALIBRATE_GYRO == 1
void MPU6050_gyro_calibration();
#endif
// void MPU6050_acc_calibration();

#endif
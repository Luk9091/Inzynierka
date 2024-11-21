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
        // axis_t threshold;
        float dps_per_digit;
    } gyro;
} mpu6050_calibrationData;

#define GYRO_CALIBRATION_TIME 10e6
#define ACC_CALIBRATION_TIME 10e6

static const axis_t MPU6050_CALIBRATION_DATA_GYRO_OFFSET    = {-630, 311,  65};
// static const axis_t MPU6050_CALIBRATION_DATA_GYRO_OFFSET    = {-703, -155, 101};

static const axis_t MPU6050_CALIBRATION_DATA_ACC_OFFSET = {272, -66, -864};
// static const axis_t MPU6050_CALIBRATION_DATA_ACC_OFFSET = {477, -91, -1000};


void FIFO_OVERFLOW_STOP(){
    I2C_writeReg(I2C_MPU6050_ADDRESS, MPU6050_FIFO_EN, 0);
    // printf("Overflow\n");
}



int MPU6050_acc_setRange(MPU6050_ACC_RANGE_t range){
    int status = I2C_writeRegMask(I2C_MPU6050_ADDRESS, MPU6050_ACCEL_CONFIG, 0b11 << 3, range<<3);
    if(status >= 0) {
        switch(range){
             case RANGE_2G:
                mpu6050_calibrationData.acc.range_per_digit = 0.000061f; // * GRAVITY_CONTAIN ;//* MPU6050_RANGE_MULTIPLY;
                break;
            case RANGE_4G:
                mpu6050_calibrationData.acc.range_per_digit = 0.000122f; // * GRAVITY_CONTAIN ;//* MPU6050_RANGE_MULTIPLY;
                break;
            case RANGE_8G:
                mpu6050_calibrationData.acc.range_per_digit = 0.000244f; // * GRAVITY_CONTAIN ;//* MPU6050_RANGE_MULTIPLY;
                break;
            case RANGE_16G:
                mpu6050_calibrationData.acc.range_per_digit = 0.0004882f; // * GRAVITY_CONTAIN ;//* MPU6050_RANGE_MULTIPLY;
                break;

            default:
                mpu6050_calibrationData.acc.range_per_digit = 1;
                break;
        }
    }
    return status;
}

int MPU6050_gyro_setRange(MPU6050_GYRO_RANGE_t range){
    int status = I2C_writeRegMask(I2C_MPU6050_ADDRESS, MPU6050_GYRO_CONFIG, 0b11 << 3, range << 3 );
    if (status >= 0) {
        switch (range){
        case RANGE_250DPS:
            mpu6050_calibrationData.gyro.dps_per_digit = 0.007633f; //* MPU6050_RANGE_MULTIPLY;
            break;
        case RANGE_500DPS:
            mpu6050_calibrationData.gyro.dps_per_digit = 0.015267f; //* MPU6050_RANGE_MULTIPLY;
            break;
        case RANGE_1000DPS:
            mpu6050_calibrationData.gyro.dps_per_digit = 0.030487f; //* MPU6050_RANGE_MULTIPLY;
            break;
        case RANGE_2000DPS:
            mpu6050_calibrationData.gyro.dps_per_digit = 0.060975f; //* MPU6050_RANGE_MULTIPLY;
            break;

        default:
            mpu6050_calibrationData.gyro.dps_per_digit = 1;
            break;
        }
    }


    return status;
}

void MPU6050_reset(){
    I2C_setBit(I2C_MPU6050_ADDRESS, MPU6050_POW_MANAGEMENT_1_REG, DEV_RESET_BIT);
    int status = 1;
    while(status){
        status = I2C_readReg(I2C_MPU6050_ADDRESS, MPU6050_POW_MANAGEMENT_1_REG) & (1 << DEV_RESET_BIT);
        sleep_ms(1);
    }
}



void MPU6050_Init(){
    MPU6050_reset();

    gpio_init(MPU6050_INT_GPIO);
    gpio_set_dir(MPU6050_INT_GPIO, GPIO_IN);
    /* Set interrupt pin to:
     * Active high
     * Push-pull
     * Keep high until interrupt is cleared
     * Clear on read operation
    */
    I2C_writeReg(I2C_MPU6050_ADDRESS, MPU6050_INT_PIN_CONFIG, 0b00110010);
    // Interrupt when data is ready
    I2C_writeReg(I2C_MPU6050_ADDRESS, MPU6050_INT_EN, 0b00000001);

    I2C_writeReg(I2C_MPU6050_ADDRESS, MPU6050_POW_MANAGEMENT_1_REG, 0b00001011);

    MPU6050_acc_setRange(RANGE_2G);
    MPU6050_gyro_setRange(RANGE_500DPS);
    // Set LPF in 5Hz
    I2C_writeReg(I2C_MPU6050_ADDRESS, MPU6050_CONFIG, 4 << 3 | 6 << 0);
    
    #if MPU6050_CALIBRATE_GYRO == true
        MPU6050_gyro_calibration();
    #else
        mpu6050_calibrationData.gyro.offset    = MPU6050_CALIBRATION_DATA_GYRO_OFFSET;
    #endif

    // MPU6050_fifoInit();
    // #if MPU6050_CALIBRATION_ACC
    // mpu6050_acc_calibration();
    // #else
    // mpu6050_calibrationData.acc.offset = MPU6050_CALIBRATION_DATA_ACC_OFFSET;
    // #endif

}

uint8_t MPU6050_getIrqStatus(){
    return I2C_readReg(I2C_MPU6050_ADDRESS, MPU6050_INT_STATUS);
}

void MPU6050_intPinConfiguration(){
    gpio_init(MPU6050_INT_GPIO);
    gpio_set_dir(MPU6050_INT_GPIO, GPIO_IN);

    I2C_writeReg(I2C_MPU6050_ADDRESS, MPU6050_INT_PIN_CONFIG, (1 << LATCH_INT_EN_BIT));

    gpio_set_irq_enabled_with_callback(MPU6050_INT_GPIO, GPIO_IRQ_EDGE_RISE, true, FIFO_OVERFLOW_STOP);
}



void MPU6050_fifoInit(){
    I2C_setBit(I2C_MPU6050_ADDRESS, MPU6050_USER_CTRL, FIFO_RESET_BIT);
    int status = 1;
    while(status){
        status = I2C_readReg(I2C_MPU6050_ADDRESS, MPU6050_USER_CTRL) & (1 << FIFO_RESET_BIT);
        sleep_ms(1);
    }

    MPU6050_fifoRun();
    printf("FIFO set up: 0x%X\n", I2C_readReg(I2C_MPU6050_ADDRESS, MPU6050_FIFO_EN));
    I2C_writeReg(I2C_MPU6050_ADDRESS, MPU6050_INT_EN, 1<<FIFO_OV_EN_INT_BIT);
    I2C_setBit(I2C_MPU6050_ADDRESS, MPU6050_USER_CTRL, FIFO_EN_BIT);
}

void MPU6050_fifoRun(){
    I2C_writeReg(I2C_MPU6050_ADDRESS, MPU6050_FIFO_EN, (1 << GZ_FIFO_EN_BIT) | (1 << ACCEL_FIFO_EN_BIT));
}


int MPU6050_fifoGetCount(){
    uint8_t data[2];
    int status = I2C_readNReg(I2C_MPU6050_ADDRESS, MPU6050_FIFO_COUNTH, data, 2);
    if (status < 0){
        return status;
    }

    int count = ((int)(data[0] << 8)) | data[1];
    if(count > 1024){
        return 0;
    }


    return count;
}

int MPU6050_fifoRead(int16_t *data, int deep){
    uint8_t buffer[1024];
    int status;

    status = I2C_readNReg(I2C_MPU6050_ADDRESS, MPU6050_FIFO_RW, buffer, deep*2);

    for(uint i = 0; i < 2*deep; i+= 2){
        data[i/2] = (int16_t)(buffer[i] << 8) | buffer[i+1];
    }
    return status;
}


int MPU6050_readRawAcc(axis_t *acc){
    uint8_t buffer[6];

    int status = I2C_readNReg(I2C_MPU6050_ADDRESS, MPU6050_ACCEL_XH, buffer, 6);
    if (status >= 0){
        acc->x = (int16_t)((buffer[0]) << 8) | buffer[1];
        acc->y = (int16_t)((buffer[2]) << 8) | buffer[3];
        acc->z = (int16_t)((buffer[4]) << 8) | buffer[5];
        return 0;
    }
    return status;
}

int MPU6050_readAcc(axis_ft_t *acc){
    axis_t raw;
    int status = MPU6050_readRawAcc(&raw);
    if (status >= 0){
        acc->x = (raw.x - mpu6050_calibrationData.acc.offset.x) * mpu6050_calibrationData.acc.range_per_digit;
        acc->y = (raw.y - mpu6050_calibrationData.acc.offset.y) * mpu6050_calibrationData.acc.range_per_digit;
        acc->z = (raw.z - mpu6050_calibrationData.acc.offset.z) * mpu6050_calibrationData.acc.range_per_digit;
        return 0;
    }
    return status;
}


int MPU6050_readRawGyro(axis_t *gyro){
    uint8_t buffer[6];

    int status = I2C_readNReg(I2C_MPU6050_ADDRESS, MPU6050_GYRO_XH, buffer, 6);
    if (status){
        gyro->x = (int16_t)((buffer[0]) << 8) | buffer[1];
        gyro->y = (int16_t)((buffer[2]) << 8) | buffer[3];
        gyro->z = (int16_t)((buffer[4]) << 8) | buffer[5];
        return 0;
    }

    return status;
}

int MPU6050_readGyro(axis_ft_t *gyro){
    axis_t raw;
    int status = MPU6050_readRawGyro(&raw);
    if (status >= 0){
        gyro->x = (float)(raw.x - mpu6050_calibrationData.gyro.offset.x) * mpu6050_calibrationData.gyro.dps_per_digit;
        gyro->y = (float)(raw.y - mpu6050_calibrationData.gyro.offset.y) * mpu6050_calibrationData.gyro.dps_per_digit;
        gyro->z = (float)(raw.z - mpu6050_calibrationData.gyro.offset.z) * mpu6050_calibrationData.gyro.dps_per_digit;
        return 0;
    }
    return status;
}

int MPU6050_readGyroWithLPF(axis_ft_t *gyro){
    static axis_ft_t old = {0, 0, 0};
    int status = MPU6050_readGyro(gyro);

    if (status < 0){
        return status;
    }

    old = LPF_af(0.75f, old, *gyro);

    gyro->x = old.x;
    gyro->y = old.y;
    gyro->z = old.z;

    return 0;
}

int MPU6050_readRawTemp(float *temp){
    int16_t data;
    uint8_t buffer[2];
    int status = I2C_readNReg(I2C_MPU6050_ADDRESS, MPU6050_TEMP_H, buffer, 2);
    if (status < 0){
        return status;
    }

    data = (int16_t)((buffer[0]) << 8) | buffer[1];

    *temp = (float)data/340+36.53f;
    return 0;
}


int MPU6050_readData(axis_ft_t *acc, axis_ft_t *gyro){
    if (!gpio_get(MPU6050_INT_GPIO)) return PICO_ERROR_CONNECT_FAILED;
    int status = MPU6050_readAcc(acc);

    if (status < 0){
        return status;
    }

#if MPU6050_USE_ADDITIONAL_LPF_GYRO
    return MPU6050_readGyroWithLPF(gyro);
#else
    return MPU6050_readGyro(gyro);
#endif
}

int MPU6050_readDataWithTemp(axis_ft_t *acc, axis_ft_t *gyro, float *temp){
    if (!gpio_get(MPU6050_INT_GPIO)) return 1;
    int status = MPU6050_readAcc(acc);
    if (status < 0){
        return status;
    }

    status = MPU6050_readGyro(gyro);
    if (status < 0){
        return status;
    }
    status = MPU6050_readRawTemp(temp);

    return status;
}


axis_ft_t MPU6050_removeGravity(axis_ft_t *accel){
    static axis_ft_t gravity = {.x = 0, .y = 0, .z = 0};
    const float ALPHA = 0.2f;

    gravity.x = LPF_f(ALPHA, gravity.x, accel->x);
    gravity.y = LPF_f(ALPHA, gravity.y, accel->y);
    gravity.z = LPF_f(ALPHA, gravity.z, accel->z);

    
    accel->x -= gravity.x;
    accel->y -= gravity.y;
    accel->z -= gravity.z;
    

    return gravity;
}



void MPU6050_angleWithGyro(axis_ft_t *angles, axis_ft_t *gyro, axis_ft_t *noGravityAccel, float deltaT){
    deltaT = deltaT/2;

    angles->x = angles->x + gyro->x*deltaT + deltaT*noGravityAccel->x * GRAVITY_CONTAIN;
    angles->y = angles->y + gyro->y*deltaT + deltaT*noGravityAccel->y * GRAVITY_CONTAIN;
    angles->z = angles->z + gyro->z*deltaT + deltaT*noGravityAccel->z * GRAVITY_CONTAIN;
}

void MPU6050_gyro_calibration(){
    axis_t measure;
    int64_t sumX = 0, sumY = 0, sumZ = 0;
    uint16_t sigmaX = 0, sigmaY = 0, sigmaZ = 0;
    uint sample = 0;

    printf("Start GYROSCOPE calibration process, dont move\n");
    for(int i = 3; i >= 0; i--){
        printf("t -%i\n", i);
        sleep_ms(1000);
    }

    const uint calibrationTime = make_timeout_time_us(GYRO_CALIBRATION_TIME);
    while(calibrationTime > time_us_32()){
        MPU6050_readRawGyro(&measure);

        sumX += measure.x;
        sumY += measure.y;
        sumZ += measure.z;

        sample++;
        sleep_ms(10);
    }

    printf("Measure time: %uus\n", calibrationTime - time_us_32() + GYRO_CALIBRATION_TIME);
    printf("Sample: %u\n", sample);

    mpu6050_calibrationData.gyro.offset.x = round(sumX/(float)sample);
    mpu6050_calibrationData.gyro.offset.y = round(sumY/(float)sample);
    mpu6050_calibrationData.gyro.offset.z = round(sumZ/(float)sample);


    printf("Gyroscope calibration value\n");
    printf("Offset:    %4hi, %4hi, %4hi\n", mpu6050_calibrationData.gyro.offset.x, mpu6050_calibrationData.gyro.offset.y,mpu6050_calibrationData.gyro.offset.z);
    printf("\n\n");
}
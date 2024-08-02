#include "tof.h"

void ToF_Init(VL53L0X_Dev_t *left, VL53L0X_Dev_t *center, VL53L0X_Dev_t *right){
    uint pinMask = 1 << ToF_LEFT_PIN | 1 << ToF_CENTER_PIN | 1 << ToF_RIGHT_PIN;
    gpio_init_mask(pinMask);
    gpio_set_dir_masked(pinMask, pinMask);
    gpio_put_masked(pinMask, ~pinMask);

    ToF_sensorInit(left, ToF_LEFT_ADDRESS, ToF_LEFT_PIN);
    ToF_sensorInit(center, ToF_CENTER_ADDRESS, ToF_CENTER_PIN);
    ToF_sensorInit(right, ToF_RIGHT_ADDRESS, ToF_RIGHT_PIN);
}

VL53L0X_Error ToF_sensorInit(VL53L0X_Dev_t *tof, uint8_t address, uint32_t pin){
    VL53L0X_Error error;
    tof->I2cDevAddr = ToF_DEFAULT_ADDRESS;
    tof->comms_speed_khz = I2C_FREQUENCY/1000;
    tof->comms_type = 1;
    gpio_put(pin, 1);
    
    VL53L0X_i2c_Init(I2C_CHANNEL, I2C_SDA_PIN, I2C_SCL_PIN, I2C_FREQUENCY/1000, false);
    sleep_ms(5);
    error = VL53L0X_SetDeviceMode(tof, VL53L0X_DEVICEMODE_SINGLE_RANGING);
    return VL53L0X_dev_init_i2c_newAddress(tof, VL53L0X_DEFAULT_MODE, address);
}

void ToF_startContinuesMeasuring(VL53L0X_Dev_t *tof){
    VL53L0X_Error error = VL53L0X_ContinuousRanging(tof, NULL, 0, NULL);
    if (error != VL53L0X_ERROR_NONE) printf("ToF_startContinuesMeasuring error: %d\n", error);
}


int16_t ToF_oneShotMeasure(VL53L0X_Dev_t *tof){
    int16_t data;
    int16_t error = VL53L0X_SingleRanging(tof, &data);
    if (error != VL53L0X_ERROR_NONE) return error;
    
    return data;
}
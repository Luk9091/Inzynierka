#include "tof.h"
repeating_timer_t ToF_timer;
ToF_t ToF_left, ToF_center, ToF_right;

bool ToF_measureTimer(repeating_timer_t *timer){
    int16_t left, center, right;
    uint distance = maxf(Motor_get_leftDistance(), Motor_get_rightDistance());
    // left = ToF_oneShotMeasure(&ToF_left);
    // center = ToF_oneShotMeasure(&ToF_center);
    // right = ToF_oneShotMeasure(&ToF_right);
    left = ToF_readData(&ToF_left);
    center = ToF_readData(&ToF_center);
    right = ToF_readData(&ToF_right);

    UDP_send("ToF: %i,  %i, %i, %i\n", distance, left, center, right);
    if (center < TOF_STOP_DISTANCE && center > 0){
        Motor_stop();
        UDP_send("Stop ToF\n");
        return false;
    }

    return timer->alarm_id != 0;
}

int ToF_Init(VL53L0X_Dev_t *left, VL53L0X_Dev_t *center, VL53L0X_Dev_t *right){
    uint pinMask = 1 << ToF_LEFT_PIN | 1 << ToF_CENTER_PIN | 1 << ToF_RIGHT_PIN;
    gpio_init_mask(pinMask);
    gpio_set_dir_masked(pinMask, pinMask);
    gpio_clr_mask(pinMask);

    VL53L0X_Error error;
    int sumError = 0;
    if (error = ToF_sensorInit(left, ToF_LEFT_ADDRESS, ToF_LEFT_PIN)      != VL53L0X_ERROR_NONE){
        printf("ToF_Init error: %i on device left\n", error);
        gpio_put(ToF_LEFT_PIN, 0);
        sumError |= 0b1;
    }
    if (error = ToF_sensorInit(center, ToF_CENTER_ADDRESS, ToF_CENTER_PIN)!= VL53L0X_ERROR_NONE) {
        printf("ToF_Init error: %i on device center\n", error);
        gpio_put(ToF_CENTER_PIN, 0);
        sumError |= 0b10;
    }
    if (error = ToF_sensorInit(right, ToF_RIGHT_ADDRESS, ToF_RIGHT_PIN)   != VL53L0X_ERROR_NONE) {
        printf("ToF_Init error: %i on device right\n", error);
        gpio_put(ToF_RIGHT_PIN, 0);
        sumError |= 0b100;
    }
    return sumError;
}

VL53L0X_Error ToF_sensorInit(VL53L0X_Dev_t *tof, uint8_t address, uint32_t pin){
    VL53L0X_Error error;
    VL53L0X_Error status;
    tof->comms_speed_khz = I2C_FREQUENCY/1000;
    tof->comms_type = 1;
    gpio_put(pin, 1);
    sleep_ms(5);
    
    VL53L0X_i2c_Init(I2C_CHANNEL, I2C_SDA_PIN, I2C_SCL_PIN, I2C_FREQUENCY/1000, false);
    
    tof->I2cDevAddr = address;
    error = VL53L0X_GetDeviceErrorStatus(tof, &status);
    if (error == VL53L0X_ERROR_NONE){
        return error;
    }

    tof->I2cDevAddr = ToF_DEFAULT_ADDRESS;
    error = VL53L0X_dev_init_i2c_newAddress(tof, VL53L0X_DEFAULT_MODE, address);
    if (error != VL53L0X_ERROR_NONE) return error;
    // error = VL53L0X_SetDeviceMode(tof, VL53L0X_DEVICEMODE_SINGLE_RANGING);
    error = VL53L0X_SetDeviceMode(tof, VL53L0X_DEVICEMODE_CONTINUOUS_RANGING);
    if (error != VL53L0X_ERROR_NONE) return error;
    error = VL53L0X_SetInterMeasurementPeriodMilliSeconds(tof, TOF_MEASURE_TIME);
    if (error != VL53L0X_ERROR_NONE) return error;
    error = VL53L0X_StartMeasurement(tof);
    return error;
}


int16_t ToF_oneShotMeasure(VL53L0X_Dev_t *tof){
    int16_t data;
    int16_t error = VL53L0X_SingleRanging(tof, &data);
    if (error != VL53L0X_ERROR_NONE) return error;
    
    return data;
}

int16_t ToF_readData(VL53L0X_Dev_t *tof){
    VL53L0X_RangingMeasurementData_t measurementData;
    int16_t data;
    data = VL53L0X_GetRangingMeasurementData(tof, &measurementData);
    if (data == VL53L0X_ERROR_NONE) {
        data = measurementData.RangeMilliMeter;
    }

    // Clear the interrupt flag
    VL53L0X_ClearInterruptMask(tof, VL53L0X_REG_SYSTEM_INTERRUPT_CLEAR);


    return data;
}


bool ToF_startRepeatMeasuring(){
    return add_repeating_timer_ms(TOF_MEASURE_TIME, ToF_measureTimer, NULL, &ToF_timer);
}


bool ToF_stopRepeatMeasuring(){
    return cancel_repeating_timer(&ToF_timer);
}
#include <stdio.h>
#include <pico/stdlib.h>
#include <pico/stdio.h>
#include <pico/cyw43_arch.h>

// #include <hardware/gpio.h>
#include "i2c.h"
#include "tof.h"
#include "mpu6050.h"
// #include "Servo/servo.h"

// #include "vl53l0x_api.h"
// #include "vl53l0x_rp2040.h"

static void ToF_init(VL53L0X_Dev_t *left, VL53L0X_Dev_t *center, VL53L0X_Dev_t *right){
    uint pinMask = 1 << ToF_LEFT_PIN | 1 << ToF_CENTER_PIN | 1 << ToF_RIGHT_PIN;
    gpio_init_mask(pinMask);
    gpio_set_dir_masked(pinMask, pinMask);
    gpio_put_masked(pinMask, ~pinMask);

    ToF_sensorInit(left, ToF_LEFT_ADDRESS, ToF_LEFT_PIN);
    ToF_sensorInit(center, ToF_CENTER_ADDRESS, ToF_CENTER_PIN);
    ToF_sensorInit(right, ToF_RIGHT_ADDRESS, ToF_RIGHT_PIN);
    // I2C_scan();
}

int main(){
    stdio_init_all();
    
    if(cyw43_arch_init()){
        printf("WiFi init failed\n");
        return -1;
    }

    I2C_Init();
    I2C_scan();

    VL53L0X_Dev_t ToF_left;
    VL53L0X_Dev_t ToF_center;
    VL53L0X_Dev_t ToF_right;
    ToF_init(&ToF_left, &ToF_center, &ToF_right);

    mpu6050_Init();
    
    int left, center, right;
    axis_t acc, gyro;
    float temp;


    while(1){
        cyw43_arch_gpio_put(CYW43_WL_GPIO_LED_PIN, !cyw43_arch_gpio_get(CYW43_WL_GPIO_LED_PIN));

        left = ToF_measure(&ToF_left);
        center = ToF_measure(&ToF_center);
        right= ToF_measure(&ToF_right);

        mpu6050_readData(&acc, &gyro, &temp);


        // printf("Distance: %4hi, %4hi, %4hi\n", left, center, right);
        // printf("Acc:  %4hi, %4hi, %4hi\n", acc.x, acc.y, acc.z);
        // printf("Gyro: %4hi, %4hi, %4hi\n", gyro.x, gyro.y, gyro.z);
        // printf("Temperature: %.2f\n", temp);


        sleep_ms(500);
    }

}

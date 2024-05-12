#include <stdio.h>
#include <pico/stdlib.h>
#include <pico/stdio.h>
#include <pico/cyw43_arch.h>

#include "i2c.h"
#include "tof.h"
#include "mpu6050.h"

#include <FreeRTOS.h>
#include <task.h>

void blink(void *params){
    while(1) {
        cyw43_arch_gpio_put(CYW43_WL_GPIO_LED_PIN, !cyw43_arch_gpio_get(CYW43_WL_GPIO_LED_PIN));
        vTaskDelay(pdMS_TO_TICKS(500));
    }
}

int main(){
    TaskHandle_t blink_handler;
    stdio_init_all();

    if(cyw43_arch_init()){
        printf("WiFi init failed\n");
        return -1;
    }

    if (pdPASS != xTaskCreate(blink, "Blink", 1024, NULL, 1, &blink_handler)){
        printf("Cannot create task\tBlink\n");
    }

    vTaskStartScheduler();

    while (1){
        printf("Hello World\n");
        sleep_ms(1000);
    }
    return 0;
}


/*
int main(){
    stdio_init_all();
    
    if(cyw43_arch_init()){
        printf("WiFi init failed\n");
        return -1;
    }
    cyw43_arch_gpio_put(CYW43_WL_GPIO_LED_PIN, !cyw43_arch_gpio_get(CYW43_WL_GPIO_LED_PIN));

    I2C_Init();
    // I2C_scan();

    // VL53L0X_Dev_t ToF_left;
    // VL53L0X_Dev_t ToF_center;
    // VL53L0X_Dev_t ToF_right;
    // ToF_init(&ToF_left, &ToF_center, &ToF_right);

    mpu6050_Init();
    mpu6050_gyro_calibration();
    // int left, center, right;
    axis_t acc, gyro;
    float temp;

    while(1){
        cyw43_arch_gpio_put(CYW43_WL_GPIO_LED_PIN, !cyw43_arch_gpio_get(CYW43_WL_GPIO_LED_PIN));
        // left = ToF_measure(&ToF_left);
        // center = ToF_measure(&ToF_center);
        // right= ToF_measure(&ToF_right);

        mpu6050_readData(&acc, &gyro, &temp);


        // printf("%hi %hi %hi %hi %hi %hi\n", acc.x, acc.y, acc.z, gyro.x, gyro.y, gyro.z);
        // printf("Distance: %4hi, %4hi, %4hi\n", left, center, right);
        // printf("Acc:  %4hi, %4hi, %4hi\n", acc.x, acc.y, acc.z);
        printf("Gyro: %4hi, %4hi, %4hi\n", gyro.x, gyro.y, gyro.z);
        // printf("Temperature: %.2f\n", temp);


        sleep_ms(10);
    }

}
// */
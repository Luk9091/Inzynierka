#include <stdio.h>
#include <pico/stdlib.h>
#include <pico/stdio.h>
#include <pico/cyw43_arch.h>

#include <hardware/clocks.h>
#include <hardware/pio.h>

#include "utilites.h"
#include "i2c.h"
#include "tof.h"
#include "mpu6050.h"
#include "compass.h"
#include "steppermotor.h"

#define STEP 8

int main(){
    stdio_init_all();
    // if(cyw43_arch_init()){
    //     printf("WiFi init failed\n");
    //     return -1;
    // }
    I2C_Init();
    // I2C_scan(I2C_CHANNEL);


    StepperMotor_t motor1 ;
    StepperMotor_init(&motor1, 512, (uint[4]){2, 3, 4, 5}, pio0);
    float anglePerStep = 360.f/(512.f) * STEP;
    float angle = 0;
    bool ADD = FORWARD;

    COMPASS_Init();
    axis_t axis;
       
    // StepperMotor_step(&motor1, 512, FORWARD);
    // while(motor1.stepsToTake > 0){
    //     StepperMotor_nextStep(&motor1);
    //     sleep_ms(1);
    // }
    // StepperMotor_step(&motor1, 512, BACKWARD);
    // while(motor1.stepsToTake > 0){
    //     StepperMotor_nextStep(&motor1);
    //     sleep_ms(1);
    // }

    printf("i, angle, x, y, z\n");
    int i = 0;
    while(i < 10){
        for (int i = 0; i < 25; i++){
            sleep_ms(100);
            COMPASS_rawRead(&axis);
            printf("%i, %3.2f, %hi, %hi, %hi\n", i, angle, axis.x, axis.y, axis.z);
        }

        if (ADD){
            angle += anglePerStep;
        } else {
            angle -= anglePerStep;
        }

        StepperMotor_step(&motor1, STEP, ADD);
        while(motor1.stepsToTake > 0){
            StepperMotor_nextStep(&motor1);
            sleep_ms(1);
        }

        if (angle > 360.f){
            ADD = BACKWARD;
        } else if (angle < 0.f){
            ADD = FORWARD;
            i++;
        }
    }

    gpio_put_masked(motor1.maskedPin, motor1.moving[motor1.substep] << motor1.pins[0]);
    return 0;

}
#include <pico/stdlib.h>
#include <pico/stdio.h>
#include <pico/cyw43_arch.h>


#include "servo.h"
#include "motor.h"


int main(){
    stdio_init_all();
    cyw43_arch_init_with_country(CYW43_COUNTRY_POLAND);
    cyw43_arch_gpio_put(CYW43_WL_GPIO_LED_PIN, 1);

    Servo_t servo;
    servo.GPIO = 15;
    servo.min = 70;
    servo.max = 110;
    Servo_init(&servo);
    Servo_setAngle(&servo, 90);

    Motor_init();
    Motor_forward();
    // sleep_ms(1000);
    // Motor_backward();
    // sleep_ms(1000);
    // Motor_stop();
    while(1){

    }

    return 0;
}
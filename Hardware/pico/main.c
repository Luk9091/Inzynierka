#include <pico/stdlib.h>
#include <pico/stdio.h>
#include <pico/cyw43_arch.h>



#include "servo.h"


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


    while(1){
    }

    return 0;
}
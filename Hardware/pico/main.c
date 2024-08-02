#include <pico/stdlib.h>
#include <pico/stdio.h>
#include <pico/cyw43_arch.h>


#include "my_gpio.h"
#include "i2c.h"

#include "tof.h"
#include "servo.h"
#include "motor.h"
#include "counter.h"



void Front_servo_Init(Servo_t * servo){
    servo->GPIO = GPIO_FRONT_SERVO;
    servo->min = FRONT_SERVO_MIN;
    servo->max = FRONT_SERVO_MAX;
    Servo_init(servo);
    Servo_setAngle(servo, 90);
}




int main(){
    Servo_t servo;
    ToF_t left, center, right;
    int16_t left_distance, center_distance, right_distance;
    stdio_init_all();
    cyw43_arch_init_with_country(CYW43_COUNTRY_POLAND);
    cyw43_arch_gpio_put(CYW43_WL_GPIO_LED_PIN, 1); 
    Motor_init(MOTOR_HALF_SPEED);
    Front_servo_Init(&servo);
    I2C_Init();
    ToF_Init(&left, &center, &right);

    char c = 0;
    while (c != ' '){
        c = getchar();
    }
    printf("Start\n");

    
    uint oldImpulse = 0;
    while(1){
        c = getchar();
        switch(c){
            case 'w':
                Motor_forwardDistance(10* CM);
            break;

            case 's':
                Motor_backwardDistance(10* CM);
            break;

            case 'a':
                servo.angle = servo.current_angle + 20;
                Servo_setAngle(&servo, servo.angle);
                printf("Angle: %d\n", servo.current_angle);
            break;

            case 'd':
                servo.angle = servo.current_angle - 20;
                Servo_setAngle(&servo, servo.angle);
                printf("Angle: %d\n", servo.current_angle);
            break;
        }
        printf("%c\n", c);

        if (Counter_getCount(MOTOR_ENCODER_LEFT_UP) != oldImpulse){
            oldImpulse = Counter_getCount(MOTOR_ENCODER_LEFT_UP);
            printf("Impulse: %d\n", oldImpulse);
        }
        sleep_ms(10);
    }

    return 0;
}
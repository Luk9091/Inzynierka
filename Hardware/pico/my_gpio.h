#ifndef __MY_GPIO_H__
#define __MY_GPIO_H__

#include <hardware/i2c.h>
#include <hardware/spi.h>

#define CAR_WIDTH                       (135)
#define CAR_LENGTH                      (155)

#define NC                              (-1)


#define GPIO_SPI_CHANNEL                spi0
#define GPIO_SPI_CLK                    2
#define GPIO_SPI_TX                     3
#define GPIO_SPI_RX                     4

#define GPIO_FLASH_CS                   5


#define GPIO_MOTOR_RIGHT_PIN_FORWARD    9
#define GPIO_MOTOR_RIGHT_PIN_BACKWARD   8
#define GPIO_MOTOR_ENCODER_RIGHT_UP     21
#define GPIO_MOTOR_ENCODER_RIGHT_DOWN   NC

#define GPIO_MOTOR_LEFT_PIN_FORWARD     7
#define GPIO_MOTOR_LEFT_PIN_BACKWARD    6
#define GPIO_MOTOR_ENCODER_LEFT_UP      19
#define GPIO_MOTOR_ENCODER_LEFT_DOWN    NC

#define GPIO_MOTOR_RIGHT_PWM            16
#define GPIO_MOTOR_LEFT_PWM             17

#define GPIO_FRONT_SERVO                13
#define FRONT_SERVO_MIN                 60
#define FRONT_SERVO_MAX                 120
#define FRONT_SERVO_OFFSET              -6

#define GPIO_RIGHT_BACK_SENSOR          27
#define GPIO_LEFT_BACK_SENSOR           26


#define GPIO_I2C_CHANNEL                i2c1
#define GPIO_I2C_SDA                    14
#define GPIO_I2C_SCL                    15

#define GPIO_ToF_RIGHT_CONTROL_PIN      11
#define GPIO_ToF_CENTER_CONTROL_PIN     12
#define GPIO_ToF_LEFT_CONTROL_PIN       10

#define GPIO_COMPASS_INT_PIN            22
#define GPIO_MPU6050_INT_PIN            28


#endif
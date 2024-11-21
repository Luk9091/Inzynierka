#include <pico/stdlib.h>
#include <pico/stdio.h>
#include <pico/multicore.h>
#include <pico/util/queue.h>
#include <hardware/timer.h>
#include <math.h>


#include "my_gpio.h"
#include "utilites.h"
#include "i2c.h"

#include "udp.h"
#include "tof.h"
#include "mpu6050.h"
#include "compass.h"
#include "servo.h"
#include "motor.h"
#include "counter.h"
#include "ir.h"
#include "eeprom.h"
#include "flash.h"

#define ANGLE_DT            50
static float old_angle    = 0;
Servo_t servo;
float angleError   = 0;
static int reachAngle = 0;
repeating_timer_t angle_timer;
static PID_t turning_pid;

static ToF_t ToF_left, ToF_center, ToF_right;

#define FORWARD_DIRECTION  0
#define BACKWARD_DIRECTION 1
static bool direction = FORWARD_DIRECTION;



bool angle_update(repeating_timer_t *timer){
    axis_ft_t acc, gyro;
    const float dt = ANGLE_DT/1000.f;

    int status = MPU6050_readData(&acc, &gyro);
    if (status != PICO_OK){
        return true;
    }

    axis_ft_t angles = {0, 0, 0};
    MPU6050_removeGravity(&acc);
    MPU6050_angleWithGyro(&angles, &gyro, &acc, dt);
    if (fabsf(angles.z) >= 0.005f){
        angleError += angles.z;
    }


    if (servo.angle == 90 && Motor_state() == MOTOR_RUN){
        int error = angleError;
        if (direction == BACKWARD_DIRECTION){
            error = -error;
        } 

        int pid = PID_update(&turning_pid, error, dt);
        Servo_setAngle(&servo, servo.angle + pid);
    }
    if (reachAngle != 0){
        if (abs(angleError) >= abs(reachAngle)){
            Motor_stop();
            reachAngle = 0;
        }
    }
    return true;
}


void Front_servo_Init(Servo_t * servo){
    servo->GPIO = GPIO_FRONT_SERVO;
    servo->min = FRONT_SERVO_MIN;
    servo->max = FRONT_SERVO_MAX;
    servo->offset = FRONT_SERVO_OFFSET;
    Servo_init(servo, false);
    Servo_setAngle(servo, 90);
    servo->angle = 90;
}

void Init_motion(){
    servo.angle = 90;
    Servo_start(&servo);
    Motor_setDistance(1);
    Motor_forward();

    Motor_setSpeed(MOTOR_HALF_SPEED);
}


void myReadLine(){
    char line[128] = {0};
    char c = 0;
    uint index = 0;


    while (true){
        c = 0;
        index = 0;

        while (!UDP_read(line)){
            sleep_ms(10);
        }

        float read = findFloat(line, 128);
        
        switch (line[0]){
            case 's':{
                static float speed;
                if (strlen(line) != 1){
                    speed = read;
                    uint16_t data = read * MOTOR_FULL_SPEED;
                    Motor_setSpeed(data);
                    UDP_send("Speed set to: %hu\n", data);
                }
                UDP_send("Speed: %2.2f\n", speed);
            } break;

            case 'i':{
                UDP_send("Impulse: %6i, %6i\n", Counter_getCount(MOTOR_ENCODER_LEFT_UP), Counter_getCount(MOTOR_ENCODER_RIGHT_UP));
                UDP_send("Distance: %6.2f, %6.2f\n", pulseToDistance(Counter_getCount(MOTOR_ENCODER_LEFT_UP)), pulseToDistance(Counter_getCount(MOTOR_ENCODER_RIGHT_UP)));
                const PID_t *pid = Motor_getPID();
                UDP_send("PID: %6.2f", pid->P + pid->I + pid->D);
                UDP_send("Angle: %6.2f\n", angleError);
            } break;

            case 'f': {
                if (strlen(line) > 1){
                    Motor_setDistance(read);
                    UDP_send("Distance set to: %6.2fmm\n", read);
                }   
                Servo_start(&servo);
                sleep_ms(250);
                IR_disable();
                angleError = 0;
                if (line[1] == 'l') {
                    Motor_forwardLeft();
                } else if (line[1] == 'r') {
                    Motor_forwardRight();
                } else {
                    Motor_forward();
                }
                direction = FORWARD_DIRECTION;
                UDP_send("Motor forward\n");
            } break;

            case 'b': {
                if (strlen(line) > 1){
                    Motor_setDistance(read);
                    UDP_send("Distance set to: %6.2fmm\n", read);
                }   
                Servo_start(&servo);
                sleep_ms(250);
                IR_enabled();
                angleError = 0;
                Motor_backward();
                direction = BACKWARD_DIRECTION;
                UDP_send("Motor backward\n");
            } break;

            case 'd': {
                Motor_setDistance(read);
                UDP_send("Distance set to: %6.2fmm\n", read);
                // stop = false;
            }break;

            case 'r':{
                if (line[1] == 'o'){
                    if (strlen(line) > 2){
                        servo.offset = read;
                    }
                    UDP_send("Servo offset: %i\n", servo.offset);
                    break;
                }

                if (line[1] == 's'){
                    if (servo.run){
                        Servo_stop(&servo);
                        UDP_send("Servo stop\n");
                    } else {
                        Servo_start(&servo);
                        UDP_send("Servo run\n");
                    }
                }

                if (strlen(line) > 2){
                    servo.angle = read;
                    Servo_overshoot(&servo, servo.angle);
                    #if MOTOR_DIFFERENTIAL
                        Motor_setDifferential(servo.angle);
                    #endif
                    angleError = 0;
                    PID_reset(&turning_pid);
                }
                UDP_send("Servo angle %i\n", servo.angle);
            } break;

            case 'c':{
                if (line[1] == 'r'){
                    cancel_repeating_timer(&angle_timer);

                    UDP_send("Reset bus\n");
                    I2C_resetBus();
                    UDP_send("Bus init\n");

                    add_repeating_timer_ms(ANGLE_DT, angle_update, NULL, &angle_timer);
                }

            #if COMPASS_AUTO_CALIBRATION_PROCESS
                cancel_repeating_timer(&angle_timer);
                axis_t offset = COMPASS_applyCalibration();
                add_repeating_timer_ms(ANGLE_DT, angle_update, NULL, &angle_timer);
                UDP_send("Calibration parameter: %i, %i, %i\n", offset.x, offset.y, offset.z);
            #endif
                axis_t magneto;
                COMPASS_read(&magneto);
                float azimuth = COMPASS_getAzimuth(magneto.x, magneto.y);
                UDP_send("Magnetometr: %6i, %6i, %6i\n", magneto.x, magneto.y, magneto.z);
                UDP_send("Azimuth: %6.2f\n", azimuth);
                UDP_send("Move angle: %.2f\n", angleError);
            } break;

            case 'm':{
                if (line[1] == 'r'){
                    int error = ToF_Init(&ToF_left, &ToF_center, &ToF_right);
                    if(error & 0b001)     UDP_send("ToF left error\n");
                    if(error & 0b010)     UDP_send("ToF center error\n");
                    if(error & 0b100)     UDP_send("ToF right error\n");
                }

                int left = ToF_oneShotMeasure(&ToF_left);
                int center = ToF_oneShotMeasure(&ToF_center);
                int right = ToF_oneShotMeasure(&ToF_right);
                UDP_send("M %i %i %i\n", left, center, right);
            } break;

            case '[':{
                int devs[20];
                int numOfDev = I2C_scan(i2c1, devs);
                UDP_send("I2C devices: %d\n", numOfDev);
                for (int i = 0; i < numOfDev; i++){
                    UDP_send("\tAddress: 0x%02X\n", devs[i]);
                }
            } break;

            default:
                break;
        }

        if (strlen(line) == 0){
            return;
        }
    }
}

void sendStatistic(){
    uint count_left, count_right;
    float distance_left, distance_right;
    uint16_t PWM_left, PWM_right;
    uint time = time_us_32();
    uint endTime = 0;

    while (time > endTime){
        count_left = Counter_getCount(MOTOR_ENCODER_LEFT_UP);
        count_right = Counter_getCount(MOTOR_ENCODER_RIGHT_UP);
        distance_left = pulseToDistance(count_left);
        distance_right = pulseToDistance(count_right);
        PWM_left = Motor_getSpeed(MOTOR_LEFT_PWM);
        PWM_right = Motor_getSpeed(MOTOR_RIGHT_PWM);

        time = time_us_32();
        UDP_send(
            "Time: %10uus, Count: %6u, %6u, distance: %6.2f, %6.2f, Angle: %6.2f, PWM: %6hu, %6hu\n",
            time, count_left, count_right, distance_left, distance_right, angleError, PWM_left, PWM_right
        );
        sleep_ms(100);
        
        if (Motor_state() == MOTOR_STOP){
            endTime = make_timeout_time_ms(2000);
        }
    }
}


int main(){
    stdio_init_all();
    multicore_launch_core1(UDP_main);
    IR_init();

    I2C_Init();
    I2C_resetBus();
    ToF_Init(&ToF_left, &ToF_center, &ToF_right);
    MPU6050_Init();
    COMPASS_Init();

    PID_init(&turning_pid, 0.5f, 1.f, 0.5f);

    Motor_init();
    Front_servo_Init(&servo);
    // Counter_disabledIRQ(MOTOR_ENCODER_LEFT_UP);
    // Counter_disabledIRQ(MOTOR_ENCODER_RIGHT_UP);


    float speed = 0.f;
    float _time;
#if MOTOR_DIFFERENTIAL
    Motor_setDifferential(90);
#endif
    add_repeating_timer_ms(ANGLE_DT, angle_update, NULL, &angle_timer);
    sleep_ms(ANGLE_DT * 3);
    angleError = 0;

    Init_motion();
    Servo_stop(&servo);
    Motor_pid_run();

    while(1){
        myReadLine();
        sendStatistic();
    }

    return 0;
}
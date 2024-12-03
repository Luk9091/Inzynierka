#include "test.h"


int SPEED_TEST__main(){
    Motor_init();
    Motor_pid_run();

    uint motorSpeed = MOTOR_FULL_SPEED * 0.2;
    uint speedDelta = MOTOR_FULL_SPEED * 0.05;
    const float distance = 1;//mm
    uint time = 0;
    uint countLeft, countRight;
    Motor_setDifferential(90);
        
    printf("Speed: %i\n", motorSpeed);
    Motor_setSpeed(motorSpeed);
    Motor_forwardDistance(distance);
    motorSpeed += speedDelta;

    while(1){
        if (Motor_state() != MOTOR_STOP){
            time = time_us_32();
            countLeft = Counter_getCount(MOTOR_ENCODER_LEFT_UP);
            countRight = Counter_getCount(MOTOR_ENCODER_RIGHT_UP);

            printf("%u,%.2f,%.2f\n", time, pulseToDistance(countLeft), pulseToDistance(countRight));
            sleep_ms(100);
        } else {
            sleep_ms(500);
            printf("Speed: %i\n", motorSpeed);
            Motor_setSpeed(motorSpeed);
            Motor_forwardDistance(distance);
            motorSpeed += speedDelta;
            if (motorSpeed >= 0.9f*MOTOR_FULL_SPEED){
                return 0;
            }
        }
    }
    return 0;
}
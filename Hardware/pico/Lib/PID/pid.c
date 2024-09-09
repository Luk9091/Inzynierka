#include "pid.h"



void PID_init(PID_t *pid, float kp, float ki, float kd){
    pid->kp = kp;
    pid->ki = ki;
    pid->kd = kd;
    PID_reset(pid);
}

void PID_reset(PID_t * pid){
    pid->P = 0;
    pid->I = 0;
    pid->D = 0;
    pid->_prevError = 0;
}

float PID_update(PID_t *pid, float error, float dt){
    pid->P = pid->kp * error;
    pid->I += pid->ki * (error*dt);
    pid->D = pid->kd * (error - pid->_prevError) / dt;
    pid->_prevError = error;

    return pid->P + pid->I + pid->D;
}

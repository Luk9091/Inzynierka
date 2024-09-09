#ifndef __PID_H__
#define __PID_H__

typedef struct {
    float kp;
    float ki;
    float kd;

    float P;
    float I;
    float D;

    float _prevError;
} PID_t;


void PID_init(PID_t *pid, float kp, float ki, float kd);
void PID_reset(PID_t * pid);
float PID_update(PID_t *pid, float error, float dt);


#endif
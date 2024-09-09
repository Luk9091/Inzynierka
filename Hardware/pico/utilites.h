#ifndef __UTILITIES__H
#define __UTILITIES__H

#include <pico/stdlib.h>

#define π 3.1415926F

typedef struct{
    int16_t x;
    int16_t y;
    int16_t z;
} axis_t;

typedef struct{
    float x;
    float y;
    float z;
} axis_ft_t;

inline float LPF_f(float alpha, float old, float new){
    return alpha * new + (1 - alpha) * old;
}

inline int LPF_i(float alpha, int old, int new){
    return alpha * new + (int)((1 - alpha) * (float)old);
}

inline axis_ft_t LPF_af(float alpha, axis_ft_t old, axis_ft_t new){
    axis_ft_t axis = {
        .x = LPF_f(alpha, old.x, new.x),
        .y = LPF_f(alpha, old.y, new.y),
        .z = LPF_f(alpha, old.z, new.z),
    };

    return axis;
}

axis_t LPF_ai(float alpha, axis_t old, axis_t new);


void getRunTime();

#endif
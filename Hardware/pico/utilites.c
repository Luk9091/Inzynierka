#include "utilites.h"
#include <stdio.h>
#include <stdlib.h>
#include <pico/stdio.h>
#include <pico/stdlib.h>



void getRunTime(){
    uint runTime = time_us_32();

    uint miliseconds,seconds, minutes, hours;

    miliseconds = runTime/1000;
    seconds = miliseconds/1000;
    minutes = seconds / 60;
    hours = seconds / 3600;
    seconds = seconds % 60;

    printf("%02i:%02i:%02i.%03i\n", hours, minutes, seconds, miliseconds);
}



axis_t LPF_ai(float alpha, axis_t old, axis_t new){
    axis_t axis = {
        .x = LPF_f(alpha, old.x, new.x),
        .y = LPF_f(alpha, old.y, new.y),
        .z = LPF_f(alpha, old.z, new.z),
    };

    return axis;
}



float findFloat(char *str, int size){
    int index = 0;
    for (index = 0; index < size; index++){
        if (str[index] >= '0' && str[index] <= '9'){
            if (index > 0 && str[index - 1] == '-'){
                index--;
            }
            return atoff(str + index);
        }
    }
    return 0;
}
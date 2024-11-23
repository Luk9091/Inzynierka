#include "utility.h"
#include <stdio.h>
#include <math.h>


float rounds(float value, uint decimalPlace){
    float factor = powf(10, decimalPlace);
    return roundf(value * factor) / factor;
}

int atoli(char *str, int *numbers){
    int find = 0;
    bool new = false;
    int sign = 1;
    int number = 0;

    int i = 0;
    char c = str[i];

    do{
        if (c >= '0' && c <= '9'){
            if (new == false){
                new = true;
                if (i != 0 && str[i-1] == '-'){
                    sign = -1;
                }
            }

            number = number * 10;
            number = number + (str[i] - '0');
        } else if (new) {
            numbers[find] = number * sign;
            find++;
            number = 0;
            new = false;
            sign = 1;
        }
        
        i++;
        c = str[i];
    } while (c > 0);

    if (new){
        numbers[find] = number * sign;
        find++;
    }

    return find;
}

inline int sleep_ms(uint miliseconds){
    return usleep(miliseconds * 1000);
}


float mapValue(float value, float fromMin, float fromMax, float toMin, float toMax){
    return (value - fromMin) * (toMax - toMin) / (fromMax - fromMin) + toMin;
}


int sign(int value){
    return value > 0 ? 1 : value < 0 ? -1 : 0;
}

int signf(float value){
    return value > 0 ? 1 : value < 0 ? -1 : 0;
}
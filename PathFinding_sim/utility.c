#include "utility.h"
#include <stdio.h>
#include <math.h>


float rounds(float value, uint decimalPlace){
    return (int)(value * powf(10, decimalPlace)) / powf(10, decimalPlace);
}

int atoli(char *str, int *numbers){
    int find = 0;
    bool new = false;
    int sign = 1;
    int number = 0;

    for (int i = 0; i < strlen(str); i++){
        if (str[i] >= '0' && str[i] <= '9'){
            if (new == false){
                new = true;
                if (i != 0 && str[i-1] == '-'){
                    sign = -1;
                } else {
                    sign = 1;
                }
            }

            number = number * 10;
            number = number + (str[i] - '0');
        } else if (new) {
            numbers[find] = number * sign;
            find++;
            number = 0;
            new = false;
        }
    }

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
#include "utility.h"
#include <math.h>


float rounds(float value, uint decimalPlace){
    return (int)(value * powf(10, decimalPlace)) / powf(10, decimalPlace);
}
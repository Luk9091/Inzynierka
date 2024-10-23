#include "servo.h"
#include <stdio.h>
#include <hardware/clocks.h>


// Servo need 50Hz, f_sys = 125HMz
#define PRESCALER   100
#define TOP       24999

#define LEVEL_MIN 600
#define LEVEL_MAX 3200

#define ADC_MAX 3850
#define ADC_MIN 260

typedef struct ServoOvershoot_t {
    Servo_t *servo;
    int shoot;
} ServoOvershoot_t;

void Servo_init(Servo_t *servo, bool enable){
    static uint ADC_STATE = 0;
    servo->run = enable;

    if (servo->step == 0)
        servo->step = 1;

    // PWM INIT
    gpio_set_function(servo->GPIO, GPIO_FUNC_PWM);
    uint slice_num = pwm_gpio_to_slice_num(servo->GPIO);

    pwm_config config = pwm_get_default_config();

    config.div = PRESCALER << PWM_CH0_DIV_INT_LSB;
    config.top = TOP;

    pwm_init(slice_num, &config, enable);

    if (servo->ADC_PIN >= 26 && servo->ADC_PIN <= 28){
        adc_gpio_init(servo->ADC_PIN);
        servo->current_angle = Servo_readAngle(servo);
    } else {
        servo->current_angle = 0;
    }
}


void Servo_setAngle(Servo_t *servo, uint angle){
    if (angle > servo->max){angle = servo->max;} else
    if (angle < servo->min){angle = servo->min;}

    uint16_t level = LEVEL_MIN + angle * ((LEVEL_MAX - LEVEL_MIN) / 180);
    servo->current_angle = angle;
    pwm_set_gpio_level(servo->GPIO, level);
}


int Servo_readAngle(Servo_t *servo){
    adc_select_input(servo->ADC_PIN - 26);
    int value = adc_read();

    value = ((value - ADC_MIN) * 180) / (ADC_MAX - ADC_MIN);
    if (servo->flipRead)
        value = 180 - value;
    return value;
}

void Servo_reachAngle(Servo_t *servo){
    if (servo->angle > servo->max){servo->angle = servo->max;} else
    if (servo->angle < servo->min){servo->angle = servo->min;}

    uint16_t level = LEVEL_MIN +  servo->angle * ((LEVEL_MAX - LEVEL_MIN) / 180);
    servo->current_angle = servo->angle;
    pwm_set_gpio_level(servo->GPIO, level);
}


void Servo_goto(Servo_t *servo){
    uint small_step = servo->step;

    if (servo->current_angle != servo->angle){
        if (abs(servo->angle - servo->current_angle) < servo->step){
            small_step = abs(servo->angle - servo->current_angle);
        }
        if(servo->current_angle > servo->angle){
            servo->current_angle -= small_step;
        } else {
            servo->current_angle += small_step;
        }

        Servo_setAngle(servo, servo->current_angle);
    }
}

uint64_t _Servo_backFromOvershoot(alarm_id_t id, void *args){
    Servo_t *servo = (Servo_t *)args;

    uint16_t level = LEVEL_MIN + (servo->angle) * ((LEVEL_MAX - LEVEL_MIN) / 180);
    pwm_set_gpio_level(servo->GPIO, level);
    servo->current_angle = servo->angle;

    return 0;
}


void Servo_overshoot(Servo_t * servo, uint angle){
    if (servo->angle > servo->max){servo->angle = servo->max;} else
    if (servo->angle < servo->min){servo->angle = servo->min;}

    int delta = angle - servo->current_angle;
    int shoot = delta / 4;
    if (shoot > 10) {shoot = 10;}
    else if (shoot < -10) {shoot = -10;}
    
    uint16_t level = LEVEL_MIN + (angle + shoot) * ((LEVEL_MAX - LEVEL_MIN) / 180);
    servo->angle = angle;
    pwm_set_gpio_level(servo->GPIO, level);

    add_alarm_in_ms(100, (alarm_callback_t)_Servo_backFromOvershoot, servo, false);
}

void Servo_start(Servo_t *servo){
    servo->run = true;
    servo->angle = servo->backUp;
    Servo_setAngle(servo, servo->current_angle);
    uint slice = pwm_gpio_to_slice_num(servo->GPIO);
    pwm_set_enabled(slice, true);
}

void Servo_stop(Servo_t *servo){
    servo->run = false;
    uint slice = pwm_gpio_to_slice_num(servo->GPIO);
    pwm_set_enabled(slice, false);
}
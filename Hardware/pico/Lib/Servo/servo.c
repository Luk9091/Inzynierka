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


static inline uint _Servo_checkValue(Servo_t *servo, uint value){
    if (value > servo->max){value = servo->max;} else
    if (value < servo->min){value = servo->min;}
    return value;
}

static inline uint _Servo_angleToLevel(Servo_t *servo, uint angle){
    return LEVEL_MIN + (angle + servo->offset) * ((LEVEL_MAX - LEVEL_MIN) / 180);
}

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

#if SERVO_READ_ADC
    if (servo->ADC_PIN >= 26 && servo->ADC_PIN <= 28){
        adc_gpio_init(servo->ADC_PIN);
        servo->current_angle = Servo_readAngle(servo);
    }
#endif
}


void Servo_setAngle(Servo_t *servo, uint angle){
    angle = _Servo_checkValue(servo, angle);

    uint16_t level = _Servo_angleToLevel(servo, angle);
    servo->current_angle = angle;
    pwm_set_gpio_level(servo->GPIO, level);
}


#if SERVO_READ_ADC
int Servo_readAngle(Servo_t *servo){
    adc_select_input(servo->ADC_PIN - 26);
    int value = adc_read();

    value = ((value - ADC_MIN) * 180) / (ADC_MAX - ADC_MIN);
    if (servo->flipRead)
        value = 180 - value;
    return value;
}
#endif

void Servo_reachAngle(Servo_t *servo){
    Servo_setAngle(servo, servo->angle);
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

    Servo_setAngle(servo, servo->angle);

    return 0;
}


void Servo_overshoot(Servo_t * servo, uint angle){
    angle = _Servo_checkValue(servo, angle);

    int delta = angle - servo->current_angle;
    int shoot = delta / 4;
    if (shoot > 10) {shoot = 10;}
    else if (shoot < -10) {shoot = -10;}
    
    uint16_t level = _Servo_angleToLevel(servo, angle + shoot);
    servo->angle = angle;
    pwm_set_gpio_level(servo->GPIO, level);

    add_alarm_in_ms(100, (alarm_callback_t)_Servo_backFromOvershoot, servo, false);
}

void Servo_start(Servo_t *servo){
    servo->run = true;
    Servo_setAngle(servo, servo->current_angle);
    uint slice = pwm_gpio_to_slice_num(servo->GPIO);
    pwm_set_enabled(slice, true);
}

void Servo_stop(Servo_t *servo){
    servo->run = false;
    uint slice = pwm_gpio_to_slice_num(servo->GPIO);
    pwm_set_enabled(slice, false);
}
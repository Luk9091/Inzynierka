#ifndef __COUNTER_H__
#define __COUNTER_H__

#include <hardware/gpio.h>
#include <hardware/pwm.h>

/*!
 * \brief Use PWM as counter,
 *
 * \param
 * pin - GPIO pin to use as counter MUST BE ODD
 * countTo - value to count to
*/
void Counter_init(uint8_t pin);

void Counter_enabledIRQ(uint8_t pin);
void Counter_disabledIRQ(uint8_t pin);
void Counter_setCountTo(uint8_t pin, uint16_t countTo);
void Counter_clear(uint8_t pin);

static inline uint16_t Counter_getCount(uint8_t pin){
    return pwm_get_counter(pwm_gpio_to_slice_num(pin));
}


#endif
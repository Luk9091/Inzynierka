#ifndef __24C04_H__
#define __24C04_H__

#include <pico/stdio.h>
#include <pico/stdlib.h>

#include "i2c.h"
#include "my_gpio.h"

#define I2C_EEPROM_ADDRESS_SECTOR_1 0x50
#define I2C_EEPROM_ADDRESS_SECTOR_2 0x51

/*
 * twr time in ms
 * max time needed to rewrite data from cash to EEPROM
*/
#define EEPROM_WRITE_TIME           5
#define EEPROM_WRITE_PAGE           16

#define EEPROM_WP_ENABLED false

#if EEPROM_WP_ENABLED == true
#define EEPROM_WRITE_PROTECTION_PIN GPIO_EEPROM_WP
void EEPROM_Init();
#endif

int EEPROM_write(uint16_t address, uint8_t *buffer, uint len);
int EEPROM_read(uint16_t address, uint len, uint8_t *buffer);


#endif
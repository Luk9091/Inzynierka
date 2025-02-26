#ifndef __MY_I2C__H
#define __MY_I2C__H

#include <stdio.h>
#include <pico/stdlib.h>

#include <hardware/gpio.h>
#include <hardware/i2c.h>
#include <hardware/irq.h>

#include "my_gpio.h"

// Value in Hz
// Example: 100 * 1000 = 100kHz
#define I2C_FREQUENCY (400 * 1000)

#define I2C_CHANNEL GPIO_I2C_CHANNEL
#define I2C_SDA_PIN GPIO_I2C_SDA
#define I2C_SCL_PIN GPIO_I2C_SCL

void I2C_Init();
int I2C_scan(i2c_inst_t *channel, int *devList);

// void I2C_send(uint8_t dev, uint8_t byte);
// #define I2C_write(dev, data) i2c_write_blocking(I2C_CHANNEL, dev, data, 1, false)
// uint8_t I2C_read(uint8_t dev);
// #define I2C_read(dev, data) i2c_read_blocking(I2C_CHANNEL, dev, data, 1, false)

int I2C_writeReg(uint8_t dev, uint8_t reg, uint8_t data);
int I2C_writeRegMask(uint8_t dev, uint8_t reg, uint8_t mask, uint8_t data);

/*
 * I2C set pin function
 * \param
 * dev - device address
 * reg - register number
 * bit - number of set bit
*/
int I2C_setBit(uint8_t dev, uint8_t reg, uint8_t bit);
/*
 * I2C set pin function
 * \param
 * dev - device address
 * reg - register number
 * bit - number of clear bit
*/
int I2C_clearBit(uint8_t dev, uint8_t reg, uint8_t bit);
/*
 * I2C set pin function
 * \param
 * dev - device address
 * reg - register number
 * bit - number of toggle bit
*/
int I2C_toggleBit(uint8_t dev, uint8_t reg, uint8_t bit);

uint8_t I2C_readReg(uint8_t dev, uint8_t reg);
int I2C_readNReg(uint8_t dev, uint8_t startReg, uint8_t *data, size_t size);

void I2C_changeReg(uint dev, uint reg, uint8_t data, uint8_t mask);


void I2C_resetBus();

#endif
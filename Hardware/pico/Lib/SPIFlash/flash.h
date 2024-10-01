#ifndef __SPI_FLASH_H__
#define __SPI_FLASH_H__

#include <pico/stdlib.h>
#include <hardware/gpio.h>
#include <hardware/spi.h>
#include <hardware/clocks.h>

#include "my_gpio.h"


#define FLASH_SPI       GPIO_SPI_CHANNEL
#define FLASH_SPI_SPEED (1 * MHZ)
#define FLASH_TX        GPIO_SPI_TX
#define FLASH_RX        GPIO_SPI_RX
#define FLASH_CLK       GPIO_SPI_CLK
#define FLASH_CS        GPIO_FLASH_CS

typedef enum {
    FLASH_4K,
    FLASH_32K,
    FLASH_64K,
    FLASH_ALL,
} FLASH_BLOCK_SIZE;


void FLASH_init();
uint16_t FLASH_getID();
uint16_t FLASH_getStatus(bool olderReg);

void FLASH_write(uint32_t address, uint8_t *buffer, size_t len);
void FLASH_read(uint32_t address, uint8_t *buffer, size_t len);

void FLASH_erase(uint32_t address, FLASH_BLOCK_SIZE block);


#endif
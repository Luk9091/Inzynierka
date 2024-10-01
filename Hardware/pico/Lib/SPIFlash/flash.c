#include "flash.h"
#include "mem_map.h"

#include <stdio.h>

void _SPI_init(){
    spi_init(FLASH_SPI, FLASH_SPI_SPEED);
    
    gpio_set_function(FLASH_CLK, GPIO_FUNC_SPI);
    gpio_set_function(FLASH_RX, GPIO_FUNC_SPI);
    gpio_set_function(FLASH_TX, GPIO_FUNC_SPI);

    gpio_init(FLASH_CS);
    gpio_set_dir(FLASH_CS, GPIO_OUT);
}

static inline void _SPI_select(){
    gpio_put(FLASH_CS, 0);
}

static inline void _SPI_deselect(){
    gpio_put(FLASH_CS, 1);
}

static inline void _FLASH_writeEnable(){
    uint8_t cmd[1] = {WRITE_ENABLE};
    _SPI_select();
    spi_write_blocking(FLASH_SPI, cmd, sizeof(cmd));
    _SPI_deselect();
}

static inline void _FLASH_writeDisenable(){
    uint8_t cmd[1] = {WRITE_DISABLE};
    _SPI_select();
    spi_write_blocking(FLASH_SPI, cmd, sizeof(cmd));
    _SPI_deselect();
}

void FLASH_init(){
    _SPI_init();
    _SPI_deselect();
}

uint16_t FLASH_getID(){
    uint8_t cmd[4] = {
        ID,
        0, 0, 0,
    };
    uint8_t readBuffer[2];

    _SPI_select();
    spi_write_blocking(FLASH_SPI, cmd, sizeof(cmd));
    spi_read_blocking(FLASH_SPI, 0, readBuffer, 2);
    _SPI_deselect();

    uint16_t read = readBuffer[0] << 8 | readBuffer[1];
    return read;
}

uint16_t FLASH_getStatus(bool olderReg){
    uint8_t cmd[1] = {READ_STATUS_REG_1};
    if (olderReg){
        cmd[0] = READ_STATUS_REG_2;
    }

    uint8_t readBuffer[2];

    _SPI_select();
    spi_write_blocking(FLASH_SPI, cmd, sizeof(cmd));
    spi_read_blocking(FLASH_SPI, 0, readBuffer, 2);
    _SPI_deselect();

    uint16_t read = readBuffer[0] << 8 | readBuffer[1];
    return read;
}


void FLASH_erase(uint32_t address, FLASH_BLOCK_SIZE block){
    uint8_t cmd[4] = {
        ERASE_4KB,
        address >> 16,
        address >> 8,
        address,
    };

    switch(block){
        case FLASH_4K: {
            cmd[0] = ERASE_4KB;
        } break;
        case FLASH_32K: {
            cmd[0] = ERASE_32KB;
        }break;
        case FLASH_64K: {
            cmd[0] = ERASE_64KB;
        } break;
        case FLASH_ALL: {
            cmd[0] = ERASE_ALL;
        } break;

        default: {
            cmd[0] = ERASE_4KB;
        } break;
    }

    _FLASH_writeEnable();
    _SPI_select();
    spi_write_blocking(FLASH_SPI, cmd, sizeof(cmd));
    _SPI_deselect();
    _FLASH_writeDisenable();
}


void FLASH_write(uint32_t address, uint8_t *buffer, size_t len){
    uint8_t cmd[4] = {
        PROGRAM_DATA,
        address >> 16,
        address >> 8,
        address
    };

    _FLASH_writeEnable();
    _SPI_select();
    spi_write_blocking(FLASH_SPI, cmd, sizeof(cmd));
    spi_write_blocking(FLASH_SPI, buffer, len);
    _SPI_deselect();
    _FLASH_writeDisenable();
}


void FLASH_read(uint32_t address, uint8_t *buffer, size_t len){
    uint8_t cmd[4] = {
        READ_DATA,
        address >> 16,
        address >> 8,
        address
    };

    _SPI_select();
    spi_write_blocking(FLASH_SPI, cmd, sizeof(cmd));
    spi_read_blocking(FLASH_SPI, 0, buffer, len);
    _SPI_deselect();
}
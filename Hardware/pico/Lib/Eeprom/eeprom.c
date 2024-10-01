#include "eeprom.h"

#include <hardware/i2c.h>
#include <memory.h>

#if EEPROM_WP_ENABLED == true
#include <hardware/gpio.h>
void EEPROM_Init(){
    gpio_init(EEPROM_WRITE_PROTECTION_PIN);
    gpio_put(EEPROM_WRITE_PROTECTION_PIN, 0);
}
#endif


int EEPROM_write(uint16_t address, uint8_t *buffer, uint len){
    uint8_t data[1 + 16];
    
    uint8_t dev = I2C_EEPROM_ADDRESS_SECTOR_1 + (address / 256);
    address = address % 256;
    data[0] = address;
    uint size = len % (EEPROM_WRITE_PAGE + 1);
    bool holdLine = true;
    memcpy(data + 1, buffer, size);
    while(len > 0){
        if (size >= len){
            holdLine = false;
        }

        int status = i2c_write_blocking(I2C_CHANNEL, dev, data, size+1, holdLine);
        if (status == PICO_ERROR_GENERIC) return PICO_ERROR_NO_DATA;
        len = len - size;
        len = len % (EEPROM_WRITE_PAGE + 1);
        // if (len > 0){
            sleep_ms(EEPROM_WRITE_TIME);
        // }
    }
    return PICO_ERROR_NONE;
}


int EEPROM_read(uint16_t address, uint len, uint8_t *buffer){
    uint8_t dev = I2C_EEPROM_ADDRESS_SECTOR_1 + (address / 256);
    uint8_t _address = address % 256;
    int status;
    
    status = i2c_write_blocking(I2C_CHANNEL, dev, &_address, 1, true);
    if (status < 0) return status;

    if (address + len <= 256){
        status = i2c_read_blocking(I2C_CHANNEL, dev, buffer, len, false);
    } else{
        int over = address + len - 256;
        status = i2c_read_blocking(I2C_CHANNEL, dev, buffer, len - over, true);
        if (status < 0) return status;

        dev++;
        _address = 0;
        status = i2c_write_blocking(I2C_CHANNEL, dev, &_address, 1, true);
        if (status < 0) return status;

        status = i2c_read_blocking(I2C_CHANNEL, dev, buffer+(len - over), over, true);
    }

    return status;
}
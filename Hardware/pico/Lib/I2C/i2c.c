#include "i2c.h"




void I2C_Init(){
    static int isInit = 0;
    if (isInit) return;
    
    i2c_init(I2C_CHANNEL, I2C_FREQUENCY);
    gpio_set_function(I2C_SDA_PIN, GPIO_FUNC_I2C);
    gpio_set_function(I2C_SCL_PIN, GPIO_FUNC_I2C);
    gpio_pull_up(I2C_SDA_PIN);
    gpio_pull_up(I2C_SCL_PIN);
    isInit = 1;
}

int I2C_scan(i2c_inst_t *channel, int *devList){
    I2C_Init();
    int ret = 0;
    uint8_t rxData;
    uint dev = 0;

    printf("\nI2C Scan\n");
    

    for (uint8_t address = 0; address < 128; ++address){
        ret = i2c_read_blocking(channel, address, &rxData, 1, false);

        if (ret >= 0){
            printf("Address: 0x");
            printf("%02X\n", address);
            if (devList != NULL){
                devList[dev] = address;
            }
            dev++;
        }
    }

    if(dev == 0){
        printf("Dont find any devices!\n");
        return -1;
    }
    return dev;
}

int I2C_writeReg(uint8_t dev, uint8_t reg, uint8_t data){
    uint8_t table[2] = {reg, data};
    return i2c_write_blocking(I2C_CHANNEL, dev, table, 2, false);
}

int I2C_writeRegMask(uint8_t dev, uint8_t reg, uint8_t mask, uint8_t data){
    uint8_t oldData = I2C_readReg(dev, reg);
    oldData = oldData & ~mask;
    data = oldData | data;
    return I2C_writeReg(dev, reg, data);
}

int I2C_setBit(uint8_t dev, uint8_t reg, uint8_t bit){
    uint8_t data = I2C_readReg(dev, reg);
    data |= 1 << bit;
    return I2C_writeReg(dev, reg, data);
}

int I2C_clearBit(uint8_t dev, uint8_t reg, uint8_t bit){
    uint8_t data = I2C_readReg(dev, reg);
    data &= ~(1 << bit);
    return I2C_writeReg(dev, reg, data);
}

int I2C_toggleBit(uint8_t dev, uint8_t reg, uint8_t bit){
    uint8_t data = I2C_readReg(dev, reg);
    data ^= 1 << bit;
    return I2C_writeReg(dev, reg, data);
}

uint8_t I2C_readReg(uint8_t dev, uint8_t reg){
    uint8_t data = 0;
    i2c_write_blocking(I2C_CHANNEL, dev, &reg, 1,  true);
    if (i2c_read_blocking(I2C_CHANNEL, dev, &data, 1, false) >= 0) return data;
    return 0;
}

int I2C_readNReg(uint8_t dev, uint8_t startReg, uint8_t *data, size_t size){
    i2c_write_blocking(I2C_CHANNEL, dev, &startReg, 1, true);
    return i2c_read_blocking(I2C_CHANNEL, dev, data, size, false);
}

void I2C_changeReg(uint dev, uint reg, uint8_t data, uint8_t mask){
    uint8_t oldData = I2C_readReg(dev, reg);
    oldData = oldData & ~mask;
    data = oldData | data;
    I2C_writeReg(dev, reg, data);
}

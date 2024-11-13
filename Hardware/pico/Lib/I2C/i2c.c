#include "i2c.h"
#include <hardware/sync.h>

#define I2C_TIMEOUT     1000



void I2C_Init(){
    i2c_init(I2C_CHANNEL, I2C_FREQUENCY);
    gpio_set_function(I2C_SDA_PIN, GPIO_FUNC_I2C);
    gpio_set_function(I2C_SCL_PIN, GPIO_FUNC_I2C);
    gpio_pull_up(I2C_SDA_PIN);
    gpio_pull_up(I2C_SCL_PIN);
}

int I2C_scan(i2c_inst_t *channel, int *devList){
    I2C_Init();
    int ret = 0;
    uint8_t rxData;
    uint dev = 0;

    printf("\nI2C Scan\n");
    

    for (uint8_t address = 0; address < 128; ++address){
        int IRQ_status = save_and_disable_interrupts();
        ret = i2c_read_timeout_us(channel, address, &rxData, 1, false, I2C_TIMEOUT);
        restore_interrupts(IRQ_status);

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
    uint IRQ_status = save_and_disable_interrupts();
    int status =  i2c_write_timeout_us(I2C_CHANNEL, dev, table, 2, false, I2C_TIMEOUT);
    restore_interrupts(IRQ_status);
    return status;
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
    int IRQ_status = save_and_disable_interrupts();
    i2c_write_timeout_us(I2C_CHANNEL, dev, &reg, 1,  true, I2C_TIMEOUT);
    int status = i2c_read_timeout_us(I2C_CHANNEL, dev, &data, 1, false, I2C_TIMEOUT);
    restore_interrupts(IRQ_status);
    if (status < 0){
        return 0;
    }
    return data;
}

int I2C_readNReg(uint8_t dev, uint8_t startReg, uint8_t *data, size_t size){
    int IRQ_status = save_and_disable_interrupts();
    i2c_write_timeout_us(I2C_CHANNEL, dev, &startReg, 1, true, I2C_TIMEOUT);
    int status =  i2c_read_timeout_per_char_us(I2C_CHANNEL, dev, data, size, false, I2C_TIMEOUT);
    restore_interrupts(IRQ_status);
    return status;
}

void I2C_changeReg(uint dev, uint reg, uint8_t data, uint8_t mask){
    uint8_t oldData = I2C_readReg(dev, reg);
    oldData = oldData & ~mask;
    data = oldData | data;
    I2C_writeReg(dev, reg, data);
}


void I2C_resetBus(){
    const uint CLK = 100;
    gpio_init(I2C_SDA_PIN);
    gpio_init(I2C_SCL_PIN);

    gpio_set_dir(I2C_SDA_PIN, GPIO_OUT);
    gpio_put(I2C_SDA_PIN, 1);

    gpio_set_dir(I2C_SCL_PIN, GPIO_OUT);
    gpio_pull_up(I2C_SCL_PIN);

    while(gpio_get(I2C_SCL_PIN) == 0){
        gpio_put(I2C_SCL_PIN, 0);
        sleep_us(CLK);
        gpio_put(I2C_SCL_PIN, 1);
        sleep_us(CLK);
    }
        
    gpio_put(I2C_SCL_PIN, 0);
    sleep_us(CLK);
    gpio_put(I2C_SCL_PIN, 1);
    sleep_us(CLK);

    gpio_deinit(I2C_SDA_PIN);
    gpio_deinit(I2C_SCL_PIN);
    I2C_Init();
}
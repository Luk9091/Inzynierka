#ifndef __COMPASS__REG_MAP__H__
#define __COMPASS__REG_MAP__H__

#define COMPASS_OUT_X_LSB_REG       0x00
#define COMPASS_OUT_X_MSB_REG       0x01
#define COMPASS_OUT_Y_LSB_REG       0x02
#define COMPASS_OUT_Y_MSB_REG       0x03
#define COMPASS_OUT_Z_LSB_REG       0x04
#define COMPASS_OUT_Z_MSB_REG       0x05
#define COMPASS_STATUS_REG          0x06
#define COMPASS_TEMPERATURE_LSB_REG 0x07
#define COMPASS_TEMPERATURE_MSB_REG 0x08
#define COMPASS_CONTROL_1_REG       0x09
#define COMPASS_CONTROL_2_REG       0x0A
#define COMPASS_PERIOD_REG          0x0B
#define COMPASS_CHIP_IP             0x0D

typedef enum {
    STANDBY_MODE  = 0 << 0,
    CONTINUE_MODE = 1 << 0
} COMPASS_MODE;


typedef enum {
    FREQ_10HZ  = 0 << 2,
    FREQ_50HZ  = 1 << 2,
    FREQ_100HZ = 2 << 2,
    FREQ_200HZ = 3 << 2
} COMPASS_DATA_RATE;

typedef enum {
    RANGE_2G = 0 << 4,
    RANGE_8G = 1 << 4,
} COMPASS_SCALE_RANGE;

typedef enum {
    SAMPLE_512  = 0 << 6,
    SAMPLE_256  = 1 << 6,
    SAMPLE_128  = 2 << 6,
    SAMPLE_64   = 3 << 6
} COMPASS_SAMPLE_RATE;

#endif
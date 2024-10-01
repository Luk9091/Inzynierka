#ifndef __SPI_FLASH_MEM_MAP_H__
#define __SPI_FLASH_MEM_MAP_H__

#define MAX_SIZE            32000000
#define PAGE_SIZE           256

#define PROGRAM_DATA        0x02
#define WRITE_ENABLE        0x06
#define WRITE_DISABLE       0x04

#define READ_DATA           0x03
#define FAST_READ           0x0B

#define WRITE_STATUS_REG    0x01
#define READ_STATUS_REG_1   0x05
#define READ_STATUS_REG_2   0x35

#define ERASE_4KB           0x20
#define ERASE_32KB          0x52
#define ERASE_64KB          0xD8
#define ERASE_ALL           0xC7

#define ID                  0x90
#define POWER_DOWN          0xB9


#endif
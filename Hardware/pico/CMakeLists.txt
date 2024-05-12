cmake_minimum_required(VERSION 3.25)

# select board:
set(PICO_BOARD pico_w)

include($ENV{PICO_SDK_PATH}/external/pico_sdk_import.cmake)
include($ENV{PICO_SDK_PATH}/tools/CMakeLists.txt)

project(main C CXX ASM)
set(CMAKE_C_STANDARD 11)
set(CMAKE_CXX_STANDARD 17) 

pico_sdk_init()


add_executable(${PROJECT_NAME}
    main.c
    ToF/tof.c
    I2C/i2c.c
    MPU6050/mpu6050.c

    TCP/tcp.c
)

# pico_generate_pio_header(servo_pio ${CMAKE_CURRENT_LIST_DIR}/Servo/servo.pio)

pico_add_extra_outputs(${PROJECT_NAME})
include_directories(
    .
    I2C
    ToF
    MPU6050
    TCP
)
add_subdirectory(VL53L0X/Api)
add_subdirectory(FreeRTOS)


target_link_libraries(${PROJECT_NAME} LINK_PUBLIC
    pico_stdlib

    # pico_cyw43_arch_none
    pico_cyw43_arch_lwip_poll
    hardware_gpio
    hardware_i2c
    # hardware_pio

    vl53l0x_api_rp2040
    FreeRTOS
)


pico_enable_stdio_usb(${PROJECT_NAME} 0)
pico_enable_stdio_uart(${PROJECT_NAME} 1)

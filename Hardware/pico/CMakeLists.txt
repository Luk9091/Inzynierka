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
    utilites.c
    Lib/ToF/tof.c
    Lib/I2C/i2c.c
    Lib/MPU6050/mpu6050.c
    Lib/Compass/compass.c
    Lib/Servo/servo.c
    Lib/StepperMotor/steppermotor.c
    Lib/Motor/motor.c
    Lib/Counter/counter.c
    Lib/UDP/udp.c
    Lib/PID/pid.c
    Lib/24C04/24c04.c
)

# pico_generate_pio_header(${PROJECT_NAME} ${CMAKE_CURRENT_LIST_DIR}/StepperMotor/counter.pio)

pico_add_extra_outputs(${PROJECT_NAME})
include_directories(
    .
    Lib/I2C
    Lib/ToF
    Lib/MPU6050
    Lib/Compass
    Lib/Servo
    Lib/StepperMotor
    Lib/Motor
    Lib/Counter
    Lib/UDP
    Lib/PID
    Lib/24C04
)

add_subdirectory(Lib/VL53L0X/Api)
# add_subdirectory(FreeRTOS)


target_link_libraries(${PROJECT_NAME} LINK_PUBLIC
    pico_stdlib
    pico_multicore

    # pico_cyw43_arch_none
    pico_cyw43_arch_lwip_poll
    hardware_gpio
    hardware_i2c
    hardware_pwm
    hardware_adc
    # hardware_pio

    vl53l0x_api_rp2040
    # FreeRTOS
)


pico_enable_stdio_usb(${PROJECT_NAME} 1)
pico_enable_stdio_uart(${PROJECT_NAME} 1)

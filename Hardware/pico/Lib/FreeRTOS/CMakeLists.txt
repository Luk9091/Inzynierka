# Using branch: 83da5fc95

add_library(FreeRTOS
    $ENV{FREERTOS_KERNEL_PATH}/event_groups.c
    $ENV{FREERTOS_KERNEL_PATH}/list.c
    $ENV{FREERTOS_KERNEL_PATH}/queue.c
    $ENV{FREERTOS_KERNEL_PATH}/stream_buffer.c
    $ENV{FREERTOS_KERNEL_PATH}/tasks.c
    $ENV{FREERTOS_KERNEL_PATH}/timers.c
    $ENV{FREERTOS_KERNEL_PATH}/portable/MemMang/heap_3.c
    $ENV{FREERTOS_KERNEL_PATH}/portable/GCC/ARM_CM0/port.c
)

target_include_directories(FreeRTOS PUBLIC
    .
    $ENV{FREERTOS_KERNEL_PATH}/include
    $ENV{FREERTOS_KERNEL_PATH}/portable/GCC/ARM_CM0
)
cmake_minimum_required(VERSION 3.25)

project(main C)
set(CMAKE_C_STANDARD 11)


add_executable(${PROJECT_NAME}
    main.c
    utility.c
    Map/map.c
    Map/test/test_map.c
    Pathfinding/pathfinding.c
    Car/car.c
    UDP/udp.c
    Queue/queue.c
    GUI/slider_panel.c
)

include_directories(${PROJECT_NAME}
    .
    Map
    Map/test
    Pathfinding
    Car
    UDP
    Queue
    GUI
)

target_link_libraries(${PROJECT_NAME}
    raylib
    m
)
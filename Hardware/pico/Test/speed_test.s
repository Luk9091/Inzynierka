jump main

distance = 1000
speed = 0.90
delta = 0.05

time = 1


main:
    sleep $time
    r 90
    s $speed
    f $distance
    speed = speed + delta

    if speed <= 1
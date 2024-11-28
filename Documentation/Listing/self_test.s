jump main
speed = 0.1
distance = 100
baseAngle = 90
deltaAngel = 30


main:
    print "Check I2C"
    [
    mr
    m
    c
    sleep 5

    print "Check servo"
    r $baseAngle
    rs
    sleep 5
    rs
    sleep 2
    rs $baseAngle + $deltaAngel
    sleep 5
    rs
    sleep 2
    rs $baseAngle - $deltaAngel
    sleep 5
    rs

    print "Check motor"
    s $speed
    f $distance
    sleep 5

    sleep 10
    print "Please check back sensor"
    b $distance
    sleep 10
    end


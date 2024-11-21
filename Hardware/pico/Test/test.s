s 0.1
jump main

wait = 5
distance = 500
dangle = 5
angle = 90

measure:
    r $angle
    f $distance
    sleep $wait
    i

    b $distance
    sleep $wait
    i

    end



toRight:
    angle = 90 - dangle
    jump measure

    end

toLeft:
    angle = 90 + dangle
    jump measure

    end
    


main:
    print "Delta angle:"
    print dangle

    jump toRight
    jump toLeft

    dangle = dangle + 5
    print "##############################"
    if dangle <= 30
        jump main

    end

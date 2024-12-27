jump main           # Rozpocznij od funkcji main, linia 8
speed = 0.1
distance = 100
baseAngle = 90
deltaAngel = 30


main:
    print "Check I2C"
    [               # Wyświetl listę urządzeń I2C
    mr              # Zresetuj ToF
    m               # Zmierz odległość
    c               # Zmierz dane z akcelerometru
    sleep 5

    print "Check servo"
    r $baseAngle    # Ustaw kąt serwa na 90 stopni
    rs              # Uruchom serwo
    sleep 5
    rs              # Zatrzymaj serwo
    sleep 2
    rs $baseAngle + $deltaAngel # Ustaw kąt serwa na 120deg
    sleep 5
    rs
    sleep 2
    rs $baseAngle - $deltaAngel # Ustaw kąt serwa na 60deg
    sleep 5
    rs

    print "Check motor"
    s $speed        # Ustaw prędkość na 0.1 mocy maksymalnej
    f $distance     # Przejedź dystans 100mm
    sleep 15

    print "Please check back sensor"
    # Jeśli czujniki IR wykryją przeszkodę, robot zatrzyma się
    b $distance    # Przejedź 100mm do tyłu
    sleep 10
    end            # Zakończ program lub funkcję


import serial

device = serial.Serial('/dev/ttyACM0', 115200)
file = open('data.csv', 'w')
print('Reading data from the device')
print('Press Ctrl+C to stop the program')

try:
    while True:
        line: str = device.readline().decode('utf-8')
        print(line, end='')
        line = line.strip(" ")
        file.write(line)
except KeyboardInterrupt:
    file.close()
    device.close()

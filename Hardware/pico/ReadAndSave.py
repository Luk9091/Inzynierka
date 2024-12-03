import serial
import sys

device = serial.Serial('/dev/ttyACM0', 115200)

args = sys.argv[1:]
if len(args) > 0:
    file = open(f"{args[0]}.csv", 'w')
else:
    file = open('data.csv', 'w')
print('Reading data from the device')
print('Press Ctrl+C to stop the program')

device.write(b'1')

try:
    while True:
        line: str = device.readline().decode('utf-8')
        print(line, end='')
        line = line.strip(" ")
        file.write(line)
except KeyboardInterrupt:
    file.close()
    device.close()
except serial.SerialException as err:
    print("Lost connection with serial")
    print(f"Error: {err}")
    device.close()
    file.close()

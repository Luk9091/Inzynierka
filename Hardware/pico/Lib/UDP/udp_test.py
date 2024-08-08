import socket
import time
from time import sleep

udp_socket = socket.socket(socket.AF_INET, socket.SOCK_DGRAM)
udp_socket.bind(("10.42.0.1", 4444))
pico_address = ("10.42.0.210", 4444)
# upd_socket.settimeout(0.5)

msg = "s"
request = "request"

try:
    for _ in range(1):
        # localTime = time.strftime("%H:%M:%S", time.localtime())
        # msg = f"Local time: {localTime}"
        # print(msg)
        # # print(msg.encode())
        # # data, address = udp_socket.recvfrom(127)
        # # print(f"Received from {address}")
        # # print(data.decode())
        # # udp_socket.sendto(request.encode(), address)

        udp_socket.sendto(msg.encode(), pico_address)
        sleep(1)
except (KeyboardInterrupt, EOFError):
    print("Closing UDP socket")
    
udp_socket.close()

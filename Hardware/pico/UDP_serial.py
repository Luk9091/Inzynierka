import sys
from click import getchar, echo
import threading
import socket
import time
from time import sleep


UDP_SERVER_PORT = 4444
UDP_CLIENT_PORT = 4445
run = True
pc_address   = ("10.42.0.1", UDP_SERVER_PORT)
MAX_UDP_PACKET_SIZE = 128


def receive(udp_socket: socket.socket):
    global run
    while run:
        data, addr = udp_socket.recvfrom(MAX_UDP_PACKET_SIZE)
        data = data.strip(b'\x00')
        data = data.decode("utf-8")

        try: 
            with open("data.csv", "a") as file:
                file.write(data)
        except IndexError:
            pass

        if data.endswith("\n"):
            data = data + "\r"
        print(data, end="")



def main():
    args = sys.argv[1:]
    global run

    if len(args) > 0:
        address = f"10.42.0.{args[0]}"
    else:
        address = "10.42.0.22"

    pico_address = (address, UDP_CLIENT_PORT)
    print(f"UDP Serial IP: {pico_address[0]}:{pico_address[1]}")


    udp_socket = socket.socket(socket.AF_INET, socket.SOCK_DGRAM)
    udp_socket.setsockopt(socket.SOL_SOCKET, socket.SO_REUSEADDR, 1)
    udp_socket.bind(pc_address)
    
    receive_thread = threading.Thread(target=receive, args=(udp_socket,))
    receive_thread.start()
    

    try:
        while run:
            char = input()

            udp_socket.sendto(char.encode(), pico_address)
    except (KeyboardInterrupt, EOFError):
        run = False
        print("\nClosing UDP socket")

    try:
        udp_socket.shutdown(socket.SHUT_RD)
    except OSError:
        pass
    
    udp_socket.close()
    receive_thread.join()


if __name__ == "__main__":
    main()
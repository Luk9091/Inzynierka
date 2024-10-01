import sys
from click import getchar, echo
import threading
import socket
import time
from time import sleep


UDP_SERVER_PORT = 4444
UDP_CLIENT_PORT = 4445
run = True
MAX_UDP_PACKET_SIZE = 128
ROUTER_ADDRESS       = "12.12.0"
DEFAULT_PC_ADDRESS   = "12.12.0.150"
DEFAULT_PICO_ADDRESS = "148"

END_LINE = ""
STORE_DATA = True

def readArgs() -> None:
    args = sys.argv[1:]
    if "--addNewLine" in args:
        global END_LINE
        END_LINE="\n"
        args.remove("--addNewLine")

    if "--noSave" in args:
        global STORE_DATA
        STORE_DATA = False
        args.remove("--noSave")


    PC_IP = DEFAULT_PC_ADDRESS
    if len(args) > 0:
        if args[0] == "localhost":
            address = "127.0.0.1"
            PC_IP = address
        else:
            address = f"{ROUTER_ADDRESS}.{args[0]}"
    else:
        address = f"{ROUTER_ADDRESS}.{DEFAULT_PICO_ADDRESS}"
    
    if len(args) > 1:
        global UDP_SERVER_PORT
        UDP_SERVER_PORT = int(args[1])

    pc_address   = (PC_IP, UDP_SERVER_PORT)

    return pc_address, address



def storeLine(data) -> None:
    try: 
        with open("data.csv", "a") as file:
            file.write(data)
    except IndexError:
        pass



def receive(udp_socket: socket.socket):
    while run:
        data, addr = udp_socket.recvfrom(MAX_UDP_PACKET_SIZE)
        data = data.strip(b'\x00')
        data = data.decode("utf-8")

        if STORE_DATA:
            storeLine(data)

        if data.endswith("\n"):
            data = data + "\r"
            end_line = ""
        else:
            end_line = END_LINE
        print(data, end=end_line)



def main():
    global run
    global END_LINE
    pc_address, address = readArgs()

        

    pico_address = (address, UDP_CLIENT_PORT)
    print(f"Server IP:     {pc_address[0]}:{pc_address[1]}")
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

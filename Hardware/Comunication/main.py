#! /usr/bin/python3


import os, sys
import socket
from collections import deque
from colorama import Fore, Back, Style
import threading
from time import sleep
from input_params import param_parser
import default
import log
import interpreter


def receive(udp_socket: socket.socket):
    while default.RUN:
        data, _ = udp_socket.recvfrom(default.MAX_UDP_PACKET_SIZE)
        data = data.strip(b'\x00')
        if default.DISPLAY_BINARY:
            print(data)
        else:
            data = data.decode("utf-8")
            if data.endswith("\n"):
                data = data + "\r"
                end_line = ""
            else:
                end_line = default.END_LINE

            print(data, end=end_line)

        if (default.STORE_DATA):
            log.save(data)

def send(string: str, udp_socket: socket.socket) -> None:
    string = string.encode()
    data = bytearray([*string, *b'\x00'])
    udp_socket.sendto(data, default.PICO_ADDRESS)


def read(udp_socket: socket.socket) -> bool:
    try:
        with open(default.READ_FILE_PATH, "r") as file:
            lines = file.readlines()
    except FileNotFoundError:
        print(Fore.RED + f"Error: File '{default.READ_FILE_PATH.stem}' not found" + Style.RESET_ALL)
        return False

    lines = interpreter.interpretation(lines)
    for line in lines:
        print(line)
        send(line, udp_socket)
        sleep(default.READ_FILE_DELAY)
    
    return True

    


def main():
    try:
        param_parser()
    except ValueError as e:
        print(Fore.RED + f"Error: {e}" + Style.RESET_ALL)
        sys.exit(1)

    try:
        udp_socket = socket.socket(socket.AF_INET, socket.SOCK_DGRAM)
        udp_socket.bind(default.PC_ADDRESS)
    except OSError:
        print(Fore.RED + "Error: Address already in use" + Style.RESET_ALL)
        print("Close other instances of the program or change the port")
        sys.exit(1)

    receive_thread = threading.Thread(target=receive, args=(udp_socket,))
    receive_thread.start()

    print(f"Server IP:     {default.PC_ADDRESS[0]}:{default.PC_ADDRESS[1]}")
    print(f"UDP Serial IP: {default.PICO_ADDRESS[0]}:{default.PICO_ADDRESS[1]}")

    if default.READ_FILE_PATH is not None:
        try:
            default.RUN = read(udp_socket)
        except (KeyboardInterrupt, EOFError) as e:
            if isinstance(e, KeyboardInterrupt):
                print()
            default.RUN = False
            print(Fore.RED + "Exiting..." + Style.RESET_ALL)
            pass



    while default.RUN:
        try:
            string = input()
        except (KeyboardInterrupt, EOFError) as e:
            if isinstance(e, KeyboardInterrupt):
                print()
            default.RUN = False
            print(Fore.RED + "Exiting..." + Style.RESET_ALL)
            break

        if string.startswith("file"):
            default.READ_FILE_PATH = string.split(" ")[1]
            try:
                default.RUN = read(udp_socket)
            except (KeyboardInterrupt, EOFError) as e:
                if isinstance(e, KeyboardInterrupt):
                    print()
                print(Fore.RED + "Stop reading file" + Style.RESET_ALL)
        else:
            send(string, udp_socket)


    try:
        udp_socket.shutdown(socket.SHUT_RD)
    except OSError:
        pass
    udp_socket.close()
    receive_thread.join()


if __name__ == "__main__":
    main()
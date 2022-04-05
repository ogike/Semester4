# history.py

import sys
import socket
import struct
import random

server_addr = "localhost"
server_port = 10002

sock = socket.socket(socket.AF_INET, socket.SOCK_DGRAM)
sock.bind((server_addr, server_port))

while True:
    try:
        data, client_addr = sock.recvfrom(1024)
        print(client_addr, ":", data.decode())
    except KeyboardInterrupt:
        break
sock.close()
print("Server terminated")

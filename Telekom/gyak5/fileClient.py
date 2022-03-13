from socket import socket, AF_INET, SOCK_STREAM
import sys

server_addr = (sys.argv[1], int(sys.argv[2]))

with socket(AF_INET, SOCK_STREAM) as client:
    with open(sys.argv[3], "rb") as f:
        client.connect(server_addr)
        l = f.read(128)
        while l:
            client.sendall(l)
            l = f.read(128)

print("Elküldtem!")

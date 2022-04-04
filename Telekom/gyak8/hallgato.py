import sys
import socket
import random
import struct
import time

BUFFER_SIZE = 1024 #TODO: calc

server_addr = "localhost" #sys.argv[1]
server_port = 10000 #int(sys.argv[2])

sock = socket.socket(socket.AF_INET, socket.SOCK_STREAM)

sock.connect((server_addr, server_port))

## SENDING ######################################X

msg_out = b'Kerek egy feladatot'
print("Sending message: ", msg_out)
sock.sendall(msg_out)

## RECEIVING ######################################X

try:
    msg_in = sock.recv(BUFFER_SIZE).decode()
    print("Gyakvez ezt mondta: ", msg_in)

    if(msg_in == "Tessek a feladat"):
        sock.sendall(b"Koszonjuk")

        msg_in_snd = sock.recv(BUFFER_SIZE)
        print("Gyakvez ezt mondta: ", msg_in)

except:
    print("Server closed the connection.")

sock.close()

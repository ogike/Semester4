from audioop import add
from http import client
import socket
import struct
import random



BUFFER_SIZE = 1024
addr = 'localhost'
port = 10001


sock = socket.socket(socket.AF_INET, socket.SOCK_DGRAM)
sock.bind( (addr, port) )




sock.setblocking(0)
sock.settimeout(1)

while(True):
    try:
        msg_in, client_addr = sock.recvfrom(BUFFER_SIZE)
        msg_in_parsed = msg_in.decode() 

        print("Kliens üzente: ", msg_in_parsed)

        reply = "Feladat: " + str( random.randint(1, 10000) )

        sock.sendto(reply.encode(), client_addr)
    except KeyboardInterrupt:
        break
    except socket.timeout:
        continue

sock.close()


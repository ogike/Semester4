import sys
import socket
import random
import struct
import time

server_addr = sys.argv[1]
server_port = int(sys.argv[2])

sock = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
sock.bind( (server_addr, server_port) )
sock.listen(1)
conn, addr = sock.accept()
print("Valaki csatlakozott:", addr)

packer_length = struct.Struct('i')
while True:
    msg = conn.recv(packer_length.size)  #szerver kettőként olvassa
                                         # először kiolvas gyakorlatilag a bufferből egy int mennyiséget
    if msg:
        length = packer_length.unpack(msg)[0]
    else:
        print("A kliens lezárta a kapcsolatot")
        break
    print("Hossz:", length)
    msg = conn.recv(length)
    print("A kliens üzenete:",msg.decode())
sock.close()

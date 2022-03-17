from http import client
import socket
import struct
import random

TCP_IP = 'localhost'
TCP_PORT = 10000
BUFFER_SIZE = 1024

sock = socket.socket(socket.AF_INET, socket.SOCK_DGRAM)
sock.bind( (TCP_IP, TCP_PORT) )

data, client_addr = sock.recvfrom(BUFFER_SIZE)

packer_in = struct.Struct('4i 6s')
packer_out = struct.Struct('i')

sock.setblocking(0)
sock.settimeout(1)
while(True):
    try:
        data_packed, client_addr = sock.recvfrom(packer_in.size)
        data_unpacked = packer_in.unpack(data_packed)
        reply = random.random(100)
        reply_packed = 
    except KeyboardInterrupt:
        break
    except socket.timeout:
        continue

sock.close()


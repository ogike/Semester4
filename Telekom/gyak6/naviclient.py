import socket
import struct
import sys

TCP_IP = 'localhost'
TCP_PORT = 10000
BUFFER_SIZE = 1024

sock = socket.socket(socket.AF_INET, socket.SOCK_DGRAM)

coordX = int(input())
coordY = int(input())
hour   = int(input())
min    = int(input())
to_send_values = (coordX, coordY, hour, min, "HM37UQ".encode())

packer_out = struct.Struct('4i 6s')
packer_in = struct.Struct('i')
to_send_packed = packer_out.pack(*to_send_values)

sock.sendto(to_send_packed, (TCP_IP, TCP_PORT) )

reply_packed, _ = sock.recvfrom(packer_in.size)
reply_unpacked = packer_in.unpack(reply_packed)[0]
print("Ennyi idő alatt lehet eljutni: ", reply_unpacked)

sock.close()
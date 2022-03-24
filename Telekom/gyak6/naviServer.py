from http import client
import socket
import struct
import random

TCP_IP = 'localhost'
TCP_PORT = 10000
MAP_IP = 'localhost'
MAP_PORT = 10001


sock_bank = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
sock_bank.connect((MAP_IP, MAP_PORT))

sock = socket.socket(socket.AF_INET, socket.SOCK_DGRAM)
sock.bind( (TCP_IP, TCP_PORT) )




packer_in = struct.Struct('4i 6s')
packer_out = struct.Struct('i')

sock.setblocking(0)
sock.settimeout(1)

while(True):
    try:
        data_packed, client_addr = sock.recvfrom(packer_in.size)
        data_unpacked = packer_in.unpack(data_packed)

        print("Kliens üzente: ", data_unpacked)

        # reply = random.randint(1, 10000)
        # reply_packed = packer_out.pack(reply)

        sock_bank.sendall(data_packed)
        answer, _ = sock_bank.recvfrom(packer_out.size)

        sock.sendto(answer, client_addr)
    except KeyboardInterrupt:
        break
    except socket.timeout:
        continue

sock.close()


import socket
import struct
import sys

#TODO: get from argv
server_addr = 'localhost'
server_port = 10000

sock = socket.socket(socket.AF_INET, socket.SOCK_STREAM)

sock.connect((server_addr, server_port))

#same for in/out packages, number isnt used when server sends it
packer = struct.Struct('c i')

guess = 50
dir = '<'
msg_out = packer.pack(dir.encode(), guess)

sock.sendall(msg_out)

msg_in = sock.recv(packer.size)
msg_in_unpacked = packer.unpack(msg_in)
print("Kapott válasz", msg_in_unpacked[0])

sock.close()

import socket
import struct

TCP_IP = 'localhost'
TCP_PORT = 10000
BUFFER_SIZE = 1024

sock = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
sock.connect( (TCP_IP, TCP_PORT) )

while True:
    x = int( input("Operandus 1: ") )
    y = int( input("Operandus 2: ") )
    op = input("Művelet: ")
    values = (x, y, op.encode())

    packer = struct.Struct('i i c')
    packed_data = packer.pack(*values)

    sock.send(packed_data)

    #should have checked if server is still connected
        #de ezek nem elvárások a tárgyban

    reply = sock.recv(BUFFER_SIZE)
    print("Eredmény: ", reply.decode())

sock.close()


import socket
import struct

TCP_IP = 'localhost'
TCP_PORT = 10000
BUFFER_SIZE = 1024

sock = socket.socket(socket.AF_INET, socket.SOCK_DGRAM)

running = True
while running:
    try:
        x = int( input("Operandus 1: ") )
        y = int( input("Operandus 2: ") )
        op = input("Művelet: ")
        values = (x, y, op.encode())

        packer = struct.Struct('i i c')
        packed_data = packer.pack(*values)

        sock.sendto(packed_data, (TCP_IP, TCP_PORT))

        #should have checked if server is still connected
            #de ezek nem elvárások a tárgyban

        reply, _ = sock.recvfrom(BUFFER_SIZE)
        print("Eredmény: ", reply.decode())
    except KeyboardInterrupt:
        running = False
        pass


sock.close()


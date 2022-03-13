import sys
import socket
import struct
import select

server_addr = sys.argv[1]
server_port = int(sys.argv[2])

sock = socket.socket(socket.AF_INET, socket.SOCK_DGRAM)
sock.bind( (server_addr, server_port) )

# nincs listen!
#   így akármennyi kliens is "csatlakozhat" is egyszerre

while True:
    try:
        data, client_addr = sock.recvfrom(1024)
        print(client_addr,":",data.decode())
        msg = b"Hello client"
        sock.sendto(msg,client_addr) # sendto a kliensnek
    except KeyboardInterrupt:
        break
sock.close()
print("Server terminated")
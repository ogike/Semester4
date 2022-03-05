import socket
import struct

TCP_IP = 'localhost'
TCP_PORT = 10000
BUFFER_SIZE = 1024

sock = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
sock.connect( (TCP_IP, TCP_PORT) )

msgStr = input("Üzenet amit küldök szervernek: ")
msgSize = len(msgStr)

print("Üzenet mérete: ", msgSize)

msgSizePacker = struct.Struct('i')
msgSizePacked = msgSizePacker.pack(msgSize)
sock.send( msgSizePacked )

#check if received..?

msgStrPacker = struct.Struct('%ds' % int(msgSize))
msgStrPacked = msgStrPacker.pack(msgStr.encode())
sock.send( msgStrPacked )


print( sock.recv(BUFFER_SIZE).decode() )

sock.close()


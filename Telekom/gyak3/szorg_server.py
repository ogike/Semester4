# FELADAT #####################

# Készíts egy szerver-kliens alkalmazást amiben
#     a kliens egy random méretű stringet küld a szervernek, 
#     úgy, hogy mielőtt ezt megtenné, elküldi neki a hosszát.
# A szerver pontosan a megkapott mennyiségű adatot fogadjon.

###############################

import socket
import struct


TCP_IP = 'localhost'
TCP_PORT = 10000
BUFFER_SIZE = 1024

sock = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
sock.bind( (TCP_IP, TCP_PORT) )
sock.listen(1)

print("Waiting for someone to connect...")
conn, addr = sock.accept()

#string méretének fogadása
sizeFormat = 'i'
sizeStruct = struct.Struct(sizeFormat)
recSize = conn.recv( struct.calcsize(sizeFormat) )

print('Recieved size: ', recSize[0])

#egész string fogadása
msgFormat = '%ds' % int(recSize[0])
msg = conn.recv( struct.calcsize(msgFormat) )
unpackedMsg = struct.Struct(msgFormat).unpack(msg)

print("Recieved string: ", unpackedMsg[0].decode())

conn.send( "Recieved on server".encode() )
sock.close()

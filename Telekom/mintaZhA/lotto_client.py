# FELADAT ###############
# Készíts egy lottóalkalmazást, ahol a szerver generál 5 számot 1 és 20 között (csak hogy legyen esély
# nyerni). Ezek lesznek a lottószámok. A kliensek beküldik a tippjeiket, illetve hogy mennyi pénzt
# raknak fel a szelvényre. A server megmondja, hogy mik voltak a nyerőszámok, es hogy mennyit nyert
# az adott játékos. 1 találat 1x, 2 találat 2x, 3 találat 3x, stb.

# 1/A: Old meg a feladatot TCP felett
# 2/B: Az üzenet formátuma struct legyen, es legálabb 2 mező legyen benne
# 3:   A szerverre egyszerre tudjon több kliens is csatlakozni! (TCP-nél selecttel)
# 4/D: A kliens a tippeket parancssori argumentumkent kapja
# 5/A: history proxyként

import sys
import socket
import random
import struct


## CONNECTING ######################################X
server_addr = "localhost"
server_port = 10000

sock = socket.socket(socket.AF_INET, socket.SOCK_STREAM)

sock.connect((server_addr, server_port))

## GETTING DATA ######################################X

guesses : int = []
bet : int = 0

for i in range(0, 5):
    guesses.append( int( sys.argv[i+1] ) )
bet = int( sys.argv[6] )

## PACKERS ######################################X

packer_out = struct.Struct('5i i') #[5 guesses, bet]
packer_in = struct.Struct('i') #how much we won

## SENDING ######################################X

msg_out = packer_out.pack(*guesses, bet)
print("Sending message: ", guesses, " : ", bet)
sock.sendall(msg_out)

## RECEIVING ######################################X

try:
    msg_in = sock.recv(packer_in.size)
    msg_in_unpacked = packer_in.unpack(msg_in)
    print("Won this amount: ", msg_in_unpacked[0])
except:
    print("Server closed the connection.")

sock.close()
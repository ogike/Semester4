# FELADAT ###############
# Készíts egy lottóalkalmazást, ahol a szerver generál 5 számot 1 és 20 között (csak hogy legyen esély
# nyerni). Ezek lesznek a lottószámok. A kliensek beküldik a tippjeiket, illetve hogy mennyi pénzt
# raknak fel a szelvényre. A server megmondja, hogy mik voltak a nyerőszámok, es hogy mennyit nyert
# az adott játékos. 1 találat 1x, 2 találat 2x, 3 találat 3x, stb.

# 1/A: Old meg a feladatot UDP felett
# 2/B: Az üzenet formátuma bytes legyen ':' szeparátorral!
# 3:   A szerverre egyszerre tudjon több kliens is csatlakozni! (TCP-nél selecttel)
# 4/D: A kliens a tippeket json fájlból olvassa be!
# 5/A: history külön szerverként, aminek tcp-vel küld a szerver

import json
import sys
import socket
import random
import struct

BUFFER_SIZE : int = 1024


## CONNECTING ######################################X
server_addr = "localhost"
server_port = 10000

sock = socket.socket(socket.AF_INET, socket.SOCK_DGRAM)

sock.connect((server_addr, server_port))

## GETTING DATA ######################################X

if(len( sys.argv ) < 1):
    print("No filename gaven as parameter!")
    exit
filename = sys.argv[1]

guesses : int = [2,4,6,8,10]
bet : int = 5

with open(filename, "r") as guess_file:
    guess_data = json.load(guess_file)
    guesses = guess_data["guesses"]
    guesses = list( map(str, guesses) )
    bet = str( guess_data["bet"] )
    print("test: ", type( guesses[0] ), guesses[0])

## SENDING ######################################X

to_send = guesses
to_send.append(bet)
msg_out = ":".join( to_send ) 
print("Sending message: ", msg_out)
sock.sendall(msg_out.encode())

## RECEIVING ######################################X

try:
    msg_in = sock.recv(BUFFER_SIZE)
    msg_in_unpacked = msg_in.decode()
    print("Won this amount: ", msg_in_unpacked)
except:
    print("Server closed the connection.")

sock.close()
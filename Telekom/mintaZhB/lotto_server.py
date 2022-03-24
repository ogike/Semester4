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

import sys
import socket
import random
import struct
import select

BUFFER_SIZE : int = 1024

## CREATING SERVER SOCKET ###############################X
server_addr = "localhost"
server_port = 10000

sock = socket.socket(socket.AF_INET, socket.SOCK_DGRAM)
sock.bind( (server_addr, server_port) )
sock.setblocking(0) # making keyboardinterrupt possible
sock.settimeout(1)

## CONNECTING TO HISTORY SERVER
history_addr = "localhost"
history_port = 10001

history_sock = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
history_sock.connect((history_addr, history_port))

## GENERATING DATA ######################################X

numbers : int = []
for i in range(0, 5):
    numbers.append( random.randint(1, 20) )

print("Generated numbers: ", numbers)

## SERVING CLIENTS ######################################X

print("Waiting for clients...")
while True:
    try:
        msg, client_addr = sock.recvfrom(BUFFER_SIZE)
        if not msg: # client closed connection
            print("Egy kliens lezárta a kapcsolatot")
            continue

        ## HANDLING MSG ################################X
        parsed_msg = msg.decode().split(":") # [5 guesses, bet]
        print("Client ", client_addr, " sent msg: ", parsed_msg)


        ## CALCULATING RESULT ################################X
        correct_guesses = 0

        #could also check if there arent any duplicates in guesses
        for i in range(0, 5): #number index
            for j in range(0, 5): #guess index
                if numbers[i] == int( parsed_msg[j] ):
                    correct_guesses += 1
                
        result = int(parsed_msg[5]) * correct_guesses


        ## SENDING REPLY ################################X
        msg_out = str(result).encode()
        sock.sendto(msg_out, client_addr)
        print("Sent reply: ", msg_out)

        ## SENDING TO HISTORY SERVER ################################X
        history_msg = list( map(str, parsed_msg[0:5]) )
        history_msg.append( str(result) )
        history_msg.append( str(client_addr))
        history_msg = ":".join(history_msg)
        history_sock.sendall(history_msg.encode())
        
    except KeyboardInterrupt:
        break
    except socket.timeout:
        continue

print("Server closing...")
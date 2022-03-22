# FELADAT ###############
# Készíts egy lottóalkalmazást, ahol a szerver generál 5 számot 1 és 20 között (csak hogy legyen esély
# nyerni). Ezek lesznek a lottószámok. A kliensek beküldik a tippjeiket, illetve hogy mennyi pénzt
# raknak fel a szelvényre. A server megmondja, hogy mik voltak a nyerőszámok, es hogy mennyit nyert
# az adott játékos. 1 találat 1x, 2 találat 2x, 3 találat 3x, stb.

# 1/A: Old meg a feladatot TCP felett
# 2/B: Az üzenet formátuma struct legyen, es legálabb 2 mező legyen benne
# 3:   A szerverre egyszerre tudjon több kliens is csatlakozni! (TCP-nél selecttel)
# 4/D: A kliens a tippeket parancssori argumentumkent kapja
# 5/b: history külön szerverként, szerver hívja 

import sys
import socket
import random
import struct
import select

## CREATING SERVER SOCKET ###############################X
server_addr = sys.argv[1]
server_port = int( sys.argv[2] )

sock = socket.socket(socket.AF_INET, socket.SOCK_DGRAM)
sock.bind( (server_addr, server_port) )
sock.setblocking(0) # making keyboardinterrupt possible
sock.settimeout(1)

## PACKERS ######################################X

packer_in = struct.Struct('5i i') #[5 guesses, bet]
packer_out = struct.Struct('i') #how much we won

## GENERATING DATA ######################################X

numbers : int = []
for i in range(0, 5):
    numbers.append( random.randint(1, 20) )

print("Generated numbers: ", numbers)

## SERVING CLIENTS ######################################X

print("Waiting for clients...")
while True:
    try:
        msg, client_addr = sock.recvfrom(packer_in.size)
        if not msg: # client closed connection
            print("Egy kliens lezárta a kapcsolatot")
            continue

        ## HANDLING MSG ################################X
        parsed_msg = packer_in.unpack(msg) # [5 guesses, bet]
        print("Client ", client_addr, " sent msg: ", parsed_msg)


        ## CALCULATING RESULT ################################X
        correct_guesses = 0

        #could also check if there arent any duplicates in guesses
        for i in range(0, 5): #number index
            for j in range(0, 5): #guess index
                if numbers[i] == parsed_msg[j]:
                    correct_guesses += 1
                
        result = parsed_msg[5] * correct_guesses

        ## SENDING REPLY ################################X

        msg_out = packer_out.pack(result)
        sock.sendto( msg_out, client_addr)
        print("Sent reply: ", result)

    except KeyboardInterrupt:
        break
    except socket.timeout:
        continue

print("Server closing...")
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

from array import array
import sys
import socket
import struct
import select
import json

## SETTING UP SOCKETS ######################################X

proxy_addr = "localhost" # "szerverként" mi a címünk
proxy_port = 10001

# szerverként socket (kliens TCP-s kapcsolata)
sock = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
sock.bind( (proxy_addr, proxy_port) )

sock.listen(5)

BUFFER_SIZE : int = 1024

## WRITING TO JSON ######################################X
filename = "history.json" 

def writeGuessesToJson(guesses : array, reward : int, client_name: str):
    print("writing history to ", filename)
    try:
        file_data = []
        #TODO: handling file doesnt exist
        with open(filename, "r") as file:
            file_data = json.load(file)

        data_to_write = {
            client_name  : {
                "guesses" : guesses,
                "amount_won" : reward
            }
        }

        file_data["client_sessions"].append(data_to_write)
        print(file_data)

        with open(filename, "w") as file:
            json.dump(file_data, file, indent = 4)

    except Exception as e:
        print("Exception occured while writing to file.")
        print (e)


## SERVING CLIENTS ######################################X

print("Waiting for server...")

#Note: i know its overengineering to handle multiple clients, but i already wrote this so its less work
inputs = [ sock ]
while True:
    try:
        readables, _, _ = select.select( inputs, [], [], 1 )
        
        for s in readables:
            if s is sock: # new client connecting
                connection, client_info = sock.accept()
                print("Csatlakozott egy szerver: %s:%d" % client_info )
                inputs.append(connection)

            else: #already established connection
                msg = s.recv(BUFFER_SIZE)
                if not msg: # client closed connection
                    s.close()
                    print("A kliens lezárta a kapcsolatot")
                    inputs.remove(s)
                    continue

                ## HANDLING MSG ################################X
                parsed_msg = msg.decode().split(":") # [5 guesses, reward, client_name]
                print("A client sent msg: ", parsed_msg)

                writeGuessesToJson(parsed_msg[0:5], parsed_msg[5], parsed_msg[6])

    except KeyboardInterrupt: #ctrl-c event
        for s in inputs:
            s.close()
        print("Server closing...")
        break
                
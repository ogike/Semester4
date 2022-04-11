#1) Készíts egy szerver-kliens alkalmazást, amely TCP protokollt használ. 
# A szerver egy “Licit” szerver, amely egyetlen műkincs árát tartja nyilván és fogadja a licitálók ajánlatait. 
# (Van egy kikiáltási ár megadva.) 
# A kliens egyfajta üzenetet küld: (BID,ár), amivel új ajánlatot tesz. 
# A szerver (OK,új ár) formátumban válaszol ha miénk volt a legnagyobb ajánlat eddig, 
#   illetve (LOW,régi ár) üzenettel, ha alacsony az ajánlatunk. 
# Az egyszerűség kedvéért a licit álljon le, ha eléri az ár az 1 milliót. 
# Az üzenetek formátuma mindkét irányba ‘struct’ vagy ‘json’ (az egyiket kell kiválasztani). 
# (Amíg egy kliens van, addig gyakorlatilag maga ellen licitál, amíg el nem éri az 1 milliót.)

# KIVÁLASZTOTT ÜZENET FORMÁTUMOM: struct

#2) Az elküldendő számok parancssori argumentumból töltődnek be. 
#   Pl. python client.py 10000 20000 30000 stb.

#3) A szerver legyen képes kiszolgálni egyszerre több klienst is. Ezt a select függvény segítségével oldjad meg!


import sys
import socket
import random
import struct
import select
import time

# SOCKETS ####
server_addr = "localhost"
server_port = 10000

sock = socket.socket(socket.AF_INET, socket.SOCK_STREAM)

sock.connect((server_addr, server_port))

# PACKERS #####
packer_out = struct.Struct('3s i') #(BID, ár)
packer_in = struct.Struct('3s i')  #(OK, új ár) / (LOW, régi ár)

# PREPARING DATA #####
#TODO: sys argv
# bids = [10000, 20000, 30000, 1000000, 1100000 ]
bids = []
for i in range( 1, len(sys.argv) ):
    bids.append( int( sys.argv[i] ) )

# SENDING DATA #####
for bid in bids:
    
    msg_out = packer_out.pack(b'BID', bid)
    
    print("Küld licitet: ", bid)
    sock.sendall(msg_out)

    # RECEIVING REPLY ####
    try:
        msg_in = sock.recv(packer_in.size)
    except:
        print("A szerver lezárta a kapcsolatot.")
        break

    msg_in_parsed = packer_in.unpack(msg_in)
    print("Szervertől válasz: ", msg_in_parsed)

    time.sleep(1) #TODO: random
    

# CLOSING DOWN #####
sock.close()
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

#4) Készíts egy PriceLog alkalmazást.
# A szerver mindig értesítse a log szervert, a legfrissebb árról változás esetén, 
#   aki ezt kiírja a standard outputjára. 
# A Licit szerver a PriceLog szerverrel UDP protokollon keresztül kommunikáljon, 
#   a csomagvesztésekkel ne törődjön.

import sys
import socket
import random
import struct

# CONNECTIONS

UDP_ADDR = "localhost"
UDP_PORT = 10001

udp_sock = socket.socket(socket.AF_INET, socket.SOCK_DGRAM)

udp_sock.bind( (UDP_ADDR, UDP_PORT) )
udp_sock.setblocking(0) # making keyboardinterrupt possible
udp_sock.settimeout(1)


## PACKERS #####
packer_in = struct.Struct('3s i') #(BID, ár)

print("Waiting for clients...")
while True:
    try:
        msg, client_addr = udp_sock.recvfrom(packer_in.size)
        if not msg: # client closed connection
            print("Egy kliens -", client_addr ," - lezárta a kapcsolatot")
            continue

        ## HANDLING MSG ################################X
        msg_in_unpacked = packer_in.unpack(msg)
        print("Új licit: ", msg_in_unpacked[1])

    except KeyboardInterrupt:
        break
    except socket.timeout:
        continue


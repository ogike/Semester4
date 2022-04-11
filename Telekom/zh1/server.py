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

# HASZNÁLAT: parancssori argumentumként meg kell adni, hogy mi a kezdő licit!

from asyncio.windows_events import NULL
import sys
import socket
import struct
import select
import time

## SOCKETS ################
server_addr = "localhost"
server_port = 10000
UDP_ADDR = "localhost"
UDP_PORT = 10001

server_sock = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
udp_sock = socket.socket(socket.AF_INET, socket.SOCK_DGRAM)

server_sock.bind( (server_addr, server_port) )
server_sock.listen(5)

udp_sock.connect( (UDP_ADDR, UDP_PORT))


## PACKERS #####
packer_in = struct.Struct('3s i') #(BID, ár)
packer_out = struct.Struct('3s i')  #(OK, új ár) / (LOW, régi ár)

## PREPARING DATA
cur_price = int( sys.argv[1] )
max_price = 1000000

## SERVING CLIENT
print("Waiting for clients...")
inputs = [ server_sock ]
running = True
while running:
    try:
        readables, _, _ = select.select( inputs, [], [], 1 )
        
        for s in readables:
            if s is server_sock: # new client connecting
                connection, client_info = server_sock.accept()
                print("Csatlakozott valaki: %s:%d" % client_info )
                inputs.append(connection)

            else: #already established connection
                msg_in = s.recv(packer_in.size)
                if not msg_in: # client closed connection
                    s.close()
                    print("A kliens lezárta a kapcsolatot")
                    inputs.remove(s)
                    continue

                ## HANDLING MSG ################################X
                msg_in_unpacked = packer_in.unpack(msg_in)
                print("Client said: ", msg_in_unpacked)
                new_bid = int( msg_in_unpacked[1] )

                if(new_bid > cur_price):
                    cur_price = new_bid #új ár
                    msg_out = b'OK '

                    #log szervernek elküldés
                    udp_sock.sendall(msg_in)
                else:
                    msg_out = b'LOW'
                    #ilyenkor a "régi ár" marad a cur_price változatlanul

                ## SENDING REPLY ################################X
                msg_out_packed = packer_out.pack(msg_out, cur_price)  
                s.sendall(msg_out_packed)

                if(cur_price >= max_price):
                    # the server just closes down if the licit is over
                    for s in inputs:
                        s.close()
                    print("Licit is over, server closing...")
                    running = False


    except KeyboardInterrupt: #ctrl-c event
        for s in inputs:
            s.close()
        print("Server closing...")
        running = False

## CLOSING DOWN
server_sock.close()
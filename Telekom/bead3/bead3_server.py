from http import client
from multiprocessing import connection
import socket
from sqlite3 import connect
import struct
import sys
import select

#TODO: get from argv
server_addr = 'localhost'
server_port = 10000
NUM_OF_CLIENTS = 10

sock = socket.socket(socket.AF_INET, socket.SOCK_STREAM)

sock.bind( (server_addr, server_port) )
sock.listen(NUM_OF_CLIENTS)

#same for in/out packages, number isnt used when server sends it
packer = struct.Struct('c i')

inputs = [ sock ]

while True:
    try:
        readables, _, _ = select.select( inputs, [], [], 1 )

        for s in readables:
            if s is sock: #if unregistered connection
                connection, client_addr = sock.accept()
                print("Csatlakozott valaki: %s:%d" % client_addr)
                inputs.append(connection)
            
            else: #already registered connection
                msg = s.recv(packer.size)

                #lezáró üzenet
                if not msg:
                    s.close()
                    print("Egyik kliens lezárta a kapcsolatot")
                    inputs.remove(s)
                    continue

                #rendes üzenet feldolgozása
                msg_unpacked = packer.unpack(msg)
                print( "A kliens üzenete: %c %d" % 
                        (msg_unpacked[0].decode(), msg_unpacked[1]) )
                
                #TODO: feldolgozás
                reply = (b'I', 0)
                reply_packed = packer.pack(reply)
                s.sendall(reply_packed)
                print("Elküldött válasz: ", reply)
                


    except KeyboardInterrupt:
        print("Server closing")
        break

for s in inputs: 
    sock.close()

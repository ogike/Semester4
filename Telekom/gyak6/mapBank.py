import sys
import socket
import struct
import select
import random

server_addr = "localhost"
server_port = 10001

sock = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
sock.setsockopt(socket.SOL_SOCKET, socket.SO_REUSEADDR, 1)
sock.bind( (server_addr, server_port) )

sock.listen(5)

packer_in = struct.Struct('4i 6s')
packer_out = struct.Struct('i')

bank = {}

inputs = [ sock ]
print("Waiting for clients..")
while True:
    try:
        readables, _, _ = select.select( inputs, [], [], 1 )
        
        for s in readables:
            if s is sock: #éppen iterált socket egyenlő a szerver sockettel
                connection, client_info = sock.accept()
                print("Csatlakozott valaki: %s:%d" % client_info )
                inputs.append(connection)

            else: #nem szerver socket, már kiépült kapcsolattal beszélünk
                msg = s.recv(packer_in.size)
                if not msg:
                    s.close()
                    print("A kliens lezárta a kapcsolatot")
                    inputs.remove(s)
                    continue
                
                #innentől kezdve ugyan az  mint eddig
                    #csak socket-ként hivatkazunk rá, nem kapcsolatként
                parsed_msg = packer_in.unpack(msg)

                key = parsed_msg[:5]
                if(key in bank):
                    msg = bank[key]
                else:
                    bank[key] = random.randrange(1000)
                    msg = bank[key]

                msg_packed = packer_out.pack(msg)
                s.sendall(msg_packed)
                print("Elküldött válasz: ", msg)
			
    except KeyboardInterrupt: #ctrl-c a konzolon
        for s in inputs:
            s.close()
        print("Server closing")
        break

import sys
import socket
import struct
import select

server_addr = sys.argv[1]
server_port = int(sys.argv[2])

sock = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
sock.setsockopt(socket.SOL_SOCKET, socket.SO_REUSEADDR, 1)
sock.bind( (server_addr, server_port) )

sock.listen(5)

ops = { '+': lambda x,y: x+y, 
	'-': lambda x,y: x-y,
	'*': lambda x,y: x*y,
	'/': lambda x,y: int(x/y)}

packer_in = struct.Struct('i i c')
packer_out = struct.Struct('i')

inputs = [ sock ]

while True:
    try:
        # NOTE: a lényeg
        #  "amikor egy socket olvasható állapotba kerül, kiolvasom, egyből edlöntöm mit válaszoljak és válaszolok"
        #   félévben végig így használjuk, eldobhatjuk a nem olvasható socketeket
        #   utolsó paraméter: timeout (in seconds)
        readables, _, _ = select.select( inputs, [], [], 1 )
        
        for s in readables:
            if s is sock: #éppen iterált socket egyenlő a szerver sockettel
                #mert ha új kliens jön be, az a szerver socketen jön be, fogadni kell
                connection, client_info = sock.accept()
                print("Csatlakozott valaki: %s:%d" % client_info )
                inputs.append(connection)

            else: #nem szerver socket, már kiépült kapcsolattal beszélünk
                msg = s.recv(packer_in.size)
                if not msg:
                    s.close() #fontos hogy csak azt a socketet zárom le
                    print("A kliens lezárta a kapcsolatot")
                    inputs.remove(s)
                    continue
                
                #innentől kezdve ugyan az  mint eddig
                    #csak socket-ként hivatkazunk rá, nem kapcsolatként
                parsed_msg = packer_in.unpack(msg)
                print("A kliens üzenete: %d %c %d" % (parsed_msg[0], parsed_msg[2].decode(), parsed_msg[1]))
                result = ops[parsed_msg[2].decode()](parsed_msg[0], parsed_msg[1])
                msg = packer_out.pack(result)
                s.sendall(msg)
                print("Elküldött válasz: %d" % result)
			
    except KeyboardInterrupt: #ctrl-c a konzolon
        for s in inputs:
            s.close()
        print("Server closing")
        break

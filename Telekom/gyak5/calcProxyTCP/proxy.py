import sys
import socket
import struct
import select

# "szerverként" mi a címünk
proxy_addr = sys.argv[1]
proxy_port = int(sys.argv[2])
server_addr = sys.argv[3] # igazi szerver címe
server_port = int(sys.argv[4])

# szerverként socket
sock = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
sock.bind( (proxy_addr, proxy_port) )

sock.listen(5)

#kliensként socket
proxySock = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
proxySock.connect( (server_addr, server_port))

packer = struct.Struct('i i c')

inputs = [ sock ]

try:
    while True:
        readables, _, _ = select.select( inputs, [], [] )

        for s in readables:
            if s is sock:
                connection, client_info = sock.accept()
                print("Csatlakozott valaki: %s:%d" % client_info )
                inputs.append(connection)
            else: # a lényeges rész
                msg = s.recv(packer.size)
                if not msg:
                    s.close()
                    print("A kliens lezárta a kapcsolatot")
                    inputs.remove(s)
                    continue
                parsed_msg = packer.unpack(msg)
                print("A kliens üzenete: %d %c %d" % (parsed_msg[0], parsed_msg[2].decode(), parsed_msg[1]))

                print("Módosítva: %d %c %d" % (parsed_msg[0]*2, parsed_msg[2].decode(), parsed_msg[1]+1))
                msg = packer.pack(parsed_msg[0]*2, parsed_msg[1]+1, parsed_msg[2])
                proxySock.sendall( msg )
                result = proxySock.recv(packer.size) #egyből továbbküldjük kliensnek, nem is csomagoljuk kitea
                s.sendall(result)
			
finally:
	sock.close()

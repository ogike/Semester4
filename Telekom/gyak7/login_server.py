import sys
import socket
import struct
import select

BUFFER_SIZE = 1024 #TODO: calc

server_addr = 'localhost' #sys.argv[1]
server_port = 10000 #int(sys.argv[2])

sock = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
sock.setsockopt(socket.SOL_SOCKET, socket.SO_REUSEADDR, 1)
sock.bind( (server_addr, server_port) )

sock.listen(5)

inputs = [ sock ]

while True:
    try:
        readables, _, _ = select.select( inputs, [], [], 1 )
        
        for s in readables:
            if s is sock: #új kliens
                connection, client_info = sock.accept()
                print("Csatlakozott valaki: %s:%d" % client_info )
                inputs.append(connection)

            else: #nem szerver socket, már kiépült kapcsolattal beszélünk
                msg = s.recv(BUFFER_SIZE)
                if not msg:
                    s.close()
                    print("A kliens lezárta a kapcsolatot")
                    inputs.remove(s)
                    continue
                
                ## HANDLING MSG ################################X
                parsed_msg = msg.decode().split('|')
                print("A kliens üzenete: ",  (parsed_msg[0], parsed_msg[1]))

                ## CALCULATING RESULT ################################X
                #TODO:

                ## SENDING REPLY ################################X

                s.sendall(msg)
                print("Elküldött válasz: %d" % msg)

                ## SENDING TO HISTORY SERVER ################################X
                #TODO: 
			
    except KeyboardInterrupt: #ctrl-c a konzolon
        for s in inputs:
            s.close()
        print("Server closing")
        break

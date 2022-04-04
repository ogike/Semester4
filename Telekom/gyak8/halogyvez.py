import sys
import socket
import struct
import select

BUFFER_SIZE = 1024 #TODO: calc

server_addr = 'localhost' #sys.argv[1]
server_port = 10000 #int(sys.argv[2])
required_number = int(sys.argv[1])


tavalyi_addr = "localhost"
tavalyi_port = 10001
client_sock = socket.socket(socket.AF_INET, socket.SOCK_DGRAM)


sock = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
sock.setsockopt(socket.SOL_SOCKET, socket.SO_REUSEADDR, 1)
sock.bind( (server_addr, server_port) )

sock.listen(5)

inputs = [ sock ]

required_counter = 0

print("Waiting for clients...")
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
                parsed_msg = msg.decode()
                print("A kliens üzenete: ",  parsed_msg)

                if(parsed_msg == "Kerek egy feladatot"):
                    required_counter += 1
                
                reply = ""
                if(parsed_msg == "Koszonjuk"):
                    reply = "Szivesen"
                elif(required_counter >= required_number):
                    reply = "Tessek a feladat"
                    client_sock.sendto(b"Keres", (tavalyi_addr, tavalyi_port))
                    zh, _ = client_sock.recvfrom(BUFFER_SIZE)
                    print("Recieved tavalyi zh: ", zh)
                    reply = zh.decode()

                else:
                    reply = "Meg nincs"

                msg_out = reply.encode()

                ## SENDING REPLY ################################X

                s.sendall(msg_out)
                print("Elküldött válasz: ", msg_out)

                ## SENDING TO HISTORY SERVER ################################X
                #TODO: 
			
    except KeyboardInterrupt: #ctrl-c a konzolon
        for s in inputs:
            s.close()
        print("Server closing")
        break

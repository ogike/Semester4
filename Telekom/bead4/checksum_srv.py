import sys
import socket
import struct
import select
import random

#TODO: better naming:
CHSUM_MSG_SIZE : int = 128

server_addr = "localhost" #TODO: sysargv
server_port = 10001

#### SETTING UP CONNECTIONS #############################XXXX
sock = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
sock.setsockopt(socket.SOL_SOCKET, socket.SO_REUSEADDR, 1)
sock.bind( (server_addr, server_port) )
sock.listen(5)


checksums = {}

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
                msg = s.recv(CHSUM_MSG_SIZE)
                if not msg:
                    s.close()
                    print("A kliens lezárta a kapcsolatot")
                    inputs.remove(s)
                    continue

                parsed_msg = msg.decode().split('|')
                reply = '' 
                if(len( parsed_msg ) != 5):
                    print("Parsed msg has wrong format!")
                    reply = 'ERR'
                
                else: # if valid msg format
                    action = parsed_msg[0]
                    file_id = parsed_msg[1]

                    if(action == 'BE'):
                        valid_time = parsed_msg[2]
                        checksums[file_id] = {
                            'size' : parsed_msg[3],
                            'checksum': parsed_msg[4]
                        }
                        reply = 'OK'
                        #TODO: validity coroutine
                    elif(action == 'KI'):
                        if(file_id in checksums):
                            size = checksums[file_id]['size']
                            checksum = checksums[file_id]['checksum']
                            reply = size + '|' + checksum 
                        else:
                            reply = "0|"
                    else:
                        print("Parsed msg has wrong format!")
                        reply = 'ERR'
                    

                s.sendall(reply.encode())
                print("Elküldött válasz: ", reply)
			
    except KeyboardInterrupt: #ctrl-c a konzolon
        for s in inputs:
            s.close()
        print("Server closing")
        break

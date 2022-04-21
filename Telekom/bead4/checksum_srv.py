import sys
import socket
import struct
import select
import random
import asyncio
import time

#TODO: better naming:
CHSUM_MSG_SIZE : int = 128

server_addr = sys.argv[1]#"localhost" #TODO: sysargv
server_port = int( sys.argv[2]) #10001
checksums = {}
last_time = int(time.time())

# wait function
async def handleValidity(validTime: int, file_id: str):
    await asyncio.sleep(valid_time)
    del checksums[file_id]
    print("deleted csum: " + file_id)

def updateTimes():
    cur_time = int( time.time() )
    delta_time = last_time - cur_time
    for file_id in checksums:
        checksums[file_id]['time'] -= delta_time


#### SETTING UP CONNECTIONS #############################XXXX
sock = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
sock.setsockopt(socket.SOL_SOCKET, socket.SO_REUSEADDR, 1)
sock.bind( (server_addr, server_port) )
sock.listen(5)


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
                if(len( parsed_msg ) < 2):
                    print("Parsed msg has wrong format!")
                    print(parsed_msg)
                    reply = 'ERR'
                
                else: # if valid msg format
                    action = parsed_msg[0]
                    file_id = parsed_msg[1]

                    if(action == 'BE'):
                        valid_time = int( parsed_msg[2] )
                        checksums[file_id] = {
                            'size' : parsed_msg[3],
                            'checksum': parsed_msg[4],
                            'time' : valid_time
                        }
                        reply = 'OK'
                    elif(action == 'KI'):
                        updateTimes()

                        if(file_id in checksums):
                            if(checksums[file_id]['time'] > 0):
                                size = checksums[file_id]['size']
                                checksum = checksums[file_id]['checksum']
                                reply = size + '|' + checksum 
                            else:
                                reply = "0|"
                        else:
                            reply = "0|"
                    else:
                        print("Parsed msg has wrong action!")
                        reply = 'ERR'
                    

                s.sendall(reply.encode())
                print("Elküldött válasz: ", reply)
			
    except KeyboardInterrupt: #ctrl-c a konzolon
        for s in inputs:
            s.close()
        print("Server closing")
        break

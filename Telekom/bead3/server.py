import random
import socket
import struct
import sys
import select
from typing import Tuple
#TODO: clean imports

def evaluate_guess(dir : str, guess : int, num : int) -> str:
    if(dir == '='):
        if(guess == num):
            return 'Y'
        else:
            return 'K'

    elif(dir == '<'):
        if(guess < num):
            return 'I'
        else:
            return 'N'

    elif(dir == '>'):
        if(guess > num):
            return 'I'
        else:
            return 'N'

    else: #wrong input
        return 'K'

server_addr = sys.argv[1]
server_port = int( sys.argv[2] )
NUM_OF_CLIENTS = 10
    

sock = socket.socket(socket.AF_INET, socket.SOCK_STREAM)

sock.bind( (server_addr, server_port) )
sock.listen(NUM_OF_CLIENTS)

#same for in/out packages, number isnt used when server sends it
packer = struct.Struct('c i')

rand_num = random.randint(1, 100)
if( len(sys.argv) > 3 ):
    rand_num = int( sys.argv[3] )
print("Random number is: ", rand_num)

inputs = [ sock ]

playing = True
active = True
while active:
    try:
        readables, _, _ = select.select( inputs, [], [], 1 )

        if(playing == False and len(inputs) <= 1):
            # print("Evereyone disconnected, new game starting!")
            # rand_num = random.randint(1, 100)
            # print("Random number is: ", rand_num)
            # playing = True
            print("Evereyone disconnected, closing server!")
            active = False
            break
            

        for s in readables:
            if s is sock: #if unregistered connection
                connection, client_addr = sock.accept()
                print("Csatlakozott valaki: %s:%d" % client_addr)
                inputs.append(connection)
            
            else: #already playing connection
                msg = s.recv(packer.size)

                #lezáró üzenet
                if not msg:
                    s.close()
                    print("Egyik kliens lezarta a kapcsolatot")
                    inputs.remove(s)
                    continue

                #rendes üzenet feldolgozása
                msg_unpacked = packer.unpack(msg)
                msg_dir = msg_unpacked[0].decode()
                msg_guess = msg_unpacked[1]
                print( "A kliens uzenete: %c %d" % (msg_dir, msg_guess) )
                print("readable: ", s)
                
                result : str
                if (playing):
                    result = evaluate_guess(msg_dir, msg_guess, rand_num)
                else:
                    result = 'V'
                
                reply = (result.encode(), 0)
                reply_packed = packer.pack(*reply)
                s.sendall(reply_packed)
                print("Elkuldott valasz: ", reply)
                
                #Game over for client (won, lost, someone else won)
                if(result == 'Y' or result == 'K' or result == 'V'): 
                    playing = False
                    s.close() #TODO: ezt elég csak kliens oldalon?
                    print("Game over for client, disconnecting them")
                    inputs.remove(s)
                    continue

    except KeyboardInterrupt:
        print("Server closing")
        active = False
        break

for s in inputs: 
    sock.close()
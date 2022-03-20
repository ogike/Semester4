import socket
import struct
import sys
import random
from time import sleep

server_addr = sys.argv[1]
server_port = int( sys.argv[2] )


sock = socket.socket(socket.AF_INET, socket.SOCK_STREAM)

sock.connect((server_addr, server_port))

#same for in/out packages, number isnt used when server sends it
packer = struct.Struct('c i')

#'<' = is the guess smaller?
#'>' = is the guess bigger?
possible_dirs = ['<', '>']
upper = 100
lower = 1

playing = True
while playing:
    if(lower < upper):
        dir = random.choice(possible_dirs)
        guess = int( (upper+lower) / 2 ) #the middle
        print("Lower is ", lower, ", upper is ", upper, ', guess: ', guess, ', dir: ', dir)
    else: #if we know the number OR they have intersected
        dir = '='
        guess = lower
        print("Lower and upper is ", lower, ", going for it!")

    msg_out = packer.pack(dir.encode(), guess)

    sock.sendall(msg_out)

    msg_in = sock.recv(packer.size)
    msg_in_unpacked = packer.unpack(msg_in)[0].decode()
    print("Kapott valasz", msg_in_unpacked)

    if(msg_in_unpacked == 'K'):
        print("Kiestel!")
        playing = False
        break
    elif(msg_in_unpacked == 'Y'):
        print("Nyertel!")
        playing = False
        break
    elif(msg_in_unpacked == 'I'):
        if(dir == '>'):
            lower = guess + 1
        else:
            upper = guess - 1
    elif(msg_in_unpacked == 'N'):
        if(dir == '>'):
            upper = guess
        else:
            lower = guess
    else: #case 'V' and wrong input
        print("Vege a jateknak!")
        playing = False
        break

    sleep(random.random() * 4 + 1)


sock.close()

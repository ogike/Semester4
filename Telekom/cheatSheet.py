#### IMPORTS ############################################XXXX
import sys
import socket
import random
import struct
import select
import json
import hashlib


#### SETTING UP CONNECTIONS #############################XXXX
#region connections
TCP_ADDR = "localhost"
TCP_PORT = 10000
UDP_ADDR = "localhost"
UDP_PORT = 10001

## SETTING UP SOCKETS #####################################
tcp_sock = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
udp_sock = socket.socket(socket.AF_INET, socket.SOCK_DGRAM)

## CONNECTING #########################################
tcp_sock.connect((TCP_ADDR, TCP_PORT))
udp_sock.connect( (UDP_ADDR, UDP_ADDR))

## TCP HOSTING ###########################################
tcp_sock.bind( (TCP_ADDR, TCP_PORT) )
tcp_sock.listen(5)

## UDP HOSTING ############################################
udp_sock.bind( (TCP_ADDR, TCP_PORT) )
udp_sock.setblocking(0) # making keyboardinterrupt possible
udp_sock.settimeout(1)
#endregion


#### PACKERS ############################################XXXX
#region packers

#i: integer, c: character
packer_in = struct.Struct('5i i')
packer_out = struct.Struct('i')

# RECEIVING
msg = tcp_sock.recv(packer_in.size)
parsed_msg = packer_in.unpack(packer_in)
# SENDING
msg_out = [1,2,3,4,5]
msg_out_packed = packer_out.pack(*msg_out, 60) #this can be a *tuple, felsorolas, stb

## PACKING VARIABLE LENGTH STRING (2-steps) ##############
#region varlen

# SENDING ##################################
msgStr = "my message"
msgSize = len("my message")
msgSizePacker = struct.Struct('i')
msgSizePacked = msgSizePacker.pack(msgSize)
tcp_sock.send( msgSizePacked )
#check if received..?
msgStrPacker = struct.Struct('%ds' % int(msgSize))
msgStrPacked = msgStrPacker.pack(msgStr.encode())
tcp_sock.send( msgStrPacked )

# RECEIVING ###############################
sizeFormat = 'i' #TODO: do i need?
sizeStruct = struct.Struct(sizeFormat)
recSize = tcp_sock.recv( struct.calcsize(sizeFormat) )
msgFormat = '%ds' % int(recSize[0])
msg = tcp_sock.recv( struct.calcsize(msgFormat) )
unpackedMsg = struct.Struct(msgFormat).unpack(msg)
#endregion varlen

#endregion packers


#### BYTESTRING MANIPULATION ############################XXXX
#region bytestrings

## SENDING BYTESTRING ##########################
#the input
numbers = [0,1,2,3,4]
neptun = "ABCDEF"

#turning it into a list of strings
msg_out = list(map(str, numbers))
msg_out.append(neptun)

#joining with separator, turning into bytestring
msg_out = "|".join(msg_out)
msg_out_encoded = msg_out.encode()

## RECIEVING BYTESTRING ########################
parsed_msg = msg.decode().split(":")
print("first: ", parsed_msg[0])
part1, part2 = msg.decode().split(":") #if simple tuple

#endregion bytestrings

## TCP SERVING CLIENTS ##################################XXXX
#region tcp serving
print("Waiting for clients...")
inputs = [ tcp_sock ]
while True:
    try:
        readables, _, _ = select.select( inputs, [], [], 1 )
        
        for s in readables:
            if s is tcp_sock: # new client connecting
                connection, client_info = tcp_sock.accept()
                print("Csatlakozott valaki: %s:%d" % client_info )
                inputs.append(connection)

            else: #already established connection
                msg = s.recv(packer_in.size)
                if not msg: # client closed connection
                    s.close()
                    print("A kliens lezárta a kapcsolatot")
                    inputs.remove(s)
                    continue

                ## HANDLING MSG ################################X
                msg #TODO:

                ## SENDING REPLY ################################X
                msg_out #TODO: pack it
                s.sendall(msg_out)

    except KeyboardInterrupt: #ctrl-c event
        for s in inputs:
            s.close()
        print("Server closing...")
        break
#endregion


## TCP SERVING CLIENTS ##################################XXXX
#region udp serving

print("Waiting for clients...")
while True:
    try:
        msg, client_addr = udp_sock.recvfrom(packer_in.size)
        if not msg: # client closed connection
            print("Egy kliens -", client_addr ," - lezárta a kapcsolatot")
            continue

        ## HANDLING MSG ################################X
        msg #TODO:

        ## SENDING REPLY ################################X
        msg_out #TODO: pack it
        udp_sock.sendto( msg_out, client_addr)

    except KeyboardInterrupt:
        break
    except socket.timeout:
        continue

#endregion


#### VARIOUS I/O ########################################XXXX
# region various i/o 

# reading form console
inp_num = int( input("Kérek egy számot: ") )

# reading from argv list
my_name = sys.argv[0] #0th is file name

# reading line-by-line
with open('alma.txt', 'r') as file:
    for line in file:
        print( line.strip() )

#reading all the lines
with open(sys.argv[1]) as f:
    lines_list = f.readlines()
    #still contains the newlines, strip them
# endregion various i/o


#### JSON FILE APPENDING ################################XXXX
#region json appending

filename = "history.json" 

try:
    file_data = []
    ## LOADING THE JSON
    #! doesnt handle if file doesnt exist 
    with open(filename, "r") as file:
        file_data = json.load(file) # loading the whole file
        #NOTE: probably need to convert these values
        #       useful: list(map(str, read_val))

    ## CREATING DATA TO ADD #TODO: do
    key = "name"
    data_to_write = {
        key  : {
            "field1" : 6,
            "field2" : "almafa"
        }
    }
    file_data["lista"].append(data_to_write)
    # print(file_data)

    ## APPENDING DATA TO FILE
    with open(filename, "w") as file:
        json.dump(file_data, file, indent = 4)

except Exception as e:
    print("Exception occured while writing to file.")
    print (e)

# endregion json appending


#### HASHING ###########################################XXXX
#region hashing

stored_hash = "hash value" #TODO: probably read from file
password = b"pwd"

# GENERATING HASH
password_hasher = hashlib.md5(password.encode())
password_hash = password_hasher.hexdigest()

result = "correct" if password_hash == stored_hash else "wrong"

#endregion hashing

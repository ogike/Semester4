import hashlib
import os
import re
import sys
import socket

BUFFER_SIZE : int = 1024
CHSUM_MSG_SIZE : int = 128

server_addr = sys.argv[1]# "localhost" #TODO: parancssori argv
server_port = int( sys.argv[2] )# 10005
checks_addr = sys.argv[3]# "localhost" #TODO:
checks_port = int( sys.argv[4] )# 10001

file_id : int = int( sys.argv[5] )
file_path : str = sys.argv[6]

## CONNECTING TO CHECKSUM ###################################
checksum_sock = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
checksum_sock.connect((checks_addr, checks_port))

## CONNECTING TO CLIENT ######################################
sock = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
sock.bind((server_addr, server_port))
sock.listen(1)

print("Waiting for a client...")
client, client_addr = sock.accept()


## READING THE FILE ##########################################
try:
    with open( file_path, 'wb' ) as file:
        writing : bool = True

        #writing the gile
        while writing:
            data = client.recv(BUFFER_SIZE)

            #TODO: file vége jel olvasása?

            if(data):
                file.write(data)
            else:
                client.close()
                writing = False
    print("File recieved!")

    #checking validity ###############
    # generating our recieved file's hash
    with open( file_path, 'rb' ) as file:
        toHash : bytes = file.read()
        hasher : object = hashlib.md5( toHash )
        hash : str = hasher.hexdigest()
    
    #asking for correct checksum
    queryMsg : str = "KI|" + str(file_id)
    checksum_sock.sendall( queryMsg.encode() )

    reply = checksum_sock.recv(CHSUM_MSG_SIZE)
    print("Checksum servers reply: ")
    print(reply)

    if(reply == False):
        print("CSUM CORRUPTED")
    
    if(reply):
        reply_parsed = reply.decode().split('|')
        if( len(reply_parsed) != 2 or reply_parsed[0] == '0' ):
            print("CSUM CORRUPTED")
        else:
            if(reply_parsed[1] == hash):
                print("CSUM OK")
            else:
                print("Hash not good, servers: " +  reply_parsed[1] + ", mine: " + hash)


except Exception as e:
    print("Exception occured while sending the file.")
    print (e)

sock.close()
checksum_sock.close()
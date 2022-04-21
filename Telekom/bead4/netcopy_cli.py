import os
import sys
import socket
import hashlib

CHSUM_MSG_SIZE : int = 128
BUFFER_SIZE : int = 1024
VALIDITY_TIME : int = 60

server_addr = sys.argv[1] #"localhost" #TODO: parancssori argv
server_port = int( sys.argv[2] ) #10005
checks_addr = sys.argv[3] #"localhost" #TODO:
checks_port = int( sys.argv[4] ) #10001

file_id : str = sys.argv[5] #actually a number
file_path : str = sys.argv[6]

## CONNECTING TO CHECKSUM ###################################
checksum_sock = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
checksum_sock.connect((checks_addr, checks_port))


## CONNECTING TO SERVER ######################################
sock = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
sock.connect((server_addr, server_port))

## READING THE FILE ##########################################
try:
    # 
    with open( file_path, 'rb' ) as file:
        buffer = file.read(BUFFER_SIZE)
        while buffer: #while there is still sth to read
            sock.sendall(buffer)
            buffer = file.read(BUFFER_SIZE)

        sock.close()
        print("File sent!")

        #sending checksum message
        file.seek(0, os.SEEK_SET)
        toHash : bytes = file.read()
        hasher : object = hashlib.md5( toHash )
        hash : str = hasher.hexdigest()
        hashLen : int = len( hash.encode() ) #checksum hossza bájtszámban
        toSendStr : str = "BE|" + file_id + '|' + str(VALIDITY_TIME) + '|' + str(hashLen) + '|' + hash
        checksum_sock.sendall( toSendStr.encode() )

    #waiting for checksum server reply
    checksum_reply = checksum_sock.recv(CHSUM_MSG_SIZE)
    print( "checksum server's reply: " + checksum_reply.decode() )


except Exception as e:
    print("Exception occured while sending the file.")
    print (e)
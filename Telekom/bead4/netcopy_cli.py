import sys
import socket

TCP_ADDR = "localhost" #TODO: parancssori argv
TCP_PORT = 10000

BUFFER_SIZE = 128

file_id : int = int( sys.argv[1] )
file_path : str = sys.argv[2]


## CONNECTING TO SERVER ######################################
sock = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
sock.connect((TCP_ADDR, TCP_PORT))

## READING THE FILE ##########################################
try:
    with open( file_path, 'rb' ) as file:
        part = file.read(BUFFER_SIZE)
        while part: #while there is still sth to read
            sock.sendall(part)
            part = file.read(BUFFER_SIZE)

    print("File sent!")
except Exception as e:
    print("Exception occured while sending the file.")
    print (e)

sock.close()
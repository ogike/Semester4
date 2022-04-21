import sys
import socket

from bead4.netcopy_cli import BUFFER_SIZE
from bead4.checksum_srv import CHSUM_MSG_SIZE

TCP_ADDR = "localhost" #TODO: parancssori argv
TCP_PORT = 10000

file_id : int = int( sys.argv[1] )
file_path : str = sys.argv[2]


## CONNECTING TO CLIENT ######################################
sock = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
sock.bind((TCP_ADDR, TCP_PORT))
sock.listen(1)

print("Waiting for a client...")
client, client_addr = sock.accept()


## READING THE FILE ##########################################
try:
    with open( file_path, 'wb' ) as file:
        writing : bool = True

        while writing:
            data = client.recv(BUFFER_SIZE)

            #TODO: file vége jel olvasása?

            if(data):
                file.write(data)
            else:
                client.close()
                writing = False

    print("File recieved!")
except Exception as e:
    print("Exception occured while sending the file.")
    print (e)

sock.close()
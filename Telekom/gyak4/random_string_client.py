import sys
import socket
import random
import struct
import time
import string

server_addr = sys.argv[1]
server_port = int(sys.argv[2])

sock = socket.socket(socket.AF_INET, socket.SOCK_STREAM)

sock.connect((server_addr, server_port))

for i in range(5):
    length = random.randint(1, 10)
    print("Választott hossz:",length)
    ran = ''.join(random.choices(string.printable, k = length))
    encoded_ran = ran.encode()
    signature = "i " + str(len(encoded_ran)) + "s"
    packer = struct.Struct(signature)
    packed = packer.pack(len(encoded_ran), encoded_ran) #kliens egybe küldi el a két üzenetet
    sock.sendall(packed)
    print("sent:",ran)
    time.sleep(2)
sock.close()

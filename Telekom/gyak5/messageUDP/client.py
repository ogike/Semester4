import sys
import socket
import random
import struct
import time

server_addr = sys.argv[1]
server_port = int(sys.argv[2])

sock = socket.socket(socket.AF_INET, socket.SOCK_DGRAM) #DGRAM: DATAGRAM
msg = b"Hello server"
print( "Üzenet:",msg.decode())
sock.sendto(msg,(server_addr,server_port)) #sendto(msg, tuple)
answer,_ = sock.recvfrom(1024) # szervertől várjuk az üzenetet (de ez lehet bárkitől is, eldobjuk hogy honnan jön)
print( "Kapott válasz:",answer.decode())
sock.close()



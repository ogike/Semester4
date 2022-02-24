import socket
import os

c = 0

for i in range (1000):
    try :
        print(i, ":", socket.getservbyport(i) )
    except OSError as error:
        # print(i, ": port not found") 
        c += 1

print(c, " ports not found")
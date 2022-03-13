import sys
import socket
import random
import struct
import time
import json

server_addr = sys.argv[1]
server_port = int(sys.argv[2])

sock = socket.socket(socket.AF_INET, socket.SOCK_STREAM)

sock.connect( (server_addr, server_port) )

packer_length = struct.Struct('i')
#print(struct.calcsize('i i c'))
#vagy
#print(packer.size)

name = input("Név:")
percentages = [0] * 3
valid = False
while not valid:
    try:
        percentages[0] = int(input("Python beadandó százalék:"))
        valid = True
    except ValueError:
        pass # ha az int(input())-nak nem int-et adunk, ValueError, utána továbblépünk a while ciklusban

valid = False
while not valid:
    try:
        percentages[1] = int(input("ZH százalék:"))
        valid = True
    except ValueError:
        pass

valid = False
while not valid:
    try:
        percentages[2] = int(input("Mininet százalék:"))
        valid = True
    except ValueError:
        pass

data = {'name' : name, 'percentages' : percentages}
msg_json = json.dumps(data) #jsonné alakítás

#egyben küldjük hossz + msg
msg = packer_length.pack(len(msg_json.encode())) + bytes(msg_json, encoding = 'utf-8')
print( "Üzenet:", msg_json )    
sock.sendall( msg )

msg = sock.recv(packer_length.size) # mennyit fogadjak
unpacked_values = packer_length.unpack(msg)
print(unpacked_values)
length = unpacked_values[0]
msg = sock.recv(length)
parsed_msg = json.loads(msg.decode("utf-8")) # szótárrá fordítom
print( "Kapott eredmény:", parsed_msg)
print("Összegzett százalék:%.2f, jegy:%d" % (parsed_msg['final_percentage'], parsed_msg['grade']))
sock.close()



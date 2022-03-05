# FELADAT #####################

# Készítsünk egy szerver-kliens alkalmazást,
#     ahol a kliens elküld 2 számot és egy operátort a szervernek,
#     amely kiszámolja és visszaküldi az eredményt.
# A kliens üzenete legyen struktúra.

###############################

from ast import match_case
import socket
import hashlib
import struct

def calculate(x : int, y : int, op : str) -> int | str: 
    match op:
        case '+':
            return x + y
        case '-':
            return x - y
        case '*':
            return x * y
        case '/':
            return (int)(x / y)
        case _:
            return 'Ismeretlen operandus!'


TCP_IP = 'localhost'
TCP_PORT = 10000
BUFFER_SIZE = 1024

sock = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
sock.bind( (TCP_IP, TCP_PORT) )
sock.listen(1)

print("Waiting for someone to connect...")
conn, addr = sock.accept()

while True: 
    data = conn.recv(BUFFER_SIZE) #itt ez 100, ki lehetne számolni packer.size-al
    if not data:
        print("Client disconnected, stopping...")
        break

    unpacker = struct.Struct('i i c')
    data_struct = unpacker.unpack(data)

    result = calculate(data_struct[0], data_struct[1], data_struct[2].decode())

    print("Calculated: ", result)

    conn.send(str(result).encode())


conn.close()
sock.close()
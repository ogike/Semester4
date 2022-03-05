import sys
import socket
import random
import struct
import time

server_addr = sys.argv[1]
server_port = int(sys.argv[2])

sock = socket.socket(socket.AF_INET, socket.SOCK_STREAM)

sock.connect((server_addr, server_port))

ops = ['+', '-', '*', '/']

packer_out = struct.Struct('i i c')
# print(struct.calcsize('i i c'))
# vagy
# print(packer_out.size)

packer_in = struct.Struct('i')

for nrnd in range(5):
    oper1 = random.randint(1, 100)
    oper2 = random.randint(1, 100)
    op = ops[nrnd % len(ops)]

    msg = packer_out.pack(oper1, oper2, op.encode())
    print("Üzenet: %d %c %d" % (oper1, op, oper2))
    sock.sendall(msg)
    try:
        msg = sock.recv(packer_in.size)
    except:
        print("A szerver lezárta a kapcsolatot")
        break
    parsed_msg = packer_in.unpack(msg)
    print("Kapott eredmény: %d" % parsed_msg[0])
    time.sleep(2)
sock.close()

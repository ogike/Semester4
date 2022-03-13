import sys
import socket
import struct
import select
import json

def choose_grade(percentage, zh_above_limit):
    if zh_above_limit:
        limits = [50,60,75,85]
        i = 0
        while i < len(limits) and percentage >= limits[i]:
            i += 1
        return i + 1
    else:
        return 1

server_addr = sys.argv[1]
server_port = int(sys.argv[2])

sock = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
sock.bind( (server_addr, server_port) )

sock.listen(5)
    
packer_length = struct.Struct('i')

inputs = [ sock ]
while True:
    try:
        readables, _, _ = select.select( inputs, [], [], 1 )
        
        for s in readables:
            if s is sock:
                connection, client_info = sock.accept()
                print("Csatlakozott valaki: %s:%d" % client_info )
                inputs.append(connection)
            else:
                #length fogadása
                msg = s.recv(packer_length.size)
                if not msg:
                    s.close()
                    print("A kliens lezárta a kapcsolatot")
                    inputs.remove(s)
                    continue
                length = packer_length.unpack(msg)[0]

                #msg fogadása
                msg = s.recv(length)
                parsed_msg = json.loads(msg.decode("utf-8"))
                print("A kliens üzenete:" , parsed_msg)

                #kiszámítjuk a választ
                final_percentage = sum(parsed_msg['percentages']) * (1/3)
                zh_above_limit = parsed_msg['percentages'][1] >= 50
                result = {'final_percentage' : final_percentage, "grade" : choose_grade(final_percentage, zh_above_limit)}

                #csomagoljuk és küldjük a választ
                result = bytes(json.dumps(result), encoding = "utf-8")
                msg = packer_length.pack(len(result)) + result
                s.sendall(msg)
                print("Elküldött válasz:", result)
    except KeyboardInterrupt:
        print("Server closing")
        for s in inputs:
            s.close()
        break

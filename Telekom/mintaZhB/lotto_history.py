import sys
import socket
import struct
import select
import json

## SETTING UP SOCKETS ######################################X

proxy_addr = sys.argv[1] # "szerverként" mi a címünk
proxy_port = int(sys.argv[2])
server_addr = sys.argv[3] # igazi szerver címe
server_port = int(sys.argv[4])

# szerverként socket (kliens TCP-s kapcsolata)
sock = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
sock.bind( (proxy_addr, proxy_port) )

sock.listen(5)

#kliensként socket (igazi szerver felé UDP-s kapcsolat)
proxySock = socket.socket(socket.AF_INET, socket.SOCK_DGRAM)
print("Trying to connect to server...")
proxySock.connect( (server_addr, server_port))
print("Connected to server")


## PACKERS ######################################X

packer_in = struct.Struct('5i i') #[5 guesses, bet]
packer_out = struct.Struct('i') #how much we won

## SERVING CLIENTS ######################################X

print("Waiting for clients...")

inputs = [ sock ]
while True:
    try:
        readables, _, _ = select.select( inputs, [], [], 1 )
        
        for s in readables:
            if s is sock: # new client connecting
                connection, client_info = sock.accept()
                print("Csatlakozott valaki: %s:%d" % client_info )
                inputs.append(connection)

            else: #already established connection
                msg = s.recv(packer_in.size)
                if not msg: # client closed connection
                    s.close()
                    print("A kliens lezárta a kapcsolatot")
                    inputs.remove(s)
                    continue

                ## HANDLING MSG ################################X
                parsed_msg = packer_in.unpack(msg) # [5 guesses, bet]
                print("A client sent msg: ", parsed_msg)


                ## FORWARDING MSG ################################X
                proxySock.sendall(msg)
                
                ## GETTING REPLY ################################X
                try:
                    msg_in = proxySock.recv(packer_in.size)
                    msg_in_parsed = packer_out.unpack(msg_in)

                    ## STORING CLIENT HISTORY ################################X
                    filename = "history.json" 
                    print("writing history to ", filename)
                    try:
                        file_data = []
                        #TODO: handling file doesnt exist
                        with open(filename, "r") as file:
                            file_data = json.load(file)

                        client_name = s.getpeername()[0] + str(s.getpeername()[1])
                        data_to_write = {
                            client_name  : {
                                "guesses" : parsed_msg[:-1],
                                "amount_won" : msg_in_parsed[0]
                            }
                        }

                        file_data["client_sessions"].append(data_to_write)
                        print(file_data)

                        with open(filename, "w") as file:
                            json.dump(file_data, file, indent = 4)

                    except Exception as e:
                        print("Exception occured while writing to file.")
                        print (e)
                    
                    ## SENDING BACVK REPLY ################################X

                    print("Sending this back to client: ", msg_in)
                    s.sendall(msg_in)
                except Exception as e:
                    print("Exception occured while receiving reply")
                    print(e)
                    continue

    except KeyboardInterrupt: #ctrl-c event
        for s in inputs:
            s.close()
        print("Server closing...")
        break
                
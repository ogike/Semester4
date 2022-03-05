from socket import socket,AF_INET, SOCK_STREAM
import select
import sys

# hasznalat: pl: python netProxy.py ggombos.web.elte.hu 80
# bongeszoben: localhost:10000
# bongeszoben: http://localhost:10000/oktatas/SzamHalo		# letiltott tartalom

webaddres = (sys.argv[1],int(sys.argv[2]))

proxy  = socket(AF_INET, SOCK_STREAM)

proxy_addr = ('',10000)

proxy.bind(proxy_addr)
proxy.listen(10)

inputs = [proxy]

while True:
    timeout = 1
    try:
        r, w, e = select.select(inputs,inputs,inputs,timeout)
        
        if not (r or w or e):
            continue
        
        for s in r:
            if s is proxy:
                print("Kliens csatlakozik")
                client, client_addr = s.accept()
                inputs.append(client)
            else:
                data = s.recv(65000)
                if not data:
                    inputs.remove(s)
                    s.close()
                else:
                    print("get-send")
                    if "SzamHalo" in data.decode():
                        header = "HTTP/1.1 404 Not Found\n\n".encode('utf-8')
                        
                        recv_data = '<html><body>404 Sok lesz a halozatokbol</body></html>'.encode()
                        
                        s.sendall(header+recv_data)
                        inputs.remove(s)
                        s.close()
                    else:
                        proxy_client = socket(AF_INET,SOCK_STREAM)
                        proxy_client.connect(webaddres)
                        data = data.decode().replace('localhost:10000', sys.argv[1]).encode()
                        
                        proxy_client.send(data)
                        
                        recv_data = proxy_client.recv(65000)
                        s.sendall(recv_data)
                        proxy_client.close()
                        inputs.remove(s)
                        s.close()
    except KeyboardInterrupt:
        for s in inputs:
            s.close()
        print("Proxy closing")
        break
				





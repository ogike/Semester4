import select
import socket
import sys


class SimpleTCPSelectServer:
    def __init__(self, addr='localhost', port=10001, timeout=1):
        self.server = self.setupServer(addr, port)
        # Sockets from which we expect to read
        self.inputs = [self.server]
        # Wait for at least one of the sockets to be ready for processing
        self.timeout = timeout

    def setupServer(self, addr, port):
        # Create a TCP/IP socket
        server = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
        server.setblocking(0)
        server.setsockopt(socket.SOL_SOCKET, socket.SO_REUSEADDR, 1)

        # Bind the socket to the port
        server_address = (addr, port)
        server.bind(server_address)

        # Listen for incoming connections
        server.listen(5)
        return server

    def handleNewConnection(self, sock):
        # A "readable" server socket is ready to accept a connection
        connection, client_address = sock.accept()
        connection.setblocking(0)  # or connection.settimeout(1.0)
        self.inputs.append(connection)

    # a fontos része
    def handleDataFromClient(self, sock):
        data = sock.recv(1024)
        if data:
            sock.sendall(b"OK")   #itt visszaküldhetünk valamit
        else:
            # Interpret empty result as closed connection
            # Stop listening for input on the connection
            self.inputs.remove(sock)
            sock.close()

    def handleInputs(self, readable):
        for sock in readable:
            if sock is self.server:
                self.handleNewConnection(sock)
            else:
                self.handleDataFromClient(sock)

    def handleExceptionalCondition(self, exceptional):
        for sock in exceptional:
            # Stop listening for input on the connection
            self.inputs.remove(sock)
            sock.close()

    def handleConnections(self):
        while self.inputs:
            try:
                readable, writable, exceptional = select.select(
                    self.inputs, [], self.inputs, self.timeout)

                if not (readable or writable or exceptional):
                    continue

                self.handleInputs(readable)
                self.handleExceptionalCondition(exceptional)
            except KeyboardInterrupt:
                print("A szerver leáll")
                for c in self.inputs:
                    c.close()
                self.inputs = []

if len(sys.argv) == 3:
	simpleTCPSelectServer = SimpleTCPSelectServer(sys.argv[1],int(sys.argv[2]))
else:
	simpleTCPSelectServer = SimpleTCPSelectServer()
simpleTCPSelectServer.handleConnections()
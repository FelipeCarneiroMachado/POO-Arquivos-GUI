import socket
import threading

PORT = 5050
IP = socket.gethostbyname(socket.gethostname())
ADDR = (IP, PORT)




class Server:
    def __init__(self, address : tuple) -> None:
        self.socket = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
        self.address = address
        self.socket.bind(self.address)



    def run(self) -> None:
        self.socket.listen()
        while True:
            connection, address = self.socket.accept()
            threading.Thread(target = self.handleClient, args = (connection, address)).start()
            print("Connected")


    def handleClient(self, connection : socket.socket, address : tuple) -> None:
        connected = True
        while connected:
            header : bytes = connection.recv(5)
            if(header):
                print(type(header), type(header[0]))
                task = header[0].encode("ascii")
                argLen = int.from_bytes(header[1:5], "little")   
                print(task, argLen)                  
            
        connection.close()

    def defineFunction(self, header : bytes) -> None:
        pass



def main():
    server = Server(ADDR)
    server.run()


if __name__ == "__main__":
    main()



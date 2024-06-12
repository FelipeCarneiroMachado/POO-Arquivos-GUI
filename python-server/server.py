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



    def run(self):
        self.socket.listen()
        while True:
            connection, address = self.socket.accept()
            threading.Thread(target = self.handleClient, args = (connection, address)).start()


    def handleClient(self, connection, address):
        pass






def main():
    pass


if __name__ == "__main__":
    main()



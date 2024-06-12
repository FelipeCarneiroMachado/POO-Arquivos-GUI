import socket
import threading
import database

PORT = 5050
IP = socket.gethostbyname(socket.gethostname())
ADDR = (IP, PORT)
ERROR_NO_FILE_LOADED = b"ERROR: no file loaded."



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
        print(threading.get_ident(), threading.get_native_id())
        connected = True
        db : database.Database = None
        while connected:
            header : bytes = connection.recv(5)
            if(header):
                task = chr(header[0])
                argLen = int.from_bytes(header[1:5], "little")   
                print(task, argLen)     
                arg = connection.recv(argLen).decode("ascii")
                print(arg)
                if(task == "L"):
                    db = database.Database(arg)            
                if(db == None):
                    connection.send(b"E"+ len(ERROR_NO_FILE_LOADED).to_bytes(4, "little"))
                    connection.send(ERROR_NO_FILE_LOADED)

            
        connection.close()

    def defineFunction(self, header : bytes) -> None:
        pass



def main():
    server = Server(ADDR)
    server.run()


if __name__ == "__main__":
    main()



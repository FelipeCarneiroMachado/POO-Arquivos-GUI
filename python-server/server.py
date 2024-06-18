import socket
import threading
import database
import logging

log = logging.getLogger(__name__)
logging.basicConfig(filename="serverLog.log", level=logging.DEBUG)

PORT = 5052
IP = socket.gethostbyname(socket.gethostname())
ADDR = (IP, PORT)
ERROR_NO_FILE_LOADED = b"ERROR: no file loaded."




intToBytes = lambda x : x.to_bytes(4, "little")

class closeServerException(Exception):
    pass 



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
            log.info(f"Connceted to : {address}")


    def handleClient(self, connection : socket.socket, address : tuple) -> None:
        connected = True
        db : database.Database = None
        while connected:
            header : bytes = connection.recv(5)
            if(header):
                try:
                    task = chr(header[0])
                    argLen = int.from_bytes(header[1:5], "little")       
                    arg = connection.recv(argLen).decode("ascii")
                    log.debug(f"Msg received : {task} {arg}")
                    if task == "E":
                        log.info("Closing Server normally")
                        raise closeServerException()
                    if task == "L":
                        log.info("Loading file")
                        db = database.Database(arg)
                        connection.send(B"S" + intToBytes(0))
                        continue     
                    if db == None:
                        log.warning("No file Loaded")
                        connection.send(b"E"+ len(ERROR_NO_FILE_LOADED).to_bytes(4, "little"))
                        connection.send(ERROR_NO_FILE_LOADED)
                        continue
                    out = db.command(task, arg)
                    log.debug(f"Output received: {out}")
                except database.dbException as e:
                    connection.send(b"E" + len(str(e).encode("ascii")).to_bytes(4, "little"))
                    connection.send(str(e).encode("ascii"))
                except Exception as e:
                    log.critical(f"Connection closed due to exception: {e}")
                    connection.close()
                    raise e
                else:
                    if out:
                        connection.send(B"A" + len(out).to_bytes(4, "little") + out)    
                    else:
                        connection.send(B"S" + intToBytes(0))


            
        connection.close()




def main():
    try:
        server = Server(ADDR)
        server.run()
    except closeServerException as e:
        server.socket.close()
    except Exception as e:
        server.socket.close()
        raise e


if __name__ == "__main__":
    main()



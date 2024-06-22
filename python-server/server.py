import socket
import threading
import database
import logging
from time import sleep

#setup do log do servidor
log = logging.getLogger(__name__)
logging.basicConfig(filename="serverLog.log", level=logging.DEBUG)
#iconstantes
PORT = 5555
IP = "127.0.0.1"
ADDR = (IP, PORT)
ERROR_NO_FILE_LOADED = b"ERROR: no file loaded."

log.info("\nStarting\n")

#funcao auxiliar para converter inteiro literal em bytes
intToBytes = lambda x : x.to_bytes(4, "little")

#exception para fechamento do servidor
class closeServerException(Exception):
    pass 


#Class servidor, responsavel pela conexao com o cliente e encapsulacao do banco de dados
class Server:
    def __init__(self, address : tuple) -> None:
        #Setup da socket
        self.socket = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
        self.address = address
        self.socket.bind(self.address)
        #Flag para sinalizar o fechamento de todas as conexoes
        self.killFlag = False 

    #Metodo para iniciar operacao do servidor
    def run(self) -> None:
        self.socket.listen() 
        #COntinuamente busca por conexoes
        while True:
            connection, address = self.socket.accept()
            #Logica para encerramento do servidor
            if self.killFlag:
                connection.close()
                raise closeServerException()
            threading.Thread(target = self.handleClient, args = (connection, address)).start()
            log.info(f"Connceted to : {address}")

    #Funcao que engloba todas as atividades do cliente
    def handleClient(self, connection : socket.socket, address : tuple) -> None:
        connected = True
        db : database.Database = None
        while connected:
            #Logica de encerramento do servidor
            if self.killFlag:
                break
            #recebe o header estipulado no protocolo
            header : bytes = connection.recv(1)
            argLen : int = int.from_bytes(connection.recv(4), "big")
            #Caso haja uma mensagem
            if header:
                try:
                    #Decoding
                    task = chr(header[0])
                    #   argLen = int.from_bytes(header[1:9], "little")
                    log.debug(f"{header} {argLen}")       
                    arg = connection.recv(argLen).decode("ascii")
                    log.debug(f"Msg received : {task} {argLen} {arg}")
                    #Funcoes realizadas pelo servidor

                    #Encerramento das atividades
                    if task == "E":
                        db.close()
                        log.info("Closing Server normally")
                        raise closeServerException()
                    #Carregamento do arquivo (criacao do banco de dados)
                    if task == "L":
                        log.info("Loading file")
                        db = database.Database(arg)
                        log.debug("db not crashed")
                        #connection.send(B"S" + intToBytes(0))
                        continue     
                    #Caso o banco de dados nao tenha sido iniciado
                    if db == None:
                        log.warning("No file Loaded")
                        connection.send(b"E"+ len(ERROR_NO_FILE_LOADED).to_bytes(4, "little"))
                        connection.send(ERROR_NO_FILE_LOADED)
                        continue
                    #Demais comandos sao gerenciados diretamente pelo banco de dados
                    out = db.command(task, arg)
                    log.debug(f"Output received: {out}")
                #Erros na database sao retornados ao usuario
                except database.dbException as e:
                    connection.send(b"E" + len(str(e).encode("ascii")).to_bytes(4, "little"))
                    connection.send(str(e).encode("ascii"))
                    log.error(f"Database error: {e}")
                #Logica de encerramento do servidor
                except closeServerException as e:
                    #A flag eh setada e uma conexao eh enviada para a socket do server, 
                    #Assim, p server pode finalizar
                    self.killFlag = True
                    connected = False 
                    sleep(1) #excesso de zelo para garatir que todas as threads terao tempo de fechar suas conexoes antes do server fechar
                    socket.socket(socket.AF_INET, socket.SOCK_STREAM).connect(ADDR)
                #Erros inesperados
                except Exception as e:
                    log.critical(f"Connection closed due to exception: {e.__repr__()}")
                    connection.close()
                    raise e
                #Caso base, sem erros
                else:
                    #Output, retorno ao usuario
                    if out:
                        log.debug(f"Response sent: {out}")
                        connection.send(b"A")
                        connection.send(len(out).to_bytes(4 , "big"))
                        connection.send(out)
                        #log.debug(b"A" + len(out).to_bytes(8 , "big") +str(connection.send(out)))    
                    #Confirmacao de sucesso
                    else:
                        pass
                        #connection.send(B"S")


        #Fecha a conexao
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



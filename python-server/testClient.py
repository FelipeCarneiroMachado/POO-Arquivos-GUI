import socket
import threading



PORT = 5050
IP = socket.gethostbyname(socket.gethostname())
ADDR = (IP, PORT)



client = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
path = "/home/dont_close_update_tabs/Documents/Usp/poo/POO-Arquivos-GUI/python-server/server.py"
msg = b"L" + len(path).to_bytes(4, "little")
client.connect(ADDR)
client.send(msg)
client.send(path.encode("ascii"))
client.close()
        

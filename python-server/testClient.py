import socket
import threading



PORT = 5053
IP = socket.gethostbyname(socket.gethostname())
ADDR = (IP, PORT)



client = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
path = "/home/dont_close_update_tabs/Documents/Usp/poo/POO-Arquivos-GUI/python-server/server.py"
b0 = 0

msg = b"L" + len(path).to_bytes(4, "little")
client.connect(ADDR)
try:
    while True:
        s = input().encode("ascii")
        if len(s) == 0:
            client.send(s[:1] + b0.to_bytes(4, "little"))    
        client.send(s[:1] + len(s[2:]).to_bytes(4, "little") + s[2:])
        ret = client.recv(5)
        print(chr(ret[0])+" " + client.recv(int.from_bytes(ret[1:5], "little")).decode("ascii"))
except Exception:
    client.close()


client.send(msg)
client.send(path.encode("ascii"))
buf = client.recv(5)
print(client.recv(int.from_bytes(buf[1:])))
        

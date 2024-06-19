import ctypes.util
import threading
import os
import sys
import ctypes
import subprocess
import logging

#setup do logger
log = logging.getLogger(__name__)
logging.basicConfig(filename="serverLog.log", level=logging.DEBUG)

#classe que gerencia nome de arquivos e caminhos
class FileHandler:
    def __init__(self, filePath : str) -> None:
        self.srcFilePath = filePath
        self.os = sys.platform
        self.csv = filePath.split(".")[-1] == "csv"
        if self.os == "linux":
            self.localFilePath = "./env/bin" + str(threading.get_ident())
            
        else:
            self.localFilePath = ".\\env\\bin" + str(threading.get_ident())
        if self.os == "linux":
            self.indexPath = "./env/index" + str(threading.get_ident())
            
        else:
            self.indexPath = ".\\env\\index" + str(threading.get_ident())
            

    def copyBin(self):
        if self.os == "linux":
            proc = subprocess.Popen(["cp", self.srcFilePath, self.localFilePath], stdout=subprocess.PIPE, stderr=subprocess.PIPE)
            out, err = proc.communicate()
            if err:
                raise dbException(str(err))
        else:   
            os.popen("copy " + self.srcFilePath + " " + self.localFilePath)
    def close(self):
         os.remove(self.localFilePath)
         os.remove(self.indexPath)


#exception para o banco de daods
class dbException(Exception):
    pass


class Database:
    def __init__(self, filePath : str) -> None:
        self.fileHandler = FileHandler(filePath)
        self.os = sys.platform
        self.csv = filePath.split(".")[-1] == "csv"
        if self.csv:
            proc = subprocess.Popen(["./sgbd"], shell=True,stdin=subprocess.PIPE, 
                                stdout=subprocess.PIPE, stderr=subprocess.PIPE)
            out, err = proc.communicate(b"1" + b" "+ self.fileHandler.srcFilePath.encode("ascii") + b" "
                             + self.fileHandler.localFilePath.encode("ascii") + b"\n")
            if b"Falha no processamento do arquivo.\n" in out:
                raise dbException("Error at loading")
            if err:
                raise dbException(str(err))
        else:
            self.fileHandler.copyBin()
        proc = subprocess.Popen(["./sgbd"], shell=True,stdin=subprocess.PIPE, 
                                stdout=subprocess.PIPE, stderr=subprocess.PIPE)
        out, err = proc.communicate(b"4"+ b" " + self.fileHandler.localFilePath.encode("ascii")+ b" "
                         +self.fileHandler.indexPath.encode("ascii"))
        if b"Falha no processamento do arquivo.\n" in out:
                raise dbException("Error at indexation")
        if err:
            raise dbException(str(err))

        
    

    def insert(self, register : str) -> None:
        logStr = (b"6 "+ self.fileHandler.localFilePath.encode("ascii")+ b" "+
                                     self.fileHandler.indexPath.encode("ascii") + b" 1\n" +
                                     register.encode("ascii") + b"\n")
        log.debug(f"Insertion command: {logStr}")
        proc = subprocess.Popen(["./sgbd"], shell=True,stdin=subprocess.PIPE, 
                                stdout=subprocess.PIPE, stderr=subprocess.PIPE)
        out, err = proc.communicate(b"6 "+ self.fileHandler.localFilePath.encode("ascii")+ b" "+
                                     self.fileHandler.indexPath.encode("ascii") + b" 1\n" +
                                     register.encode("ascii") + b"\n")
        if b"Falha no processamento do arquivo.\n" in out:
                raise dbException("Error at insertion")
        if err:
            raise dbException(str(err))
        
    def save(self) -> None:
        if self.csv:
            with open(self.fileHandler.srcFilePath, "w") as originFile :
                originFile.write("id,idade,nomeJogador,nacionalidade,nomeClube\n" +
                    self.returnAll().decode("ascii").replace("|", "\n"))
        else:
            proc = subprocess.Popen(["cp", self.fileHandler.localFilePath, self.fileHandler.srcFilePath], stdout=subprocess.PIPE, stderr=subprocess.PIPE)
            out, err = proc.communicate()
            if err:
                raise dbException(str(err))
        


    def query(self, query : dict) -> bytes:
        s = f"{len(query.keys())}"
        for k, v in query.items():
            s += " "
            if k.startswith("id"):
                s += f"{k} {v}"
            else:
                s += f"{k} \"{v}\""
        s += "\n"
        proc = subprocess.Popen(["./sgbd"], shell=True,stdin=subprocess.PIPE, 
                                stdout=subprocess.PIPE, stderr=subprocess.PIPE)
        out, err = proc.communicate(b"3 " +  self.fileHandler.localFilePath.encode("ascii")+ b" "+
                                     self.fileHandler.indexPath.encode("ascii") + b" 1\n" +
                                     s.encode("ascii"))
        if b"Falha no processamento do arquivo.\n" in out:
            raise dbException("Error at querying")
        if err:
            raise dbException(str(err))
        return out.rstrip(b"|")
    

    def decodeQuery(self, query : str) -> dict:
        return {val[0] : val[1] for val in map(lambda x: x.split(":"), query.split("&"))}

    def command(self, command : str, param : str = None) -> bytes | None:
        log.debug(f"Executing: {command} {param}")
        match command:
            case "I":
                self.insert(param)
                return None
            case "Q":
                return self.query(self.decodeQuery(param))
            case "A":
                return self.returnAll()
            case "R":
                self.remove(self.decodeQuery(param))
            case "S":
                self.save()
            case _:
                raise dbException("Command Not Recognized")



    def remove(self, query : dict) -> None:
        s = f"{len(query.keys())}"
        for k, v in query.items():
            s += " "
            if k.startswith("id"):
                s += f"{k} {v}"
            else:
                s += f"{k} \"{v}\""
        s += "\n"
        proc = subprocess.Popen(["./sgbd"], shell=True,stdin=subprocess.PIPE, 
                                stdout=subprocess.PIPE, stderr=subprocess.PIPE)
        out, err = proc.communicate(b"5 " +  self.fileHandler.localFilePath.encode("ascii")+ b" "+
                                     self.fileHandler.indexPath.encode("ascii") + b" 1\n" +
                                     s.encode("ascii"))
        if b"Falha no processamento do arquivo.\n" in out:
            raise dbException("Error at removing")
        if err:
            raise dbException(str(err))

    def returnAll(self) -> bytes:
        proc = subprocess.Popen(["./sgbd"], shell=True,stdin=subprocess.PIPE, 
                                stdout=subprocess.PIPE, stderr=subprocess.PIPE)
        out, err = proc.communicate(b"2 " + self.fileHandler.localFilePath.encode("ascii") + b"\n")
        if b"Falha no processamento do arquivo.\n" in out:
            raise dbException("Error at querying")
        if err:
            raise dbException(str(err))
        return out.rstrip(b"|")

    def close(self):
        self.fileHandler.close()

        
        
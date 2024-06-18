import ctypes.util
import threading
import os
import sys
import ctypes
import subprocess

class FileHandler:
    def __init__(self, filePath : str) -> None:
        self.srcFilePath = filePath
        self.os = sys.platform
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



class dbException(Exception):
    pass


class Database:
    calls = 0
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
        proc = subprocess.Popen(["./sgbd"], shell=True,stdin=subprocess.PIPE, 
                                stdout=subprocess.PIPE, stderr=subprocess.PIPE)
        out, err = proc.communicate(b"6 "+ self.fileHandler.localFilePath.encode("ascii")+ b" "+
                                     self.fileHandler.indexPath.encode("ascii") + b" 1\n" +
                                     register.encode("ascii") + b"\n")
        if b"Falha no processamento do arquivo.\n" in out:
                raise dbException("Error at insertion")
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
    

    def decodeQuery(query : str) -> dict:
        return {val[0] : val[1] for val in map(lambda x: x.split(":"), query.split("&"))}

    def command(self, command : str, param : str = None) -> bytes | None:
        match command:
            case "I":
                self.insert(param)
            case "Q":
                return self.query(self.decodeQuery(param))
            case "A":
                return self.returnAll()
            case _:
                raise dbException("Command Not Recognized")

                    
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

        
        



#d = Database("/home/dont_close_update_tabs/Documents/Usp/poo/POO-Arquivos-GUI/srcFiles/dadoTeste.csv")
#d.command("I", "123456 69 \"ANDRE\" \"BAHIA\" \"REMO\"")
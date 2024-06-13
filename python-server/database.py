import ctypes.util
import threading
import os
import sys
import ctypes


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
            os.popen("cp " + self.srcFilePath + " " + self.localFilePath)
        else:   
            os.popen("copy " + self.srcFilePath + " " + self.localFilePath)
    def close(self):
         os.remove(self.localFilePath)






class Database:
    calls = 0
    def __init__(self, filePath : str) -> None:
        self.fileHandler = FileHandler(filePath)
        self.os = sys.platform
        self.lib = ctypes.util.find_library("pyDB")
        if self.os == "linux":
                self.libPath = "./libpyDb.so"
                self.lib = ctypes.CDLL(self.libPath)
        else:
                self.libPath = ".\\c_sgdb\\libpyDb.so"
                self.lib = ctypes.CDLL(self.libPath)
        self.csv = filePath.split(".")[-1] == "csv"
        if self.csv:
            self.lib.loadCsv(( self.fileHandler.srcFilePath).encode("ascii"),
                                     (self.fileHandler.localFilePath).encode("ascii"))
        else:
            self.fileHandler.copyBin()
        self.lib.buildIndex(self.fileHandler.indexPath.encode("ascii"),
                            self.fileHandler.localFilePath.encode("ascii"))
        

        

    def command(self, command : str, param : str) -> str:
        match command:
             case "I":
                    print(self.calls)
                    self.calls += 1
                    self.lib.insertDb(ctypes.c_char_p(param.encode("ascii")), 
                                    ctypes.c_char_p(self.fileHandler.localFilePath.encode("ascii")),
                                    ctypes.c_char_p(self.fileHandler.indexPath.encode("ascii")))



    def close(self):
        self.fileHandler.close()


d = Database("/home/dont_close_update_tabs/Documents/Usp/poo/POO-Arquivos-GUI/srcFiles/dadoTeste.csv")
print("a")
d.command("I", "123456,69,ANDRE,BAHIA,REMO")
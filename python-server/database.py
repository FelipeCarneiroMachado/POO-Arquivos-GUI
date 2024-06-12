import threading
import os
import sys


class FileHandler:
    def __init__(self, filePath : str) -> None:
        self.srcFilePath = filePath
        self.os = sys.platform
        if self.os == "linux":
            self.localFilePath = "./env/tempfile" + str(threading.get_ident())
            os.popen("cp " + self.srcFilePath + " " + self.localFilePath)
        else:
            self.localFilePath = ".\\env\\tempfile" + str(threading.get_ident())
            os.popen("copy " + self.srcFilePath + " " + self.localFilePath)
                
        





class Database:
    def __init__(self, filePath : str) -> None:
        self.fileHandler = FileHandler(filePath)
        self.lib
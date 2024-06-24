run_GUI:
	javac GUI.java
	java GUI.java

run_server: sgbd
	python3 server.py

clean:
	rm *.class
	rm serverLog.log

sgbd:
	gcc -g -o sgbd ./c_sgbd/utils.c ./c_sgbd/main.c ./c_sgbd/interface.c ./c_sgbd/binFile.c ./c_sgbd/player.c ./c_sgbd/index.c 

clear:
	rm env/*

clean_sgbd:
	rm sgbd

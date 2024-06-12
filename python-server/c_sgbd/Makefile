#Makefile do projeto
#diretiva de teste local
test:
	gcc player.c interface.c utils.c index.c binFile.c testmain.c -o main -g -march=native
#diretiva para compilacao final
all:
	gcc player.c interface.c utils.c index.c binFile.c main.c -o main  -march=native
run:
	./main
clean:
	rm main
zip:
	zip Grupo12_Trabalho1.zip *.c *.h Makefile 
#diretivas de test local
clearBin:
	cp Entrada/binario1.bin Entrada/binarioTeste.bin
envdo:
	gcc player.c interface.c utils.c index.c binFile.c main.c -o ./env/main -g -march=native
	cp Entrada/* in/* out/*  ./env/ 


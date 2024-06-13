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
	rm main *..so
	zip Grupo12_Trabalho1.zip *.c *.h Makefile 
#diretivas de test local
clearBin:
	cp Entrada/binario1.bin Entrada/binarioTeste.bin
envdo:
	gcc player.c interface.c utils.c index.c binFile.c main.c -o ./env/main -g -march=native
	cp Entrada/* in/* out/*  ./env/ 
linuxShared: player binFile utils interface index pyLink
	gcc -shared -fPIC player.o interface.o utils.o index.o binFile.o pyLink.o -o ../libpyDb.so
#gcc -shared -o libpyDB.so pyDb.o

player:
	gcc -c -fPIC player.c -o player.o
binFile:
	gcc -c -fPIC binFile.c -o binFile.o
utils:
	gcc -c -fPIC utils.c -o utils.o
interface:
	gcc -c -fPIC interface.c -o interface.o
index:
	gcc -c -fPIC index.c -o index.o
pyLink:
	gcc -c -fPIC pyLink.c -o pyLink.o


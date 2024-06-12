/*
================================================
Arquivo interface para manipulacao de binario
================================================
*/

#ifndef BIN_H
    #define BIN_H
    #include<stdbool.h>
    #include<stdlib.h>
    #include<string.h>
    #include<stdio.h>
    #include<stdint.h>
    #include"player.h"
    //#include"index.h"
    #include"utils.h"
    #define NO_SEEK -1
    //struct para o header do arquivo binario
    typedef struct _header{
        char status;
        int64_t topo;
        int64_t offset;
        int nReg;
        int nRem;
    }HEADER;
    FILE* initFile(char* filename);
    HEADER* extraiHeader(FILE *fd);
    void csvToBin(FILE *src, FILE* data);
    void escreveRegistro(FILE* data, int64_t offset, PLAYER* player);
    void setStatus(FILE *fd, int8_t status);
    void removeInDisk(FILE* bin, HEADER* h , int64_t offset);
    int64_t insertPlayer(FILE *bin, HEADER *h, PLAYER* p);
    void updateHeader(FILE *bin, HEADER* h);


#endif
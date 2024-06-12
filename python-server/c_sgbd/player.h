/*
================================================
Arquivo interface da struct PLAYER
Aqui estao definida a interface e o formato da struct
================================================
*/
#ifndef H_PLAYER
    #define H_PLAYER
    #include<stdbool.h>
    #include<stdlib.h>
    #include<string.h>
    #include<stdio.h>
    #include<stdint.h>
    //#include"binFile.h"
    #include"utils.h"
    
    //definicao da struct que contem as informacoes do jogador
    struct _player{
        char status;
        int64_t prox;
        int tamanho;
        int id;
        int idade;
        int nomeLen;
        char* nome;
        int paisLen;
        char *pais;
        int clubeLen;
        char* clube;
    };
    typedef struct _player PLAYER;
    //Compara os campos de uma struct com uma lista de valores
    bool checkPlayer(PLAYER* p, int numOfParameters, char** fields, char** values);
    //Recebe 2 arrays de strings, 1 com os campos a serem comparados e outro com os valores
    //Os arrays devem ser pareados (campo[i] corresponde a valor[i])
    PLAYER* playerInit();
    PLAYER *parseLine(char *line); //A partir de uma linha do .csv, gera uma struct com as informacoes
    PLAYER* playerFromBin(FILE*fd, int64_t offset); //Extrai uma struct de um binario no offset parametro
    void playerSetIdade(PLAYER* p, int idade);
    void playerSetId(PLAYER* p, int id);
    //Todas as funcoes que setam campos com strings criam copias das strings passadas como argumentos
    //nao eh preocupacao do usuario deste header se preocupar com essa logistica
    void playerSetNome(PLAYER* p, char* nome);
    void playerSetClube(PLAYER* p, char* clube);
    void playerSetPais(PLAYER* p, char* pais);
    int playerTamanho(PLAYER* p, bool set); //Seta e retorna o tamanho
    int* idFromBin(FILE* fd, int64_t offset);
    void playerPrint(PLAYER *p);
    void playerFree(PLAYER** p); //libera memoria, deve ser passado por referencia
    PLAYER* playerInput();



#endif
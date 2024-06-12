#include"player.h"
#define NO_SEEK -1
/*
================================================
Arquivo fonte da struct PLAYER
Aqui estao definidas todas as funcoes para manipulacao 
dessa struct, incluindo conversao entre formatos (texto, binario no arquivo, struct)
================================================
*/

//Funcoes para uso externo

PLAYER* playerInit(){
    //Aloca e inicializa a struct com valores propicios
    PLAYER* newPlayer = (PLAYER*)malloc(sizeof(PLAYER));
    newPlayer->status = '0';
    newPlayer->prox = -1;
    newPlayer->tamanho = 0;
    newPlayer->id = -1;
    newPlayer->idade = -1;
    newPlayer->clube = NULL;
    newPlayer->pais = NULL;
    newPlayer->nome = NULL;
    newPlayer->clubeLen = 0;
    newPlayer->nomeLen = 0;
    newPlayer->paisLen = 0;
    return newPlayer;
}

void playerSetIdade(PLAYER* p, int idade){
    if(idade == 0)  //Lida com o caso nulo
        idade =  -1;
    p->idade = idade;

}
void playerSetId(PLAYER* p, int id){
    p->id = id;
}
//Todas as funcoes que setam campos com strings criam copias das strings passadas como argumentos
//nao eh preocupacao do usuario deste header se preocupar com essa logistica

// Escreve na struct on nome do jogador
void playerSetNome(PLAYER* p, char* nome){
    int len = strlen(nome);
    if(len == 0) //caso nulo
        return;
    char* nomeCp = (char*)malloc(len + 1);
    strcpy(nomeCp, nome);
    p->nome = nomeCp;
    p->nomeLen = len;
}


//Escreve na struct o clube do jogador
void playerSetClube(PLAYER* p, char* clube){
    int len = strlen(clube); //caso nulo
    if(len == 0)
        return;
    char* clubeCp = (char*)malloc(len + 1);
    strcpy(clubeCp, clube);
    p->clube = clubeCp;
    p->clubeLen = len;
}
// Escreve na struct a nacionalidade do jogador
void playerSetPais(PLAYER* p, char* pais){
    int len = strlen(pais); //caso nulo
    if(len == 0)
        return;
    char* paisCp = (char*)malloc(len + 1);
    strcpy(paisCp, pais);
    p->pais = paisCp;
    p->paisLen = len;
}
int playerTamanho(PLAYER* p, bool set){
    int size = p->tamanho;
    if(set){
        int size = 33; //tamanho fixo = 1 + 8 + 6 * 4
        size += p->clubeLen + p->nomeLen + p->paisLen;
        p->tamanho = size; //Retorna e seta o tamanho
    }
    return size;
}
void playerPrint(PLAYER *p){
    if(p->nome != NULL)
        printf("Nome do Jogador: %s\n", p->nome);
    else
        printf("Nome do Jogador: SEM DADO\n");
    if(p->pais != NULL)
        printf("Nacionalidade do Jogador: %s\n", p->pais);
    else
        printf("Nacionalidade do Jogador: SEM DADO\n");
    if(p->clube != NULL)
        printf("Clube do Jogador: %s\n\n", p->clube);
    else
        printf("Clube do Jogador: SEM DADO\n\n");
    }

void playerFree(PLAYER** p){
    //Liberacao de memoria, a struct deve ser passada por referencia
    if((*p)->nome != NULL)
        free((*p)->nome);
    if((*p)->clube != NULL)
        free((*p)->clube);
    if((*p)->pais != NULL)
        free((*p)->pais);
    free(*p);
    *p = NULL;
}






bool checkPlayer(PLAYER* p, int numOfParameters, char** fields, char** values){
    //Compara os campos de uma struct com uma lista de valores
    //Recebe 2 arrays de strings, 1 com os campos a serem comparados e outro com os valores
    //Os arrays devem ser pareados (campo[i] corresponde a valor[i])
    if(p->status == '1')//Se o registro esta logicamente removido, retorna falso
        return false;
    for(int i = 0; i < numOfParameters; i++){
        if(strcmp(fields[i], "id") == 0){
            if(atoi(values[i]) != p->id)
                return false; 
        }
        if(strcmp(fields[i], "idade") == 0)
            if(atoi(values[i]) != p->idade)
                return false;
        if(strcmp(fields[i], "nomeJogador") == 0){
            if(p->nome == NULL)
                return false;
            if(strcmp(values[i], p->nome) != 0)
                return false;
        }
        if(strcmp(fields[i], "nacionalidade") == 0){
            if(p->pais == NULL)
                return false;
            if(strcmp(values[i], p->pais) != 0)
                return false;
        }
        if(strcmp(fields[i], "nomeClube") == 0){
            if(p->clube == NULL)
                return false;
            if(strcmp(values[i], p->clube) != 0)
                return false;
        }
    }
    return true;
}


PLAYER* parseLine(char *line){
    //A partir de uma linha do .csv, gera uma struct com as informacoes
    PLAYER* newPlayer = playerInit();
    char iterChar, tempStr[64];
    int i = 0, j = 0;
    while((iterChar = line[i]) != ','){
        //loop para ler id
        tempStr[j++] = iterChar;
        i++;
    }
    tempStr[j] = '\0';
    playerSetId(newPlayer, atoi(tempStr));
    j = 0; i++;
    memset(tempStr, 0, 64);
    while((iterChar = line[i]) != ','){
        //loop para ler idade
        tempStr[j++] = iterChar;
        i++;
    }
    tempStr[j] = '\0';
    playerSetIdade(newPlayer, atoi(tempStr));
    j = 0; i++;
    memset(tempStr, 0, 64);
    while((iterChar = line[i]) != ','){
        //loop para ler nome
        tempStr[j++] = iterChar;
        i++;
    }
    tempStr[j] = '\0';
    playerSetNome(newPlayer, tempStr);
    j = 0; i++;
    memset(tempStr, 0, 64);
    while((iterChar = line[i]) != ','){
        //loop para ler pais
        tempStr[j++] = iterChar;
        i++;
    }
    tempStr[j] = '\0';
    playerSetPais(newPlayer, tempStr);
    j = 0; i++;
    memset(tempStr, 0, 64);
    while((iterChar = line[i]) != '\n' && iterChar != '\r'){
        //loop para ler clube
        tempStr[j++] = iterChar;
        i++;
    }
    tempStr[j] = '\0';
    playerSetClube(newPlayer, tempStr);
    j = 0; i++;
    playerTamanho(newPlayer, true);
    return newPlayer;
}

//Retorna um player a partir de uma linha de texto em conformidade
//com o formato da funcionalidade 6
PLAYER* playerInput(){
    PLAYER *p = playerInit();
    char buffer[256];
    scanf("%s", buffer);
    p->id = atoi(buffer);
    scan_quote_string(buffer);
    if(buffer[0] != '\0')
        p->idade = atoi(buffer);
    scan_quote_string(buffer);
    if(buffer[0] != '\0')
        playerSetNome(p,buffer);
    scan_quote_string(buffer);
    if(buffer[0] != '\0')
        playerSetPais(p,buffer);
    scan_quote_string(buffer);
    if(buffer[0] != '\0')
        playerSetClube(p,buffer);
    getchar();
    playerTamanho(p, true);
    return p;
}


//Retorna um array onde [0] eh o id e [1] eh o tamanho
int* idFromBin(FILE* fd, int64_t offset){
    //Esta funcao eh utilizada na criacao do indice, faz menos leituras que a playerFromBin
    char status;
    int id, size, *buffer;
    buffer = malloc(8);
    if(ftell(fd) != offset) //Se o arquivo ja esta no offset, evita um seek
        fseek(fd, offset, SEEK_SET);
    fread(&status, 1, 1, fd);
    fread(&size, 4, 1, fd);
    buffer[0] = size;
    if(status == '1'){
        buffer[0] = -1;
    }
    fseek(fd, 8, SEEK_CUR);
    fread(&id, 4, 1, fd);
    buffer[1] = id;
    return buffer;
}



PLAYER* playerFromBin(FILE*fd, int64_t offset){
    //Extrai uma struct de um binario no offset parametro
    char regBuffer[128];
    char *tempPtr;
    char fieldBuffer[64]; //buffers teporarios
    PLAYER* p = playerInit();
    if(offset != NO_SEEK) //Se o arquivo ja esta no offset, evita um seek
        fseek(fd, offset, SEEK_SET);
    fread(regBuffer, 1, 5, fd); //Primeiro fread = status e tamanho
    p->status = regBuffer[0];
    tempPtr = regBuffer + 1; //ponteiro temporario, aponta para um offset dentro do buffer
    memcpy(&(p->tamanho), tempPtr, 4); //Depois copia a informacao para a struct, realiza essa operacao para todos os campos
    fread(&regBuffer, 1, p->tamanho - 5, fd); //fread no resto do registro
    tempPtr = regBuffer;
    memcpy(&p->prox, tempPtr, 8); //leitura do prox
    tempPtr += 8;
    memcpy(&(p->id), tempPtr, 4);//leitura do id
    tempPtr += 4;
    memcpy(&(p->idade), tempPtr, 4);//leitura da idade
    tempPtr += 4;
    memcpy(&(p->nomeLen), tempPtr, 4);//leitura do tamanho do nome
    tempPtr += 4;
    memset(fieldBuffer, 0, 64);
    memcpy(fieldBuffer, tempPtr, p->nomeLen); //leitura do nome
    tempPtr += p->nomeLen;
    playerSetNome(p, fieldBuffer);
    memcpy(&(p->paisLen), tempPtr, 4); //leitura do tamanho da nacionalidade
    tempPtr += 4;
    memset(fieldBuffer, 0, 64);
    memcpy(fieldBuffer, tempPtr, p->paisLen); //leitura da nacionalidade
    tempPtr += p->paisLen;
    playerSetPais(p, fieldBuffer);
    memcpy(&(p->clubeLen), tempPtr, 4); //leitura do tamanho do clube
    tempPtr += 4;
    memset(fieldBuffer, 0, 64);
    memcpy(fieldBuffer, tempPtr, p->clubeLen); //leitura do clube
    tempPtr += p->clubeLen;
    playerSetClube(p, fieldBuffer);
    return p;
}
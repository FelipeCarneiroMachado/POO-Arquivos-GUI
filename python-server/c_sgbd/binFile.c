#include"binFile.h"
/*
================================================
Arquivo para manipulacao direta dos arquivos binarios
================================================
*/


//altera o status do arquivo 
void setStatus(FILE *fd, int8_t status){
    fseek(fd,0, SEEK_SET);
    fwrite(&status, 1, 1, fd);
}

//Seta o numero de registros no cabecalho
void setNumDeRegistros(FILE *fd, int32_t num){
    fseek(fd, 17, SEEK_SET);
    fwrite(&num, 4, 1 ,fd);
}
//Seta o proximo offset livre no cabecalho
void setProxOffset(FILE *fd, int64_t num){
    fseek(fd, 9, SEEK_SET);
    fwrite(&num, 4, 1 ,fd);
}
//Extrai as informacoes do header e poe na struct
HEADER* extraiHeader(FILE *fd){
    HEADER* h = malloc(sizeof(HEADER));
    if(ftell(fd) != 0) //acessa inicio do arquivo
        fseek(fd, 0, SEEK_SET);
    char hBuffer[25], *ptr; //Buffer para ler e ponteiro para percorrer o buffer
    fread(hBuffer, 1, 25, fd);
    ptr = hBuffer;
    h->status = hBuffer[0];
    ptr += 1;
    memcpy(&(h->topo), ptr, 8); //Copia as informacoes do buffer para a struct usando memcpy
    ptr += 8;
    memcpy(&(h->offset), ptr, 8);
    ptr += 8;
    memcpy(&(h->nReg), ptr, 4);
    ptr += 4;
    memcpy(&(h->nRem), ptr, 4);
    return h;
}
//Escreve as informcacoes de uma struct header no arquivo binario
void updateHeader(FILE *bin, HEADER* h){
    fseek(bin, 0, SEEK_SET);
    fwrite(&(h->status), 1, 1, bin);
    fwrite(&(h->topo), 8, 1, bin);
    fwrite(&(h->offset), 8, 1, bin);
    fwrite(&(h->nReg), 4, 1, bin);
    fwrite(&(h->nRem), 4, 1, bin);
}


//Converte o arquivo .csv para um binario, seguindo as especificacoes do trabalho
void csvToBin(FILE *src, FILE* data){
    int64_t offset = 25;   
    int32_t nRegistros = 0;


    setStatus(data, '0'); //Arquivo inconsistente nesse momento
    fseek(data, offset, SEEK_SET);
    PLAYER *player;
    char tempstr[100]; 
    fgets(tempstr, 100, src);
    memset(tempstr, 0, 100);

    while(fgets(tempstr, 100, src) != NULL){ //itera pelo .csv
        player = parseLine(tempstr); //obtem player a partir de linha
        escreveRegistro(data, offset, player);
        offset += playerTamanho(player, true);
        memset(tempstr, 0, 100); //limpa o buffer
        playerFree(&player);
        nRegistros += 1;
    }

    setStatus(data, '1');
    setNumDeRegistros(data, nRegistros);
    setProxOffset(data, offset);
    fclose(data);
    fclose(src);
}

//Inicializa o cabecalho do arquivo
FILE* initFile(char* filename){
    FILE *data = fopen(filename, "wb");
    int64_t temp8bytes;
    int32_t temp4bytes;
    int8_t tempByte = '1';
    //Escreve o indicador de status
    fwrite(&tempByte, 1, 1, data);
    //Escreve o proximo logicamente removido
    temp8bytes = -1;
    fwrite(&temp8bytes, 8, 1, data);
    //Escreve o proximo offset disponivel
    temp8bytes = 0;
    fwrite(&temp8bytes, 8, 1, data);
    //Escreve o n de registros
    temp4bytes = 0;
    fwrite(&temp4bytes, 4, 1, data);
    //Escreve o n de registros removidos logicamente
    fwrite(&temp4bytes, 4, 1, data);
    return data;
}



//Escreve no arquivo a info de um jogador a partir da struct
void escreveRegistro(FILE* data, int64_t offset, PLAYER* player){
    //Se o offset for um valor valido, realiza um fseek, caso seja a constante NO_SEEK, evita
    if(offset != NO_SEEK)
        fseek(data, offset, SEEK_SET);
    int8_t tempByte = '0';
    int64_t temp8bytes = -1; //variaveis temporarias para escrita
    fwrite(&tempByte, 1, 1, data); //removido
    int32_t regSize = playerTamanho(player, false);
    fwrite(&regSize,4, 1, data); //tamanho do registro
    fwrite(&temp8bytes, 8, 1, data); //prox offset
    fwrite(&(player->id), 4, 1, data); //id
    fwrite(&(player->idade), 4, 1, data);
    fwrite(&(player->nomeLen), 4, 1, data); //tamanho nome
    if(player->nomeLen > 0)
        fwrite((player->nome), 1,player->nomeLen, data);
    fwrite(&(player->paisLen), 4, 1, data); //tamanho pais
    if(player->paisLen > 0)
        fwrite((player->pais), 1,player->paisLen, data);
    fwrite(&(player->clubeLen), 4, 1, data); //tamanho clube
    if(player->clubeLen > 0)
        fwrite((player->clube), 1,player->clubeLen, data);   
} 




//Realiza a remocao logica do registro em disco a partir de um offset e 
//faz a insercao dele na lista de registros removidos
void removeInDisk(FILE* bin, HEADER* h, int64_t offset){
    PLAYER *toRemove = playerFromBin(bin, offset); //Extrai o player para pegaar seu tamanho
    //Marca como logicamente removido
    fseek(bin, offset, SEEK_SET);
    char tempByte = '1';
    fwrite(&tempByte, 1, 1, bin);
    //Se a lista de removidos esta vazia o removido se torna o topo da lista
    if(h->topo == -1){
        fseek(bin, 1, SEEK_SET);
        fwrite(&offset, 8, 1, bin); //Atualiza topo
        h->topo = offset;
        h->nReg--;
        h->nRem++;
        return;
    }
    int64_t prevOff = 0, curOff = h->topo;
    PLAYER *prev = NULL, *current = playerFromBin(bin, h->topo);
    //Iteracao pela lista de removidos, insere ordenadamente
    while(true){
        if(toRemove->tamanho < current->tamanho){
            //Insercao na lista
            fseek(bin, offset + 5, SEEK_SET);
            fwrite(&curOff, 8, 1, bin);
            //Caso o seja o menor registro
            if(prev == NULL){
                h->topo = offset;
            }
            else{
                fseek(bin, prevOff + 5, SEEK_SET);
                fwrite(&offset, 8, 1, bin);
                playerFree(&prev);
            }
            playerFree(&toRemove); playerFree(&current);
            if(prev != NULL) playerFree(&prev);
            break;
        }
        //Iteracao
        if(prev != NULL)
            playerFree(&prev);
        prevOff = curOff;
        curOff = current->prox;
        prev = current;
        //Caso chegue ao fim da lista
        if(curOff == -1){
            fseek(bin, prevOff + 5, SEEK_SET);
            fwrite(&offset, 8, 1, bin);
            playerFree(&toRemove); playerFree(&prev);
            break;
        }
        //Iteracao
        current = playerFromBin(bin, current->prox);
    }
        //Atualiza header
        h->nReg--;
        h->nRem++;
        fflush(bin);
}


//Insere um registro em disco e atualiza a lista de removidos
//Retorna o offset onde foi inserido
int64_t insertPlayer(FILE *bin, HEADER *h, PLAYER* p){
    if(h->topo == -1){
        //Caso nao haja removidos, vai ao fim do arquivo
        escreveRegistro(bin, h->offset, p);
        h->nReg++;
        h->offset += p->tamanho;
        fflush(bin);
        return h->offset - p->tamanho;
    }
    int ncifroes;
    char cifrao =  '$';
    PLAYER *prev = NULL, *current = playerFromBin(bin,h->topo);
    int64_t prevOff = -1, curOff = h->topo;
    //Itera sobre a lista de removidos
    while(true){
        //Caso encontre a posicao adequada
        if(p->tamanho <= current->tamanho){
            ncifroes = current->tamanho - p->tamanho;
            p->tamanho = current->tamanho; //o tamanho escrito em disco deve corresponder a todo o espaco utilizado
            if(prev == NULL){
                //Caso de ser o primeiro elemento
                fseek(bin, 1, SEEK_SET);
                fwrite(&(current->prox), 8, 1, bin);          
                escreveRegistro(bin, curOff, p);
                //Escreve "$" no espaco nao utilizado
                for(int i = 0; i < ncifroes; i++)
                    fwrite(&cifrao, 1, 1, bin);
                h->topo = current->prox;
                h->nReg++;
                h->nRem--;    
                break;
            }
            //Escrita padrao e remocao da lista de removidos
            fseek(bin, prevOff + 5, SEEK_SET);
            fwrite(&(current->prox), 8, 1, bin);
            escreveRegistro(bin, curOff, p);
            //Escreve "$" no espaco nao utilizado
            for(int i = 0; i < ncifroes; i++)
                fwrite(&cifrao, 1, 1, bin);
            h->nReg++;
            h->nRem--;
            break;
        }
        //Iteracao
        if(prev != NULL)
            playerFree(&prev);
        prevOff = curOff;
        curOff = current->prox;
        prev = current;
        //Fim da lista, escreve no final
        if(curOff == -1){
            escreveRegistro(bin, h->offset, p);
            h->offset += p->tamanho;
            h->nReg++;
            return h->offset - p->tamanho; 
        }
        //Iteracao
        current = playerFromBin(bin, current->prox);
    }
    fflush(bin);
    return curOff;
}



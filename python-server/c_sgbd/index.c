#include"index.h"


/*
Arquivo para definicao do funcionamento do indice
*/



//Funcoes para o sorting do indice

//Funcao de swap
void swap(struct data* arr, int a, int b){
    struct data temp = arr[a];
    arr[a] = arr[b];
    arr[b] = temp;
}


//Particao do quicksort
int partition(struct data* arr, int low, int high){
    struct data pivot = arr[(low + high) / 2];
    int i = low, j = high;
    while(i < j){
        if(arr[i].id >= pivot.id && arr[j].id <= pivot.id)
            swap(arr, i, j);
        if(arr[i].id < pivot.id)
            i++;
        if(arr[j].id > pivot.id)
            j--;
    }
}

//Implementacao do algoritmo quicksort para ordenacao do indice
void quickSort(struct data* arr, int low, int high){
    if(low < high){
        int pvt = partition(arr, low, high);
        quickSort(arr, low, pvt - 1);
        quickSort(arr, pvt + 1, high);
    }
}

//Wrapper do quicksort
void indexSort(INDEX* i){
    quickSort(i->array, 0, i->nReg - 1);
}

//Printar para debug, printa num arquivo de log
void printArr(INDEX *i){
    FILE * fd = fopen("arr.log", "w");
    for(int j = 0; j < i->nReg - 1; j++){
        fprintf(fd, "index = %d / id = %d / off = %ld\n ",j, i->array[j].id, i->array[j].offset);
    }
    fclose(fd);
}

//Inicializacao da struct do indice
INDEX* indexInit(int base_len){
    INDEX *i = (INDEX*)malloc(sizeof(INDEX));
    i->nReg = 0;
    //Tamanho base do array eh passado como parametro
    //Permite evitar muitas realocacoes quando ja eh sabido o numero de registros
    i->arrLen = base_len; 
    i->status = '1';
    i->array = (struct data*)malloc(base_len * sizeof(struct data));
}
//Desalocacao de memoria do indice
void indexFree(INDEX** i){
    free((*i)->array);
    free(*i);
    *i = NULL;
}


//Cria o indice a partir do arquivo de dados
INDEX* makeIndex(FILE *fd, HEADER* h){
    //Alocacoes
    INDEX *i = indexInit(h->nReg);

    //Realiza o seek apenas se necessario
    int64_t offset = 25;
    if(ftell(fd) != offset) 
        fseek(fd, offset, SEEK_SET);
        
        
    //Iteracao pelo arquivo de dados
    while(offset < h->offset){
        PLAYER *p = playerFromBin(fd, NO_SEEK);//Recupera id e tamanho
        if(p->status == '0'){
            //Append na lista
            i->array[i->nReg].id = p->id;
            i->array[i->nReg].offset = offset;
            i->nReg++;
        }
        //Logica de realocacao
        if(i->nReg == i->arrLen){
            i->arrLen *= 2;
            i->array =(struct data*) realloc(i->array, i->arrLen * sizeof(struct data));
        }
        offset += p->tamanho;
        playerFree(&p);
    }
    //Ordena o indice uma vez na criacao
    indexSort(i);
    return i;
}

//Busca binaria que retorna o offset dado um id, -1 caso nao encontre
int64_t indexSearchAux(struct data *arr, int target, int lo, int hi){
    if(lo <= hi){
        struct data half = arr[(lo + hi)/2]; 
        if(half.id == target)
            return half.offset;
        if(half.id > target)
            return indexSearchAux(arr, target, lo, (lo + hi)/2);
        if(half.id < target)
            return indexSearchAux(arr, target, ((lo + hi)/2) + 1, hi);
    }
    return -1;
}
//Wrapper da busca binaria, retornando offset, -1 caso nao encontre
int64_t indexSearch(INDEX* index, int id){
    return indexSearchAux(index->array, id, 0, index->nReg - 1);
}


//Busca no indice que retorna a posicao no array, para insercao ou remocao, busca binaria
int indexSearchPositionAux(struct data *arr, int target, int lo, int hi){
    if(lo < hi){
        int half = (lo + hi)/2;
        if(arr[half].id <= target && arr[half + 1].id >= target) //Usa desigualdade para determinar a posicao a se inserir
            return (lo + hi)/2;
        if(arr[half].id > target)
            return indexSearchPositionAux(arr, target, lo, (lo + hi)/2);
        if(arr[half].id < target)
            return indexSearchPositionAux(arr, target, ((lo + hi)/2) + 1, hi);
    }
    return -1;
}
//Busca no indice que retorna a posicao para remccao, foi necessaria implementacao diferente da insercao, busca binaria
int indexSearchPositionRemoveAux(struct data *arr, int target, int lo, int hi){
    if(lo <= hi){
        int half = (lo + hi)/2; 
        if(arr[half].id == target)
            return half;
        if(arr[half].id > target)
            return indexSearchPositionRemoveAux(arr, target, lo, (lo + hi)/2);
        if(arr[half].id < target)
            return indexSearchPositionRemoveAux(arr, target, ((lo + hi)/2) + 1, hi);
    }
    return -1;
}
//Busca no indice que retorna a posicao para remccao, foi necessaria implementacao diferente da insercao, busca binaria
int indexSearchPositionRemove(INDEX* index, int id){
    return indexSearchPositionRemoveAux(index->array, id, 0, index->nReg -1);
}

//Busca no indice que retorna a posicao no array, para insercao ou remocao, busca binaria
int64_t indexSearchPosition(INDEX* index, int id){
    if(index->array[0].id >= id)
        return 0;
    if(index->array[index->nReg - 1].id <= id)
        return index->nReg - 1;
    return indexSearchPositionAux(index->array, id, 0, index->nReg - 1);
}
//Desloca o array para a direita, necessario para insercao
void shiftRight(INDEX* index, int pos){
    for(int i = index->nReg - 1; i >= pos; i--){
        index->array[i + 1] = index->array[i];
    }
}
//Insere ordenadamente no indice
void indexInsert(INDEX *index, int id, int offset){
    int indToInsert = indexSearchPosition(index, id) + 1;//Posicao para insercao
    shiftRight(index, indToInsert);//Deslocamento no array
    //Atualiza valores
    index->array[indToInsert].id = id; 
    index->array[indToInsert].offset = offset;
    index->nReg++;
    //Logica de realocacao
    if(index->nReg == index->arrLen){
        index->arrLen *= 2;
        index->array =(struct data*) realloc(index->array, index->arrLen * sizeof(struct data));
    }
}


//Funcoes de escrita do indice em arquivo de indice

//Escreve um registro do indice no arquivo
void writeIndexReg(struct data reg, FILE *fd, int64_t offset){
    //Constante NO_SEEK determina se eh necessario realizar o fseek ou se a escrita eh sequencial
    if(offset != NO_SEEK)
        fseek(fd, offset, SEEK_SET);
    fwrite(&(reg.id), 4, 1, fd);
    fwrite(&(reg.offset), 8, 1, fd);
}

//Escreve em disco todo o indice
//Abre e fecha o arquivo
void writeIndex(INDEX* index, char* filename){
    FILE *fd = fopen(filename, "wb");
    char tempByte = '0';
    fwrite(&tempByte, 1, 1, fd); //Sinaliza inconsitencia
    //Itera e escreve
    for(int offset = 1, i = 0; i < index->nReg; offset += sizeof(struct data), i++){
        writeIndexReg(index->array[i], fd, NO_SEEK);
    }
    fseek(fd, 0, SEEK_SET);
    tempByte = '1';
    fwrite(&tempByte, 1, 1, fd); //Marca como consistente
    fclose(fd);
}

//Funcoes de carregamento do indice

//Lida com inconsistencia do indice, atualmente vazia devido as especificacoes do trabalho
//mas pode ser responsavel pela reconstrucao do indice
INDEX *inconsistentIndexHandler(){
    printf("Falha no processamento do arquivo.\n");
    return NULL;
}
//Leitura de um registro em disco
void indexRegFromBin(FILE *fd, struct data *reg, int64_t offset){
    //Flag NO_SEEK determina se eh possivel evitar o fseek
    if(offset != NO_SEEK)
        fseek(fd, offset, SEEK_SET);
    fread(&(reg->id), 4, 1, fd);
    fread(&(reg->offset), 8, 1, fd);
}
//Carregamento do indice em disco para memora principal
INDEX* loadIndex(char *filename){
    FILE *fd = fopen(filename, "rb");
    INDEX *index = indexInit(100);
    char status;
    fread(&status, 1, 1 ,fd);
    if(status == '0') //Verifica consistencia
        return inconsistentIndexHandler();
    int64_t offset = 1;
    //Itera sobre o arquivo de indice e realiza a leitura
    while(!feof(fd)){
        indexRegFromBin(fd, &(index->array[index->nReg]), NO_SEEK);
        index->nReg++;
        offset += sizeof(struct data);
        //Logica de realocao
        if(index->nReg == index->arrLen){
            index->arrLen *= 2;
            index->array =(struct data*) realloc(index->array, index->arrLen * sizeof(struct data));
        }
    }
    fclose(fd);
    return index;
}

//Deslocamento do array para esquerda, necessario para remocao
void shiftLeft(INDEX* index, int position){
    for(int i = position; i < index->nReg - 1; i++)
        index->array[i] = index->array[i+1];
}
//Remove um id do indice
void indexRemove(INDEX* index, int id){
    int position;
    //busca da posicao
    if((position = indexSearchPositionRemove(index, id)) == -1)
        return;
    //deslocamento
    shiftLeft(index, position);
    index->nReg--;//Atualliza valores do indice
}
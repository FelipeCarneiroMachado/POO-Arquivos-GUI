#include "binFile.h"
#include "index.h"

void indexAtualizarOuInserir(INDEX* index, int id, int offset) {
    int indToUpdate = indexSearchPosition(index, id);
    if (indToUpdate != '$' && index->array[indToUpdate].id == id) {

        index->array[indToUpdate].offset = offset;
    } else {

        shiftRight(index, indToUpdate + 1);
        index->array[indToUpdate + 1].id = id;
        index->array[indToUpdate + 1].offset = offset;
        index->nReg++;
        if (index->nReg == index->arrLen) {
            index->arrLen *= 2;
            index->array = (struct data*) realloc(index->array, index->arrLen * sizeof(struct data));
        }
    }
}

void escreverRegistro(FILE *arquivoDados, PLAYER *newPlayer, long posicao, FILE *arquivoIndice) {
    fwrite(&newPlayer->id, sizeof(int), 1, arquivoDados);
    fwrite(&newPlayer->idade, sizeof(int), 1, arquivoDados);
    fwrite(newPlayer->nome, sizeof(char), newPlayer->nomeLen, arquivoDados);
    fwrite(newPlayer->pais, sizeof(char), newPlayer->paisLen, arquivoDados);
    fwrite(newPlayer->clube, sizeof(char), newPlayer->clubeLen, arquivoDados);

        // Insere ou atualiza o índice
    fseek(arquivoDados, posicao, SEEK_SET);
    indexAtualizarOuInserir(arquivoIndice, newPlayer->id, posicao);
}

void insertion(char *arqDados, INDEX *arqInd, int n) {
    FILE *arquivoDados;
    INDEX arquivoIndice = arqInd;
    arquivoDados = fopen(arqDados, "r+b");

    if (arquivoDados == NULL || arquivoIndice == NULL) {
        printf("Falha no processamento do arquivo.\n");
        return EXIT_FAILURE;
    }

    for (int i = 0; i < n; i++) {
        PLAYER *newPlayer;
        char str1[32], str2[32], str3[32];
        newPlayer = playerInit();
        newPlayer->nome, newPlayer->pais, newPlayer->clube

        scanf("%d, %d, %s, %s, %s", &newPlayer->id, &newPlayer->idade, str1, str2, str3);
        scan_quote_string(str1);
        scan_quote_string(str2);
        scan_quote_string(str3);
        newPlayer->nome = strdup(str1);
        newPlayer->pais = strdup(str2);
        newPlayer->clube = strdup(str3);
        newPlayer->nomeLen = strlen(newPlayer->nome) + 1; // +1 do caractere nulo
        newPlayer->paisLen = strlen(newPlayer->pais) + 1;
        newPlayer->clubeLen = strlen(newPlayer->clube) + 1;

        long posicao = ftell(arquivoDados); // Salva a posição atual no arquivo de dados
        escreverRegistro(arquivoDados, newPlayer, posicao, arquivoIndice);
        free(newPlayer);

    }

    fclose(arquivoDados);
}

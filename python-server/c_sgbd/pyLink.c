#include"pyLink.h"



void sscan_quote_string(char *str, char *src) {

	/*
	*	Use essa função para ler um campo string delimitado entre aspas (").
	*	Chame ela na hora que for ler tal campo. Por exemplo:
	*
	*	A entrada está da seguinte forma:
	*		nomeDoCampo "MARIA DA SILVA"
	*
	*	Para ler isso para as strings já alocadas str1 e str2 do seu programa, você faz:
	*		scanf("%s", str1); // Vai salvar nomeDoCampo em str1
	*		scan_quote_string(str2); // Vai salvar MARIA DA SILVA em str2 (sem as aspas)
	*
	*/

	char R;

	while((R = getchar()) != '\0' && !isspace(R)); // ignorar espaços, \r, \n...

	if(R == 'N' || R == 'n') { // campo NULO
		getchar(); getchar(); getchar(); // ignorar o "ULO" de NULO.
		strcpy(str, ""); // copia string vazia
	} else if(R == '\"') {
		if(sscanf(src, "%[^\"]", str) != 1) { // ler até o fechamento das aspas
			strcpy(str, "");
		}
		getchar(); // ignorar aspas fechando
	} else if(R != '\0'){ // vc tá tentando ler uma string que não tá entre aspas! Fazer leitura normal %s então, pois deve ser algum inteiro ou algo assim...
		str[0] = R;
		sscanf(src, "%s", &str[1]);
	} else { // EOF
		strcpy(str, "");
	}
}


PLAYER* playerFromString(char *src){
    PLAYER *p = playerInit();
    char buffer[256];
    sscanf(src, "%s", buffer);
    src += strlen(buffer) + 1;
    p->id = atoi(buffer);
    sscan_quote_string(buffer, src);
    src += strlen(buffer) + 1;
    if(buffer[0] != '\0'){
        p->idade = atoi(buffer);
        src += strlen(buffer) + 1;
    }
    else
        src += 5;
    sscan_quote_string(buffer, src);
    if(buffer[0] != '\0'){
        playerSetNome(p,buffer);
        src += strlen(buffer) + 1;
    }
    sscan_quote_string(buffer, src);
    if(buffer[0] != '\0'){
        playerSetPais(p,buffer);
        src += strlen(buffer) + 1;
    }
    sscan_quote_string(buffer, src);
    if(buffer[0] != '\0')
        playerSetClube(p,buffer);
    playerTamanho(p, true);
    return p;
}

bool loadCsv(char srcPath[],char localName[]){
    printf("1");
    ///printf(localName);
    FILE *srcCsv, *destBin;
    srcCsv = fopen(srcPath, "r");
    destBin = fopen(localName, "wb");
    createTable(srcCsv, destBin);
    return true;
}



bool buildIndex( char *indexName, char *binName){
    FILE *bin = fopen(binName, "rb");
    HEADER *h = extraiHeader(bin);
    createIndex(bin, h, indexName);
    fclose(bin);
}

bool insertDb( char *player, char* binName, char *indexName){
    PLAYER *p = parseLine(player);
    FILE *bin;
    bin = fopen(binName, "r+b");
    HEADER *header = extraiHeader(bin);
    INDEX *ind = loadIndex(indexName);
    insert(bin, header, ind, p);
    writeIndex(ind, indexName);
    updateHeader(bin, header);
    fclose(bin);
    return true;
}
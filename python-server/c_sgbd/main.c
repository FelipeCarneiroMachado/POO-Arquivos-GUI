#include"binFile.h"
#include"interface.h"	
#include"utils.h"
//Link do video: https://drive.google.com/file/d/1AyvIsoHaJeqoMSfXkC6RaOFIbvVBA3Zb/view?usp=sharing
// Realizado pela dupla (Grupo 12):
//     Felipe Carneiro Machado - NUSP: 14569373
//     Matheus Araujo Pinheiro - NUSP: 14676810
/*
================================================
Arquivo main do projeto
================================================
*/
int main(){
	FILE *bin = NULL, *csv = NULL;
	HEADER *header = NULL;
	INDEX* index = NULL;
	char commandBuffer[1024];
	char *src, *dest, *intBuffer, *indexName, **fields, **values;
	int nOfQueries, nOfFields;
	while (fgets(commandBuffer,1024, stdin) != NULL){
		switch(commandBuffer[0]){
			case '1':	//Criacao de tabela a partir de .csv (funcionalidade 1)	
				strtok(commandBuffer, " ");
				src = strtok(NULL, " ");
				dest = strtok(NULL, "\n");
				createTable(src, dest);
				break;
			case '2':    //retornar todos os registros (funcionalidade 2)
				strtok(commandBuffer, " ");
				src = strtok(NULL, "\n");
				if((bin = fopen(src, "rb")) == NULL ){
					printf("Falha no processamento do arquivo.\n");
					break;
				}
				header = extraiHeader(bin);
				if(header->status == '0'){
					printf("Falha no processamento do arquivo.\n");
					fclose(bin);
					break;
				}
				selectAll(bin, header);
				fclose(bin);
				free(header);
				break;
			case '3':    //retornar condicionalmente os registros (funcionalidade 3)
				strtok(commandBuffer, " ");
				src = strtok(NULL, " ");
				if((bin = fopen(src, "rb")) == NULL ){
					printf("Falha no processamento do arquivo.\n");
					break;
				}
				header = extraiHeader(bin);
				if(header->status == '0'){
					printf("Falha no processamento do arquivo.\n");
					fclose(bin);
					break;
				}
				indexName = strtok(NULL, " ");
				index = createIndex(bin, header, indexName);
				nOfQueries = atoi(strtok(NULL, "\n"));
				fields = stringArray(5, 32);
				values = stringArray(5, 32);
				for(int i = 0; i < nOfQueries; i++){
					//printf("Busca %d\n\n", i + 1);
					scanf("%d", &nOfFields);
					for(int j = 0; j < nOfFields; j++){
						scanf("%s", fields[j]);
						if(strcmp(fields[j], "nomeJogador") == 0 || strcmp(fields[j], "nomeClube") == 0 || strcmp(fields[j], "nacionalidade") == 0)
							scan_quote_string(values[j]);
						else
							scanf("%s", values[j]);
					}
					selectWhere(bin, header, index, nOfFields, fields, values);
				}
				freeStringArray(&fields, 5);
				freeStringArray(&values, 5);
				fclose(bin);
				free(header);
				break;
			case '4':
				strtok(commandBuffer, " ");
				src = strtok(NULL, " ");
				indexName = strtok(NULL, "\n");
				if((bin = fopen(src, "rb")) == NULL ){
					printf("Falha no processamento do arquivo.\n");
					break;
				}
				header = extraiHeader(bin);
				if(header->status == '0'){
					printf("Falha no processamento do arquivo.\n");
					fclose(bin);
					free(header);
					break;
				}
				index = createIndex(bin, header, indexName);
				// binarioNaTela(indexName);
				fclose(bin);
				free(header);
				indexFree(&index);
				break;
			case '5':
				strtok(commandBuffer, " ");
				src = strtok(NULL, " ");
				if((bin = fopen(src, "r+b")) == NULL ){
					printf("Falha no processamento do arquivo.\n");
					break;
				}
				header = extraiHeader(bin);
				if(header->status == '0'){
					printf("Falha no processamento do arquivo.\n");
					fclose(bin);
					free(header);
					break;
				}
				indexName = strtok(NULL, " ");
				index = createIndex(bin, header, indexName);
				nOfQueries = atoi(strtok(NULL, "\n"));
				fields = stringArray(5, 32);
				values = stringArray(5, 32);
				for(int i = 0; i < nOfQueries; i++){
					scanf("%d", &nOfFields);
					for(int j = 0; j < nOfFields; j++){
						scanf("%s", fields[j]);
						if(strcmp(fields[j], "nomeJogador") == 0 || strcmp(fields[j], "nomeClube") == 0 || strcmp(fields[j], "nacionalidade") == 0)
							scan_quote_string(values[j]);
						else
							scanf("%s", values[j]);
					}
					delete(bin, header, index, nOfFields, fields, values);
				}
				updateHeader(bin, header);
				writeIndex(index, indexName);
				free(header);
				fclose(bin);
				binarioNaTela(src);
				binarioNaTela(indexName);
				indexFree(&index);
				freeStringArray(&fields, 5);
				freeStringArray(&values, 5);
				break;
			case '6':
				strtok(commandBuffer, " ");
				src = strtok(NULL, " ");
				if((bin = fopen(src, "r+b")) == NULL ){
					printf("Falha no processamento do arquivo.\n");
					break;
				}
				header = extraiHeader(bin);
				if(header->status == '0'){
					printf("Falha no processamento do arquivo.\n");
					fclose(bin);
					free(header);
					break;
				}
				indexName = strtok(NULL, " ");
				index = loadIndex(indexName);
				//index = createIndex(bin, header, indexName);
				nOfQueries = atoi(strtok(NULL, "\n"));
				for(int i = 0; i < nOfQueries; i++){
					PLAYER *p = playerInput(commandBuffer);
					playerPrint(p);
					insert(bin, header, index, p);
					playerFree(&p);
				}
				updateHeader(bin, header);
				writeIndex(index, indexName);
				fclose(bin);
				free(header);
				indexFree(&index);
			default:
				break;
			
		}
	}
	
}
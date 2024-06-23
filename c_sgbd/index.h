#ifndef INDEX_H
    #define INDEX_H
    #include<stdbool.h>
    #include<stdlib.h>
    #include<string.h>
    #include<stdio.h>
    #include<stdint.h>
    #include"player.h"
    #include"utils.h"
    #include"binFile.h"
    typedef struct index_{
        int8_t status;
        int32_t nReg;
        int32_t arrLen;
        struct data{
            int32_t id;
            int64_t offset; 
        }*array;
    }INDEX;
    INDEX* makeIndex(FILE *fd, HEADER* h);
    INDEX* loadIndex(char *filename);
    void printArr(INDEX *i);
    int64_t indexSearch(INDEX* index, int id);
    void indexInsert(INDEX *index, int id, int offset);
    void indexFree(INDEX** i);
    void writeIndex(INDEX* index, char* filename);
    void indexRemove(INDEX* index, int id);

#endif
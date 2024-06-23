#ifndef PY_H_
    #define PY_H_
    #include<stdbool.h>
    #include<stdlib.h>
    #include<string.h>
    #include<stdio.h>
    #include<stdint.h>
    #include<string.h>
    #include"interface.h"
    extern bool loadCsv(char srcPath[], char localName[]);
    extern bool insertDb( char *player,  char* binName, char *indexName);
    extern bool buildIndex( char *indexName, char *binName);
#endif
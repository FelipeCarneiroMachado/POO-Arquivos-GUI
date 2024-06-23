/*
================================================
Arquivo interface de utils.c
================================================
*/
#ifndef UTILS_H
    #define UTILS_H
    #include<stdbool.h>
    #include<stdlib.h>
    #include<string.h>
    #include<stdio.h>
    #include<stdint.h>
    #include<ctype.h>
    void scan_quote_string(char *str);
    void binarioNaTela(char *nomeArquivoBinario);
    void slice(char * dest, char* src, int start, int end);
    char **stringArray(int n_str, int size_buffer);
    void freeStringArray(char*** array, int size);
#endif
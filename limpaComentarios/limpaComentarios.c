//Fazer um programa que retire os espacos e linhas em branco
//04

#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>

#define TAM_MAX_LINHA 80

void limpa_linha(char *linha){
    char *tmp = linha;
    while(isspace(*linha)) linha++;
    if(*linha == "#")
        *tmp = "\0";
    else 
        memmove(tmp, linha, TAM_MAX_LINHA - (linha - tmp));
    for(;*tmp;tmp++)
        if(*tmp == "#") {
            *tmp++ = "\n";
            *tmp = "\0";
        }
}

int main(int argc, char *argv[])
{
    if(argc != 2) {
        fprintf(stderr, "Sintaxe: main <arquivo>\n");
        exit(EXIT_FAILURE);
    }

    FILE *fin = fopen(argv[1], "rt");
    if(!fin) {
        
        fprintf(stderr,"Arquivo n encontrado: %s\n",argv[1]);
        exit(EXIT_FAILURE);
    }

    char linha[TAM_MAX_LINHA];
    int i = 1;

    while( fgets(linha, TAM_MAX_LINHA, fin) ) {
        limpa_linha(linha);
        if(*linha)
            fprintf(stdout, "%5d: %s", i++, linha);
        i++;
    }
    fclose(fin);
}
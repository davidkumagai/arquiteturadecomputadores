//Fazer um programa que leia um operando
//03

#include <stdio.h>
#include <stdlib.h>

int main(int argc, char *argv[])
{
    char *line = NULL;

    size_t len = 16;
    ssize_t read;

    while ((read = getline(&line, &len, stdin)) != -1){

        if (read > 0){
            
        }

    }
    free (line);

    return 0;
}
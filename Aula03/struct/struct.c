#include <stdio.h>
#include <stdlib.h>

struct neander {
    int linha;
    int label;
    int instrucao;
    int operando;
    int posmemoria;
};

typedef neander_t struct neander *;
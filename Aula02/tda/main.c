#include <stdio.h>
#include <stdlib.h>

struct lista {
    int info;
    int lista *p; //ponteiro para o proximo no
};

typedef lista_t struct lista *; //fazendo com que o lista_t seja da struct lista

void panico(const char *msg){
    fprintf(stderr, msg);
    exit(EXIT_FAILURE);
}

lista_t lista_criar(){
    lista_t cabeca = malloc( sizeof(struct lista) );
    if( !cabeca ){
        panico("Falta de memoria");
    }
    cabeca->p = NULL;
    return cabeca;
}

lista_t lista_incluir(lista_t lista, int info){
    if(!lista)
        return NULL;
    lista_t no = malloc( sizeof(struct lista) );
    if (!no){
        panico("Falta de memoria");
    }
    no->info = info;
    no->p = NULL;
    lista_t no_atual = lista;
    while(no_atual->p != NULL)
        no_atual = no_atual->p;
    no_atual->p = no;
    return no;
}

void lista_imprimir(lista_t lista){
    if (!lista)
        return;

    lista_t no_atual = lista->p;
    while(no_atual) {
        printf("%d ", no_atual->info);
        no_atual = no_atual->p;
    }
    printf("\n");
}

void lista_destruir(lista_t lista){
    if(!lista)
        return;
    lista_t no, no_atual = lista->p;
    while(no_atual){
        no = no_atual->p;
        free(no_atual);
        no_atual=no;
    }
    free(lista);
}


int main(void){
    lista_t lst = lista_criar(), l;

    l = lista_incluir(lst, 10);
    l = lista_incluir(l, 15);
    l = lista_incluir(l, 20);
    l = lista_incluir(l, 25);
    l = lista_incluir(l, 30);
    l = lista_incluir(l, 35);
    l = lista_incluir(l, 40);
    l = lista_incluir(l, 45);
    lista_imprimir(lst);
    lista_destruir(lst);
}
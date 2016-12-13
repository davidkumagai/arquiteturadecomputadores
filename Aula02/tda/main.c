#include <stdio.h>
#include <stdlib.h>

struct lista {
    int info;
    int lista *p; //ponteiro para o proximo no
};

typedef lista_t struct lista *; //fazendo com que o lista_t seja da struct lista

lista_t lista_criar(){

    lista_t *p = malloc(sizeof(lista_t));

    return p;
}

lista_t lista_incluir(lista_t l, int i){
    lista_t *p = NULL;
    lista_t *a = lista_t<-p;

    if(lista_t == NULL){
        lista_t *p -> *a;

    lista_t *a -> lista_t *p;
    lista_t info <- lista *p;

    }
        
    return NULL;
}

void lista_imprimir(lista_t l){
    return;
}

void lista_destruir(lista_t l){
    return;
}


int main(void){
    lista_t lst = lista_criar();
    lista_t l = NULL;

    l = lista_incluir(lst, 10);
    l = lista_incluir(l, 15);
    l = lista_incluir(l, 20);
    lista_imprimir(lst);
    lista_destruir(lst);






}
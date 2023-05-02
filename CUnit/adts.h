#include <stdio.h>  
#include <string.h>
#include <stdlib.h>
#include <stdbool.h>

// defines node structure
// used in the priority queue
// and in the huffman tree 
typedef struct NO {
    void *item;
    int frequencia;
    struct NO *prox;
    struct NO *esq;
    struct NO *dir;
} NO;

// priority queue structure
typedef struct FILA {
    struct NO *cabeca;
} FILA;

// hash table element structure
typedef struct ELEMENTO {
    char caminho[31]; // Max path length
    long long int frequencia; // Frequency
} ELEMENTO;

// hash table structure
typedef struct HT {
    ELEMENTO* tabela[256]; // Max ASCII value
} HT;

NO* criar_no(void* item, int frequencia) //utilizado na criacao da arvore de huffman.
{
    NO *novo_no = (NO*) malloc(sizeof(NO));
    novo_no->item = item;
    novo_no->frequencia = frequencia;
    novo_no->prox = NULL;
    novo_no->esq = NULL;
    novo_no->dir = NULL;
    return novo_no;
}

FILA* criar_fila()
{
    FILA *nova_fila = (FILA*) malloc(sizeof(FILA));
    nova_fila->cabeca = NULL;
    return nova_fila;
}


void enfileirar(FILA *fila, NO *no)
{
    if((fila->cabeca == NULL) || (no->frequencia <= (fila->cabeca->frequencia)))
    {
        no->prox = fila->cabeca;
        fila->cabeca = no;
    }
    else
    {
        NO *auxiliar = fila->cabeca;
        while ((auxiliar->prox != NULL) && (auxiliar->prox->frequencia < no->frequencia))
        {
            auxiliar = auxiliar->prox;
        }
        no->prox = auxiliar->prox;
        auxiliar->prox = no;
    }
}

NO* desenfileirar(FILA *fila)
{
    if (fila->cabeca == NULL)
        return NULL;

    NO *auxiliar = fila->cabeca;
    fila->cabeca = fila->cabeca->prox;
    auxiliar->prox = NULL;
    return auxiliar;
}

ELEMENTO* criar_elemento()
{
    ELEMENTO *novo_elemento = (ELEMENTO*) malloc(sizeof(ELEMENTO));
    novo_elemento->frequencia = 0;
    return novo_elemento;
}

HT* criar_hash_table()
{
    HT *nova_ht = (HT*) malloc(sizeof(HT)); //Alocacao Dinâmica.
    int i;
    for(i = 0; i < 256; i++)
    {
        nova_ht->tabela[i] = criar_elemento();
    }
    return nova_ht;
}


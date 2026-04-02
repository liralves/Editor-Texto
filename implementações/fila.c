#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "fila.h"

/* ==========================================================
 * Implementacao do TAD - Fila Circular com Vetor Dinamico
 * ========================================================== */

/* Inicializa a fila com capacidade inicial via malloc */
void fila_inicializar(Fila **f) {
    *f = (Fila *)malloc(sizeof(Fila));
    if (*f == NULL) {
        fprintf(stderr, "[ERRO] Falha ao alocar memoria para a Fila.\n");
        exit(EXIT_FAILURE);
    }

    /* Aloca o vetor de ponteiros para strings */
    (*f)->dados = (char **)malloc(FILA_CAPACIDADE_INICIAL * sizeof(char *));
    if ((*f)->dados == NULL) {
        fprintf(stderr, "[ERRO] Falha ao alocar vetor da Fila.\n");
        free(*f);
        exit(EXIT_FAILURE);
    }

    (*f)->inicio    = 0;
    (*f)->fim       = 0;
    (*f)->tamanho   = 0;
    (*f)->capacidade = FILA_CAPACIDADE_INICIAL;
}

/* Retorna 1 se a fila estiver vazia */
int fila_vazia(Fila *f) {
    return (f == NULL || f->tamanho == 0);
}

/* Retorna 1 se a fila estiver cheia */
int fila_cheia(Fila *f) {
    return (f != NULL && f->tamanho == f->capacidade);
}

/* Redimensiona o vetor da fila (dobra a capacidade via realloc) */
static void fila_redimensionar(Fila **f) {
    int nova_capacidade = (*f)->capacidade * 2;

    /* Aloca novo vetor temporario para reorganizar os elementos */
    char **novo_dados = (char **)malloc(nova_capacidade * sizeof(char *));
    if (novo_dados == NULL) {
        fprintf(stderr, "[ERRO] Falha ao redimensionar a Fila.\n");
        return;
    }

    /*
     * Reorganiza os elementos em ordem linear no novo vetor.
     * Necessario porque a fila circular pode ter "volta" no indice.
     * Usa aritmetica de ponteiros para percorrer o vetor antigo.
     */
    for (int i = 0; i < (*f)->tamanho; i++) {
        int idx = ((*f)->inicio + i) % (*f)->capacidade;
        *(novo_dados + i) = *((*f)->dados + idx); /* Aritmetica de ponteiros */
    }

    free((*f)->dados);            /* Libera o vetor antigo */
    (*f)->dados      = novo_dados;
    (*f)->inicio     = 0;
    (*f)->fim        = (*f)->tamanho;
    (*f)->capacidade = nova_capacidade;

    printf("[Fila] Redimensionada para capacidade %d.\n", nova_capacidade);
}

/* Insere uma frase no fim da fila */
void fila_enqueue(Fila **f, const char *frase) {
    /* Redimensiona se necessario */
    if (fila_cheia(*f)) {
        fila_redimensionar(f);
    }

    /* Aloca e copia a string */
    (*f)->dados[(*f)->fim] = (char *)malloc((strlen(frase) + 1) * sizeof(char));
    if ((*f)->dados[(*f)->fim] == NULL) {
        fprintf(stderr, "[ERRO] Falha ao alocar string na Fila.\n");
        return;
    }
    strcpy((*f)->dados[(*f)->fim], frase);

    /* Avanca o indice fim de forma circular */
    (*f)->fim = ((*f)->fim + 1) % (*f)->capacidade;
    (*f)->tamanho++;

    printf("[Fila] Frase enfileirada. Total: %d/%d.\n", (*f)->tamanho, (*f)->capacidade);
}

/* Remove e retorna a frase do inicio da fila (caller deve liberar) */
char *fila_dequeue(Fila **f) {
    if (fila_vazia(*f)) {
        printf("[Fila] A fila de salvamento esta vazia.\n");
        return NULL;
    }

    char *frase = (*f)->dados[(*f)->inicio]; /* Salva ponteiro para retornar */
    (*f)->dados[(*f)->inicio] = NULL;

    /* Avanca o indice inicio de forma circular */
    (*f)->inicio = ((*f)->inicio + 1) % (*f)->capacidade;
    (*f)->tamanho--;

    return frase;
}

/* Exibe todos os elementos da fila */
void fila_exibir(Fila *f) {
    if (fila_vazia(f)) {
        printf("[Fila] A fila de salvamento esta vazia.\n");
        return;
    }

    printf("\n--- Fila de Salvamento (Inicio -> Fim) ---\n");
    for (int i = 0; i < f->tamanho; i++) {
        int idx = (f->inicio + i) % f->capacidade;
        printf("  [%d] %s\n", i + 1, f->dados[idx]);
    }
    printf("------------------------------------------\n");
}

/* Libera toda a memoria da fila */
void fila_destruir(Fila **f) {
    if (*f == NULL) return;

    /* Libera cada string ainda na fila */
    for (int i = 0; i < (*f)->tamanho; i++) {
        int idx = ((*f)->inicio + i) % (*f)->capacidade;
        free((*f)->dados[idx]);
    }

    free((*f)->dados); /* Libera o vetor de ponteiros */
    free(*f);           /* Libera a estrutura da fila  */
    *f = NULL;
    printf("[Fila] Memoria liberada com sucesso.\n");
}
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "lista.h"

/* ==========================================================
 * Implementacao do TAD - Lista Duplamente Encadeada Ordenada
 * ========================================================== */

/* Inicializa a lista */
void lista_inicializar(ListaDupla **l) {
    *l = (ListaDupla *)malloc(sizeof(ListaDupla));
    if (*l == NULL) {
        fprintf(stderr, "[ERRO] Falha ao alocar memoria para a Lista.\n");
        exit(EXIT_FAILURE);
    }
    (*l)->cabeca  = NULL;
    (*l)->cauda   = NULL;
    (*l)->tamanho = 0;
}

/* Retorna 1 se a lista estiver vazia */
int lista_vazia(ListaDupla *l) {
    return (l == NULL || l->cabeca == NULL);
}

/* Insere uma palavra na lista de forma ordenada (alfabetica) */
void lista_inserir_ordenado(ListaDupla **l, const char *palavra) {
    /* Verifica se a palavra ja existe */
    if (lista_buscar(*l, palavra)) {
        printf("[Lista] A palavra '%s' ja existe no dicionario.\n", palavra);
        return;
    }

    /* Cria o novo no */
    NoDuplo *novo = (NoDuplo *)malloc(sizeof(NoDuplo));
    if (novo == NULL) {
        fprintf(stderr, "[ERRO] Falha ao alocar no para a Lista.\n");
        return;
    }

    novo->palavra = (char *)malloc((strlen(palavra) + 1) * sizeof(char));
    if (novo->palavra == NULL) {
        fprintf(stderr, "[ERRO] Falha ao alocar string na Lista.\n");
        free(novo);
        return;
    }
    strcpy(novo->palavra, palavra);
    novo->proximo  = NULL;
    novo->anterior = NULL;

    /* --- Caso 1: Lista vazia --- */
    if (lista_vazia(*l)) {
        (*l)->cabeca = novo;
        (*l)->cauda  = novo;
        (*l)->tamanho++;
        printf("[Lista] Palavra '%s' inserida. Total: %d.\n", palavra, (*l)->tamanho);
        return;
    }

    /* --- Caso 2: Insere no inicio (menor que a cabeca) --- */
    if (strcmp(palavra, (*l)->cabeca->palavra) < 0) {
        novo->proximo = (*l)->cabeca;
        (*l)->cabeca->anterior = novo;
        (*l)->cabeca = novo;
        (*l)->tamanho++;
        printf("[Lista] Palavra '%s' inserida. Total: %d.\n", palavra, (*l)->tamanho);
        return;
    }

    /* --- Caso 3: Encontra a posicao correta no meio ou fim --- */
    NoDuplo *atual = (*l)->cabeca;
    while (atual->proximo != NULL && strcmp(palavra, atual->proximo->palavra) > 0) {
        atual = atual->proximo;
    }

    /* Encadeia o novo no apos 'atual' */
    novo->proximo  = atual->proximo;
    novo->anterior = atual;

    if (atual->proximo != NULL) {
        atual->proximo->anterior = novo; /* Atualiza anterior do no seguinte */
    } else {
        (*l)->cauda = novo; /* Novo no e o ultimo */
    }

    atual->proximo = novo;
    (*l)->tamanho++;

    printf("[Lista] Palavra '%s' inserida. Total: %d.\n", palavra, (*l)->tamanho);
}

/* Remove uma palavra especifica da lista */
int lista_remover(ListaDupla **l, const char *palavra) {
    if (lista_vazia(*l)) {
        printf("[Lista] A lista esta vazia.\n");
        return 0;
    }

    /* Busca o no a ser removido */
    NoDuplo *atual = (*l)->cabeca;
    while (atual != NULL && strcmp(atual->palavra, palavra) != 0) {
        atual = atual->proximo;
    }

    if (atual == NULL) {
        printf("[Lista] Palavra '%s' nao encontrada.\n", palavra);
        return 0;
    }

    /* Ajusta os ponteiros dos nos vizinhos */
    if (atual->anterior != NULL) {
        atual->anterior->proximo = atual->proximo;
    } else {
        (*l)->cabeca = atual->proximo; /* Removendo a cabeca */
    }

    if (atual->proximo != NULL) {
        atual->proximo->anterior = atual->anterior;
    } else {
        (*l)->cauda = atual->anterior; /* Removendo a cauda */
    }

    free(atual->palavra);
    free(atual);
    (*l)->tamanho--;

    printf("[Lista] Palavra '%s' removida. Total: %d.\n", palavra, (*l)->tamanho);
    return 1;
}

/* Busca uma palavra na lista */
int lista_buscar(ListaDupla *l, const char *palavra) {
    if (lista_vazia(l)) return 0;

    NoDuplo *atual = l->cabeca;
    while (atual != NULL) {
        int cmp = strcmp(atual->palavra, palavra);
        if (cmp == 0)  return 1; /* Encontrou */
        if (cmp > 0)   break;    /* Passou da posicao (lista ordenada) */
        atual = atual->proximo;
    }
    return 0;
}

/* Exibe a lista do inicio para o fim */
void lista_exibir_frente(ListaDupla *l) {
    if (lista_vazia(l)) {
        printf("[Lista] O dicionario esta vazio.\n");
        return;
    }

    printf("\n--- Dicionario (A -> Z) ---\n");
    NoDuplo *atual = l->cabeca;
    int i = 1;
    while (atual != NULL) {
        printf("  [%d] %s\n", i++, atual->palavra);
        atual = atual->proximo;
    }
    printf("---------------------------\n");
}

/* Exibe a lista do fim para o inicio */
void lista_exibir_reverso(ListaDupla *l) {
    if (lista_vazia(l)) {
        printf("[Lista] O dicionario esta vazio.\n");
        return;
    }

    printf("\n--- Dicionario (Z -> A) ---\n");
    NoDuplo *atual = l->cauda;
    int i = l->tamanho;
    while (atual != NULL) {
        printf("  [%d] %s\n", i--, atual->palavra);
        atual = atual->anterior;
    }
    printf("---------------------------\n");
}

/* Libera toda a memoria da lista */
void lista_destruir(ListaDupla **l) {
    if (*l == NULL) return;

    NoDuplo *atual = (*l)->cabeca;
    while (atual != NULL) {
        NoDuplo *proximo = atual->proximo;
        free(atual->palavra);
        free(atual);
        atual = proximo;
    }

    free(*l);
    *l = NULL;
    printf("[Lista] Memoria liberada com sucesso.\n");
}
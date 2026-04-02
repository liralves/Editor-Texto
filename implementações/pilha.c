#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "pilha.h"

/* ==========================================================
 * Implementacao do TAD - Pilha Dinamicamente Encadeada
 * ========================================================== */

/* Inicializa a pilha alocando sua estrutura */
void pilha_inicializar(Pilha **p) {
    *p = (Pilha *)malloc(sizeof(Pilha));
    if (*p == NULL) {
        fprintf(stderr, "[ERRO] Falha ao alocar memoria para a Pilha.\n");
        exit(EXIT_FAILURE);
    }
    (*p)->topo = NULL;
    (*p)->tamanho = 0;
}

/* Retorna 1 se a pilha estiver vazia */
int pilha_vazia(Pilha *p) {
    return (p == NULL || p->topo == NULL);
}

/* Insere uma nova frase no topo da pilha */
void pilha_push(Pilha **p, const char *frase) {
    /* Aloca o novo no */
    No *novo = (No *)malloc(sizeof(No));
    if (novo == NULL) {
        fprintf(stderr, "[ERRO] Falha ao alocar no para a Pilha.\n");
        return;
    }

    /* Copia a frase para o no (alocacao dinamica da string) */
    novo->frase = (char *)malloc((strlen(frase) + 1) * sizeof(char));
    if (novo->frase == NULL) {
        fprintf(stderr, "[ERRO] Falha ao alocar string na Pilha.\n");
        free(novo);
        return;
    }
    strcpy(novo->frase, frase);

    /* Encadeia o novo no no topo */
    novo->proximo = (*p)->topo;
    (*p)->topo = novo;
    (*p)->tamanho++;

    printf("[Pilha] Frase inserida no topo. Total: %d frase(s).\n", (*p)->tamanho);
}

/* Remove e retorna a frase do topo (caller deve liberar a string) */
char *pilha_pop(Pilha **p) {
    if (pilha_vazia(*p)) {
        printf("[Pilha] A pilha esta vazia. Nada para desfazer.\n");
        return NULL;
    }

    No *removido = (*p)->topo;
    char *frase = removido->frase; /* Salva o ponteiro da string para retornar */

    (*p)->topo = removido->proximo;
    (*p)->tamanho--;

    free(removido); /* Libera apenas o no; a string e responsabilidade do caller */
    return frase;
}

/* Retorna a frase do topo sem remover */
const char *pilha_peek(Pilha *p) {
    if (pilha_vazia(p)) {
        return NULL;
    }
    return p->topo->frase;
}

/* Exibe todas as frases da pilha (topo -> base) */
void pilha_exibir(Pilha *p) {
    if (pilha_vazia(p)) {
        printf("[Pilha] A pilha esta vazia.\n");
        return;
    }

    printf("\n--- Pilha (Topo -> Base) ---\n");
    No *atual = p->topo;
    int i = 1;
    while (atual != NULL) {
        printf("  [%d] %s\n", i++, atual->frase);
        atual = atual->proximo;
    }
    printf("----------------------------\n");
}

/* Libera toda a memoria da pilha */
void pilha_destruir(Pilha **p) {
    if (*p == NULL) return;

    No *atual = (*p)->topo;
    while (atual != NULL) {
        No *proximo = atual->proximo;
        free(atual->frase); /* Libera a string */
        free(atual);         /* Libera o no     */
        atual = proximo;
    }

    free(*p);   /* Libera a estrutura da pilha */
    *p = NULL;
    printf("[Pilha] Memoria liberada com sucesso.\n");
}
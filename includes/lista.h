#ifndef LISTA_H
#define LISTA_H

/* ==========================================================
 * TAD - Lista Duplamente Encadeada Ordenada
 * Dicionario de palavras-chave inseridas pelo usuario.
 * Permite travessia do inicio ao fim e do fim ao inicio.
 * ========================================================== */

/* Estrutura de um no da lista dupla */
typedef struct NoDuplo {
    char *palavra;          /* Palavra-chave armazenada (alocada dinamicamente) */
    struct NoDuplo *proximo; /* Ponteiro para o proximo no                       */
    struct NoDuplo *anterior; /* Ponteiro para o no anterior                     */
} NoDuplo;

/* Estrutura da lista duplamente encadeada */
typedef struct {
    NoDuplo *cabeca;  /* Primeiro no da lista */
    NoDuplo *cauda;   /* Ultimo no da lista   */
    int tamanho;       /* Quantidade de nos    */
} ListaDupla;

/* --- Funcoes do TAD Lista Dupla --- */

/* Inicializa a lista */
void lista_inicializar(ListaDupla **l);

/* Retorna 1 se a lista estiver vazia, 0 caso contrario */
int lista_vazia(ListaDupla *l);

/* Insere uma palavra na lista de forma ordenada (alfabetica) */
void lista_inserir_ordenado(ListaDupla **l, const char *palavra);

/* Remove uma palavra especifica da lista (retorna 1 se removeu, 0 se nao encontrou) */
int lista_remover(ListaDupla **l, const char *palavra);

/* Busca uma palavra na lista (retorna 1 se encontrou, 0 caso contrario) */
int lista_buscar(ListaDupla *l, const char *palavra);

/* Exibe a lista do inicio para o fim */
void lista_exibir_frente(ListaDupla *l);

/* Exibe a lista do fim para o inicio */
void lista_exibir_reverso(ListaDupla *l);

/* Libera toda a memoria da lista */
void lista_destruir(ListaDupla **l);

#endif /* LISTA_H */
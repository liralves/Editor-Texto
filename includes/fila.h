#ifndef FILA_H
#define FILA_H

/* ==========================================================
 * TAD - Fila Circular com Vetor Dinamico
 * Buffer de salvamento: frases aguardam processamento/escrita.
 * Usa malloc para alocacao inicial e realloc para expansao.
 * ========================================================== */

#define FILA_CAPACIDADE_INICIAL 4 /* Capacidade inicial do vetor */

/* Estrutura da fila circular */
typedef struct {
    char **dados;    /* Vetor dinamico de ponteiros para strings */
    int inicio;      /* Indice do primeiro elemento              */
    int fim;         /* Indice do proximo espaco livre           */
    int tamanho;     /* Quantidade atual de elementos            */
    int capacidade;  /* Capacidade total do vetor                */
} Fila;

/* --- Funcoes do TAD Fila --- */

/* Inicializa a fila com capacidade inicial */
void fila_inicializar(Fila **f);

/* Retorna 1 se a fila estiver vazia, 0 caso contrario */
int fila_vazia(Fila *f);

/* Retorna 1 se a fila estiver cheia, 0 caso contrario */
int fila_cheia(Fila *f);

/* Insere uma frase no fim da fila (redimensiona se necessario) */
void fila_enqueue(Fila **f, const char *frase);

/* Remove e retorna a frase do inicio da fila (caller deve liberar) */
char *fila_dequeue(Fila **f);

/* Exibe todos os elementos da fila (do inicio ao fim) */
void fila_exibir(Fila *f);

/* Libera toda a memoria da fila */
void fila_destruir(Fila **f);

#endif /* FILA_H */
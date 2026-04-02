#ifndef PILHA_H
#define PILHA_H

/* ==========================================================
 * TAD - Pilha Dinamicamente Encadeada
 * Armazena frases inseridas pelo usuario no editor de texto.
 * Cada no contem uma string e um ponteiro para o proximo no.
 * ========================================================== */

/* Estrutura de um no da pilha */
typedef struct No {
    char *frase;        /* Frase armazenada (alocada dinamicamente) */
    struct No *proximo; /* Ponteiro para o no abaixo na pilha       */
} No;

/* Estrutura da pilha */
typedef struct {
    No *topo;   /* Ponteiro para o no do topo */
    int tamanho; /* Quantidade de elementos    */
} Pilha;

/* --- Funcoes do TAD Pilha --- */

/* Inicializa a pilha (deve ser chamada antes de qualquer uso) */
void pilha_inicializar(Pilha **p);

/* Retorna 1 se a pilha estiver vazia, 0 caso contrario */
int pilha_vazia(Pilha *p);

/* Insere uma nova frase no topo da pilha */
void pilha_push(Pilha **p, const char *frase);

/* Remove e retorna a frase do topo (caller deve liberar a string retornada) */
char *pilha_pop(Pilha **p);

/* Retorna a frase do topo sem remover (somente leitura) */
const char *pilha_peek(Pilha *p);

/* Exibe todas as frases da pilha (do topo para a base) */
void pilha_exibir(Pilha *p);

/* Libera toda a memoria da pilha */
void pilha_destruir(Pilha **p);

#endif /* PILHA_H */
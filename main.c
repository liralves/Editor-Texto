#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "pilha.h"
#include "fila.h"
#include "lista.h"


#define TAM_BUFFER 512 /* Tamanho maximo de entrada do usuario */

/* ---------------------------------------------------------- */
/*                    Funcoes Auxiliares                       */
/* ---------------------------------------------------------- */

/* Limpa o buffer de entrada do teclado */
void limpar_buffer() {
    int c;
    while ((c = getchar()) != '\n' && c != EOF);
}

/* Le uma linha do usuario com seguranca */
void ler_entrada(char *destino, int tamanho) {
    fgets(destino, tamanho, stdin);
    /* Remove o '\n' final, se houver */
    size_t len = strlen(destino);
    if (len > 0 && destino[len - 1] == '\n') {
        destino[len - 1] = '\0';
    }
}

/* ---------------------------------------------------------- */
/*                    Submenu: Dicionario                      */
/* ---------------------------------------------------------- */

void menu_dicionario(ListaDupla **lista) {
    int opcao;
    char palavra[TAM_BUFFER];

    do {
        printf("\n========== DICIONARIO DE PALAVRAS-CHAVE ==========\n");
        printf("  1. Inserir palavra\n");
        printf("  2. Remover palavra\n");
        printf("  3. Buscar palavra\n");
        printf("  4. Listar (A -> Z)\n");
        printf("  5. Listar (Z -> A)\n");
        printf("  0. Voltar ao menu principal\n");
        printf("===================================================\n");
        printf("Opcao: ");
        scanf("%d", &opcao);
        limpar_buffer();

        switch (opcao) {
            case 1:
                printf("Digite a palavra-chave: ");
                ler_entrada(palavra, TAM_BUFFER);
                lista_inserir_ordenado(lista, palavra);
                break;

            case 2:
                printf("Digite a palavra a remover: ");
                ler_entrada(palavra, TAM_BUFFER);
                lista_remover(lista, palavra);
                break;

            case 3:
                printf("Digite a palavra a buscar: ");
                ler_entrada(palavra, TAM_BUFFER);
                if (lista_buscar(*lista, palavra)) {
                    printf("[Lista] Palavra '%s' ENCONTRADA no dicionario.\n", palavra);
                } else {
                    printf("[Lista] Palavra '%s' NAO encontrada.\n", palavra);
                }
                break;

            case 4:
                lista_exibir_frente(*lista);
                break;

            case 5:
                lista_exibir_reverso(*lista);
                break;

            case 0:
                printf("Voltando ao menu principal...\n");
                break;

            default:
                printf("[AVISO] Opcao invalida.\n");
        }

    } while (opcao != 0);
}

/* ---------------------------------------------------------- */
/*                      Menu Principal                         */
/* ---------------------------------------------------------- */

int main() {
    /* Inicializa os tres TADs */
    Pilha *pilha = NULL;
    Fila  *fila  = NULL;
    ListaDupla *lista = NULL;

    pilha_inicializar(&pilha);
    fila_inicializar(&fila);
    lista_inicializar(&lista);

    int opcao;
    char frase[TAM_BUFFER];

    printf("\n*** Bem-vindo ao TextManager ***\n");

    do {
        printf("\n============= TEXTMANAGER =============\n");
        printf("  1. Inserir frase          (Push na Pilha)\n");
        printf("  2. Desfazer ultima frase  (Pop na Pilha)\n");
        printf("  3. Ver pilha atual\n");
        printf("  4. Enviar para salvamento (Pilha -> Fila)\n");
        printf("  5. Processar salvamento   (Dequeue da Fila)\n");
        printf("  6. Ver fila de salvamento\n");
        printf("  7. Gerenciar Dicionario   (Submenu)\n");
        printf("  0. Sair\n");
        printf("=======================================\n");
        printf("Opcao: ");
        scanf("%d", &opcao);
        limpar_buffer();

        switch (opcao) {

            /* 1. Inserir frase na pilha */
            case 1:
                printf("Digite a frase: ");
                ler_entrada(frase, TAM_BUFFER);
                if (strlen(frase) == 0) {
                    printf("[AVISO] Frase vazia. Operacao cancelada.\n");
                } else {
                    pilha_push(&pilha, frase);
                }
                break;

            /* 2. Desfazer (pop) da pilha */
            case 2: {
                char *removida = pilha_pop(&pilha);
                if (removida != NULL) {
                    printf("[Undo] Frase removida: \"%s\"\n", removida);
                    free(removida); /* Caller e responsavel por liberar */
                }
                break;
            }

            /* 3. Exibir pilha */
            case 3:
                pilha_exibir(pilha);
                break;

            /* 4. Enviar frase do topo da pilha para a fila de salvamento */
            case 4: {
                char *para_salvar = pilha_pop(&pilha);
                if (para_salvar != NULL) {
                    printf("[Editor] Enviando frase para fila de salvamento...\n");
                    fila_enqueue(&fila, para_salvar);
                    free(para_salvar); /* fila_enqueue fez sua propria copia */
                }
                break;
            }

            /* 5. Processar (dequeue) da fila de salvamento */
            case 5: {
                char *processada = fila_dequeue(&fila);
                if (processada != NULL) {
                    printf("[Salvamento] Processando: \"%s\"\n", processada);
                    printf("[Disco] Frase gravada com sucesso!\n");
                    free(processada); /* Caller e responsavel por liberar */
                }
                break;
            }

            /* 6. Exibir fila */
            case 6:
                fila_exibir(fila);
                break;

            /* 7. Submenu do dicionario */
            case 7:
                menu_dicionario(&lista);
                break;

            /* 0. Sair e liberar toda a memoria */
            case 0:
                printf("\n[Sistema] Liberando toda a memoria...\n");
                pilha_destruir(&pilha);
                fila_destruir(&fila);
                lista_destruir(&lista);
                printf("[Sistema] Encerrando TextManager. Ate logo!\n");
                break;

            default:
                printf("[AVISO] Opcao invalida. Tente novamente.\n");
        }

    } while (opcao != 0);

    return EXIT_SUCCESS;
}
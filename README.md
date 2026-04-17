# UNIVERSIDADE CATÓLICA DE PERNAMBUCO
<p align="center">
   <img src="http://www1.unicap.br/icam/wp-content/uploads/2019/06/marca_nova.svg" />
  </p>

# TextManager — Editor de Texto em C

> Um editor de texto de linha de comando implementado em C puro, demonstrando a aplicação prática de três Tipos Abstratos de Dados (TADs) lineares trabalhando em conjunto: **Pilha**, **Fila** e **Lista Duplamente Encadeada**.

---

## Índice

- [Sobre o Projeto](#sobre-o-projeto)
- [Objetivos](#objetivos)
- [Arquitetura e Estrutura de Arquivos](#arquitetura-e-estrutura-de-arquivos)
- [TADs Utilizados](#tads-utilizados)
  - [Pilha — Histórico de Edição](#pilha--histórico-de-edição)
  - [Fila — Fila de Salvamento](#fila--fila-de-salvamento)
  - [Lista Duplamente Encadeada — Dicionário](#lista-duplamente-encadeada--dicionário)
- [Fluxo Completo do Sistema](#fluxo-completo-do-sistema)
- [Menu e Funcionalidades](#menu-e-funcionalidades)
- [Como Compilar e Executar](#como-compilar-e-executar)
- [Exemplo de Uso](#exemplo-de-uso)
- [Conceitos Aplicados](#conceitos-aplicados)
- [Licença](#licença)

---

## Sobre o Projeto

O **TextManager** é um editor de texto minimalista de terminal desenvolvido em C como projeto acadêmico de Estruturas de Dados. Em vez de simplesmente implementar os TADs isoladamente, o projeto os combina em um sistema coeso que simula o comportamento real de um editor de texto:

- Frases são **redigidas** e empilhadas (como digitar no editor)
- Erros são **desfeitos** com pop da pilha (como Ctrl+Z)
- Frases prontas são **enviadas para salvamento** em uma fila (como uma fila de impressão)
- Palavras-chave são **gerenciadas em um dicionário** ordenado

O projeto é 100% em C padrão, sem dependências externas, e organizado com separação clara entre interface (`.h`) e implementação (`.c`).

---

## Objetivos

| Objetivo | Descrição |
|---|---|
| **Educacional** | Demonstrar o uso prático de TADs lineares em um sistema real |
| **Modularidade** | Cada TAD tem seu próprio par `.h` / `.c`, completamente encapsulado |
| **Encapsulamento** | O `main.c` não acessa campos internos das structs — só usa a interface pública |
| **Gestão de memória** | Uso explícito de `malloc` e `free`, com responsabilidade clara sobre quem libera |
| **FIFO + LIFO + Ordenação** | Cada TAD aplica uma política diferente, adequada ao seu papel |

---

## Arquitetura e Estrutura de Arquivos

```
Editor-Texto/
│
├── main.c                    ← Ponto de entrada; orquestra os três TADs
│
├── includes/                 ← Interfaces públicas (TADs — o "contrato")
│   ├── pilha.h               ← Interface da Pilha
│   ├── fila.h                ← Interface da Fila
│   └── lista.h               ← Interface da Lista Duplamente Encadeada
│
├── implementações/           ← Implementações concretas (detalhes ocultos)
│   ├── pilha.c               ← Pilha com lista encadeada simples
│   ├── fila.c                ← Fila com lista encadeada (head + tail)
│   └── lista.c               ← Lista duplamente encadeada ordenada
│
├── main                      ← Binário compilado (Linux)
├── LICENSE                   ← MIT License
└── README.md                 ← Este arquivo
```

### Separação TAD → Interface → Implementação

```
┌─────────────────────────────────────────────────────────────┐
│                        main.c                               │
│   usa apenas: pilha_push(), fila_enqueue(), lista_inserir() │
│   NUNCA acessa campos internos das structs                  │
└──────────────┬──────────────┬──────────────┬────────────────┘
               │              │              │
        ┌──────▼──────┐ ┌─────▼─────┐ ┌────▼──────────┐
        │  pilha.h    │ │  fila.h   │ │   lista.h     │
        │  (interface)│ │(interface)│ │  (interface)  │
        └──────┬──────┘ └─────┬─────┘ └────┬──────────┘
               │              │             │
        ┌──────▼──────┐ ┌─────▼─────┐ ┌────▼──────────┐
        │  pilha.c    │ │  fila.c   │ │   lista.c     │
        │(implementa.)│ │(implementa│ │(implementação)│
        └─────────────┘ └───────────┘ └───────────────┘
```

---

## TADs Utilizados

### Pilha — Histórico de Edição

**Política:** LIFO — Last In, First Out

A pilha funciona como o **histórico de edição** do editor. Cada frase digitada pelo usuário é empilhada. O desfazer (`undo`) simplesmente executa um `pop` no topo.

```
Estado da pilha após inserir "Olá", "Mundo", "TextManager":

     ┌─────────────────────────┐
     │     "TextManager"       │  ← topo (mais recente)
     ├─────────────────────────┤
     │       "Mundo"           │
     ├─────────────────────────┤
     │        "Olá"            │  ← base (mais antigo)
     └─────────────────────────┘
              base

Após desfazer (pop):
     ┌─────────────────────────┐
     │       "Mundo"           │  ← novo topo
     ├─────────────────────────┤
     │        "Olá"            │
     └─────────────────────────┘
```

**Interface pública (`pilha.h`):**

```c
void  pilha_inicializar(Pilha **p);
void  pilha_push(Pilha **p, const char *frase);
char *pilha_pop(Pilha **p);         // caller responsável por free()
void  pilha_exibir(const Pilha *p);
void  pilha_destruir(Pilha **p);
```

**Complexidade de todas as operações:** O(1)

**Implementação:** Lista encadeada simples com `head` = topo. `push` insere no início; `pop` remove do início. Sem limite de tamanho — só memória do sistema.

---

### Fila — Fila de Salvamento

**Política:** FIFO — First In, First Out

A fila simula uma **fila de salvamento em disco**. Quando o usuário decide salvar uma frase (opção 4), o topo da pilha é transferido para o final da fila. O processamento (opção 5) retira do início — ou seja, salva na ordem em que as frases foram enviadas.

```
Fluxo Pilha → Fila (opção 4 do menu):

PILHA                          FILA DE SALVAMENTO
┌──────────────┐               ┌────────────────────────────────┐
│  "Frase C"   │ ──pop()──►   │  "Frase A" │ "Frase B" │ "Frase C" │
├──────────────┤  enqueue()   └────────────────────────────────┘
│  "Frase B"   │               front ────────────────────► rear
├──────────────┤
│  "Frase A"   │
└──────────────┘

Após processar (dequeue — opção 5):
→ "Frase A" é salva no disco primeiro (FIFO)
```

**Interface pública (`fila.h`):**

```c
void  fila_inicializar(Fila **f);
void  fila_enqueue(Fila **f, const char *frase);  // faz cópia interna
char *fila_dequeue(Fila **f);     // caller responsável por free()
void  fila_exibir(const Fila *f);
void  fila_destruir(Fila **f);
```

**Complexidade:** O(1) para `enqueue` e `dequeue` — mantidos os ponteiros `head` (front) e `tail` (rear).

**Detalhe crítico:** `fila_enqueue` faz sua própria cópia da string (`strdup` internamente). Por isso, após chamar `enqueue`, o `main.c` libera o ponteiro retornado por `pilha_pop` — cada módulo é responsável pela sua memória.

---

### Lista Duplamente Encadeada — Dicionário

**Política:** Inserção ordenada alfabeticamente

A lista duplamente encadeada implementa um **dicionário de palavras-chave** que pode ser navegado nos dois sentidos (A→Z e Z→A), algo impossível com lista simples sem percorrer tudo de novo.

```
Estrutura de um nó:

┌──────┬──────────────┬──────┐
│ prev │    dado      │ next │
│  ◄── │  "exemplo"   │ ──►  │
└──────┴──────────────┴──────┘

Lista com palavras inseridas em ordem ("banco", "dado", "fila"):

NULL ◄─ [banco] ⇄ [dado] ⇄ [fila] ─► NULL
         head                  tail

Percurso A→Z:  banco → dado → fila
Percurso Z→A:  fila → dado → banco
```

**Interface pública (`lista.h`):**

```c
void lista_inicializar(ListaDupla **l);
void lista_inserir_ordenado(ListaDupla **l, const char *palavra);
void lista_remover(ListaDupla **l, const char *palavra);
int  lista_buscar(const ListaDupla *l, const char *palavra);
void lista_exibir_frente(const ListaDupla *l);   // A → Z
void lista_exibir_reverso(const ListaDupla *l);  // Z → A
void lista_destruir(ListaDupla **l);
```

**Complexidade:**

| Operação | Complexidade |
|---|---|
| `inserir_ordenado` | O(n) — encontra a posição certa |
| `remover` | O(n) — busca pelo valor |
| `buscar` | O(n) — percurso linear |
| `exibir_frente` | O(n) — percorre do head ao tail |
| `exibir_reverso` | O(n) — percorre do tail ao head (ponteiro `prev`) |

---

## Fluxo Completo do Sistema

O diagrama abaixo ilustra como os três TADs interagem durante uma sessão típica:

```
                        USUÁRIO
                           │
              ┌────────────▼────────────┐
              │       TextManager       │
              │      (main.c loop)      │
              └─────────────────────────┘
                     │    │    │
          ┌──────────┘    │    └──────────────┐
          ▼               ▼                   ▼
   ┌─────────────┐  ┌───────────────┐  ┌──────────────────┐
   │   PILHA     │  │     FILA      │  │  LISTA DUPLA     │
   │  (histórico)│  │ (salvamento)  │  │  (dicionário)    │
   │             │  │               │  │                  │
   │ push(frase) │  │enqueue(frase) │  │inserir_ordenado()│
   │ pop() →undo │  │dequeue()→save │  │remover()         │
   │ exibir()    │  │ exibir()      │  │buscar()          │
   └──────┬──────┘  └───────▲───────┘  │exibir_frente()  │
          │                 │          │exibir_reverso()  │
          │    op. 4:       │          └──────────────────┘
          └── pop() ────────┘
              + enqueue()
         (transfere pilha→fila)


Ciclo de vida de uma frase:
─────────────────────────────────────────────────────────
  1. Usuário digita         →  pilha_push()
  2. Usuário erra           →  pilha_pop()  (undo)
  3. Usuário aprova         →  pilha_pop() + fila_enqueue()
  4. Sistema salva no disco →  fila_dequeue()
─────────────────────────────────────────────────────────
```

---

## Menu e Funcionalidades

### Menu principal

```
============= TEXTMANAGER =============
 1. Inserir frase         (Push na Pilha)
 2. Desfazer última frase (Pop na Pilha)
 3. Ver pilha atual
 4. Enviar para salvamento (Pilha → Fila)
 5. Processar salvamento  (Dequeue da Fila)
 6. Ver fila de salvamento
 7. Gerenciar Dicionário  (Submenu)
 0. Sair
=======================================
```

### Submenu — Dicionário de palavras-chave

```
========== DICIONÁRIO DE PALAVRAS-CHAVE ==========
 1. Inserir palavra
 2. Remover palavra
 3. Buscar palavra
 4. Listar (A → Z)
 5. Listar (Z → A)
 0. Voltar ao menu principal
===================================================
```

### Descrição detalhada de cada opção

| Opção | Ação | TAD envolvido | Comportamento |
|---|---|---|---|
| `1` | Inserir frase | Pilha | Lê até 512 caracteres; frase vazia é rejeitada |
| `2` | Desfazer | Pilha | `pop()` — exibe a frase removida; caller faz `free()` |
| `3` | Ver pilha | Pilha | Exibe todas as frases do topo à base |
| `4` | Enviar para salvamento | Pilha + Fila | `pop()` da pilha → `enqueue()` na fila; fila faz `strdup` internamente |
| `5` | Processar salvamento | Fila | `dequeue()` — simula gravação em disco; exibe a frase "salva" |
| `6` | Ver fila | Fila | Exibe do front ao rear |
| `7` | Dicionário | Lista dupla | Abre submenu com 5 operações |
| `0` | Sair | Todos | `destruir()` nos três TADs; liberação total de memória |

---

## Como Compilar e Executar

### Pré-requisitos

- GCC (ou qualquer compilador C99+)
- Sistema operacional Linux/macOS (ou WSL no Windows)

### Compilação

```bash
# Clone o repositório
git clone https://github.com/liralves/Editor-Texto.git
cd Editor-Texto

# Compile todos os arquivos C
gcc main.c implementações/pilha.c implementações/fila.c implementações/lista.c \
    -I includes/ -o textmanager -Wall -Wextra

# Execute
./textmanager
```
---

## Conceitos Aplicados

### Gestão de memória em C

O projeto adota uma política clara: **quem aloca é quem libera**, ou **o caller é responsável pelo ponteiro retornado**.

```c
// Exemplo em main.c — opção 2 (pop)
char *removida = pilha_pop(&pilha);
if (removida != NULL) {
    printf("[Undo] Frase removida: \"%s\"\n", removida);
    free(removida);  // ← caller faz free; pilha_pop transfere ownership
}

// Exemplo em main.c — opção 4 (pilha → fila)
char *para_salvar = pilha_pop(&pilha);
if (para_salvar != NULL) {
    fila_enqueue(&fila, para_salvar);  // fila faz strdup internamente
    free(para_salvar);                 // caller libera a cópia original
}
```

### Leitura segura de entrada

O projeto usa `fgets` + remoção do `\n` em vez de `gets` (inseguro) ou `scanf` com strings (sem limite):

```c
void ler_entrada(char *destino, int tamanho) {
    fgets(destino, tamanho, stdin);
    size_t len = strlen(destino);
    if (len > 0 && destino[len - 1] == '\n')
        destino[len - 1] = '\0';
}
```

### Encapsulamento de TADs em C

Cada TAD usa o padrão **opaque pointer**: o tipo (`Pilha *`, `Fila *`, `ListaDupla *`) é declarado no `.h` mas definido apenas no `.c`. O código cliente nunca vê os campos internos — só opera através da interface.

```c
// pilha.h — só declara, não define
typedef struct Pilha Pilha;

// pilha.c — define internamente
struct Pilha {
    No *topo;
    int tamanho;
};
```

Isso garante que mudar a implementação interna (ex: trocar lista encadeada por array) não afeta nenhum código que usa o TAD.

---

## Licença

Distribuído sob a licença MIT. Veja o arquivo [LICENSE](LICENSE) para mais detalhes.

---

<p align="center">
  Desenvolvido como projeto de <strong>Estruturas de Dados</strong> em C
</p>

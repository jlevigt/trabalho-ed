#ifndef LISTA_DUPLA_H
#define LISTA_DUPLA_H

#include <stdbool.h> // Para usar bool

// --- Estruturas ---
typedef struct Produto {
    int id;
    char nome[50];
    float preco;
    int quantidade;
} Produto;

typedef struct Node {
    Produto produto;
    struct Node *prev;
    struct Node *next;
} Node;

typedef struct Lista {
  int nElementos;
  Node *first;
  Node *last;
  Node *current;
} Lista;

// --- Protótipos das Funções de Manipulação da Lista (CRUD) ---
void Lista_cria(Lista *lista);
void Lista_destroi(Lista *lista);
int Lista_getSize(Lista *lista);
bool Lista_inserir(Lista *lista, Produto *data);
bool Lista_atualizar(Lista *lista, int id_produto, Produto *novos_dados);
bool Lista_remover(Lista *lista, int id_produto);
bool Lista_next(Lista *lista);
bool Lista_prev(Lista *lista);
void Lista_goFirst(Lista *lista);
void Lista_goLast(Lista *lista);
Produto *Lista_getCurrent(Lista *lista);
Node *Lista_getNodeById(Lista *lista, int id_produto);

#endif // LISTA_DUPLA_H
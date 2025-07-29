// src/lista_dupla.c
#include <stdio.h>   // Para printf, NULL
#include <stdlib.h>  // Para malloc, free
#include <string.h>  // Para strcpy, strncpy
#include <stdbool.h> // Para tipo bool
#include "lista_dupla.h" // Inclui as definições de structs e protótipos

/**
 * @brief Inicializa uma nova lista duplamente ligada.
 * @param lista Ponteiro para a estrutura Lista a ser inicializada.
 */
void Lista_cria(Lista *lista) {
    if (lista == NULL) {
        fprintf(stderr, "Erro: Ponteiro de lista nulo em Lista_cria.\n");
        return;
    }
    lista->first = NULL;
    lista->last = NULL;
    lista->current = NULL;
    lista->nElementos = 0;
}

/**
 * @brief Destrói a lista, liberando toda a memória alocada para os nós e os produtos.
 * @param lista Ponteiro para a estrutura Lista a ser destruída.
 */
void Lista_destroi(Lista *lista) {
    if (lista == NULL) {
        return; // Nada para destruir se a lista for nula
    }

    Node *current_node = lista->first;
    Node *next_node;

    while (current_node != NULL) {
        next_node = current_node->next;
        // Se o produto foi alocado dinamicamente dentro do nó, libere-o aqui.
        // No seu caso, Produto é uma struct direta no Node, então não precisa de free(current_node->produto)
        free(current_node); // Libera a memória do nó
        current_node = next_node;
    }
    lista->first = NULL;
    lista->last = NULL;
    lista->current = NULL;
    lista->nElementos = 0;
}

/**
 * @brief Retorna o número de elementos na lista.
 * @param lista Ponteiro para a estrutura Lista.
 * @return O número de elementos na lista.
 */
int Lista_getSize(Lista *lista) {
    if (lista == NULL) {
        return 0; // Lista nula não tem elementos
    }
    return lista->nElementos;
}

/**
 * @brief Insere um novo produto no final da lista.
 * @param lista Ponteiro para a estrutura Lista onde o produto será inserido.
 * @param data Ponteiro para os dados do Produto a serem inseridos.
 * @return true se a inserção foi bem-sucedida, false caso contrário.
 */
bool Lista_inserir(Lista *lista, Produto *data) {
    if (lista == NULL || data == NULL) {
        fprintf(stderr, "Erro: Ponteiro de lista ou dados nulos em Lista_inserir.\n");
        return false;
    }

    // Aloca memória para o novo nó
    Node *newNode = (Node *)malloc(sizeof(Node));
    if (newNode == NULL) {
        fprintf(stderr, "Erro: Falha na alocação de memória para o novo nó.\n");
        return false;
    }

    // Copia os dados do produto para o nó
    newNode->produto = *data; // Copia a struct Produto diretamente
    newNode->next = NULL;
    newNode->prev = NULL;

    if (lista->first == NULL) { // Se a lista estiver vazia
        lista->first = newNode;
        lista->last = newNode;
    } else { // Se a lista não estiver vazia, adiciona no final
        lista->last->next = newNode;
        newNode->prev = lista->last;
        lista->last = newNode;
    }
    lista->nElementos++;
    lista->current = newNode; // Define o novo nó como o nó atual
    return true;
}

/**
 * @brief Atualiza os dados de um produto existente na lista pelo seu ID.
 * O ID do produto existente nao sera alterado.
 * Campos em 'novos_dados' com valores sentinela (nome vazio, preco -1.0f, quantidade -1)
 * nao serao alterados.
 * @param lista Ponteiro para a estrutura Lista.
 * @param id_produto O ID do produto a ser atualizado.
 * @param novos_dados Ponteiro para os novos dados do Produto (nome, preco, quantidade).
 * @return true se a atualização foi bem-sucedida, false caso contrário.
 */
bool Lista_atualizar(Lista *lista, int id_produto, Produto *novos_dados) {
    if (lista == NULL || novos_dados == NULL) {
        fprintf(stderr, "Erro: Ponteiro de lista ou novos dados nulos em Lista_atualizar.\n");
        return false;
    }

    Node *nodeToUpdate = Lista_getNodeById(lista, id_produto);
    if (nodeToUpdate != NULL) {
        // Atualiza o nome se a string nao estiver vazia (nao for o sentinela)
        if (strlen(novos_dados->nome) > 0) {
            strncpy(nodeToUpdate->produto.nome, novos_dados->nome, sizeof(nodeToUpdate->produto.nome) - 1);
            nodeToUpdate->produto.nome[sizeof(nodeToUpdate->produto.nome) - 1] = '\0'; // Garante terminação nula
        }

        // Atualiza o preco se nao for o valor sentinela
        if (novos_dados->preco != -1.0f) {
            nodeToUpdate->produto.preco = novos_dados->preco;
        }

        // Atualiza a quantidade se nao for o valor sentinela
        if (novos_dados->quantidade != -1) {
            nodeToUpdate->produto.quantidade = novos_dados->quantidade;
        }
        return true;
    }
    return false; // Produto não encontrado
}

/**
 * @brief Remove um produto da lista pelo seu ID.
 * @param lista Ponteiro para a estrutura Lista.
 * @param id_produto O ID do produto a ser removido.
 * @return true se a remoção foi bem-sucedida, false caso contrário.
 */
bool Lista_remover(Lista *lista, int id_produto) {
    if (lista == NULL) {
        fprintf(stderr, "Erro: Ponteiro de lista nulo em Lista_remover.\n");
        return false;
    }

    Node *nodeToRemove = Lista_getNodeById(lista, id_produto);
    if (nodeToRemove == NULL) {
        return false; // Produto não encontrado
    }

    // Se o nó a ser removido é o primeiro
    if (nodeToRemove->prev == NULL) {
        lista->first = nodeToRemove->next;
    } else {
        nodeToRemove->prev->next = nodeToRemove->next;
    }

    // Se o nó a ser removido é o último
    if (nodeToRemove->next == NULL) {
        lista->last = nodeToRemove->prev;
    } else {
        nodeToRemove->next->prev = nodeToRemove->prev;
    }

    // Ajusta o ponteiro current se o nó removido for o current
    if (lista->current == nodeToRemove) {
        if (nodeToRemove->next != NULL) {
            lista->current = nodeToRemove->next;
        } else if (nodeToRemove->prev != NULL) {
            lista->current = nodeToRemove->prev;
        } else {
            lista->current = NULL; // Lista ficou vazia
        }
    }

    free(nodeToRemove); // Libera a memória do nó
    lista->nElementos--;
    return true;
}

/**
 * @brief Move o ponteiro 'current' para o próximo nó na lista.
 * @param lista Ponteiro para a estrutura Lista.
 * @return true se o 'current' foi movido, false se já estava no final.
 */
bool Lista_next(Lista *lista) {
    if (lista == NULL || lista->current == NULL || lista->current->next == NULL) {
        return false;
    }
    lista->current = lista->current->next;
    return true;
}

/**
 * @brief Move o ponteiro 'current' para o nó anterior na lista.
 * @param lista Ponteiro para a estrutura Lista.
 * @return true se o 'current' foi movido, false se já estava no início.
 */
bool Lista_prev(Lista *lista) {
    if (lista == NULL || lista->current == NULL || lista->current->prev == NULL) {
        return false;
    }
    lista->current = lista->current->prev;
    return true;
}

/**
 * @brief Move o ponteiro 'current' para o primeiro nó da lista.
 * @param lista Ponteiro para a estrutura Lista.
 */
void Lista_goFirst(Lista *lista) {
    if (lista != NULL) {
        lista->current = lista->first;
    }
}

/**
 * @brief Move o ponteiro 'current' para o último nó da lista.
 * @param lista Ponteiro para a estrutura Lista.
 */
void Lista_goLast(Lista *lista) {
    if (lista != NULL) {
        lista->current = lista->last;
    }
}

/**
 * @brief Retorna um ponteiro para o Produto no nó 'current'.
 * @param lista Ponteiro para a estrutura Lista.
 * @return Ponteiro para o Produto atual, ou NULL se 'current' for nulo.
 */
Produto *Lista_getCurrent(Lista *lista) {
    if (lista == NULL || lista->current == NULL) {
        return NULL;
    }
    return &(lista->current->produto); // Retorna o endereço da struct Produto dentro do nó
}

/**
 * @brief Busca um nó na lista pelo ID do produto.
 * @param lista Ponteiro para a estrutura Lista.
 * @param id_produto O ID do produto a ser buscado.
 * @return Ponteiro para o Nó encontrado, ou NULL se não for encontrado.
 */
Node *Lista_getNodeById(Lista *lista, int id_produto) {
    if (lista == NULL || lista->first == NULL) {
        return NULL;
    }

    Node *temp = lista->first;
    while (temp != NULL) {
        if (temp->produto.id == id_produto) {
            return temp;
        }
        temp = temp->next;
    }
    return NULL; // Não encontrado
}

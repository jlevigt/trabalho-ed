#include "produto.h"
#include <string.h> // Para strncpy
#include <stdio.h>  // Para printf (se houvesse funções de exibição aqui)

/**
 * @brief Cria e retorna uma nova estrutura Produto.
 * @param id ID único do produto.
 * @param nome Nome do produto.
 * @param preco Preço do produto.
 * @param quantidade Quantidade em estoque do produto.
 * @return Uma estrutura Produto inicializada.
 */
Produto criarProduto(int id, const char* nome, float preco, int quantidade) {
    Produto p;
    p.id = id;
    // Garante que o nome seja copiado de forma segura, evitando estouro de buffer
    strncpy(p.nome, nome, sizeof(p.nome) - 1);
    p.nome[sizeof(p.nome) - 1] = '\0'; // Garante terminação nula
    p.preco = preco;
    p.quantidade = quantidade;
    return p;
}

/**
 * @brief Exibe os detalhes de um produto.
 * @param p Ponteiro para a estrutura Produto a ser exibida.
 */
void exibir_detalhes_produto(Produto *p) {
    if (p == NULL) {
        printf("Produto nulo.\n");
        return;
    }
    printf("=========================================\n");
    printf("ID: %d\nNome: %s\nPreco: %.2f R$\nQuantidade: %d\n",
           p->id, p->nome, p->preco, p->quantidade);
}


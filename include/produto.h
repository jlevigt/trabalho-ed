#ifndef PRODUTO_H
#define PRODUTO_H

// A estrutura Produto já foi definida em lista_dupla.h,
// então apenas incluímos para garantir a visibilidade.
// Se Produto fosse exclusivo deste arquivo, a definição estaria aqui.
#include "lista_dupla.h" // Inclui a definição de Produto

/**
 * @brief Cria e retorna uma nova estrutura Produto com os dados fornecidos.
 * @param id ID único do produto.
 * @param nome Nome do produto.
 * @param preco Preço do produto.
 * @param quantidade Quantidade em estoque do produto.
 * @return Uma estrutura Produto inicializada com os dados.
 */
Produto criarProduto(int id, const char* nome, float preco, int quantidade);
void exibir_detalhes_produto(Produto *p);

#endif // PRODUTO_H

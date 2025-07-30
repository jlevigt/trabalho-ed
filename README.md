# Gerenciador de Produtos

Este é um projeto simples de gerenciamento de produtos implementado em **C**. <br>
Utilizando uma estrutura de dados de **Lista Duplamente Ligada**. <br>
O programa oferece uma interface de terminal interativa com navegação por setas e cores ANSI, projetada para funcionar em sistemas Unix-like, como o macOS.

---

## Funcionalidades

O gerenciador de produtos permite as seguintes operações:

- **Inserir Produto**: Adiciona um novo produto à lista.
- **Remover Produto**: Remove um produto existente pelo seu ID.
- **Atualizar Produto**: Altera os detalhes (nome, preço, quantidade) de um produto existente, identificado pelo seu ID. **O ID não pode ser alterado**, e os campos deixados em branco (ao pressionar `Enter`) não serão modificados.
- **Buscar Produto por ID**: Encontra e exibe os detalhes de um produto específico.
- **Exibir Todos os Produtos (Frente)**: Lista todos os produtos na ordem de inserção.
- **Exibir Todos os Produtos (Trás)**: Lista todos os produtos na ordem inversa de inserção.
- **Navegar na Lista (Atual)**: Permite percorrer a lista item por item usando as setas para a esquerda e direita.
- **Tamanho da Lista**: Exibe o número total de produtos atualmente na lista.
- **Sair**: Encerra o programa, liberando toda a memória alocada.

---

## Como Compilar e Executar

Para compilar e executar este projeto em seu ambiente macOS (ou outro sistema Unix-like com `gcc` e `make` instalados), siga os passos abaixo:

1.  **Clone ou Baixe o Projeto**:
    Se você estiver usando Git, clone o repositório:

    ```bash
    git clone https://github.com/jlevigt/trabalho-ed
    ```

    Com a estrutura de pastas:

    ```
    trabalho-ed/
    ├── src/
    │   ├── main.c
    │   ├── produto.c
    │   └── lista_dupla.c
    ├── include/
    │   ├── produto.h
    │   └── lista_dupla.h
    ├── doc/
    │   ├── README.md
    └── Makefile
    └── .gitignore
    ```

2.  **Navegue até o Diretório Raiz do Projeto**:
    Abra o terminal e use o comando `cd` para entrar na pasta principal do projeto (onde está o `Makefile`):

    ```bash
    cd caminho/para/seu/trabalho-ed
    ```

3.  **Compilar o Projeto**:
    Utilize o `Makefile` para compilar todo o código-fonte. Isso criará um executável na pasta `bin/`.

    ```bash
    make
    ```

4.  **Executar o Programa**:
    Após a compilação bem-sucedida, você pode executar o programa usando:

    ```bash
    make run
    ```

    Ou diretamente:

    ```bash
    ./bin/gerenciador_produtos
    ```

---

## Uso

Ao iniciar o programa, um menu interativo será exibido no terminal.

- **Navegação**: Use as **setas para CIMA** e **para BAIXO** do teclado para mover a seleção entre as opções do menu.
- **Seleção**: Pressione **ENTER** para selecionar a opção desejada.
- **Entrada de Dados**: Para opções que requerem entrada de dados (como inserir ou atualizar), o programa solicitará as informações.
- **Navegação Interna**: Na opção "Navegar na Lista (Atual)", use as **setas para a ESQUERDA** e **para a DIREITA** para percorrer os produtos. Pressione **'q'** para sair desta navegação e retornar ao menu principal.

---

## Estrutura do Projeto

O projeto é organizado modularmente para facilitar a leitura e a manutenção:

- **`src/`**: Contém todos os arquivos de implementação (`.c`).
  - `main.c`: Lógica principal do programa, interface do usuário e manipulação do terminal.
  - `produto.c`: Funções auxiliares para a criação de produtos.
  - `lista_dupla.c`: Implementação de todas as operações da lista duplamente ligada (inserção, remoção, busca, etc.).
- **`include/`**: Contém os arquivos de cabeçalho (`.h`) com as definições das estruturas e protótipos das funções.
  - `produto.h`: Declarações relacionadas à estrutura `Produto`.
  - `lista_dupla.h`: Declarações das estruturas `Node`, `Lista` e dos protótipos das funções de manipulação da lista.
- **`Makefile`**: Arquivo de script para automatizar o processo de compilação e limpeza do projeto.
- **`bin/`**: Diretório onde o executável compilado é armazenado.
- **`build/`**: Diretório para arquivos objeto (`.o`) intermediários da compilação.

---

// src/main.c
#include <stdio.h>   // Para printf, scanf, fgets, NULL
#include <stdlib.h>  // Para malloc, free, atexit
#include <string.h>  // Para strncpy, strcspn
#include <stdbool.h> // Para tipo bool
#include <termios.h> // Para controle do terminal (tcgetattr, tcsetattr)
#include <unistd.h>  // Para STDIN_FILENO, read

#include "lista_dupla.h" // Inclui as definições da lista e do produto
#include "produto.h"      // Inclui funções de criação de produto

// --- Variáveis Globais para o Terminal ---
// Armazenam as configurações originais do terminal para restaurá-las ao sair.
static struct termios old_tio, new_tio;

// --- Funções de Manipulação do Terminal ---

/**
 * @brief Configura o terminal para o modo "raw" (bruto).
 * Neste modo, os caracteres são lidos imediatamente (sem buffer de linha)
 * e não são ecoados na tela. Essencial para capturar teclas de seta.
 */
void set_raw_mode() {
    // Salva as configurações atuais do terminal
    tcgetattr(STDIN_FILENO, &old_tio);
    new_tio = old_tio;
    // Desabilita o modo canônico (buffer de linha) e o eco de caracteres
    new_tio.c_lflag &= (~ICANON & ~ECHO);
    // Aplica as novas configurações imediatamente
    tcsetattr(STDIN_FILENO, TCSANOW, &new_tio);
}

/**
 * @brief Restaura as configurações originais do terminal.
 * É crucial chamar esta função ao final do programa para que o terminal
 * volte ao seu comportamento normal. Usada com atexit().
 */
void reset_terminal_mode() {
    tcsetattr(STDIN_FILENO, TCSANOW, &old_tio); // Restaura as configurações salvas
}

/**
 * @brief Lê um único caractere do teclado e lida com sequências de escape para teclas de seta.
 * Esta função bloqueia até que uma tecla seja pressionada.
 * @return O código ASCII do caractere lido, ou códigos especiais para setas:
 * - 1000: Seta para Cima
 * - 1001: Seta para Baixo
 * - 1002: Seta para Direita
 * - 1003: Seta para Esquerda
 * - Outros caracteres ASCII normais (ex: 'q', '\n', etc.).
 */
int read_key() {
    char c;
    read(STDIN_FILENO, &c, 1); // Lê um caractere por vez

    if (c == '\x1b') { // Se o caractere for ESC (início de uma sequência de escape)
        char seq[3];
        read(STDIN_FILENO, &seq[0], 1); // Lê o próximo caractere
        if (seq[0] == '[') { // Se for '[', é uma sequência de controle ANSI
            read(STDIN_FILENO, &seq[1], 1); // Lê o terceiro caractere
            if (seq[1] == 'A') return 1000; // Seta para Cima
            if (seq[1] == 'B') return 1001; // Seta para Baixo
            if (seq[1] == 'C') return 1002; // Seta para Direita
            if (seq[1] == 'D') return 1003; // Seta para Esquerda
        }
        return c; // Retorna ESC se não for uma seta conhecida (outras sequências de escape)
    }
    return c; // Retorna o caractere normal lido
}

/**
 * @brief Limpa a tela do terminal usando códigos de escape ANSI.
 */
void clear_screen() {
    printf("\x1b[2J"); // Limpa a tela inteira
    printf("\x1b[H");  // Move o cursor para a posição inicial (linha 1, coluna 1)
}

// --- Definições de Cores ANSI ---
#define ANSI_COLOR_RED     "\x1b[31m"
#define ANSI_COLOR_GREEN   "\x1b[32m"
#define ANSI_COLOR_YELLOW  "\x1b[33m"
#define ANSI_COLOR_BLUE    "\x1b[34m"
#define ANSI_COLOR_MAGENTA "\x1b[35m"
#define ANSI_COLOR_CYAN    "\x1b[36m"
#define ANSI_COLOR_RESET   "\x1b[0m" // Reseta a cor para o padrão do terminal

/**
 * @brief Define a cor do texto no terminal usando códigos de escape ANSI.
 * @param color_code O código ANSI da cor (ex: ANSI_COLOR_RED).
 */
void set_color(const char* color_code) {
    printf("%s", color_code);
}

/**
 * @brief Reseta a cor do texto para o padrão do terminal.
 */
void reset_color() {
    printf("%s", ANSI_COLOR_RESET);
}

// --- Funções do Menu e Interação ---

/**
 * @brief Exibe o menu principal de opções, destacando a opção selecionada.
 * @param selected_option O número da opção atualmente selecionada (1-baseado).
 */
void display_menu(int selected_option) {
    clear_screen();
    set_color(ANSI_COLOR_CYAN);
    printf("--- Gerenciamento de Produtos (Lista Duplamente Ligada) ---\n");
    reset_color();
    printf("\n");

    const char* options[] = {
        "1. Inserir Produto",
        "2. Remover Produto",
        "3. Atualizar Produto",
        "4. Buscar Produto por ID",
        "5. Exibir Todos os Produtos (Frente)",
        "6. Exibir Todos os Produtos (Tras)",
        "7. Navegar na Lista (Atual)",
        "8. Tamanho da Lista",
        "9. Sair"
    };
    int num_options = sizeof(options) / sizeof(options[0]);

    for (int i = 0; i < num_options; i++) {
        if (i + 1 == selected_option) {
            set_color(ANSI_COLOR_YELLOW); // Destaca a opção selecionada em amarelo
            printf("-> %s\n", options[i]);
            reset_color();
        } else {
            printf("   %s\n", options[i]);
        }
    }
    printf("\nUse as setas (CIMA/BAIXO) para navegar e ENTER para selecionar.\n");
}

/**
 * @brief Solicita e lê um ID de produto do usuário.
 * @return O ID do produto lido.
 */
int get_product_id_input() {
    int id;
    printf("Digite o ID do produto: ");
    // Temporariamente desabilita o modo raw para permitir entrada normal com buffer
    reset_terminal_mode();
    scanf("%d", &id);
    // Limpa o buffer do teclado após scanf
    while (getchar() != '\n');
    // Restaura o modo raw
    set_raw_mode();
    return id;
}

/**
 * @brief Solicita e lê todos os dados de um novo produto do usuário.
 * @return Uma estrutura Produto com os dados lidos.
 */
Produto get_product_data_input() {
    Produto p;
    // Temporariamente desabilita o modo raw para permitir entrada normal com buffer
    reset_terminal_mode();

    printf("ID: ");
    scanf("%d", &p.id);
    while (getchar() != '\n'); // Limpa buffer

    printf("Nome: ");
    fgets(p.nome, sizeof(p.nome), stdin);
    p.nome[strcspn(p.nome, "\n")] = 0; // Remove o caractere de nova linha

    printf("Preco: ");
    scanf("%f", &p.preco);
    while (getchar() != '\n'); // Limpa buffer

    printf("Quantidade: ");
    scanf("%d", &p.quantidade);
    while (getchar() != '\n'); // Limpa buffer

    // Restaura o modo raw
    set_raw_mode();
    return p;
}

/**
 * @brief Solicita e lê os dados de nome, preco e quantidade de um produto para atualização.
 * O ID do produto nao e solicitado, pois nao pode ser alterado.
 * @param p Ponteiro para a estrutura Produto onde os dados serao armazenados.
 */
void get_updated_product_data_input(Produto *p) {
    // Temporariamente desabilita o modo raw para permitir entrada normal com buffer
    reset_terminal_mode();

    printf("Novo Nome: ");
    fgets(p->nome, sizeof(p->nome), stdin);
    p->nome[strcspn(p->nome, "\n")] = 0; // Remove o caractere de nova linha

    printf("Novo Preco: ");
    scanf("%f", &p->preco);
    while (getchar() != '\n'); // Limpa buffer

    printf("Nova Quantidade: ");
    scanf("%d", &p->quantidade);
    while (getchar() != '\n'); // Limpa buffer

    // Restaura o modo raw
    set_raw_mode();
}


// --- Função Principal ---

int main() {
    Lista minhaLista;
    Lista_cria(&minhaLista);

    // Adiciona alguns produtos de exemplo para iniciar
    Produto p1 = criarProduto(101, "Teclado Mecanico", 350.00, 15);
    Lista_inserir(&minhaLista, &p1);
    Produto p2 = criarProduto(102, "Mouse Gamer RGB", 120.50, 30);
    Lista_inserir(&minhaLista, &p2);
    Produto p3 = criarProduto(103, "Monitor Ultrawide", 1800.00, 8);
    Lista_inserir(&minhaLista, &p3);

    int selected_option = 1; // Opção inicial selecionada no menu
    int key;
    bool running = true;
    const int num_menu_options = 9; // Total de opções no menu

    // Configura o terminal para o modo raw ao iniciar o programa
    set_raw_mode();

    // Registra a função para resetar o terminal quando o programa terminar (normalmente ou por erro)
    atexit(reset_terminal_mode);

    while (running) {
        display_menu(selected_option); // Exibe o menu com a opção destacada

        key = read_key(); // Lê a tecla pressionada

        switch (key) {
            case 1000: // Seta para Cima
                selected_option = (selected_option == 1) ? num_menu_options : selected_option - 1;
                break;
            case 1001: // Seta para Baixo
                selected_option = (selected_option == num_menu_options) ? 1 : selected_option + 1;
                break;
            case '\n': // Tecla Enter
                clear_screen(); // Limpa a tela antes de executar a ação selecionada
                switch (selected_option) {
                    case 1: { // Inserir Produto
                        set_color(ANSI_COLOR_GREEN); printf("--- Inserir Produto ---\n"); reset_color();
                        Produto novo_p = get_product_data_input();
                        if (Lista_inserir(&minhaLista, &novo_p)) {
                            set_color(ANSI_COLOR_GREEN); printf("Produto inserido com sucesso!\n"); reset_color();
                        } else {
                            set_color(ANSI_COLOR_RED); printf("Falha ao inserir produto.\n"); reset_color();
                        }
                        break;
                    }
                    case 2: { // Remover Produto
                        set_color(ANSI_COLOR_GREEN); printf("--- Remover Produto ---\n"); reset_color();
                        int id_rem = get_product_id_input();
                        if (Lista_remover(&minhaLista, id_rem)) {
                            set_color(ANSI_COLOR_GREEN); printf("Produto ID %d removido com sucesso!\n", id_rem); reset_color();
                        } else {
                            set_color(ANSI_COLOR_RED); printf("Produto ID %d nao encontrado ou falha na remocao.\n", id_rem); reset_color();
                        }
                        break;
                    }
                    case 3: { // Atualizar Produto
                        set_color(ANSI_COLOR_GREEN); printf("--- Atualizar Produto ---\n"); reset_color();
                        int id_att = get_product_id_input();
                        printf("Digite os NOVOS dados para o produto (ID nao sera alterado):\n");
                        Produto novos_dados; // Cria uma struct Produto temporária para os novos dados
                        // Apenas preenche nome, preco e quantidade. O ID nao e solicitado.
                        get_updated_product_data_input(&novos_dados);
                        if (Lista_atualizar(&minhaLista, id_att, &novos_dados)) {
                            set_color(ANSI_COLOR_GREEN); printf("Produto ID %d atualizado com sucesso!\n", id_att); reset_color();
                        } else {
                            set_color(ANSI_COLOR_RED); printf("Produto ID %d nao encontrado ou falha na atualizacao.\n", id_att); reset_color();
                        }
                        break;
                    }
                    case 4: { // Buscar Produto por ID
                        set_color(ANSI_COLOR_GREEN); printf("--- Buscar Produto por ID ---\n"); reset_color();
                        int id_busca = get_product_id_input();
                        Node *found_node = Lista_getNodeById(&minhaLista, id_busca);
                        if (found_node != NULL) {
                            set_color(ANSI_COLOR_YELLOW); printf("Produto encontrado:\n"); reset_color();
                            exibir_detalhes_produto(&(found_node->produto));
                        } else {
                            set_color(ANSI_COLOR_RED); printf("Produto ID %d nao encontrado.\n", id_busca); reset_color();
                        }
                        break;
                    }
                    case 5: { // Exibir Todos os Produtos (Frente)
                        set_color(ANSI_COLOR_GREEN); printf("--- Produtos na Lista (Frente) ---\n"); reset_color();
                        if (Lista_getSize(&minhaLista) == 0) {
                            set_color(ANSI_COLOR_YELLOW); printf("A lista esta vazia.\n"); reset_color();
                        } else {
                            // Iteração segura usando um nó temporário para evitar loop infinito
                            Node *temp = minhaLista.first;
                            while (temp != NULL) {
                                exibir_detalhes_produto(&(temp->produto));
                                temp = temp->next;
                            }
                        }
                        break;
                    }
                    case 6: { // Exibir Todos os Produtos (Trás)
                        set_color(ANSI_COLOR_GREEN); printf("--- Produtos na Lista (Tras) ---\n"); reset_color();
                        if (Lista_getSize(&minhaLista) == 0) {
                            set_color(ANSI_COLOR_YELLOW); printf("A lista esta vazia.\n"); reset_color();
                        } else {
                            // Iteração segura usando um nó temporário para evitar loop infinito
                            Node *temp = minhaLista.last;
                            while (temp != NULL) {
                                exibir_detalhes_produto(&(temp->produto));
                                temp = temp->prev;
                            }
                        }
                        break;
                    }
                    case 7: { // Navegar na Lista (Atual)
                        set_color(ANSI_COLOR_GREEN); printf("--- Navegacao na Lista ---\n"); reset_color();
                        if (Lista_getSize(&minhaLista) == 0) {
                            set_color(ANSI_COLOR_YELLOW); printf("A lista esta vazia. Nao ha o que navegar.\n"); reset_color();
                            break;
                        }
                        Lista_goFirst(&minhaLista); // Inicia a navegação do primeiro produto
                        int nav_key;
                        do {
                            clear_screen();
                            set_color(ANSI_COLOR_MAGENTA); printf("Navegando (Setas ESQ/DIR para mover, 'q' para sair):\n"); reset_color();
                            Produto *current_p = Lista_getCurrent(&minhaLista);
                            if (current_p != NULL) {
                                exibir_detalhes_produto(current_p);
                            } else {
                                // Isso pode acontecer se a lista ficar vazia durante a navegação
                                set_color(ANSI_COLOR_YELLOW); printf("Fim da lista ou lista vazia. Nao ha mais produtos para exibir.\n"); reset_color();
                            }
                            nav_key = read_key(); // Lê a tecla de navegação
                            if (nav_key == 1002) { // Seta para Direita (próximo)
                                if (!Lista_next(&minhaLista) && Lista_getCurrent(&minhaLista) != NULL) {
                                    set_color(ANSI_COLOR_YELLOW); printf("Ja esta no ultimo produto.\n"); reset_color();
                                }
                            } else if (nav_key == 1003) { // Seta para Esquerda (anterior)
                                if (!Lista_prev(&minhaLista) && Lista_getCurrent(&minhaLista) != NULL) {
                                    set_color(ANSI_COLOR_YELLOW); printf("Ja esta no primeiro produto.\n"); reset_color();
                                }
                            }
                        } while (nav_key != 'q' && nav_key != 'Q'); // Continua navegando até 'q' ser pressionado
                        break;
                    }
                    case 8: { // Tamanho da Lista
                        set_color(ANSI_COLOR_GREEN); printf("--- Tamanho da Lista ---\n"); reset_color();
                        printf("A lista contem %d produtos.\n", Lista_getSize(&minhaLista));
                        break;
                    }
                    case 9: // Sair do programa
                        running = false;
                        set_color(ANSI_COLOR_BLUE); printf("Saindo do programa. Ate mais!\n"); reset_color();
                        break;
                    default:
                        set_color(ANSI_COLOR_RED); printf("Opcao invalida. Por favor, selecione uma opcao valida.\n"); reset_color();
                        break;
                }
                if (running) { // Se o programa ainda estiver rodando (não selecionou "Sair")
                    printf("\nPressione qualquer tecla para continuar...");
                    read_key(); // Espera por qualquer tecla antes de voltar ao menu
                }
                break;
            default:
                // Ignora outras teclas que não sejam setas ou Enter para não atrapalhar a navegação
                break;
        }
    }

    Lista_destroi(&minhaLista); // Libera toda a memória alocada para a lista antes de encerrar
    return 0;
}
  
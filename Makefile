# Makefile simples para o projeto da lista duplamente ligada

# Compilador C
CC = gcc

# Flags do compilador
# -Wall: Habilita todos os avisos
# -Iinclude: Adiciona o diretório 'include' para busca de arquivos .h
# -g: Inclui informações de depuração
CFLAGS = -Wall -Iinclude -g

# Diretórios
SRC_DIR = src
INCLUDE_DIR = include
BIN_DIR = bin
OBJ_DIR = build

# Arquivos de objeto (agora inclui produto.o)
OBJS = $(OBJ_DIR)/main.o $(OBJ_DIR)/produto.o $(OBJ_DIR)/lista_dupla.o

# Nome do executável
TARGET = $(BIN_DIR)/gerenciador_produtos

# Regras "phony" para evitar conflitos com arquivos de mesmo nome
.PHONY: all clean run

# Regra padrão: compila tudo
all: $(TARGET)

# Regra para construir o executável
$(TARGET): $(OBJS)
	@mkdir -p $(BIN_DIR) # Garante que o diretório bin exista
	$(CC) $(OBJS) -o $@  # Linka os arquivos objeto para criar o executável

# Regra para compilar arquivos .c em .o
# $<: o primeiro pré-requisito (o arquivo .c)
# $@: o nome do alvo (o arquivo .o)
$(OBJ_DIR)/%.o: $(SRC_DIR)/%.c
	@mkdir -p $(OBJ_DIR) # Garante que o diretório build exista
	$(CC) $(CFLAGS) -c $< -o $@ # Compila o arquivo .c em .o

# Regra para limpar arquivos gerados pela compilação
clean:
	@rm -rf $(OBJ_DIR) $(BIN_DIR) # Remove os diretórios de objetos e executáveis

# Regra para compilar e executar o programa
run: $(TARGET)
	@./$(TARGET) # Executa o programa

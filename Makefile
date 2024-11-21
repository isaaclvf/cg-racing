TARGET = game

# Compilador
CC = gcc

# Flags de compilação
CFLAGS = -Wall -Wextra -std=c11 -O2

# Flags para linkar as bibliotecas OpenGL e GLUT
LDFLAGS = -lGL -lGLU -lglut -lm

# Arquivos fonte e objetos
SRC = main.c graphics.c
OBJ = $(SRC:.c=.o)

# Regra principal
all: $(TARGET)

# Regra para criar o executável
$(TARGET): $(OBJ)
	$(CC) $(OBJ) -o $(TARGET) $(LDFLAGS)

# Regra para compilar arquivos .c em .o
%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

# Limpeza dos arquivos temporários
clean:
	rm -f $(OBJ) $(TARGET)

# Regra para forçar a recompilação
rebuild: clean all


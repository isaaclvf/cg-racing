TARGET = game

CC = gcc

CFLAGS = -Wall -Wextra -std=c11 -O2

LDFLAGS = -lGL -lGLU -lglut -lm

SRC = main.c graphics.c
OBJ = $(SRC:.c=.o)

all: $(TARGET)

$(TARGET): $(OBJ)
	$(CC) $(OBJ) -o $(TARGET) $(LDFLAGS)

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -f $(OBJ) $(TARGET)

rebuild: clean all

run: all
	./$(TARGET)

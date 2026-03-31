CC = gcc
CFLAGS = -Wall -Wextra -g -I include

SRC = $(wildcard src/*.c)
OBJ = $(SRC:.c=.o)

server: $(OBJ)
	$(CC) $(CFLAGS) -o server $(OBJ)

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -f $(OBJ) server

.PHONY: clean

CC=gcc

CFLAGS=-Wall -Wextra -std=c99 -D_DEFAULT_SOURCE
LDFLAGS=-lm

OBJ=main.o avl.o factory.o histogram.o parser.o leaks.o

all: wildwater

wildwater: $(OBJ)
	$(CC) -o $@ $^ $(LDFLAGS)

# Cette règle s'assure que si un .h change, les .o sont refaits
%.o: %.c common.h avl.h factory.h histogram.h leaks.h parser.h
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -f *.o wildwater *.dat *.png
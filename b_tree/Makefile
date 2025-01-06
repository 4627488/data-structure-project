CC = gcc
CFLAGS = -Wall -g
TARGET = b_tree
OBJS = main.o b_tree.o

all: $(TARGET) prime_gen main

$(TARGET): $(OBJS)
	$(CC) $(CFLAGS) -o $(TARGET) $(OBJS)

prime_gen: prime_gen.o
	$(CC) $(CFLAGS) -o prime_gen prime_gen.o

prime_gen.o: prime_gen.c
	$(CC) $(CFLAGS) -c prime_gen.c

main.o: main.c b_tree.h
	$(CC) $(CFLAGS) -c main.c

b_tree.o: b_tree.c b_tree.h
	$(CC) $(CFLAGS) -c b_tree.c

main: main.o b_tree.o
	$(CC) $(CFLAGS) -o main main.o b_tree.o

clean:
	rm -f $(TARGET) $(OBJS) prime_gen prime_gen.o

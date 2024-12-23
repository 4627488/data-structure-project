CC = gcc
CFLAGS = -Wall -g
TARGET = main
OBJS = main.o avl_tree.o

all: $(TARGET)

$(TARGET): $(OBJS)
	$(CC) $(CFLAGS) -o $(TARGET) $(OBJS)

main.o: main.c avl_tree.h
	$(CC) $(CFLAGS) -c main.c

avl_tree.o: avl_tree.c avl_tree.h
	$(CC) $(CFLAGS) -c avl_tree.c

clean:
	rm -f $(TARGET) $(OBJS)

.PHONY: all clean

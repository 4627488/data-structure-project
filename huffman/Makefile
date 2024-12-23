CC = gcc
CFLAGS = -Wall -g
TARGET = huffman
SRCS = main.c huffman.c
OBJS = $(SRCS:.c=.o)

all: $(TARGET)

$(TARGET): $(OBJS)
	$(CC) $(CFLAGS) -o $(TARGET) $(OBJS)

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

test: $(TARGET)
	./$(TARGET)
	diff -q recode.txt source.txt && echo -e "\033[32mTest passed!\033[0m" || echo -e "\033[31mTest failed!\033[0m"

clean:
	rm -f $(OBJS) $(TARGET) recode.txt code.dat Huffman.txt

.PHONY: all clean test

.PHONY: all test

all: subway

subway: subway.cpp
	g++ -o subway subway.cpp

test: subway
	./subway < test/test.in > test/test.out
	diff -q test/test.out test/test.ans && echo -e "\033[32mTest passed!\033[0m" || echo -e "\033[31mTest failed!\033[0m"

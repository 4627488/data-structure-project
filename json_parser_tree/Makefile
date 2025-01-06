CXX = g++
CXXFLAGS = -std=c++17 -Wall
TARGET = json_parser
SRCS = JsonParser.cpp main.cpp
OBJS = $(SRCS:.cpp=.o)

all: $(TARGET)

$(TARGET): $(OBJS)
	$(CXX) $(CXXFLAGS) -o $(TARGET) $(OBJS)

%.o: %.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

clean:
	rm -f $(OBJS) $(TARGET)

test: $(TARGET)
	@echo "Running Test 1..."
	./$(TARGET) tests/test1.json tests/queries1.txt tests/expected1.txt -v
	@if [ $$? -eq 0 ]; then echo -e "\033[0;32mTest 1 passed\033[0m"; else echo -e "\033[0;31mTest 1 failed\033[0m"; fi
	@echo "Running Test 2..."
	./$(TARGET) tests/test2.json tests/queries2.txt tests/expected2.txt -v
	@if [ $$? -eq 0 ]; then echo -e "\033[0;32mTest 2 passed\033[0m"; else echo -e "\033[0;31mTest 2 failed\033[0m"; fi
	@echo "Running Test 3..."
	./$(TARGET) tests/test3.json tests/queries3.txt tests/expected3.txt -v
	@if [ $$? -eq 0 ]; then echo -e "\033[0;32mTest 3 passed\033[0m"; else echo -e "\033[0;31mTest 3 failed\033[0m"; fi

.PHONY: all clean

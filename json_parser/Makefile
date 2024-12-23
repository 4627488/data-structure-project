CXX = g++
CXXFLAGS = -Wall -std=c++11
TARGET = json_parser
SRC = main.cpp json_parser.cpp
HEADERS = json_parser.h

all: $(TARGET)

$(TARGET): $(SRC) $(HEADERS)
	$(CXX) $(CXXFLAGS) -o $(TARGET) $(SRC)

clean:
	rm -f $(TARGET)

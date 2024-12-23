CXX = g++
CXXFLAGS = -std=c++11 -Wall -I$(JSON_PARSER_DIR)
LDFLAGS = 

SRC_DIR = .
OBJ_DIR = ./obj
BIN_DIR = ./bin
JSON_PARSER_DIR = ../json_parser

SRC_FILES = $(wildcard $(SRC_DIR)/*.cpp) $(wildcard $(JSON_PARSER_DIR)/json_parser.cpp)
OBJ_FILES = $(patsubst $(SRC_DIR)/%.cpp, $(OBJ_DIR)/%.o, $(SRC_FILES))
TARGET = $(BIN_DIR)/family_tree

all: $(TARGET)

$(TARGET): $(OBJ_FILES) | $(BIN_DIR)
	$(CXX) $(CXXFLAGS) -o $@ $^ $(LDFLAGS)

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.cpp | $(OBJ_DIR)
	$(CXX) $(CXXFLAGS) -c $< -o $@

$(OBJ_DIR)/%.o: $(JSON_PARSER_DIR)/%.cpp | $(OBJ_DIR)
	$(CXX) $(CXXFLAGS) -c $< -o $@

$(OBJ_DIR):
	mkdir -p $(OBJ_DIR)

$(BIN_DIR):
	mkdir -p $(BIN_DIR)
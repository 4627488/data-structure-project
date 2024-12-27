#include "JsonParser.h"
#include <cassert>
#include <fstream>
#include <iostream>
#include <memory>
#include <sstream>

bool verbose = false;

void runTest(const std::string &jsonFile, const std::string &queryFile,
             const std::string &expectedFile) {
    std::ifstream jsonInput(jsonFile);
    std::ifstream queryInput(queryFile);
    std::ifstream expectedInput(expectedFile);

    if (!jsonInput.is_open() || !queryInput.is_open() || !expectedInput.is_open()) {
        std::cerr << "Unable to open test files" << std::endl;
        return;
    }

    std::string jsonString;
    std::string line;
    while (std::getline(jsonInput, line)) {
        jsonString += line;
    }
    jsonInput.close();

    JsonParser parser;
    auto root = parser.parse(jsonString);

    while (std::getline(queryInput, line)) {
        std::string expected;
        std::getline(expectedInput, expected);

        try {
            if (verbose) {
                std::cout << "Query: " << line << std::endl;
            }
            auto result = root->query(line);
            // 判断能否转换为JsonStringNode
            if (auto strNode = std::dynamic_pointer_cast<JsonStringNode>(result)) {
                assert("STRING " + strNode->value == expected);
            } else if (auto boolNode = std::dynamic_pointer_cast<JsonBoolNode>(result)) {
                assert(std::string("BOOL ") + (boolNode->value ? "true" : "false") == expected);
            } else {
                assert(expected == "OBJECT");
            }
        } catch (const std::invalid_argument &e) {
            assert(expected == "NOTEXIST");
        }
    }

    queryInput.close();
    expectedInput.close();
}

int main(int argc, char *argv[]) {
    if (argc < 4 || argc > 5) {
        std::cerr << "Usage: " << argv[0] << " <json-file> <query-file> <expected-file> [-v]"
                  << std::endl;
        return 1;
    }
    if (argc == 5 && std::string(argv[4]) == "-v") {
        verbose = true;
    }
    runTest(argv[1], argv[2], argv[3]);
    return 0;
}

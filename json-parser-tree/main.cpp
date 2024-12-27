#include "JsonParser.h"
#include <cassert>
#include <fstream>
#include <iostream>
#include <memory>
#include <sstream>

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
            auto result = root->query(line);
            if (result->children.empty()) {
                assert(expected == "OBJECT");
            } else {
                assert(expected == "STRING " + std::dynamic_pointer_cast<JsonStringNode>(result)->value);
            }
        } catch (const std::runtime_error &) {
            assert(expected == "NOTEXIST");
        }
    }

    queryInput.close();
    expectedInput.close();
}

int main() {
    try {
        runTest("tests/test1.json", "tests/queries1.txt", "tests/expected1.txt");
        runTest("tests/test2.json", "tests/queries2.txt", "tests/expected2.txt");
        runTest("tests/test3.json", "tests/queries3.txt", "tests/expected3.txt");
    } catch (const std::exception &e) {
        std::cerr << e.what() << std::endl;
    }
    return 0;
}

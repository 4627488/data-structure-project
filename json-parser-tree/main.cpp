#include "JsonParser.h"
#include <cassert>
#include <fstream>
#include <iostream>
#include <memory>
#include <sstream>

bool verbose = false;

std::string testJson(const std::string &json, const std::string &queries) {
    std::istringstream jsonStream(json);
    std::istringstream queryStream(queries);
    JsonParser parser;
    auto root = parser.parse(jsonStream.str());
    std::string result;
    std::string query;
    while (std::getline(queryStream, query)) {
        try {
            auto node = root->query(query);
            if (auto strNode =
                    std::dynamic_pointer_cast<JsonStringNode>(node)) {
                result += "STRING " + strNode->value + "\n";
            } else if (auto boolNode =
                           std::dynamic_pointer_cast<JsonBoolNode>(node)) {
                result += "BOOL " +
                          std::string(boolNode->value ? "true" : "false") +
                          "\n";
            } else {
                result += "OBJECT\n";
            }
        } catch (const std::invalid_argument &e) {
            result += "NOTEXIST\n";
        }
    }
    return result;
}

void runTest(const std::string &jsonFile, const std::string &queryFile,
             const std::string &expectedFile) {
    std::ifstream jsonInput(jsonFile);
    std::ifstream queryInput(queryFile);
    std::ifstream expectedInput(expectedFile);

    if (!jsonInput.is_open() || !queryInput.is_open() ||
        !expectedInput.is_open()) {
        std::cerr << "Unable to open test files" << std::endl;
        return;
    }

    std::string jsonString;
    std::string line;
    while (std::getline(jsonInput, line)) {
        jsonString += line;
    }
    jsonInput.close();

    std::string query;
    while (std::getline(queryInput, line)) {
        query += line + "\n";
    }
    queryInput.close();

    std::string expected;
    while (std::getline(expectedInput, line)) {
        expected += line + "\n";
    }
    expectedInput.close();

    auto result = testJson(jsonString, query);
    if (result == expected) {
        std::cout << "Test passed" << std::endl;
    } else {
        std::cerr << "Test failed" << std::endl;
        std::cerr << "Expected:" << std::endl;
        std::cerr << expected;
        std::cerr << "Got:" << std::endl;
        std::cerr << result;
        throw std::runtime_error("Test failed");
    }
}

int main(int argc, char *argv[]) {
    if (argc == 1) {
        int n, m;
        std::cin >> n >> m;
        std::string jsonFile, queryFile, expectedFile;
        for (int i = 0; i < n; i++) {
            std::string s;
            std::cin >> std::ws;
            std::getline(std::cin, s);
            jsonFile += s;
        }
        for (int i = 0; i < m; i++) {
            std::string s;
            std::cin >> std::ws;
            std::getline(std::cin, s);
            queryFile += s + "\n";
        }
        std::cout << testJson(jsonFile, queryFile);
        return 0;
    }
    if (argc < 4 || argc > 5) {
        std::cerr << "Usage: " << argv[0]
                  << " <json-file> <query-file> <expected-file> [-v]"
                  << std::endl;
        return 1;
    }
    if (argc == 5 && std::string(argv[4]) == "-v") {
        verbose = true;
    }
    runTest(argv[1], argv[2], argv[3]);
    return 0;
}

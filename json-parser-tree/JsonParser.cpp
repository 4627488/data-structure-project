#include "JsonParser.h"
#include <cassert>
#include <iostream>
#include <memory>
#include <sstream>
#include <string>
#include <unordered_map>
#include <vector>

extern bool verbose;

std::shared_ptr<JsonNode> JsonNode::getChild(const std::string &key) {
    if (verbose) {
        std::cout << "JsonNode::getChild(" << key << ")" << std::endl;
    }
    if (children.find(key) != children.end()) {
        return children[key];
    }
    throw std::invalid_argument("Invalid key");
}

std::vector<std::pair<std::string, std::shared_ptr<JsonNode>>> JsonNode::getChildren() {
    std::vector<std::pair<std::string, std::shared_ptr<JsonNode>>> childrenVec;
    for (const auto &child : children) {
        childrenVec.push_back(child);
    }
    return childrenVec;
}

std::shared_ptr<JsonNode> JsonNode::query(const std::string &queryStr) {
    std::istringstream stream(queryStr);
    std::string token;
    auto currentNode = shared_from_this();

    while (std::getline(stream >> std::ws, token, '.')) {
        currentNode = currentNode->getChild(token);
        if (verbose) {
            std::cerr << "JsonNode::query(" << queryStr << ") -> " << currentNode->to_string()
                      << std::endl;
        }
        if (!currentNode) {
            throw std::invalid_argument("Invalid query string");
        }
    }
    return currentNode;
}

std::string JsonNode::to_string() {
    std::string str;
    for (const auto &child : children) {
        str += child.first + ": " + child.second->to_string() + ", ";
    }
    str.pop_back(), str.pop_back();
    return "Object{" + str + "}";
}

JsonStringNode::JsonStringNode(const std::string &val) : value(val) {}

std::string JsonStringNode::to_string() {
    return "String(" + value + ")";
}

JsonListNode::JsonListNode(const std::vector<std::shared_ptr<JsonNode>> &val) : value(val) {}

std::string JsonListNode::to_string() {
    std::string str;
    for (const auto &val : value) {
        str += val->to_string() + ", ";
    }
    str.pop_back(), str.pop_back();
    return "List[" + str + "]";
}

JsonBoolNode::JsonBoolNode(bool val) : value(val) {}

std::string JsonBoolNode::to_string() {
    return "Bool(" + std::string(value ? "true" : "false") + ")";
}

std::shared_ptr<JsonNode> JsonParser::parseObject(std::istringstream &stream) {
    std::shared_ptr<JsonNode> node;
    char ch;
    stream >> std::ws;
    ch = stream.peek();
    if (ch == '{') {
        node = std::make_shared<JsonNode>();
        while (stream >> std::ws >> ch && ch != '}') {
            stream >> std::ws >> ch, assert(ch == '"');
            auto key = parseString(stream);
            stream >> std::ws >> ch, assert(ch == ':');
            auto valueNode = parseObject(stream);
            node->children[key] = valueNode;
        }
    } else if (ch == '[') {
        node = std::make_shared<JsonListNode>();
        while (stream >> std::ws >> ch && ch != ']') {
            auto valueNode = parseObject(stream);
            std::dynamic_pointer_cast<JsonListNode>(node)->value.push_back(valueNode);
        }
    } else if (ch == '"') {
        stream >> ch;
        auto value = parseString(stream);
        node = std::make_shared<JsonStringNode>(value);
    } else {
        std::string value;
        while (stream >> std::ws >> ch && ch != ',' && ch != '}') {
            value += ch;
        }
        stream.putback(ch);
        if (value == "true" || value == "false") {
            node = std::make_shared<JsonBoolNode>(value == "true");
        } else {
            throw std::invalid_argument("Invalid JSON");
        }
    }
    return node;
}

std::string JsonParser::parseString(std::istringstream &stream) {
    std::string str;
    char ch;
    while (stream >> ch) {
        if (ch == '"') {
            break;
        }
        if (ch == '\\') {
            stream >> ch;
        }
        str += ch;
    }
    return str;
}

std::shared_ptr<JsonNode> JsonParser::parse(const std::string &jsonString) {
    std::istringstream stream(jsonString);
    return parseObject(stream);
}

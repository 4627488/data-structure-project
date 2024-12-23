#ifndef JSON_PARSER_H
#define JSON_PARSER_H

#include <iostream>
#include <string>
#include <vector>
#include <list>
#include <stdexcept>
#include <sstream>

class HashTable
{
public:
    HashTable(size_t size = 101);
    void insert(const std::string &key, const std::string &value);
    std::string find(const std::string &key) const;

private:
    std::vector<std::list<std::pair<std::string, std::string>>> table;
    size_t hashFunction(const std::string &key) const;
};

class JsonParser
{
public:
    JsonParser(const std::string &jsonString);
    std::string query(const std::string &key) const;
    std::vector<std::string> queryList(const std::string &key) const;

private:
    HashTable jsonObject;
    void parse(const std::string &jsonString);
    void parseObject(std::istringstream &stream);
    std::string parseString(std::istringstream &stream) const;
    std::string parseObjectString(std::istringstream &stream);
    std::string parseBoolean(std::istringstream &stream);
    std::vector<std::string> parseList(std::istringstream &stream) const;
};

#endif // JSON_PARSER_H

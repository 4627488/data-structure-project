#include "json_parser.h"

// HashTable 类方法实现
HashTable::HashTable(size_t size) : table(size) {}

void HashTable::insert(const std::string &key, const std::string &value)
{
    size_t index = hashFunction(key) % table.size();
    for (auto &pair : table[index])
    {
        if (pair.first == key)
        {
            pair.second = value;
            return;
        }
    }
    table[index].emplace_back(key, value);
}

std::string HashTable::find(const std::string &key) const
{
    size_t index = hashFunction(key) % table.size();
    for (const auto &pair : table[index])
    {
        if (pair.first == key)
        {
            return pair.second;
        }
    }
    throw std::runtime_error("Key not found");
}

size_t HashTable::hashFunction(const std::string &key) const
{
    size_t hash = 0;
    for (char ch : key)
    {
        hash = 31 * hash + ch;
    }
    return hash;
}

// JsonParser 类方法实现
JsonParser::JsonParser(const std::string &jsonString)
{
    parse(jsonString);
}

std::string JsonParser::query(const std::string &key) const
{
    size_t dotPos = key.find('.');
    if (dotPos == std::string::npos)
    {
        return jsonObject.find(key);
    }
    else
    {
        std::string firstKey = key.substr(0, dotPos);
        std::string remainingKey = key.substr(dotPos + 1);
        std::string nestedJson = jsonObject.find(firstKey);
        JsonParser nestedParser(nestedJson);
        return nestedParser.query(remainingKey);
    }
}

void JsonParser::parse(const std::string &jsonString)
{
    std::istringstream stream(jsonString);
    char ch;
    while (stream >> ch)
    {
        if (ch == '{')
        {
            parseObject(stream);
        }
    }
}

void JsonParser::parseObject(std::istringstream &stream)
{
    std::string key, value;
    char ch;
    while (stream >> ch)
    {
        if (ch == '"')
        {
            key = parseString(stream);
            stream >> ch; // skip ':'
            stream >> ch; // skip whitespace
            if (ch == '"')
            {
                value = parseString(stream);
            }
            else if (ch == '{')
            {
                value = parseObjectString(stream);
            }
            jsonObject.insert(key, value);
        }
        else if (ch == '}')
        {
            return;
        }
    }
}

std::string JsonParser::parseString(std::istringstream &stream)
{
    std::string result;
    char ch;
    while (stream.get(ch))
    {
        if (ch == '\\') // escape sequence
        {
            stream.get(ch);
            if (ch == '"' || ch == '\\')
            {
                result += ch;
            }
            else if (ch == 'n')
            {
                result += '\n';
            }
            else if (ch == 't')
            {
                result += '\t';
            }
            else
            {
                throw std::runtime_error("Invalid escape sequence");
            }
        }
        else if (ch == '"')
        {
            break;
        }
        else
        {
            result += ch;
        }
    }
    return result;
}

std::string JsonParser::parseObjectString(std::istringstream &stream)
{
    std::string result = "{";
    char ch;
    while (stream.get(ch))
    {
        result += ch;
        if (ch == '}')
        {
            break;
        }
    }
    return result;
}

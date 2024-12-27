#ifndef JSONPARSER_H
#define JSONPARSER_H

#include <memory>
#include <sstream>
#include <string>
#include <unordered_map>
#include <vector>

class JsonNode : public std::enable_shared_from_this<JsonNode> {
  public:
    std::unordered_map<std::string, std::shared_ptr<JsonNode>> children;
    virtual ~JsonNode() = default;
    std::shared_ptr<JsonNode> getChild(const std::string &key);
    std::vector<std::pair<std::string, std::shared_ptr<JsonNode>>> getChildren();
    std::shared_ptr<JsonNode> query(const std::string &queryStr);
    virtual std::string to_string();
};

class JsonStringNode : public JsonNode {
  public:
    JsonStringNode(const std::string &val);
    std::string value;
    std::string to_string() override;
};

class JsonListNode : public JsonNode {
  public:
    JsonListNode() = default;
    JsonListNode(const std::vector<std::shared_ptr<JsonNode>> &val);
    std::vector<std::shared_ptr<JsonNode>> value;
    std::string to_string() override;
};

class JsonBoolNode : public JsonNode {
  public:
    JsonBoolNode(bool val);
    bool value;
    std::string to_string() override;
};

class JsonParser {
  public:
    std::shared_ptr<JsonNode> parse(const std::string &jsonString);

  private:
    std::shared_ptr<JsonNode> parseObject(std::istringstream &stream);
    std::string parseString(std::istringstream &stream);
};

#endif // JSONPARSER_H
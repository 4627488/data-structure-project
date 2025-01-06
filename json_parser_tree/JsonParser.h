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
    std::vector<std::pair<std::string, std::shared_ptr<JsonNode>>>
    getChildren();
    std::shared_ptr<JsonNode> query(const std::string &queryStr);
    std::shared_ptr<JsonNode> operator[](const std::string &key);
    virtual std::string to_string();
    virtual std::string display(int depth);
    virtual std::string display();
    virtual std::string to_json();
};

class JsonStringNode : public JsonNode {
  public:
    JsonStringNode(const std::string &val);
    std::string value;
    std::string to_string() override;
    std::string to_json() override;
    std::string display(int depth) override;
};

class JsonListNode : public JsonNode {
  public:
    JsonListNode();
    JsonListNode(const std::vector<std::shared_ptr<JsonNode>> &val);
    std::vector<std::shared_ptr<JsonNode>> value;
    std::string to_string() override;
    std::string to_json() override;
    std::string display(int depth) override;
};

class JsonBoolNode : public JsonNode {
  public:
    JsonBoolNode(bool val);
    bool value;
    std::string to_string() override;
    std::string to_json() override;
    std::string display(int depth) override;
};

class JsonParser {
  public:
    std::shared_ptr<JsonNode> parse(const std::string &jsonString);

  private:
    std::shared_ptr<JsonNode> parseObject(std::istringstream &stream);
    std::string parseString(std::istringstream &stream);
};

#endif // JSONPARSER_H
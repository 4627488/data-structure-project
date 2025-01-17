#ifndef FAMILYTREE_H
#define FAMILYTREE_H

#include "JsonParser.h"
#include "Member.h"
#include <map>
#include <memory>
#include <string>

class FamilyTree {
  public:
    std::map<std::string, Member>
        members; // 成员列表, key 为姓名, value 为成员对象
    std::map<std::string, std::vector<std::string>>
        children;         // 孩子列表, key 为父亲姓名, value 为孩子姓名列表
    std::string rootName; // 家谱根节点姓名

    void loadFromFile(const std::string &filename);    // 从文件加载数据
    void saveToFile(const std::string &filename);      // 保存数据到文件
    void displayFamilyTree();                          // 显示家谱
    void displayGeneration(int n);                     // 显示第 n 代所有人信息
    Member &findMemberByName(const std::string &name); // 按姓名查找成员
    void searchByBirthDate(const std::string &date);   // 按出生日期查询成员
    void determineRelationship(const std::string &name1,
                               const std::string &name2);       // 确定两人关系
    void addChild(const std::string &parentName, Member child); // 添加孩子
    void deleteMember(const std::string &name, bool isForce,
                      bool isRoot);               // 删除成员
    void modifyMember(const std::string &name);   // 修改成员信息
    bool verifyDate(const std::string &rootName); // 验证家族树的日期是否合法
  private:
    void displayTree(const std::string &name, int level,
                     std::vector<bool> isLast); // 递归显示家谱
    std::string loadFromJsonNode(
        const std::shared_ptr<JsonNode> &root); // 从 JsonNode 加载数据
    std::shared_ptr<JsonNode>
    buildJsonNode(const std::string &name); // 从成员构建 JsonNode
};

#endif // FAMILYTREE_H
#include "FamilyTree.h"
#include "JsonParser.h"
#include <fstream>
#include <functional>
#include <iostream>
#include <memory>
#include <sstream>
#include <unordered_map>

bool verbose = false;

std::string FamilyTree::loadFromJsonNode(const std::shared_ptr<JsonNode> &root) {
    std::string name = std::dynamic_pointer_cast<JsonStringNode>(root->getChild("name"))->value;
    Member member = {
        name,
        std::dynamic_pointer_cast<JsonStringNode>(root->getChild("birthDate"))->value,
        std::dynamic_pointer_cast<JsonBoolNode>(root->getChild("isMarried"))->value,
        std::dynamic_pointer_cast<JsonStringNode>(root->getChild("address"))->value,
        std::dynamic_pointer_cast<JsonBoolNode>(root->getChild("isAlive"))->value,
        std::dynamic_pointer_cast<JsonStringNode>(root->getChild("deathDate"))->value,
        "xxx"
        };
    std::shared_ptr<JsonListNode> childrenNode = std::dynamic_pointer_cast<JsonListNode>(root->getChild("children"));
    if (childrenNode) {
        for (const auto &child : childrenNode->value) {
            std::string childName = loadFromJsonNode(child);
            children[name].push_back(childName);
            members[childName].fatherName = name;
        }
    }
    members[name] = member;
    return name;
}

void FamilyTree::loadFromFile(const std::string &filename) {
    std::ifstream file(filename);
    if (!file.is_open()) {
        throw std::runtime_error("无法打开文件");
    }

    std::stringstream buffer;
    buffer << file.rdbuf();
    std::string jsonString = buffer.str();
    file.close();

    JsonParser parser;
    auto family = parser.parse(jsonString);
    std::cout << family->to_json() << std::endl;
    rootName = loadFromJsonNode(family);
}

void FamilyTree::saveToFile(const std::string &filename) {
    std::ofstream file(filename);
    if (!file.is_open()) {
        throw std::runtime_error("无法打开文件");
    }

    file << "{";
    file << "\"members\":[";
    for (auto it = members.begin(); it != members.end(); ++it) {
        if (it != members.begin()) {
            file << ",";
        }
        file << "\"" << it->first << "\"";
    }
    file << "],";

    for (auto it = members.begin(); it != members.end(); ++it) {
        if (it != members.begin()) {
            file << ",";
        }
        file << "\"" << it->first << "\":{";
        file << "\"name\":\"" << it->second.name << "\",";
        file << "\"birthDate\":\"" << it->second.birthDate << "\",";
        file << "\"isMarried\":\"" << (it->second.isMarried ? "true" : "false") << "\",";
        file << "\"address\":\"" << it->second.address << "\",";
        file << "\"isAlive\":\"" << (it->second.isAlive ? "true" : "false") << "\",";
        file << "\"deathDate\":\"" << it->second.deathDate << "\",";
        file << "\"fatherName\":\"" << it->second.fatherName << "\"";
        file << "}";
    }
    file << "}";
    file.close();
}

void FamilyTree::displayTree(const std::string &name, int level, std::vector<bool> isLast) {
    for (int i = 0; i < level; ++i) {
        if (i == level - 1) {
            std::cout << (isLast[i] ? "└──" : "├──");
        } else {
            std::cout << (isLast[i] ? "     " : "│    ");
        }
    }
    std::cout << name << " (" << members[name].birthDate << "~" << members[name].deathDate << ")"
              << std::endl;
    if (children.find(name) != children.end()) {
        for (size_t i = 0; i < children[name].size(); ++i) {
            isLast.push_back(i == children[name].size() - 1);
            displayTree(children[name][i], level + 1, isLast);
            isLast.pop_back();
        }
    }
}

void FamilyTree::displayFamilyTree() {
    if (!rootName.empty()) {
        displayTree(rootName, 0, {});
    } else {
        std::cerr << "家谱根节点未找到。" << std::endl;
    }
}

void FamilyTree::displayGeneration(int n) {
    std::function<void(const std::string &, int)> displayGen = [&](const std::string &name,
                                                                   int level) {
        if (level == n) {
            std::cout << "=== 第 " << n << " 代 ===" << std::endl;
            members[name].Print();
        }
        if (children.find(name) != children.end()) {
            for (const auto &child : children[name]) {
                displayGen(child, level + 1);
            }
        }
    };

    if (!rootName.empty()) {
        displayGen(rootName, 0);
    } else {
        std::cerr << "家谱根节点未找到。" << std::endl;
    }
}

Member &FamilyTree::findMemberByName(const std::string &name) {
    if (members.find(name) == members.end()) {
        throw std::runtime_error("未找到成员");
    }
    return members[name];
}

void FamilyTree::searchByBirthDate(const std::string &date) {
    for (const auto &member : members) {
        if (member.second.birthDate == date) {
            member.second.Print();
        }
    }
}

void FamilyTree::determineRelationship(const std::string &name1, const std::string &name2) {
    std::unordered_map<std::string, int> depth;
    std::function<void(const std::string &, int)> calculateDepth = [&](const std::string &name,
                                                                       int level) {
        depth[name] = level;
        if (children.find(name) != children.end()) {
            for (const auto &child : children[name]) {
                calculateDepth(child, level + 1);
            }
        }
    };

    if (!rootName.empty()) {
        calculateDepth(rootName, 0);
    } else {
        std::cerr << "家谱根节点未找到。" << std::endl;
        return;
    }

    if (depth.find(name1) == depth.end() || depth.find(name2) == depth.end()) {
        std::cerr << "未找到成员。" << std::endl;
        return;
    }

    int depth1 = depth[name1];
    int depth2 = depth[name2];

    if (depth1 == depth2) {
        std::cout << name1 << " 和 " << name2 << " 是同一代人。" << std::endl;
    } else if (depth1 < depth2) {
        std::cout << name1 << " 是 " << name2 << " 的长辈。" << std::endl;
    } else {
        std::cout << name2 << " 是 " << name1 << " 的长辈。" << std::endl;
    }
}

void FamilyTree::addChild(const std::string &parentName, Member child) {
    if (members.find(parentName) == members.end()) {
        throw std::runtime_error("未找到父母成员");
    }
    child.fatherName = parentName;
    members[child.name] = child;
    children[parentName].push_back(child.name);
}

void FamilyTree::deleteMember(const std::string &name) {
    if (members.find(name) == members.end()) {
        throw std::runtime_error("未找到成员");
    }

    // 递归删除成员的孩子
    if (children.find(name) != children.end()) {
        for (const auto &child : children[name]) {
            deleteMember(child);
        }
        children.erase(name);
    }

    // 从父母的孩子列表中删除该成员
    if (!members[name].fatherName.empty()) {
        auto &siblings = children[members[name].fatherName];
        for (auto it = siblings.begin(); it != siblings.end(); ++it) {
            if (*it == name) {
                siblings.erase(it);
                break;
            }
        }
    }

    // 删除成员
    members.erase(name);
}

void FamilyTree::modifyMember(const std::string &name) {
    if (members.find(name) == members.end()) {
        throw std::runtime_error("未找到成员");
    }

    Member &member = members[name];
    std::cout << "修改成员信息：" << std::endl;
    std::cout << "当前姓名：" << member.name << std::endl;
    std::cout << "请输入新姓名（按 Enter 保持不变）：";
    std::string newName;
    std::getline(std::cin >> std::ws, newName);
    if (!newName.empty()) {
        member.name = newName;
    }

    std::cout << "当前出生日期：" << member.birthDate << std::endl;
    std::cout << "请输入新出生日期（按 Enter 保持不变）：";
    std::string newBirthDate;
    std::getline(std::cin, newBirthDate);
    if (!newBirthDate.empty()) {
        member.birthDate = newBirthDate;
    }

    std::cout << "当前婚否：" << (member.isMarried ? "是" : "否") << std::endl;
    std::cout << "请输入新婚否（true/false，按 Enter 保持不变）：";
    std::string newIsMarried;
    std::getline(std::cin, newIsMarried);
    if (!newIsMarried.empty()) {
        member.isMarried = (newIsMarried == "true");
    }

    std::cout << "当前地址：" << member.address << std::endl;
    std::cout << "请输入新地址（按 Enter 保持不变）：";
    std::string newAddress;
    std::getline(std::cin, newAddress);
    if (!newAddress.empty()) {
        member.address = newAddress;
    }

    std::cout << "当前健在否：" << (member.isAlive ? "是" : "否") << std::endl;
    std::cout << "请输入新健在否（true/false，按 Enter 保持不变）：";
    std::string newIsAlive;
    std::getline(std::cin, newIsAlive);
    if (!newIsAlive.empty()) {
        member.isAlive = (newIsAlive == "true");
    }

    std::cout << "当前死亡日期：" << member.deathDate << std::endl;
    std::cout << "请输入新死亡日期（按 Enter 保持不变）：";
    std::string newDeathDate;
    std::getline(std::cin, newDeathDate);
    if (!newDeathDate.empty()) {
        member.deathDate = newDeathDate;
    }

    std::cout << "当前父亲姓名：" << member.fatherName << std::endl;
    std::cout << "请输入新父亲姓名（按 Enter 保持不变）：";
    std::string newFatherName;
    std::getline(std::cin, newFatherName);
    if (!newFatherName.empty()) {
        member.fatherName = newFatherName;
    }
}
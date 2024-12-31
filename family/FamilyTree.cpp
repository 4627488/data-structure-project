#include "FamilyTree.h"
#include "JsonParser.h"
#include <fstream>
#include <functional>
#include <iostream>
#include <memory>
#include <sstream>
#include <unordered_map>

bool verbose = false;

std::string
FamilyTree::loadFromJsonNode(const std::shared_ptr<JsonNode> &root) {
    std::string name =
        std::dynamic_pointer_cast<JsonStringNode>(root->getChild("name"))
            ->value;
    Member member = Member(
        name,
        std::dynamic_pointer_cast<JsonStringNode>(root->getChild("birthDate"))
            ->value,
        std::dynamic_pointer_cast<JsonBoolNode>(root->getChild("isMarried"))
            ->value,
        std::dynamic_pointer_cast<JsonStringNode>(root->getChild("address"))
            ->value,
        std::dynamic_pointer_cast<JsonBoolNode>(root->getChild("isAlive"))
            ->value,
        std::dynamic_pointer_cast<JsonStringNode>(root->getChild("deathDate"))
            ->value);
    std::shared_ptr<JsonListNode> childrenNode =
        std::dynamic_pointer_cast<JsonListNode>(root->getChild("children"));
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
    if (verbose) {
        std::cout << "家谱数据：" << std::endl;
        std::cout << family->to_json() << std::endl;
    }
    rootName = loadFromJsonNode(family);
}

std::shared_ptr<JsonNode> FamilyTree::buildJsonNode(const std::string &name) {
    auto node = std::make_shared<JsonNode>();
    auto childrenNode = std::make_shared<JsonListNode>();
    for (const auto &member : children[name]) {
        auto childNode = buildJsonNode(member);
        childrenNode->value.push_back(childNode);
    }
    node->children["name"] = std::make_shared<JsonStringNode>(name);
    node->children["birthDate"] =
        std::make_shared<JsonStringNode>(members[name].birthDate.toString());
    node->children["isMarried"] =
        std::make_shared<JsonBoolNode>(members[name].isMarried);
    node->children["address"] =
        std::make_shared<JsonStringNode>(members[name].address);
    node->children["isAlive"] =
        std::make_shared<JsonBoolNode>(members[name].isAlive);
    node->children["deathDate"] =
        std::make_shared<JsonStringNode>(members[name].deathDate.toString());
    node->children["children"] = childrenNode;
    return node;
}

void FamilyTree::saveToFile(const std::string &filename) {
    std::ofstream file(filename);
    if (!file.is_open()) {
        throw std::runtime_error("无法打开文件 " + filename);
    }
    auto family = buildJsonNode(rootName);
    file << family->to_json();
    file.close();
}

void FamilyTree::displayTree(const std::string &name, int level,
                             std::vector<bool> isLast) {
    for (int i = 0; i < level; ++i) {
        if (i == level - 1) {
            std::cout << (isLast[i] ? "└──" : "├──");
        } else {
            std::cout << (isLast[i] ? "     " : "│    ");
        }
    }
    std::cout << name << " (" << members[name].birthDate << "~"
              << members[name].deathDate << ")" << std::endl;
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
    std::cout << "=== 第 " << n << " 代 ===" << std::endl;
    std::function<void(const std::string &, int)> displayGen =
        [&](const std::string &name, int level) {
            if (level == n) {
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

void FamilyTree::searchByBirthDate(const std::string &dateStr) {
    Date date(dateStr);
    bool found = false;
    for (const auto &member : members) {
        if (member.second.birthDate == date) {
            found = true;
            member.second.Print();
        }
    }
    if (!found) {
        std::cout << "未找到出生日期为 " << date << " 的成员" << std::endl;
    }
}

// 确定两人关系：直系亲属、旁系亲属、非亲属
void FamilyTree::determineRelationship(const std::string &name1,
                                       const std::string &name2) {
    if (members.find(name1) == members.end() ||
        members.find(name2) == members.end()) {
        throw std::runtime_error("未找到成员" + name1 + "或" + name2);
    }

    std::function<bool(const std::string &, const std::string &)> isAncestor =
        [&](const std::string &ancestor, const std::string &descendant) {
            if (ancestor == descendant) {
                return true;
            }
            if (children.find(ancestor) == children.end()) {
                return false;
            }
            for (const auto &child : children[ancestor]) {
                if (isAncestor(child, descendant)) {
                    return true;
                }
            }
            return false;
        };

    if (isAncestor(name1, name2)) {
        std::cout << name1 << " 是 " << name2 << " 的祖先" << std::endl;
    } else if (isAncestor(name2, name1)) {
        std::cout << name1 << " 是 " << name2 << " 的后代" << std::endl;
    } else {
        std::cout << name1 << " 和 " << name2 << " 无直系亲属关系" << std::endl;
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
    std::cout << "修改成员信息（按回车保留原值）" << std::endl;
    std::cin.ignore();
    std::cout << "修改姓名(" + member.name + ")：";
    std::string newName;
    std::getline(std::cin, newName);
    if (!newName.empty()) {
        member.name = newName;
    }

    std::cout << "修改出生日期(" << member.birthDate << ")：";
    std::string newBirthDateStr;
    std::getline(std::cin, newBirthDateStr);
    if (!newBirthDateStr.empty()) {
        member.birthDate = Date(newBirthDateStr);
    }

    std::cout << "修改是否已婚(";
    if (member.isMarried) {
        std::cout << "Y/n)：";
    } else {
        std::cout << "y/N)：";
    }
    std::string isMarried;
    std::getline(std::cin, isMarried);
    if (!isMarried.empty()) {
        member.isMarried = isMarried == "Y" || isMarried == "y";
    }

    std::cout << "修改地址(" + member.address + ")：";
    std::string newAddress;
    std::getline(std::cin, newAddress);
    if (!newAddress.empty()) {
        member.address = newAddress;
    }

    std::cout << "修改是否在世(";
    if (member.isAlive) {
        std::cout << "Y/n)：";
    } else {
        std::cout << "y/N)：";
    }
    std::string isAlive;
    std::getline(std::cin, isAlive);
    if (!isAlive.empty()) {
        member.isAlive = isAlive == "Y" || isAlive == "y";
    }
    if (!member.isAlive) {
        std::cout << "修改死亡日期(" << member.deathDate << ")：";
        std::string newDeathDateStr;
        std::getline(std::cin, newDeathDateStr);
        if (!newDeathDateStr.empty()) {
            member.deathDate = Date(newDeathDateStr);
        }
    }

    std::cout << "修改父亲姓名(" + member.fatherName + ")：";
    std::string newFatherName;
    std::getline(std::cin, newFatherName);
    auto oldFatherName = member.fatherName;
    if (!newFatherName.empty()) {
        if (members.find(newFatherName) == members.end()) {
            throw std::runtime_error("未找到这个父亲成员");
        }
        member.fatherName = newFatherName;
    }

    // 更新父母的孩子列表
    if (!member.fatherName.empty()) {
        auto &siblings = children[member.fatherName];
        for (auto it = siblings.begin(); it != siblings.end(); ++it) {
            if (*it == name) {
                *it = newName;
                break;
            }
        }
        auto &oldSiblings = children[oldFatherName];
        for (auto it = oldSiblings.begin(); it != oldSiblings.end(); ++it) {
            if (*it == name) {
                oldSiblings.erase(it);
                break;
            }
        }
    }
}
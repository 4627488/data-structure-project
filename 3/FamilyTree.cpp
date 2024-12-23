#include "FamilyTree.h"
#include "json_parser.h"
#include <iostream>
#include <fstream>
#include <sstream>
#include <functional>

void FamilyTree::loadFromFile(const std::string& filename) {
    std::ifstream file(filename);
    if (!file.is_open()) {
        throw std::runtime_error("无法打开文件");
    }

    std::stringstream buffer;
    buffer << file.rdbuf();
    std::string jsonString = buffer.str();
    file.close();

    JsonParser parser(jsonString);
    std::vector<std::string> memberKeys = parser.queryList("members");
    for (const auto& key : memberKeys) {
        std::string memberJson = parser.query(key);
        JsonParser memberParser(memberJson);
        Member member(
            memberParser.query("name"),
            memberParser.query("birthDate"),
            memberParser.query("isMarried") == "true",
            memberParser.query("address"),
            memberParser.query("isAlive") == "true",
            memberParser.query("deathDate"),
            memberParser.query("fatherName")
        );
        members[key] = member;
    }

    for(const auto& member : members) {
        children[member.second.fatherName].push_back(member.first);
    }
}

void FamilyTree::saveToFile(const std::string& filename) {
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

void FamilyTree::displayFamilyTree() {
    std::function<void(const std::string&, int)> displayMember = [&](const std::string& name, int level) {
        for (int i = 0; i < level; ++i) {
            std::cout << "  ";
        }
        std::cout << "|-- " << name << std::endl;
        if (children.find(name) != children.end()) {
            for (const auto& child : children[name]) {
                displayMember(child, level + 1);
            }
        }
    };

    for (const auto& member : members) {
        if (member.second.fatherName.empty()) {
            displayMember(member.first, 0);
        }
    }
}

void FamilyTree::displayGeneration(int n) {
    std::function<void(const std::string&, int)> displayGen = [&](const std::string& name, int level) {
        if (level == n) {
            std::cout << "=== 第 " << n << " 代 ===" << std::endl;
            members[name].Print();
        }
        if (children.find(name) != children.end()) {
            for (const auto& child : children[name]) {
                displayGen(child, level + 1);
            }
        }
    };

    for (const auto& member : members) {
        if (member.second.fatherName.empty()) {
            displayGen(member.first, 0);
        }
    }
}

Member& FamilyTree::findMemberByName(const std::string& name) {
    if (members.find(name) == members.end()) {
        throw std::runtime_error("Member not found");
    }
    return members[name];
}

void FamilyTree::searchByBirthDate(const std::string& date) {
    // ...实现按出生日期查询成员的功能...
}

void FamilyTree::determineRelationship(const std::string& name1, const std::string& name2) {
    // ...实现确定两人关系的功能...
}

void FamilyTree::addChild(const std::string& parentName, Member child) {
    // ...实现添加孩子的功能...
}

void FamilyTree::deleteMember(const std::string& name) {
    // ...实现删除成员的功能...
}

void FamilyTree::modifyMember(const std::string& name) {
    // ...实现修改成员信息的功能...
}

// ...其他成员函数实现...
#include "FamilyTree.h"
#include "json_parser.h"
#include <iostream>
#include <fstream>
#include <sstream>

FamilyTree::FamilyTree() {
    // ...初始化...
}

FamilyTree::~FamilyTree() {
    // ...释放资源...
}

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
    // ...实现显示家谱的功能...
}

void FamilyTree::displayGeneration(int n) {
    // ...实现显示第 n 代所有人信息的功能...
}

Member* FamilyTree::findMemberByName(const std::string& name) {
    // ...实现按姓名查找成员的功能...
    return nullptr;
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
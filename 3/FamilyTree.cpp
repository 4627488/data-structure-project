#include "FamilyTree.h"
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
        std::cerr << "无法打开文件: " << filename << std::endl;
        return;
    }

    std::string line;
    while (std::getline(file, line)) {
        std::istringstream iss(line);
        std::string name, birthDate, address, deathDate, fatherName;
        bool isMarried, isAlive;

        std::getline(iss, name, '|');
        std::getline(iss, birthDate, '|');
        iss >> isMarried;
        iss.ignore(1, '|');
        std::getline(iss, address, '|');
        iss >> isAlive;
        iss.ignore(1, '|');
        std::getline(iss, deathDate, '|');
        std::getline(iss, fatherName, '|');

        Member* member = new Member(name, birthDate, isMarried, address, isAlive, deathDate, nullptr);
        members[name] = member;

        if (!fatherName.empty()) {
            member->father = members[fatherName];
        }
    }

    file.close();
}

void FamilyTree::saveToFile(const std::string& filename) {
    std::ofstream file(filename);
    if (!file.is_open()) {
        std::cerr << "无法打开文件: " << filename << std::endl;
        return;
    }

    for (const auto& pair : members) {
        Member* member = pair.second;
        file << member->name << '|'
             << member->birthDate << '|'
             << member->isMarried << '|'
             << member->address << '|'
             << member->isAlive << '|'
             << member->deathDate << '|'
             << (member->father ? member->father->name : "") << '\n';
    }

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

void FamilyTree::addChild(const std::string& parentName, Member* child) {
    // ...实现添加孩子的功能...
}

void FamilyTree::deleteMember(const std::string& name) {
    // ...实现删除成员的功能...
}

void FamilyTree::modifyMember(const std::string& name) {
    // ...实现修改成员信息的功能...
}

// ...其他成员函数实现...
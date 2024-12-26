#include "Member.h"
#include <iostream>

Member::Member() : name(""), isMarried(false), isAlive(true) {
}

Member::Member(const std::string &name) : name(name), isMarried(false), isAlive(true) {
}

Member::Member(const std::string &name, const std::string &birthDate, bool isMarried, const std::string &address,
	       bool isAlive, const std::string &deathDate, const std::string &fatherName)
    : name(name), birthDate(birthDate), isMarried(isMarried), address(address), isAlive(isAlive), deathDate(deathDate),
      fatherName(fatherName) {
}

Member::Member(const Member &member)
    : name(member.name), birthDate(member.birthDate), isMarried(member.isMarried), address(member.address),
      isAlive(member.isAlive), deathDate(member.deathDate), fatherName(member.fatherName) {
}

Member &Member::operator=(const Member &member) {
    if (this != &member) {
	name = member.name;
	birthDate = member.birthDate;
	isMarried = member.isMarried;
	address = member.address;
	isAlive = member.isAlive;
	deathDate = member.deathDate;
	fatherName = member.fatherName;
    }
    return *this;
}

void Member::Print() const {
    std::cout << "=== 成员信息 ===" << std::endl;
    std::cout << "姓名：" << name << std::endl;
    std::cout << "出生日期：" << birthDate << std::endl;
    std::cout << "婚否：" << (isMarried ? "是" : "否") << std::endl;
    std::cout << "地址：" << address << std::endl;
    std::cout << "健在否：" << (isAlive ? "是" : "否") << std::endl;
    if (!isAlive) {
	std::cout << "死亡日期：" << deathDate << std::endl;
    }
    if (!fatherName.empty()) {
	std::cout << "父亲姓名：" << fatherName << std::endl;
    } else {
	std::cout << "是家谱根节点" << std::endl;
    }
}
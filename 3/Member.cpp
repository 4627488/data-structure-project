#include "Member.h"
#include <iostream>

Member::Member()
    : name(""), isMarried(false), isAlive(true)
{
}

Member::Member(const std::string &name)
    : name(name), isMarried(false), isAlive(true)
{
}

Member::Member(const std::string &name, const std::string &birthDate, bool isMarried, const std::string &address, bool isAlive, const std::string &deathDate, const std::string &fatherName)
    : name(name), birthDate(birthDate), isMarried(isMarried), address(address), isAlive(isAlive), deathDate(deathDate), fatherName(fatherName)
{
}

void Member::Print()
{
    std::cout << "姓名：" << name << std::endl;
    std::cout << "出生日期：" << birthDate << std::endl;
    std::cout << "婚否：" << (isMarried ? "是" : "否") << std::endl;
    std::cout << "地址：" << address << std::endl;
    std::cout << "健在否：" << (isAlive ? "是" : "否") << std::endl;
    std::cout << "死亡日期：" << deathDate << std::endl;
    std::cout << "父亲姓名：" << fatherName << std::endl;
}
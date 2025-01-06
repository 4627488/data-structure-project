#include "Member.h"
#include <iostream>
#include <iomanip>

Member::Member()
    : name(""), birthDate(), deathDate(), isAlive(true), isMarried(false),
      address(""), fatherName("")
{
}

Member::Member(const std::string &name)
    : name(name), birthDate(), deathDate(), isAlive(true), isMarried(false),
      address(""), fatherName("")
{
}

Member::Member(const std::string &name, const std::string &birthDate,
               bool isMarried, const std::string &address, bool isAlive,
               const std::string &deathDate)
    : name(name), birthDate(Date(birthDate)), deathDate(Date(deathDate)),
      isAlive(isAlive), isMarried(isMarried), address(address), fatherName("")
{
}

Member::Member(const Member &member)
    : name(member.name), birthDate(member.birthDate),
      deathDate(member.deathDate), isAlive(member.isAlive),
      isMarried(member.isMarried), address(member.address),
      fatherName(member.fatherName)
{
}

Member &Member::operator=(const Member &member)
{
    if (this != &member)
    {
        name = member.name;
        birthDate = member.birthDate;
        deathDate = member.deathDate;
        isAlive = member.isAlive;
        isMarried = member.isMarried;
        address = member.address;
        fatherName = member.fatherName;
    }
    return *this;
}

size_t utf8StrLen(const std::string &str)
{
    size_t len = 0;
    for (size_t i = 0; i < str.size();)
    {
        unsigned char ch = str[i];
        if (ch >= 0x80)
        { // 中文字符或者多字节字符
            // 判断字符的字节数，UTF-8中文字符是3个字节
            if ((ch & 0xE0) == 0xC0)
            {
                len += 2;
                i += 2;
            }
            else if ((ch & 0xF0) == 0xE0)
            {
                len += 2;
                i += 3;
            }
            else if ((ch & 0xF8) == 0xF0)
            {
                len += 2;
                i += 4;
            }
        }
        else
        { // 单字节字符
            len++;
            i++;
        }
    }
    return len;
}

template <typename... Args> std::string fillwith(size_t len, Args... args)
{
    std::string filledStr;
    (filledStr.append(args), ...);
    size_t strLen = utf8StrLen(filledStr);
    if (strLen < len)
    {
        filledStr.append(len - strLen, ' ');
    }
    return filledStr;
}

void Member::Print() const
{
    // ┌ ─ ┬ ┐ │ ├ ┤ └ ┴ ┘
    std::cout << "┌──────成员信息──────┐" << std::endl;
    std::cout << "│" << fillwith(20, "姓名：", name) << "│" << std::endl;
    std::cout << "│" << fillwith(20, "出生日期：", birthDate.toString()) << "│"
              << std::endl;
    std::cout << "│" << fillwith(20, "是否已婚：", isMarried ? "是" : "否")
              << "│" << std::endl;
    std::cout << "│" << fillwith(20, "地址：", address) << "│" << std::endl;
    std::cout << "│" << fillwith(20, "是否在世：", isAlive ? "是" : "否") << "│"
              << std::endl;
    if (!isAlive)
    {
        std::cout << "│" << fillwith(20, "死亡日期：", deathDate.toString())
                  << "│" << std::endl;
    }
    if (!fatherName.empty())
    {
        std::cout << "│" << fillwith(20, "父亲姓名：", fatherName) << "│"
                  << std::endl;
    }
    else
    {
        std::cout << "│" << fillwith(20, "父亲姓名：", "无") << "│"
                  << std::endl;
    }
    std::cout << "└────────────────────┘" << std::endl;
}
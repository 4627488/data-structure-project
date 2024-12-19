#ifndef MEMBER_H
#define MEMBER_H

#include <string>
#include <vector>

class Member {
public:
    std::string name;            // 姓名
    std::string birthDate;       // 出生日期
    bool isMarried;              // 婚否
    std::string address;         // 地址
    bool isAlive;                // 健在否
    std::string deathDate;       // 死亡日期
    std::vector<Member*> children; // 孩子

    Member(const std::string& name);
    Member(const std::string& name, const std::string& birthDate, bool isMarried, const std::string& address, bool isAlive, const std::string& deathDate);

    void Print();

    // ...其他成员函数声明...
};

#endif // MEMBER_H
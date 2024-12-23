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
    std::string fatherName;      // 父亲姓名，如果没有父亲则为空

    Member();
    Member(const Member& member);
    Member(const std::string& name);
    Member(const std::string& name, const std::string& birthDate, bool isMarried, const std::string& address, bool isAlive, const std::string& deathDate, const std::string& fatherName);

    Member& operator=(const Member& member);

    void Print() const;

    // ...其他成员函数声明...
};

#endif // MEMBER_H
#ifndef MEMBER_H
#define MEMBER_H

#include "Date.h"
#include <string>
#include <vector>

class Member {
  public:
    std::string name;       // 姓名
    Date birthDate;         // 出生日期
    Date deathDate;         // 死亡日期
    bool isAlive;           // 是否在世
    bool isMarried;         // 婚否
    std::string address;    // 地址
    std::string fatherName; // 父亲姓名，如果没有父亲则为空

    Member();
    Member(const Member &member);
    Member(const std::string &name);
    Member(const std::string &name, const std::string &birthDate, bool isMarried,
           const std::string &address, bool isAlive, const std::string &deathDate);

    Member &operator=(const Member &member);

    void Print() const;
};

#endif // MEMBER_H
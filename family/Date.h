#ifndef DATE_H
#define DATE_H

#include <string>
#include <ostream>

class Date {
  public:
    int year;
    int month;
    int day;

    Date() : year(0), month(0), day(0) {}

    Date(int y, int m, int d) : year(y), month(m), day(d) {}
    Date(const std::string &dateStr); // 从字符串解析日期，格式为 "YYYY-MM-DD"

    std::string toString() const; // 将日期转换为字符串，格式为 "YYYY-MM-DD"

    friend std::ostream &operator<<(std::ostream &os, const Date &date); // 重载输出运算符

    bool operator==(const Date &date) const; // 重载相等运算符
    bool operator<(const Date &date) const;
};

#endif // DATE_H

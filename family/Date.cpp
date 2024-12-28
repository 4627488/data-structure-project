#include "Date.h"
#include <sstream>
#include <iomanip>
#include <stdexcept>

Date::Date(const std::string &dateStr) {
    if (dateStr.empty()) {
        year = month = day = 0;
        return;
    }
    std::istringstream iss(dateStr);
    char delimiter;
    if (!(iss >> year >> delimiter >> month >> delimiter >> day) || delimiter != '-' || month < 1 || month > 12 || day < 1 || day > 31) {
        throw std::invalid_argument("Invalid date format");
    }
}

std::string Date::toString() const {
    if (year == 0 && month == 0 && day == 0) {
        return "";
    }
    std::ostringstream oss;
    oss << std::setw(4) << std::setfill('0') << year << '-'
        << std::setw(2) << std::setfill('0') << month << '-'
        << std::setw(2) << std::setfill('0') << day;
    return oss.str();
}

std::ostream &operator<<(std::ostream &os, const Date &date) {
    os << date.toString();
    return os;
}

bool Date::operator==(const Date &date) const {
    return year == date.year && month == date.month && day == date.day;
}

// a < b => a 在 b 之前
bool Date::operator<(const Date &date) const {
    if (year != date.year) {
        return year < date.year;
    }
    if (month != date.month) {
        return month < date.month;
    }
    return day < date.day;
}

#include "FamilyTree.h"
#include <iostream>

int main() {
    FamilyTree familyTree;
    familyTree.loadFromFile("familyData.txt");

    int choice;
    do {
        std::cout << "======= 家谱管理系统 =======" << std::endl;
        std::cout << "1. 显示家谱" << std::endl;
        std::cout << "2. 显示第 n 代所有人信息" << std::endl;
        std::cout << "3. 按姓名查询成员信息" << std::endl;
        std::cout << "4. 按出生日期查询成员名单" << std::endl;
        std::cout << "5. 确定两人关系" << std::endl;
        std::cout << "6. 添加成员孩子" << std::endl;
        std::cout << "7. 删除成员" << std::endl;
        std::cout << "8. 修改成员信息" << std::endl;
        std::cout << "9. 保存并退出" << std::endl;
        std::cout << "请选择功能编号：";
        std::cin >> choice;

        switch (choice) {
            case 1:
                familyTree.displayFamilyTree();
                break;
            case 2: {
                int generation;
                std::cout << "请输入代数：";
                std::cin >> generation;
                familyTree.displayGeneration(generation);
                break;
            }
            case 3: {
                std::string name;
                std::cout << "请输入姓名：";
                std::cin >> name;
                Member* member = familyTree.findMemberByName(name);
                if (member) {
                    std::cout << "姓名：" << member->name << std::endl;
                    std::cout << "出生日期：" << member->birthDate << std::endl;
                    std::cout << "婚否：" << (member->isMarried ? "是" : "否") << std::endl;
                    std::cout << "地址：" << member->address << std::endl;
                    std::cout << "健在否：" << (member->isAlive ? "是" : "否") << std::endl;
                    std::cout << "死亡日期：" << member->deathDate << std::endl;
                } else {
                    std::cout << "未找到该成员。" << std::endl;
                }
                break;
            }
            case 4: {
                std::string birthDate;
                std::cout << "请输入出生日期：";
                std::cin >> birthDate;
                familyTree.searchByBirthDate(birthDate);
                break;
            }
            case 5: {
                std::string name1, name2;
                std::cout << "请输入第一个人的姓名：";
                std::cin >> name1;
                std::cout << "请输入第二个人的姓名：";
                std::cin >> name2;
                familyTree.determineRelationship(name1, name2);
                break;
            }
            case 6: {
                std::string parentName, childName;
                std::cout << "请输入父母的姓名：";
                std::cin >> parentName;
                std::cout << "请输入孩子的姓名：";
                std::cin >> childName;
                Member* child = new Member(childName);
                familyTree.addChild(parentName, child);
                break;
            }
            case 7: {
                std::string name;
                std::cout << "请输入要删除的成员姓名：";
                std::cin >> name;
                familyTree.deleteMember(name);
                break;
            }
            case 8: {
                std::string name;
                std::cout << "请输入要修改的成员姓名：";
                std::cin >> name;
                familyTree.modifyMember(name);
                break;
            }
            case 9:
                familyTree.saveToFile("familyData.txt");
                std::cout << "数据已保存，程序退出。" << std::endl;
                break;
            default:
                std::cout << "无效的选择，请重新输入。" << std::endl;
                break;
        }
    } while (choice != 9);

    return 0;
}
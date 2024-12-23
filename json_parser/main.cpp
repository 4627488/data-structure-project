#include "json_parser.h"
#include <fstream>
#include <cassert>

int main()
{
    try
    {
        // 测试基本功能
        std::string jsonString2 = R"({"name": "John", "age": "30", "city": "New York"})";
        JsonParser parser2(jsonString2);
        assert(parser2.query("name") == "John");
        assert(parser2.query("age") == "30");
        assert(parser2.query("city") == "New York");
        std::cout << "name: " << parser2.query("name") << std::endl;
        std::cout << "age: " << parser2.query("age") << std::endl;
        std::cout << "city: " << parser2.query("city") << std::endl;

        std::string jsonString3 = R"({"key1": "value1", "key2": "value2", "key3": "value3"})";
        JsonParser parser3(jsonString3);
        assert(parser3.query("key1") == "value1");
        assert(parser3.query("key2") == "value2");
        assert(parser3.query("key3") == "value3");
        std::cout << "key1: " << parser3.query("key1") << std::endl;
        std::cout << "key2: " << parser3.query("key2") << std::endl;
        std::cout << "key3: " << parser3.query("key3") << std::endl;

        std::string filename = "test1.json";
        std::ifstream file(filename);
        std::string jsonString4;
        if (file.is_open())
        {
            std::string line;
            while (std::getline(file, line))
            {
                jsonString4 += line;
            }
            file.close();
        }
        else
        {
            std::cerr << "Unable to open file " << filename << std::endl;
            return 1;
        }
        // 测试转义字符和对象类型
        JsonParser parser4(jsonString4);
        assert(parser4.query("address.city") == "NewYork");
        assert(parser4.query("esc\\aped") == "\"hello\"");
        std::cout << "address.city: " << parser4.query("address.city") << std::endl;
        std::cout << "esc\\aped: " << parser4.query("esc\\aped") << std::endl;
        std::cout << "\033[32mAll tests passed\033[0m" << std::endl;
    }
    catch (const std::exception &e)
    {
        std::cerr << "Exception: " << e.what() << std::endl;
        return 1;
    }

    return 0;
}
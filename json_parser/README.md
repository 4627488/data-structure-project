# JSON Parser

## 项目简介

这是一个简单的JSON解析器项目，使用C++实现。该解析器能够解析JSON字符串并支持查询嵌套的JSON对象。

## 文件结构

- `main.cpp`: 主程序文件，包含测试用例和程序入口。
- `json_parser.h`: 头文件，包含JSON解析器和哈希表的类定义和声明。
- `json_parser.cpp`: 源文件，包含JSON解析器和哈希表的类方法实现。
- `README.md`: 项目说明文件。

## 使用方法

1. 编译项目：
    ```sh
    make
    ```

2. 运行项目：
    ```sh
    ./json_parser
    ```

## 示例

以下是一些示例JSON字符串及其查询结果：

```cpp
std::string jsonString = R"({"name": "John", "age": "30", "city": "New York"})";
JsonParser parser(jsonString);
std::cout << "name: " << parser.query("name") << std::endl; // 输出: John
std::cout << "age: " << parser.query("age") << std::endl;   // 输出: 30
std::cout << "city: " << parser.query("city") << std::endl; // 输出: New York
```

## 测试

项目中包含了一些测试用例，可以通过运行编译后的程序来验证解析器的功能。

```sh
./json_parser
```

如果所有测试通过，程序将输出 "All tests passed"。

## 注意事项

- 该解析器目前支持简单的JSON对象、字符串类型的值、布尔值和列表。
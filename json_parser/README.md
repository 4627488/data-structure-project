# 【18】树的应用 (选做)（树）

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

## 数据结构

该项目主要使用了哈希表（HashTable）来存储解析后的JSON键值对。

## 算法设计思想

1. 解析JSON字符串，将其转换为键值对存储在哈希表中。
2. 支持嵌套JSON对象的查询，通过递归解析嵌套的JSON字符串。
3. 支持列表类型的值，通过解析列表字符串并存储为字符串数组。

## 源程序

源程序包括以下文件：
- `main.cpp`: 包含测试用例和程序入口。
- `json_parser.h`: 包含JSON解析器和哈希表的类定义和声明。
- `json_parser.cpp`: 包含JSON解析器和哈希表的类方法实现。

## 测试数据和结果

测试数据存储在 `tests` 目录下，包括JSON文件、查询文件和预期结果文件。运行测试程序将验证解析器的功能。

## 算法的时间复杂度

解析JSON字符串的时间复杂度为O(n)，其中n是JSON字符串的长度。查询操作的时间复杂度为O(1)，因为哈希表的查找操作平均时间复杂度为O(1)。

## 改进方法

1. **优化哈希函数**：使用更复杂的哈希函数来减少冲突，提高查找效率。例如，使用MurmurHash或CityHash等高效哈希函数。
2. **处理冲突的方法**：采用链地址法（Separate Chaining）来处理哈希冲突。每个哈希桶存储一个链表，链表中存储所有哈希值相同的键值对。
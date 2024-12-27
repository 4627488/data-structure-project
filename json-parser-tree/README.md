# JSON Parser Tree

## 简介

这是一个简单的 JSON 解析器，使用树结构来表示 JSON 数据。支持解析 JSON 对象、字符串、列表和布尔值，并提供查询功能来访问嵌套的 JSON 数据。

## 算法设计

### JSON 解析

解析器使用递归下降解析法来解析 JSON 字符串。主要的解析函数是 `JsonParser::parseObject`，它根据当前字符的类型来决定如何解析：

- 如果当前字符是 `{`，则解析为 JSON 对象。
- 如果当前字符是 `[`，则解析为 JSON 列表。
- 如果当前字符是 `"`，则解析为 JSON 字符串。
- 否则，解析为 JSON 布尔值或其他字符串。

解析过程中，使用 `std::istringstream` 来逐字符读取输入，并根据 JSON 语法规则进行解析。

### 查询功能

查询功能使用点分隔的字符串来访问嵌套的 JSON 数据。主要的查询函数是 `JsonNode::query`，它将查询字符串拆分为多个键，并逐层访问对应的子节点。

### 数据结构

- `JsonNode`：表示 JSON 节点的基类，包含一个子节点的哈希表。
- `JsonStringNode`：继承自 `JsonNode`，表示 JSON 字符串节点。
- `JsonListNode`：继承自 `JsonNode`，表示 JSON 列表节点。
- `JsonBoolNode`：继承自 `JsonNode`，表示 JSON 布尔值节点。

## 算法复杂度

### 解析复杂度

解析 JSON 字符串的时间复杂度为 O(n)，其中 n 是 JSON 字符串的长度。每个字符只被读取和处理一次。

### 查询复杂度

查询嵌套 JSON 数据的时间复杂度为 O(m)，其中 m 是查询字符串中键的数量。每个键对应一次哈希表查找操作，平均时间复杂度为 O(1)。

## 使用方法

编译项目：

```sh
make
```

运行程序：

```sh
./json_parser
```

示例 JSON 字符串：

```json
{
    "firstName": "John",
    "lastName": "Smith",
    "address": {
        "streetAddress": "2ndStreet",
        "city": "NewYork",
        "state": "NY"
    },
    "esc\\aped": "\"hello\""
}
```

示例查询：

```cpp
auto result = root->query("address.zip");
std::cout << result->to_string() << std::endl;
```
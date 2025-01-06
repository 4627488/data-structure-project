# 【18】树的应用 (选做)（树）

## 简介

这是一个简单的 JSON 解析器，使用树结构来表示 JSON 数据。支持解析 JSON 对象、字符串、列表和布尔值，并提供查询功能来访问嵌套的 JSON 数据。

## 算法设计

### JSON 解析

解析器使用递归下降解析法来解析 JSON 字符串。主要的解析函数是 `JsonParser::parseObject`，它根据当前字符的类型来决定如何解析：

- 如果当前字符是 `{`，则解析为 JSON 对象。
- 如果当前字符是 `[`，则解析为 JSON 列表。
- 如果当前字符是 `"`，则解析为 JSON 字符串。
- 否则，解析为 JSON 布尔值或其他字符串。

解析过程中，使用 `std::istringstream` 来逐字符读取输入，并根据 JSON 语法规则进行解析。解析器会递归调用自身来解析嵌套的 JSON 数据。

### 查询功能

查询功能使用点分隔的字符串来访问嵌套的 JSON 数据。主要的查询函数是 `JsonNode::query`，它将查询字符串拆分为多个键，并逐层递归访问对应的子节点。

### 数据结构

`JsonNode`类：这是一个基类，表示一个JSON对象。它包含了一个`children`成员变量，这是一个`unordered_map`，用于存储子节点。`getChild`方法根据键值返回对应的子节点，如果键不存在则抛出异常。`to_string`方法将节点及其子节点转换为字符串表示，`to_json`方法将节点及其子节点转换为JSON格式的字符串。

`JsonStringNode`类：这是一个派生类，表示一个JSON字符串节点。它包含一个`value`成员变量，用于存储字符串值。`to_string`方法返回字符串的表示形式，`to_json`方法返回JSON格式的字符串。

`JsonListNode`类：这是一个派生类，表示一个JSON数组节点。它包含一个`value`成员变量，这是一个`vector`，用于存储数组中的元素。`to_string`方法返回数组的字符串表示，`to_json`方法返回数组的JSON格式字符串。

`JsonBoolNode`类：这是一个派生类，表示一个JSON布尔值节点。它包含一个`value`成员变量，用于存储布尔值。`to_string`方法返回布尔值的字符串表示，`to_json`方法返回布尔值的JSON格式字符串。

`JsonParser`类：这是一个解析器类，用于解析JSON字符串并生成相应的JSON节点对象。`parseObject`方法根据输入流解析JSON对象、数组、字符串和布尔值，并返回相应的节点对象。`parseString`方法解析JSON字符串并返回解析后的字符串。

## 算法复杂度

### 解析复杂度

解析 JSON 字符串的时间复杂度为 $O(n)$，其中 $n$ 是 JSON 字符串的长度。每个字符只被读取和处理一次。

### 查询复杂度

查询嵌套 JSON 数据的时间复杂度为 $O(m)$，其中 $m$ 是查询字符串中键的数量。每个键对应一次哈希表查找操作，平均时间复杂度为 $O(1)$。

## 使用方法

### 编译&运行：

```sh
make && ./json_parser
```

### 运行测试

在 `test` 目录下有一些测试用例，可以使用以下命令运行测试：

```sh
make test
```

也可以基于题目要求的输入输出格式进行测试：

直接运行 `./json_parser`，然后输入 JSON 字符串和查询字符串，程序会输出查询结果。

样例输入：

```
10 5
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
firstName
address
address.city
address.postal
esc\aped
```

样例输出：

```
STRING John
OBJECT
STRING NewYork
NOTEXIST
STRING "hello"
```

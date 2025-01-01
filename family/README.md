# 3、家谱管理系统（必做）（树）

## 项目简介

本项目是一个简单的家谱管理系统，允许用户管理家族成员的信息，包括添加、删除、修改成员信息，显示家谱结构，查询成员信息等功能。

家谱数据以 JSON 格式存储在文件中，程序启动时会加载该文件中的数据，用户对家谱数据的修改保存到文件中。文件 `familyData.json` 中包含了一个示例家谱数据，可以用于测试和演示。

本题目的核心逻辑集中在对 *父子关系* 的维护（`children`）以及对 *成员详细信息* 的维护（`members`）上，再配合一些辅助函数（如递归显示、查找、修改等），就形成了完整的家谱管理功能。

## 文件结构

- `Member.h` 和 `Member.cpp`：定义了家族成员的类及其相关操作。
- `FamilyTree.h` 和 `FamilyTree.cpp`：定义了家谱树的类及其相关操作。
- `main.cpp`：主程序入口，提供用户交互界面。
- `familyData.json`：存储家谱数据的示例文件。
- `Makefile`：用于构建项目的 Makefile。
- `Date.h` 和 `Date.cpp`：定义了日期类及其相关操作，用于处理成员的出生日期和死亡日期。

## 构建步骤
1. 至少下载 `family` 目录和 `json-parser-tree` 目录下的所有文件，因为 `json-parser-tree` 是依赖项，具体参考 `Makefile` 文件。
2. 在项目根目录下执行以下命令，构建并运行程序：
```sh
make && ./bin/family_tree
```

## 功能说明

- 显示家谱：显示整个家谱结构。
- 显示第 n 代所有人信息：按代数显示成员信息。
- 按姓名查询成员信息：根据姓名查询成员详细信息。
- 按出生日期查询成员名单：根据出生日期查询成员列表。
- 确定两人关系：确定两人之间的关系。
- 添加成员孩子：为指定成员添加孩子。
- 删除成员：删除指定成员及其后代。
- 修改成员信息：修改指定成员的详细信息。
- 保存并退出：保存当前家谱数据并退出程序。

## 核心数据结构

1. `std::map<std::string, Member> members`  
   
   - 键（key）：`std::string` 类型，用于存储成员的姓名。  
   - 值（value）：`Member` 对象，包含该成员的各项信息（出生日期、死亡日期、是否在世、是否已婚、地址、父亲姓名等）。
   
这个容器相当于一个*姓名 -> 成员信息*的快速查找表（基于红黑树实现的有序 map）。当我们需要根据姓名来获取一个成员的详细信息时，会在此处进行查找。

2. `std::map<std::string, std::vector<std::string>> children`  
   
   - 键（key）：`std::string` 类型，同样是成员的姓名（通常用作*父亲姓名*）。  
   - 值（value）：`std::vector<std::string>` 类型，存储该父亲姓名所对应的孩子姓名列表。  
   
这实际上是一个*姓名 -> 子女列表*的邻接表结构。当我们需要得到某个成员的所有孩子时，就可以在此处直接找到对应的孩子姓名列表。

3. `std::string rootName` 
    
   - 整个家谱树的根节点姓名。在家谱中，往往会有一个最早的祖先作为根节点。程序在加载完家谱 JSON 数据后，会根据 JSON 最外层对象对应的人的姓名，设置 `rootName`。

4. `Member` 类  
   
   存储与某个家族成员相关的所有重要信息，例如：

      + `name`：姓名  
      + `birthDate`：出生日期（`Date`类型）  
      + `deathDate`：死亡日期（`Date`类型）  
      + `isAlive`：是否在世  
      + `isMarried`：是否已婚  
      + `address`：地址  
      + `fatherName`：父亲姓名（用来链接到父亲）

   并提供了 `Print()` 方法用于输出该成员的基本信息。

通过这两张表（`members` 和 `children`），就可以在程序内部对家谱进行快速查询、添加、删除、修改等操作。可以把它理解为：`members` 是所有成员的详细信息库，而 `children` 则描述了*父 -> 子*的血缘关系*。

## 主要功能的执行步骤（算法）

### 1. 从文件加载家谱（`loadFromFile`）

1. 打开指定的 JSON 文件并读取其内容到一个字符串 `jsonString` 中。
2. 使用 `JsonParser` 对象将 `jsonString` 解析为一个树状的 JSON 数据结构（`JsonNode`）。
3. 调用 `loadFromJsonNode(family)`，将解析好的 JSON 树传入，进入递归加载流程。
4. 最后把最顶层的人的姓名存入 `rootName`，作为整棵家谱的根。

### 2. 从 JSON 节点递归加载家谱（`loadFromJsonNode`）

该方法做了两件事：

   1. 创建/更新一个 `Member` 对象：  
      - 从 JSON 节点中读取 `"name"`, `"birthDate"`, `"isMarried"`, `"address"`, `"isAlive"`, `"deathDate"` 这些字段来构造一个 `Member`。  
      - 存储到 `members[name] = member;`。
   2. 加载其子节点：  
      - 获取该节点下的 `"children"`，如果有（`JsonListNode`），说明当前成员有孩子。  
      - 对每一个孩子都递归调用 `loadFromJsonNode(child)`，获得孩子的名字 `childName`。  
      - 把 `childName` 加到 `children[name]` 的孩子列表中，同时在 `members[childName]` 中登记它的 `fatherName = name`。
      - 这样一层一层往下，就把整棵家谱以树的方式构造了出来。

该函数本质上是一个深度优先遍历（DFS）过程，从 JSON 根节点开始，依次递归到最底层的孩子节点，把每个节点的成员信息保存到 `members`，并在 `children` 中记录好父子关系。
函数中的JSON读取功能实则是复用 *选做题18 树的应用* 中的 JSON 解析器，用于解析家谱数据。具体的 JSON 解析过程不在此赘述。

### 3. 保存家谱到文件（`saveToFile`）

1. 调用 `buildJsonNode(rootName)`：根据根节点的姓名，开始构建一棵 JSON 树。  
2. 该函数内部会递归：  
   - 创建一个 `JsonNode`，在其中存入 `name, birthDate, isMarried, address, isAlive, deathDate` 等字段。  
   - 对当前人的孩子，遍历 `children[name]`，对每个孩子递归调用 `buildJsonNode` 得到子节点，然后将子节点加入到当前节点的 `"children"` 列表中。  
3. 递归完成后，得到整棵家谱的 JSON 树（与我们读入时是逆向过程）。  
4. 调用 `to_json()` 转成字符串，写入指定文件。

### 4. 显示家谱（`displayFamilyTree` 与 `displayTree`）

- `displayFamilyTree()` 入口处只做一件事：  
  1. 如果 `rootName` 非空，则调用 `displayTree(rootName, 0, {})`。
- `displayTree(name, level, isLast)`：  
  - 按树形结构打印当前成员 `name`，并在前面打印适当的*竖线*、*分叉*符号。  
  - 如果当前成员在 `children` 中有孩子，就循环其孩子列表，每个孩子继续调用 `displayTree`。  
  - 这是一个深度优先遍历 + 树状打印算法，通过传入 `level`（层级）与 `isLast`（当前孩子是否是最后一个）的布尔标志，来决定如何打印树形分叉和缩进。

### 5. 显示第 n 代所有人（`displayGeneration`）

1. 传入代数 `n`。  
2. 先打印一条标题，如*=== 第 n 代 ===*。  
3. 使用一个匿名函数 `displayGen(name, level)`，从 `rootName` 开始递归往下走：
   - 如果当前层级 `level == n`，则说明当前节点就属于第 n 代，调用 `members[name].Print()`。  
   - 如果当前节点有孩子，则对孩子调用 `displayGen(child, level + 1)`。  
4. 整体也是一个DFS，通过比较 `level` 判断是否打印。

### 6. 查找并返回指定姓名的成员（`findMemberByName`）

- 在 `members` map 中查找 `name`，若未找到则抛出异常；否则直接返回 `members[name]` 的引用。

### 7. 根据出生日期查询成员（`searchByBirthDate`）

1. 将传入的日期字符串转换成一个 `Date` 对象。  
2. 遍历整个 `members`，对比每个成员的 `birthDate`。  
3. 如果相等则打印该成员。  
4. 如果遍历完未找到任何符合条件的成员，则打印提示信息。

### 8. 确定两人的关系（`determineRelationship`）

1. 内部定义了一个 `isAncestor(ancestor, descendant)` 的递归函数：  
   - 如果 `ancestor == descendant`，直接返回 `true`。  
   - 如果 `children[ancestor]` 不存在或为空，则返回 `false`。  
   - 否则遍历 `ancestor` 的每个孩子，如果 `isAncestor(child, descendant)` 为真，则返回 `true`；否则继续下一个孩子。  
   - 递归结束若都不为真，返回 `false`。
2. 判断：  
   - 若 `isAncestor(name1, name2)` 为真，说明 `name1` 是 `name2` 的祖先。  
   - 若 `isAncestor(name2, name1)` 为真，说明 `name1` 是 `name2` 的后代（即 `name2` 是 `name1` 的祖先）。  
   - 否则，打印 *二者无直系亲属关系*。  

从算法上看，这通过DFS 遍历来检测一条*是否是先辈-后代*的路径是否存在。

### 9. 给某个成员添加孩子（`addChild`）

- 在 `members` 中找到 `parentName`（父亲），如果找不到则抛异常。  
- 设置新成员 `child` 的 `fatherName` = `parentName`。  
- 加入 `members[child.name] = child`。  
- 在 `children[parentName]` 的线性表里，push_back 该孩子的姓名。

### 10. 删除成员（`deleteMember`）

1. 在 `members` 中找不到该姓名则抛异常。  
2. 如果该成员有孩子（`children[name]` 不为空），则对每个孩子递归调用 `deleteMember(child)`，先删除他们；删除完再 `children.erase(name)`。  
3. 从该成员的父亲的孩子列表中去掉该成员自己。  
4. 从 `members` 中删除该成员的记录。  

基于一种递归删除方式，先把所有后代清理完，再清理自己。

### 11. 修改成员信息（`modifyMember`）

1. 在 `members` 中找到指定姓名的成员，否则抛异常。  
2. 逐个字段提示用户输入新值，如果不输入就保留旧值：  
   - 修改姓名后，如果姓名真的变了，需要在其父亲的 `children` 列表里把旧名字改成新名字；同时还要在原父亲列表里把旧名字移除。  
   - 出生日期、在世状态、地址等字段的处理类似。  
   - 如果修改为*死亡*状态，还会提示更新死亡日期。  
   - 如果修改了*父亲姓名*，则需要把旧父亲和新父亲的 `children` 列表做调整。  

这是一个基于用户输入的字段更新的过程，并且需要维护好*父 -> 子*关系一致性。


## 算法的时间复杂度

由于 `std::map` 本身是基于红黑树（或其他平衡二叉树）实现，增删查改平均复杂度 $O(\log n)$；再加上对 `children` 中的线性表进行遍历时，孩子的数量相对一般不会过多，因此效率在家谱管理的场景里通常是足够的。

在具体的功能中，主要的时间消耗在于：

- 加载/保存：$O(N)$  
- 查找：$O(\log N)$  
- 修改/删除：$O(M)$，最坏情况下 $O(N)$  
- 显示树形家谱：$O(N)$  
- 按条件查询：$O(N)$  

其中 $N$ 为家谱成员总数，$M$ 为某个成员的孩子数量。

因此，程序整体的主要操作复杂度在 $O(N)$ 和 $O(\log N)$ 之间，能够高效处理大部分家谱管理需求。
# 2、迷宫问题（必做）（栈与队列）

## 简介

本程序实现了一个迷宫问题的求解程序，使用深度优先搜索算法（DFS）及线性表数据结构（栈）来求解迷宫。程序可以生成迷宫并保存到文件中，然后读取迷宫文件并尝试求解迷宫。

## 文件结构

- `generate_maze.c`：生成迷宫并保存到文件中。
- `main.c`：读取迷宫文件并尝试求解迷宫。
- `maze.txt`：保存生成的迷宫数据。

## 使用方法

### 生成迷宫

```sh
gcc generate_maze.c -o generate_maze
./generate_maze
```

该程序会生成一个迷宫并保存到 `maze.txt` 文件中。迷宫的大小可以在程序中设置，生成的迷宫包含 0 和 1，其中 0 表示路径，1 表示墙壁。确保迷宫的入口和出口是路径。

### 求解迷宫

```sh
gcc main.c -o main
./main
```

该程序会读取 `maze.txt` 文件中的迷宫数据，并尝试找到从入口到出口的路径。如果找到路径，会输出 `Path found!`，否则输出 `No path found.`。并输出这条路径，用 `*` 表示。

## 测试数据和结果

测试用例，迷宫大小为 20x20：

```
20 20
0 1 0 0 0 1 0 0 0 0 0 1 0 0 0 0 0 0 0 1
0 1 0 1 0 1 0 1 0 1 0 1 0 1 1 1 0 1 0 1
0 1 0 1 0 0 0 1 0 1 0 1 0 1 0 0 0 1 0 1
0 1 1 1 1 1 1 1 0 1 0 1 1 1 0 1 1 1 0 1
0 1 0 0 0 1 0 0 0 1 0 0 0 1 0 0 0 1 0 1
0 1 0 1 0 1 0 1 1 1 1 1 0 1 1 1 0 1 0 1
0 0 0 1 0 0 0 1 0 0 0 1 0 0 0 1 0 1 0 1
1 1 1 1 1 1 1 1 0 1 1 1 1 1 0 1 0 1 0 1
0 0 0 1 0 0 0 1 0 0 0 0 0 1 0 1 0 1 0 1
0 1 1 1 0 1 0 1 0 1 0 1 1 1 0 1 0 1 1 1
0 1 0 0 0 1 0 0 0 1 0 1 0 0 0 1 0 0 0 1
0 1 0 1 1 1 1 1 1 1 0 1 0 1 1 1 1 1 0 1
0 1 0 0 0 1 0 0 0 1 0 1 0 0 0 0 0 1 0 1
0 1 1 1 0 1 0 1 1 1 0 1 1 1 1 1 0 1 0 1
0 0 0 0 0 1 0 1 0 0 0 1 0 0 0 1 0 0 0 1
0 1 1 1 1 1 0 1 0 1 1 1 0 1 0 1 1 1 0 1
0 0 0 1 0 0 0 1 0 1 0 0 0 1 0 0 0 1 0 1
1 1 0 1 0 1 0 1 0 1 0 1 1 1 1 1 0 1 0 1
0 0 0 0 0 1 0 1 0 0 0 0 0 0 0 1 0 0 0 1
1 1 1 1 1 1 1 1 1 1 1 1 1 1 1 1 1 1 0 0
```

程序输出：

```
Path found!
  #       #     * * * #               #
* #   #   #   # * # * #   # # #   #   #
* #   #       # * # * #   #       #   #
* # # # # # # # * # * # # #   # # #   #
* # * * * # * * * # * * * #       #   #
* # * # * # * # # # # # * # # #   #   #
* * * # * * * #       # * * * #   #   #
# # # # # # # #   # # # # # * #   #   #
      #       #           # * #   #   #
  # # #   #   #   #   # # # * #   # # #
  #       #       #   # * * * #       #
  #   # # # # # # #   # * # # # # #   #
  #       #       #   # * * * * * #   #
  # # #   #   # # #   # # # # # * #   #
          #   #       #       # * * * #
  # # # # #   #   # # #   #   # # # * #
      #       #   #       #       # * #
# #   #   #   #   #   # # # # #   # * #
          #   #               #     * #
# # # # # # # # # # # # # # # # # # * *
```



## 数据结构和算法思想

在该迷宫求解算法中，数据结构的使用主要体现在以下几个方面：

### 栈

栈是实现深度优先搜索（DFS）的核心数据结构。算法通过栈来追踪当前的位置，并按照深度优先的策略进行探索：

每次探索一个新的位置时，将当前位置（`x`, `y` 坐标）压入栈中。栈的作用是存储当前路径的所有位置，以便回溯。每次从栈中弹出一个位置后，算法尝试在该位置周围的四个方向（上、下、左、右）进行移动。如果某个方向有效，算法将新的位置压入栈中，继续探索。如果探索的路径没有找到出口，栈的弹出操作会将当前位置移除，回到之前的探索点，即深度优先搜索的回溯过程。

通过栈，算法能够实现从起点出发，逐步深入迷宫，直到找到出口或者无法继续探索。栈的先进后出特性保证了深度优先搜索的正确性。

### 二维数组

迷宫本身是一个二维网格，每个位置要么是路径，要么是墙壁。二维数组用于存储迷宫的结构，并通过它来判断当前的移动是否合法。

算法通过二维数组表示迷宫，数组中的每个元素表示迷宫的一个格子，值为 `0` 表示该格子是路径，`1` 表示是墙壁，`2` 表示该格子已被访问过。

```c
int **maze = (int **)malloc(rows * sizeof(int *));
for (int i = 0; i < rows; i++) {
      maze[i] = (int *)malloc(cols * sizeof(int));
}
```

`isValid` 函数使用二维数组来判断当前位置是否有效（即是否是一个可以走的路径）。该函数检查当前坐标是否在迷宫范围内，并且是否是路径（值为 `0`）。

```c
int isValid(int **maze, int x, int y) {
   return x >= 0 && x < rows && y >= 0 && y < cols && maze[x][y] == PATH;
}
```

在探索过程中，算法通过将当前位置的值从 `0` 改为 `2` 来标记该位置已经被访问过。这样可以防止重复访问相同的位置，从而避免死循环。

```c
maze[current.x][current.y] = VISITED;
```

## 算法的时间复杂度分析

算法的核心是深度优先搜索（DFS），通过栈来实现。在最坏情况下，算法需要访问迷宫中的所有位置，因此时间复杂度取决于迷宫的大小。

每当访问一个新位置时，算法会将该位置压入栈中；每当探索到死胡同时，栈会弹出一个位置，进行回溯。栈操作的复杂度是常数时间 $O(1)$，因为栈的每次插入和弹出都只涉及一个位置。

由于算法有记忆化搜索的特性，即标记已访问过的位置，因此算法不会重复访问相同的位置。因此每个位置最多只会被访问一次。

综上，时间复杂度为 $O(ROWS \times COLS)$ 。其中 $ROWS$ 和 $COLS$ 分别是迷宫的行数和列数。


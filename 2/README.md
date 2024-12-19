# 数据结构项目：迷宫生成与求解

## 项目简介

本项目包含两个主要功能：生成迷宫和求解迷宫。迷宫生成器会随机生成一个包含路径和墙壁的迷宫，并将其保存到文件中。迷宫求解器会读取该文件中的迷宫，并尝试找到从入口到出口的路径。

## 文件结构

- `generate_maze.c`：生成迷宫并保存到文件中。
- `main.c`：读取迷宫文件并尝试求解迷宫。
- `maze.txt`：保存生成的迷宫数据。

## 使用方法

### 生成迷宫

1. 编译 `generate_maze.c` 文件：
    ```sh
    gcc generate_maze.c -o generate_maze
    ```

2. 运行生成迷宫程序：
    ```sh
    ./generate_maze
    ```

   该程序会生成一个随机迷宫并保存到 `maze.txt` 文件中。

### 求解迷宫

1. 编译 `main.c` 文件：
    ```sh
    gcc main.c -o solve_maze
    ```

2. 运行求解迷宫程序：
    ```sh
    ./solve_maze
    ```

   该程序会读取 `maze.txt` 文件中的迷宫数据，并尝试找到从入口到出口的路径。如果找到路径，会输出 "Path found!"，否则输出 "No path found."。同时会打印出标记过的迷宫。

## 代码说明

### `generate_maze.c`

- `generateMaze` 函数：随机生成迷宫，1/3 的概率生成墙壁，确保入口和出口是路径。
- `saveMazeToFile` 函数：将生成的迷宫保存到文件中。

### `main.c`

- `initStack` 函数：初始化栈。
- `isStackEmpty` 函数：判断栈是否为空。
- `push` 函数：将位置压入栈中。
- `pop` 函数：从栈中弹出位置。
- `isValid` 函数：判断当前位置是否有效。
- `printMaze` 函数：打印迷宫。
- `solveMaze` 函数：使用深度优先搜索算法求解迷宫。

## 注意事项

- 迷宫的大小由 `ROWS` 和 `COLS` 宏定义，默认为 20x20。
- 生成的迷宫文件名为 `maze.txt`，可以根据需要修改。
- 迷宫的入口和出口位置是固定的，分别为左上角和右下角。
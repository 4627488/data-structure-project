#include <stdio.h>
#include <stdlib.h>

int rows;
int cols;

#define PATH 0
#define WALL 1
#define VISITED 2
#define PATH_FOUND 3

typedef struct {
    int x, y;
} Position;

typedef struct {
    Position *data;
    int top;
    int capacity;
} Stack;

void initStack(Stack *s, int capacity) {
    s->data = (Position *)malloc(sizeof(Position) * capacity);
    s->top = -1;
    s->capacity = capacity;
}

int isStackEmpty(Stack *s) {
    return s->top == -1;
}

void push(Stack *s, Position pos) {
    if (s->top < s->capacity - 1) {
        s->data[++(s->top)] = pos;
    }
}

Position pop(Stack *s) {
    if (!isStackEmpty(s)) {
        return s->data[(s->top)--];
    }
    Position invalid = {-1, -1};
    return invalid;
}

int isValid(int **maze, int x, int y) {
    return x >= 0 && x < rows && y >= 0 && y < cols && maze[x][y] == PATH;
}

void printMaze(int **maze) {
    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < cols; j++) {
            printf("%c ", maze[i][j] == WALL ? '#' : maze[i][j] == PATH_FOUND ? '*' : ' ');
        }
        printf("\n");
    }
}

void solveMaze(int **maze, Position start, Position end, Position **prev) {
    Stack stack;
    initStack(&stack, rows * cols);
    push(&stack, start);

    // 定义四个方向：上、下、左、右
    int directions[4][2] = {{-1, 0}, {1, 0}, {0, -1}, {0, 1}};


    while (!isStackEmpty(&stack)) {
        Position current = pop(&stack);

        if (current.x == end.x && current.y == end.y) {
            printf("Path found!\n");
            // 回溯路径并标记
            while (current.x != start.x || current.y != start.y) {
                maze[current.x][current.y] = PATH_FOUND;
                current = prev[current.x][current.y];
            }
            return;
        }

        maze[current.x][current.y] = VISITED;

        // 尝试向四个方向移动
        int foundNext = 0;
        for (int i = 0; i < 4; i++) {
            int newX = current.x + directions[i][0];
            int newY = current.y + directions[i][1];
            if (isValid(maze, newX, newY)) {
                push(&stack, (Position){newX, newY});
                prev[newX][newY] = current;
                foundNext = 1;
            }
        }
    }
    printf("No path found.\n");
}

int main() {
    FILE *file = fopen("maze.txt", "r");
    if (file == NULL) {
        printf("Failed to open file.\n");
        return 1;
    }

    fscanf(file, "%d %d", &rows, &cols);

    int **maze = (int **)malloc(rows * sizeof(int *));
    Position **prev = (Position **)malloc(rows * sizeof(Position *));
    for (int i = 0; i < rows; i++) {
        maze[i] = (int *)malloc(cols * sizeof(int));
        prev[i] = (Position *)malloc(cols * sizeof(Position));
    }

    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < cols; j++) {
            fscanf(file, "%d", &maze[i][j]);
        }
    }
    fclose(file);

    Position start = {0, 0};             // 设置入口
    Position end = {rows - 1, cols - 1}; // 设置出口

    solveMaze(maze, start, end, prev);
    printMaze(maze);

    for (int i = 0; i < rows; i++) {
        free(maze[i]);
    }
    free(maze);

    return 0;
}

#include <stdio.h>
#include <stdlib.h>

#define ROWS 20
#define COLS 20
#define PATH 0
#define WALL 1
#define VISITED 2

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

int isValid(int maze[ROWS][COLS], int x, int y) {
    return x >= 0 && x < ROWS && y >= 0 && y < COLS && maze[x][y] == PATH;
}

void printMaze(int maze[ROWS][COLS]) {
    for (int i = 0; i < ROWS; i++) {
        for (int j = 0; j < COLS; j++) {
            printf("%d ", maze[i][j]);
        }
        printf("\n");
    }
}

void solveMaze(int maze[ROWS][COLS], Position start, Position end) {
    Stack stack;
    initStack(&stack, ROWS * COLS);
    push(&stack, start);

    int directions[4][2] = {{-1, 0}, {1, 0}, {0, -1}, {0, 1}};

    while (!isStackEmpty(&stack)) {
        Position current = pop(&stack);
        if (current.x == end.x && current.y == end.y) {
            printf("Path found!\n");
            return;
        }

        maze[current.x][current.y] = VISITED;

        for (int i = 0; i < 4; i++) {
            int newX = current.x + directions[i][0];
            int newY = current.y + directions[i][1];
            if (isValid(maze, newX, newY)) {
                push(&stack, (Position){newX, newY});
            }
        }
    }

    printf("No path found.\n");
}

int main() {
    int maze[ROWS][COLS];
    FILE *file = fopen("maze.txt", "r");
    if (file == NULL) {
        printf("Failed to open file.\n");
        return 1;
    }

    for (int i = 0; i < ROWS; i++) {
        for (int j = 0; j < COLS; j++) {
            fscanf(file, "%d", &maze[i][j]);
        }
    }
    fclose(file);

    Position start = {0, 0};             // 任意入口
    Position end = {ROWS - 1, COLS - 1}; // 任意出口

    solveMaze(maze, start, end);
    printMaze(maze);

    return 0;
}

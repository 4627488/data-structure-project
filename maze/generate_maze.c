#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define ROWS 20
#define COLS 20
#define PATH 0
#define WALL 1

void generateMaze(int maze[ROWS][COLS]) {
    srand(time(NULL));
    for (int i = 0; i < ROWS; i++) {
        for (int j = 0; j < COLS; j++) {
            maze[i][j] = (rand() % 3 == 0) ? WALL : PATH; // 1/3概率生成墙
        }
    }
    maze[0][0] = PATH; // 确保入口是路径
    maze[ROWS-1][COLS-1] = PATH; // 确保出口是路径
}

void saveMazeToFile(int maze[ROWS][COLS], const char *filename) {
    FILE *file = fopen(filename, "w");
    if (file == NULL) {
        printf("Failed to open file.\n");
        return;
    }
    for (int i = 0; i < ROWS; i++) {
        for (int j = 0; j < COLS; j++) {
            fprintf(file, "%d ", maze[i][j]);
        }
        fprintf(file, "\n");
    }
    fclose(file);
}

int main() {
    int maze[ROWS][COLS];
    generateMaze(maze);
    saveMazeToFile(maze, "maze.txt");
    printf("Maze generated and saved to maze.txt\n");
    return 0;
}

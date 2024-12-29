#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define ROWS 20
#define COLS 20
#define PATH 0
#define WALL 1

int directions[4][2] = {{-1, 0}, {1, 0}, {0, -1}, {0, 1}};

int inBounds(int x, int y) {
    return x >= 0 && x < ROWS && y >= 0 && y < COLS;
}

void generateMaze(int maze[ROWS][COLS], int x, int y) {
    int dirs[4] = {0, 1, 2, 3};
    for (int i = 0; i < 4; i++) {
        int j = rand() % 4;
        int temp = dirs[i];
        dirs[i] = dirs[j];
        dirs[j] = temp;
    }

    for (int i = 0; i < 4; i++) {
        int nx = x + directions[dirs[i]][0] * 2;
        int ny = y + directions[dirs[i]][1] * 2;

        if (inBounds(nx, ny) && maze[nx][ny] == WALL) {
            maze[nx][ny] = PATH;
            maze[x + directions[dirs[i]][0]][y + directions[dirs[i]][1]] = PATH;
            generateMaze(maze, nx, ny);
        }
    }
}

void saveMazeToFile(int maze[ROWS][COLS], const char *filename) {
    FILE *file = fopen(filename, "w");
    if (file == NULL) {
        printf("Failed to open file.\n");
        return;
    }
    fprintf(file, "%d %d\n", ROWS, COLS);
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

    for (int i = 0; i < ROWS; i++) {
        for (int j = 0; j < COLS; j++) {
            maze[i][j] = WALL;
        }
    }

    maze[0][0] = PATH;
    maze[ROWS - 1][COLS - 1] = PATH;

    srand(time(NULL));
    generateMaze(maze, 0, 0);

    saveMazeToFile(maze, "maze.txt");
    printf("Maze generated and saved to maze.txt.\n");
    return 0;
}

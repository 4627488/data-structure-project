#include <limits.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_STATION 10000
#define MAX_LINE 1000
#define MAX_NAME_LEN 100

struct Station {
    char name[MAX_NAME_LEN];
    int stationId;
    int lines[MAX_LINE][2]; // <lineId, InlineId>
    int lineCount;
};

struct Line {
    char name[MAX_NAME_LEN];
    int lineId;
    int stationIds[MAX_STATION];
    int stationCount;
};

struct Station stations[MAX_STATION];
struct Line lines[MAX_LINE];

void initStationsAndLines() {
    for (int i = 0; i < MAX_STATION; i++) {
        stations[i].stationId = -1;
        stations[i].lineCount = 0;
    }
    for (int i = 0; i < MAX_LINE; i++) {
        lines[i].lineId = -1;
        lines[i].stationCount = 0;
    }
}

void readCSV(const char *filename) {
    FILE *file = fopen(filename, "r");
    if (!file) {
        perror("File opening failed");
        exit(EXIT_FAILURE);
    }
    char line[256];
    fgets(line, sizeof(line), file); // skip header

    while (fgets(line, sizeof(line), file)) {
        char *token;
        char lineName[MAX_NAME_LEN], stationName[MAX_NAME_LEN];
        int lineId, inlineId, stationId;

        token = strtok(line, ",");
        strcpy(lineName, token);

        token = strtok(NULL, ",");
        strcpy(stationName, token);

        token = strtok(NULL, ",");
        lineId = atoi(token);

        token = strtok(NULL, ",");
        inlineId = atoi(token);

        token = strtok(NULL, ",");
        stationId = atoi(token);

        if (stations[stationId].stationId == -1) {
            strcpy(stations[stationId].name, stationName);
            stations[stationId].stationId = stationId;
        }
        stations[stationId].lines[stations[stationId].lineCount][0] = lineId;
        stations[stationId].lines[stations[stationId].lineCount][1] = inlineId;
        stations[stationId].lineCount++;

        if (lines[lineId].lineId == -1) {
            strcpy(lines[lineId].name, lineName);
            lines[lineId].lineId = lineId;
        }
        lines[lineId].stationIds[lines[lineId].stationCount++] = stationId;
    }
    fclose(file);
}

typedef struct {
    int dist;
    int stationId;
} HeapNode;

typedef struct {
    HeapNode data[MAX_STATION];
    int size;
} MinHeap;

void heapPush(MinHeap *heap, int dist, int stationId) {
    int i = heap->size++;
    while (i > 0 && dist < heap->data[(i - 1) / 2].dist) {
        heap->data[i] = heap->data[(i - 1) / 2];
        i = (i - 1) / 2;
    }
    heap->data[i].dist = dist;
    heap->data[i].stationId = stationId;
}

void heapPop(MinHeap *heap) {
    HeapNode last = heap->data[--heap->size];
    int i = 0, child;
    while (2 * i + 1 < heap->size) {
        child = 2 * i + 1;
        if (child + 1 < heap->size &&
            heap->data[child].dist > heap->data[child + 1].dist) {
            child++;
        }
        if (last.dist <= heap->data[child].dist) break;
        heap->data[i] = heap->data[child];
        i = child;
    }
    heap->data[i] = last;
}

HeapNode heapTop(MinHeap *heap) {
    return heap->data[0];
}

int heapEmpty(MinHeap *heap) {
    return heap->size == 0;
}

void printPath(int end, int prev[]) {
    if (prev[end] == -1) return;
    printPath(prev[end], prev);
    printf("%s\n", stations[end].name);
}

typedef struct {
    int stationId;
    int lineId;
} PrevNode;

void printPathWithLines(int end, PrevNode prev[], int start) {
    if (prev[end].stationId == -1) return;
    printPathWithLines(prev[end].stationId, prev, start);
    if (end != start) {
        printf("  |\n");
        printf("  | 乘坐 %s\n", lines[prev[end].lineId].name);
        printf("  |\n");
    }
    printf("%s\n", stations[end].name);
}

void findTransferLeastPath(int start, int end) {
    int dist[MAX_STATION];
    PrevNode prev[MAX_STATION];
    MinHeap heap = {.size = 0};

    for (int i = 0; i < MAX_STATION; i++) {
        dist[i] = INT_MAX;
        prev[i].stationId = -1;
        prev[i].lineId = -1;
    }

    dist[start] = 0;
    heapPush(&heap, 0, start);

    while (!heapEmpty(&heap)) {
        int u = heapTop(&heap).stationId;
        heapPop(&heap);

        if (u == end) break;

        for (int i = 0; i < stations[u].lineCount; i++) {
            int lineId = stations[u].lines[i][0];
            for (int j = 0; j < lines[lineId].stationCount; j++) {
                int v = lines[lineId].stationIds[j];
                if (dist[v] > dist[u] + 1) {
                    dist[v] = dist[u] + 1;
                    prev[v].stationId = u;
                    prev[v].lineId = lineId;
                    heapPush(&heap, dist[v], v);
                }
            }
        }
    }

    if (prev[end].stationId == -1) {
        printf("无法到达终点站点\n");
        return;
    }

    printf("最少转车次数: %d\n", dist[end] - 1);
    printf("路线: \n");
    printPathWithLines(end, prev, start);
}

void findLeastStationPath(int start, int end) {
    int dist[MAX_STATION];
    PrevNode prev[MAX_STATION];
    MinHeap heap = {.size = 0};

    for (int i = 0; i < MAX_STATION; i++) {
        dist[i] = INT_MAX;
        prev[i].stationId = -1;
        prev[i].lineId = -1;
    }

    dist[start] = 0;
    heapPush(&heap, 0, start);

    while (!heapEmpty(&heap)) {
        int u = heapTop(&heap).stationId;
        heapPop(&heap);

        if (u == end) break;

        for (int i = 0; i < stations[u].lineCount; i++) {
            int lineId = stations[u].lines[i][0];
            int inlineId = stations[u].lines[i][1];

            if (inlineId + 1 < lines[lineId].stationCount) {
                int v = lines[lineId].stationIds[inlineId + 1];
                if (dist[v] > dist[u] + 1) {
                    dist[v] = dist[u] + 1;
                    prev[v].stationId = u;
                    prev[v].lineId = lineId;
                    heapPush(&heap, dist[v], v);
                }
            }
            if (inlineId - 1 >= 0) {
                int v = lines[lineId].stationIds[inlineId - 1];
                if (dist[v] > dist[u] + 1) {
                    dist[v] = dist[u] + 1;
                    prev[v].stationId = u;
                    prev[v].lineId = lineId;
                    heapPush(&heap, dist[v], v);
                }
            }
        }
    }

    if (prev[end].stationId == -1) {
        printf("无法到达终点站点\n");
        return;
    }

    printf("经过站点数量最少: %d\n", dist[end] - 1);
    printf("路线: \n");
    printPathWithLines(end, prev, start);
}

int main() {
    initStationsAndLines();
    readCSV("nanjing.csv");

    char start[MAX_NAME_LEN], end[MAX_NAME_LEN];
    printf("请输入起始站点: ");
    scanf("%s", start);
    printf("请输入终点站点: ");
    scanf("%s", end);

    int startId = -1, endId = -1;
    for (int i = 0; i < MAX_STATION; i++) {
        if (strcmp(stations[i].name, start) == 0)
            startId = stations[i].stationId;
        if (strcmp(stations[i].name, end) == 0) endId = stations[i].stationId;
    }

    if (startId != -1 && endId != -1) {
        printf("---------------------------------\n");
        findTransferLeastPath(startId, endId);
        printf("---------------------------------\n");
        findLeastStationPath(startId, endId);
    } else {
        printf("站点未找到\n");
    }
    return 0;
}

#include <dirent.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/sysinfo.h>
#include <time.h>
#include <unistd.h>

// 定义进程结构体
typedef struct Process {
    char name[512];
    int memory;
    time_t start_time;
    int updated; // 更新标记
    struct Process *next;
} Process;

// 定义已结束进程结构体
typedef struct EndedProcess {
    char name[512];
    int duration;
    time_t end_time;
    struct EndedProcess *prev;
    struct EndedProcess *next;
} EndedProcess;

// 定义链表头
Process *active_head = NULL;
EndedProcess *ended_head = NULL;

// 添加活动进程
void add_active_process(char *name, int memory, time_t start_time) {
    Process *new_process = (Process *)malloc(sizeof(Process));
    strcpy(new_process->name, name);
    new_process->memory = memory;
    new_process->start_time = start_time;
    new_process->next = NULL;
    new_process->updated = 1; // 新增的进程标记为已更新

    // 按内存使用量排序插入
    if (active_head == NULL || active_head->memory < memory) {
        new_process->next = active_head;
        active_head = new_process;
    } else {
        Process *current = active_head;
        while (current->next != NULL && current->next->memory >= memory) {
            current = current->next;
        }
        new_process->next = current->next;
        current->next = new_process;
    }
}

// 添加已结束进程
void add_ended_process(char *name, int duration, time_t end_time) {
    EndedProcess *new_process = (EndedProcess *)malloc(sizeof(EndedProcess));
    strcpy(new_process->name, name);
    new_process->duration = duration;
    new_process->end_time = end_time;
    new_process->prev = NULL;
    new_process->next = NULL;

    // 按持续时间排序插入
    if (ended_head == NULL || ended_head->duration > duration) {
        new_process->next = ended_head;
        if (ended_head != NULL) {
            ended_head->prev = new_process;
        }
        ended_head = new_process;
    } else {
        EndedProcess *current = ended_head;
        while (current->next != NULL && current->next->duration <= duration) {
            current = current->next;
        }
        new_process->next = current->next;
        if (current->next != NULL) {
            current->next->prev = new_process;
        }
        current->next = new_process;
        new_process->prev = current;
    }
}

// 更新活动进程列表
void update_active_processes() {
    // 将所有进程标记为未更新
    Process *current = active_head;
    while (current != NULL) {
        current->updated = 0;
        current = current->next;
    }

    DIR *proc_dir = opendir("/proc");
    if (proc_dir == NULL) {
        perror("无法打开 /proc 目录");
        return;
    }

    // 获取系统开机时间
    struct sysinfo info;
    if (sysinfo(&info) != 0) {
        perror("无法获取系统信息");
        closedir(proc_dir);
        return;
    }
    time_t boot_time = time(NULL) - info.uptime;

    struct dirent *entry;
    while ((entry = readdir(proc_dir)) != NULL) {
        if (entry->d_type == DT_DIR) {
            char *endptr;
            long pid = strtol(entry->d_name, &endptr, 10);
            if (*endptr == '\0') { // 确保目录名是一个数字，即进程ID
                char path[512];
                snprintf(path, sizeof(path), "/proc/%ld/stat", pid);
                FILE *stat_file = fopen(path, "r");
                if (stat_file) {
                    char name[512];           // 第2个字段
                    unsigned long start_time; // 第22个字段
                    unsigned long rss;        // 第24个字段
                    fscanf(stat_file,
                           "%*d %s %*c %*d %*d %*d %*d %*d %*u %*u %*u %*u %*u "
                           "%*u %*u %*d %*d %*d"
                           "%*d %*d %*d %lu %*d %lu %*u %*u %*u %*u %*u %*d",
                           name, &start_time, &rss);
                    fclose(stat_file);

                    // 将 start_time 转换为时间戳
                    start_time =
                        boot_time + (start_time / sysconf(_SC_CLK_TCK));
                    // rss是实际占用内存，以页为单位存放，一般是4K每页，所有要乘以4
                    long memory = rss * 4;

                    if (memory == 0) {
                        continue;
                    }

                    // 检查进程是否已经在活动列表中
                    Process *current = active_head;
                    int found = 0;
                    while (current != NULL) {
                        if (strcmp(current->name, name) == 0) {
                            found = 1;
                            current->updated = 1; // 标记为已更新
                            break;
                        }
                        current = current->next;
                    }

                    if (!found) {
                        add_active_process(name, memory, start_time);
                        // 检查进程是否已经在已结束列表中
                        EndedProcess *current = ended_head;
                        while (current != NULL) {
                            if (strcmp(current->name, name) == 0) {
                                // 从已结束列表中移除
                                if (current->prev == NULL) {
                                    ended_head = current->next;
                                } else {
                                    current->prev->next = current->next;
                                }
                                if (current->next != NULL) {
                                    current->next->prev = current->prev;
                                }
                                free(current);
                                break;
                            }
                            current = current->next;
                        }
                    }
                }
            }
        }
    }
    closedir(proc_dir);
}

// 更新已结束进程列表
void update_ended_processes() {
    Process *current = active_head;
    Process *prev = NULL;

    while (current != NULL) {
        if (current->updated == 0) { // 未更新的进程视为已结束
            time_t end_time = time(NULL);
            int duration = end_time - current->start_time;

            // 从活动链表中移除
            if (prev == NULL) {
                active_head = current->next;
            } else {
                prev->next = current->next;
            }

            // 添加到已结束链表中
            add_ended_process(current->name, duration, end_time);

            // 释放内存
            Process *temp = current;
            current = current->next;
            free(temp);
        } else {
            prev = current;
            current = current->next;
        }
    }
}

// 打印活动进程信息
void print_active_processes() {
    printf("当前活动进程:\n");
    Process *current = active_head;
    while (current != NULL) {
        printf("进程名: %s, 持续时间: %lds, 内存使用: %dKB\n", current->name,
               time(NULL) - current->start_time, current->memory);
        current = current->next;
    }
}

// 打印已结束进程信息
void print_ended_processes() {
    printf("已结束进程:\n");
    EndedProcess *current = ended_head;
    while (current != NULL) {
        printf("进程名: %s, 持续时间: %ds, 结束时间: %s", current->name,
               current->duration, ctime(&current->end_time));
        current = current->next;
    }
}

int main() {
    while (1) {
        update_active_processes();
        update_ended_processes();
        print_active_processes();
        print_ended_processes();
    }
    return 0;
}

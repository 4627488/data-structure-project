#include <dirent.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <time.h>
#include <unistd.h>

typedef struct Proc // Process struct, Linked List
{
    char name[256];
    int mem;
    int pid;
    int dur;
    time_t end;
    struct Proc *next;
    struct Proc *prev;
} Proc;

Proc *head = NULL;    // Head of the linked list
Proc *finHead = NULL; // Head of the fin processes linked list

time_t getCurrentTime() {
    return time(NULL);
}

void insertProc(char *name, int pid, int mem) {
    Proc *newProc = (Proc *)malloc(sizeof(Proc));
    strcpy(newProc->name, name);
    newProc->pid = pid;
    newProc->mem = mem;
    newProc->dur = 0;
    newProc->end = 0;
    newProc->next = NULL;
    newProc->prev = NULL;

    if (!head || head->mem < mem) {
        newProc->next = head;
        head = newProc;
    } else {
        Proc *cur = head;
        while (cur->next && cur->next->mem >= mem) {
            cur = cur->next;
        }
        newProc->next = cur->next;
        cur->next = newProc;
    }
}

void insertFinProc(Proc *proc) {
    proc->next = NULL;
    proc->prev = NULL;

    if (!finHead || finHead->dur > proc->dur) {
        proc->next = finHead;
        if (finHead) {
            finHead->prev = proc;
        }
        finHead = proc;
    } else {
        Proc *cur = finHead;
        while (cur->next && cur->next->dur <= proc->dur) {
            cur = cur->next;
        }
        proc->next = cur->next;
        if (cur->next) {
            cur->next->prev = proc;
        }
        cur->next = proc;
        proc->prev = cur;
    }
}

void clearProcs() {
    Proc *cur = head;
    while (cur) {
        Proc *temp = cur;
        cur = cur->next;
        free(temp);
    }
    head = NULL;
}

Proc *findProc(int pid) {
    Proc *cur = head;
    while (cur) {
        if (cur->pid == pid) {
            return cur;
        }
        cur = cur->next;
    }
    return NULL;
}

Proc *findFinProc(int pid) {
    Proc *cur = finHead;
    while (cur) {
        if (cur->pid == pid) {
            return cur;
        }
        cur = cur->next;
    }
    return NULL;
}

void moveToActiveProc(Proc *proc) {
    if (proc->prev) {
        proc->prev->next = proc->next;
    }
    if (proc->next) {
        proc->next->prev = proc->prev;
    }
    if (proc == finHead) {
        finHead = proc->next;
    }

    proc->next = NULL;
    proc->prev = NULL;
    insertProc(proc->name, proc->pid, proc->mem);
}

void updateOrInsertProc(char *name, int pid, int mem) {
    Proc *existingProc = findProc(pid);
    if (existingProc) {
        existingProc->mem = mem;
        existingProc->dur = -existingProc->dur;
    } else {
        Proc *finishedProc = findFinProc(pid);
        if (finishedProc) {
            moveToActiveProc(finishedProc);
        } else {
            insertProc(name, pid, mem);
        }
    }
}

void readProcs() {
    Proc *cur = head;

    while (cur) {
        cur->dur = -cur->dur;
        cur = cur->next;
    }

    DIR *procDir = opendir("/proc");
    if (!procDir) {
        perror("opendir");
        return;
    }

    struct dirent *entry;
    while ((entry = readdir(procDir)) != NULL) {
        if (entry->d_type == DT_DIR &&
            atoi(entry->d_name) > 0) // Check if the entry is a directory and the name is a number
        {
            char statPath[512];
            snprintf(statPath, sizeof(statPath), "/proc/%s/stat",
                     entry->d_name); // Get the path of the stat file

            FILE *statFile = fopen(statPath, "r");
            if (!statFile) // Check if the file exists
                continue;

            char name[256];
            int pid, mem = 0;

            fscanf(statFile, "%d %s", &pid, name); // Read the pid and name from the stat file
            fclose(statFile);

            name[strlen(name) - 1] = '\0';
            memmove(name, name + 1, strlen(name));

            char statmPath[512];
            snprintf(statmPath, sizeof(statmPath), "/proc/%s/statm", entry->d_name);

            FILE *statmFile = fopen(statmPath, "r"); //
            if (statmFile) {
                fscanf(statmFile, "%d", &mem); // Read the memory usage from the statm file
                fclose(statmFile);
            }

            mem *= 4;

            updateOrInsertProc(name, pid, mem);
        }
    }
    closedir(procDir);

    Proc *prev = NULL;
    cur = head;
    while (cur) {
        if (cur->dur < 0) {
            cur->end = getCurrentTime();
            if (prev) {
                prev->next = cur->next;
            } else {
                head = cur->next;
            }
            Proc *temp = cur;
            cur = prev ? prev->next : head;
            temp->dur = -temp->dur;
            insertFinProc(temp);
        } else {
            prev = cur;
            cur = cur->next;
        }
    }
}

void printProcs() {
    printf("=== Active Processes ===\n");
    Proc *cur = head;
    while (cur) {
        printf("PID: %d, Name: %s, Memory: %d kB, Duration: %d seconds\n", cur->pid, cur->name,
               cur->mem, cur->dur);
        cur = cur->next;
    }
}

void printFinProcs() {
    printf("=== Finnish Processes ===\n");
    Proc *cur = finHead;
    while (cur) {
        printf("PID: %d, Name: %s, Duration: %d seconds, End Time: %s", cur->pid, cur->name,
               cur->dur, ctime(&cur->end));
        cur = cur->next;
    }
}

void updateDurations() {
    Proc *cur = head;
    while (cur) {
        cur->dur++;
        cur = cur->next;
    }
}

int main() {
    while (1) {
        readProcs();
        printProcs();
        printFinProcs();
        updateDurations();
        sleep(1);
    }

    return 0;
}
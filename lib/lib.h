#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <dirent.h>
typedef struct {
    char *process_number;
    int arrival_time;
    int priority;
    int burst_time;
    int ram;
    int cpu_rate;
} ProcessInfo;
void fcfs(ProcessInfo *pr0, int n, int CPU1_RAM, int MAX_CPU_RATE, FILE *file);
void sjf(ProcessInfo *pr0, int n, int CPU2_RAM, int MAX_CPU_RATE , FILE *file);
void rr_algorithm(ProcessInfo *pr2, int n, int CPU2_RAM, int MAX_CPU_RATE, FILE *file, int quantum);
ProcessInfo* extractProcesses(const char* filename, int* numProcesses, int* PriorityCounts);
int isTextFile(const char *filename);
off_t getFileSize(const char *filename);
void parseFileContent(const char* content, ProcessInfo* processes, int* numProcesses, int* PriorityCounts);
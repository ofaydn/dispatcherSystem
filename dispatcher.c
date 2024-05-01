#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#define CPU1_RAM 512 * 1024 * 1024
#define CPU2_RAM 1536 * 1024 * 1024

typedef struct {
    char *process_number;
    int arrival_time;
    int priority;
    int burst_time;
    int ram;
    int cpu_rate;
} ProcessInfo;


ProcessInfo* extractArchive(const char* filename, int* numProcesses);
void createOutput(int argc, char *argv[]);
void fcfs_algorithm(ProcessInfo ps[]);
void sjf_algorithm();
void rr8_algorithm();
void rr16_algorithm();
int isTextFile(const char *filename);

int main(int argc, char *argv[]) {
    if (argc != 2) {				//Usage check 
        printf("Usage:\n");
        printf("To dispatch processes to system:\n");
        printf("%s <input.txt>:\n",argv[0]);
        exit(1);
    }
    int numProcesses;
    ProcessInfo* processes = extractArchive(argv[1], &numProcesses);

    if (processes == NULL) {
        return 1; // Extraction failed
    }

    // Display sorted process information
    for (int i = 0; i < numProcesses; i++) {
        printf("Process: %s\n", processes[i].process_number);
        printf("Arrival Time: %d\n", processes[i].arrival_time);
        printf("Priority: %d\n", processes[i].priority);
        printf("Burst Time: %d\n", processes[i].burst_time);
        printf("RAM: %d\n", processes[i].ram);
        printf("CPU Rate: %d\n", processes[i].cpu_rate);
        printf("\n");
    }

    // Free the memory allocated for the processArr
   for (int i = 0; i < numProcesses; i++) {
        free(processes[i].process_number);
    }
    free(processes);

    return 0;
}

int isTextFile(const char *filename) { //returns 1 if file is a text, returns 0 if its not
    FILE *file = fopen(filename, "r");
    if (file == NULL) {
        printf("Failed to open %s.\n",filename);
        exit(1);
    }

    int is_text = 1; // Assume it's a text file initially

    // Check if the file contains non-printable characters (non-ASCII)
    int c;
    while ((c = fgetc(file)) != EOF) {
        if (c < 0 || c > 127) {
            is_text = 0; // Not a text file
            break;
        }
    }

    fclose(file);

    return is_text;
}

ProcessInfo* extractArchive(const char* filename, int* numProcesses) {
    if (!isTextFile(filename)) {
        printf("Invalid file format. Please provide a text file.\n");
        exit(1);
    }

    FILE* file = fopen(filename, "r");
    if (file == NULL) {
        printf("Failed to open %s.\n", filename);
        exit(1);
    }

    // Count the number of numProcesses in the file
    int lines = 0;
    while (fgetc(file) != EOF) {
        if (fgetc(file) == '\n') {
            lines++;
        }
    }
    rewind(file);

    // Allocate memory for the array of ProcessInfo structs
    ProcessInfo* processes = malloc(lines * sizeof(ProcessInfo));
    if (processes == NULL) {
        printf("Memory allocation failed.\n");
        fclose(file);
	exit(1);
    }

	char line[256];
	int index = 0;
     while (fgets(line, sizeof(line), file)) {
        ProcessInfo process;
        char* token = strtok(line, ",");
        
        process.process_number = strdup(token);

        process.arrival_time = atoi(strtok(NULL, ","));
        process.priority = atoi(strtok(NULL, ","));
        process.burst_time = atoi(strtok(NULL, ","));
        process.ram = atoi(strtok(NULL, ","));
        process.cpu_rate = atoi(strtok(NULL, ",")); // Last value

        processes[index++] = process;
    }

    fclose(file);

    // Sort processes by priority (0 > 1 > 2 > 3)
    for (int i = 0; i < lines - 1; i++) {
        for (int j = 0; j < lines - i - 1; j++) {
            if (processes[j].priority > processes[j + 1].priority) {
                ProcessInfo temp = processes[j];
                processes[j] = processes[j + 1];
                processes[j + 1] = temp;
            }
        }
    }

    *numProcesses = lines;
    return processes;
}

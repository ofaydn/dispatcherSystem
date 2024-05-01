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
        printf("cpu_schedular <input.txt>:\n");
        exit(1);
    }
    int numProcesses = 0;
    ProcessInfo* processArr = extractArchive(argv[1], &numProcesses);
    for (int i = 0; i < numProcesses - 1; i++) {
        for (int j = 0; j < numProcesses - i - 1; j++) {
            if (processArr[j].priority > processArr[j + 1].priority) {
                ProcessInfo temp = processArr[j];
                processArr[j] = processArr[j + 1];
                processArr[j + 1] = temp;
            }
        }
    }

    // Print the processes consecutively
    for (int i = 0; i < numProcesses; i++) {
        printf("Process Number: %s\n", processArr[i].process_number);
        printf("Arrival Time: %d\n", processArr[i].arrival_time);
        printf("Priority: %d\n", processArr[i].priority);
        printf("Burst Time: %d\n", processArr[i].burst_time);
        printf("RAM: %d\n", processArr[i].ram);
        printf("CPU Rate: %d\n", processArr[i].cpu_rate);
        printf("\n");
    }

    // Free the memory allocated for the processArr
    for (int i = 0; i < numProcesses; i++) {
        free(processArr[i].process_number);
    }
    free(processArr);

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
    // Read the data from the file and populate the array of ProcessInfo structs
    for (int i = 0; i < lines; i++) {
        ProcessInfo process;
        process.process_number = malloc(10 * sizeof(char));
        if (process.process_number == NULL) {
            printf("Memory allocation failed.\n");
            exit(1);
        }printf("prescan");
        fscanf(file, "%[^,],%d,%d,%d,%d,%d", process.process_number, &process.arrival_time, &process.priority, &process.burst_time, &process.ram, &process.cpu_rate);
	printf("postscan %d",process.process_number);
	processes[i] = process;
    }

    fclose(file);
    *numProcesses = lines;
    return processes;
}

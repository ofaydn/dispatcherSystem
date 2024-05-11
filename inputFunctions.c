#include "lib.h"

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

off_t getFileSize(const char *filename) {
    struct stat st;
    if (stat(filename, &st) == 0) {
        return st.st_size; // Return file size in bytes
    }
    printf("Error occurred while getting %s's file size.\n",filename);
    return -1; // Error occurred while getting file size
}

void parseFileContent(const char* content, ProcessInfo* processes, int* numProcesses, int* PriorityCounts) {
        char* token = strtok(content, "\n");
        int index = 0;
        while (token != NULL) {
            processes[index].process_number = malloc(strlen(token) + 1);

            if (sscanf(token, "%[^,],%d,%d,%d,%d,%d", processes[index].process_number, &processes[index].arrival_time, &processes[index].priority, &processes[index].burst_time, &processes[index].ram, &processes[index].cpu_rate) == 6) {
                switch (processes[index].priority) {
                    case 0:
                        PriorityCounts[0]++;
                        break;
                    case 1:
                        PriorityCounts[1]++;
                        break;
                    case 2:
                        PriorityCounts[2]++;
                        break;
                    case 3:
                        PriorityCounts[3]++;
                        break;
                    default:
                        printf("Invalid priority number.\n");
                        break;
                }
                index++;
            }
            token = strtok(NULL, "\n");
        }
        *numProcesses = index;
}

ProcessInfo* extractProcesses(const char* filename, int* numProcesses, int* PriorityCounts) {
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
    int processCount = 1;
    rewind(file); // Reset the file pointer to the beginning
    int characters;
    int inputSize = getFileSize(filename);
    while ((characters = fgetc(file)) != EOF) {
        if (characters == '\n') {
            processCount++;
        }
    }
    printf("Number of processes: %d\n", processCount);
    rewind(file);
    // Allocate memory for the array of ProcessInfo structs
    ProcessInfo* processes = malloc(processCount * sizeof(ProcessInfo));
    if (processes == NULL) {
        printf("Memory allocation failed.\n");
        fclose(file);
	exit(1);
    }
    int processesIndex = 0;
    char buff[inputSize + 1];
    size_t sectionLength = inputSize;
    size_t bytesRead = fread(buff, sizeof(char), sectionLength, file);
    buff[bytesRead] = '\0';

    parseFileContent(buff, processes, &processCount, PriorityCounts);
    fclose(file);

    for (int i = 0; i < processCount - 1; i++) {
        for (int j = 0; j < processCount - i - 1; j++) {
            if (processes[j].priority > processes[j + 1].priority) {
                ProcessInfo temp = processes[j];
                processes[j] = processes[j + 1];
                processes[j + 1] = temp;
            }
        }
    }

    *numProcesses = processCount;
    return processes;
}
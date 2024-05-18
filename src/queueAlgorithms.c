#include "lib.h"
#define QUANTUM8 8
#define QUANTUM16 16

void fcfs(ProcessInfo *prLists, int n, int CPU1_RAM, int MAX_CPU_RATE , FILE *file) {
    int total_ram = 0;
    int total_cpu_rate = 0;

    // Sort the processes by arrival time
    for (int i = 0; i < n; i++) {
        for (int j = i + 1; j < n; j++) {
            if (prLists[i].arrival_time > prLists[j].arrival_time) {
                ProcessInfo temp = prLists[i];
                prLists[i] = prLists[j];
                prLists[j] = temp;
            }
        }
    }
    char* terminatedQueue[n];
    char buffer[256];
    // Execute the processes
    for (int i = 0; i < n; i++) {
        // Check if the process can be executed
        if (total_ram + prLists[i].ram <= CPU1_RAM && total_cpu_rate + prLists[i].cpu_rate <= MAX_CPU_RATE) {
            sprintf(buffer,"Process %s is queued to be assigned to CPU-1.\n", prLists[i].process_number);
            fprintf(file, "%s", buffer);
            sprintf(buffer,"Process %s is assigned to CPU-1.\n", prLists[i].process_number);
            fprintf(file, "%s", buffer);

            total_ram += prLists[i].ram;
            total_cpu_rate += prLists[i].cpu_rate;

            sprintf(buffer,"Process %s is completed and terminated.\n", prLists[i].process_number);
            fprintf(file, "%s\n", buffer);
            terminatedQueue[i] = prLists[i].process_number;
            // Update the total RAM and CPU rate
            total_ram -= prLists[i].ram;
            total_cpu_rate -= prLists[i].cpu_rate;
        } else {
            sprintf(buffer,"Process %s cannot be executed due to insufficient resources.\n", prLists[i].process_number);
            fprintf(file, "%s\n", buffer);
        }
    }
    // Print the CPU-1 queue
    printf("CPU-1 que1(priority-0) (FCFS)->");
    for (int i = 0; i < n; i++) {
        printf("%s-", terminatedQueue[i]);
    }
    printf("\n");
    
}

void sjf(ProcessInfo *prLists, int n, int CPU2_RAM, int MAX_CPU_RATE , FILE *file) {
    int total_ram = 0;
    int total_cpu_rate = 0;
    char buffer[256];
    char* terminatedQueue[n];
    // Sort the processes by burst time
    for (int i = 0; i < n; i++) {
        for (int j = i + 1; j < n; j++) {
            if (prLists[i].burst_time > prLists[j].burst_time) {
                ProcessInfo temp = prLists[i];
                prLists[i] = prLists[j];
                prLists[j] = temp;
            }
        }
    }

    // Execute the processes
    for (int i = 0; i < n; i++) {
        // Check if the process can be executed
        if (total_ram + prLists[i].ram <= CPU2_RAM && total_cpu_rate + prLists[i].cpu_rate <= MAX_CPU_RATE) {
            sprintf(buffer, "Process %s is placed in the que1 queue to be assigned to CPU-2.\n", prLists[i].process_number);
            fprintf(file, "%s", buffer);
            sprintf(buffer, "Process %s is assigned to CPU-2.\n", prLists[i].process_number);
            fprintf(file, "%s", buffer);

            // Update the total RAM and CPU rate
            total_ram += prLists[i].ram;
            total_cpu_rate += prLists[i].cpu_rate;

            // Simulate the execution of the process
            

            sprintf(buffer, "The operation of process %s is completed and terminated.\n", prLists[i].process_number);
            terminatedQueue[i] = prLists[i].process_number;
           fprintf(file, "%s\n", buffer);

            // Update the total RAM and CPU rate
            total_ram -= prLists[i].ram;
            total_cpu_rate -= prLists[i].cpu_rate;
        } else {
            sprintf(buffer, "Process %s cannot be executed due to insufficient resources.\n", prLists[i].process_number);
            fprintf(file, "%s\n", buffer);
        }
    }
    printf("CPU-2 que2(priority-1) (FCFS)->");
    for (int i = 0; i < n; i++) {
        printf("%s-", terminatedQueue[i]);
    }
    printf("\n");
}

void rr_algorithm(ProcessInfo *prList, int n, int CPU2_RAM, int MAX_CPU_RATE, FILE *file,int QUANTUM) {
    int total_ram = 0;
    int total_cpu_rate = 0;
    int queueNumber = 0;
    
    if (QUANTUM == 8) {
        queueNumber = 2;
    } else if (QUANTUM == 16) {
        queueNumber = 3;
    }else{
        printf("Invalid quantum number\n");
        exit(1);
    }
    
    char buffer[256];
    char* terminatedQueue[n];
    // Execute the processes
    int done = 0;  // To check if all processes are done
    while (!done) {
        done = 1;
        for (int i = 0; i < n; i++) {
            // Check if the process can be executed
            if (prList[i].burst_time > 0 && total_ram + prList[i].ram <= CPU2_RAM && total_cpu_rate + prList[i].cpu_rate <= MAX_CPU_RATE) {
                sprintf(buffer, "Process %s is placed in the que%d queue to be assigned to CPU-2.\n", prList[i].process_number, queueNumber);
                fprintf(file, "%s", buffer);
                sprintf(buffer, "Process %s is assigned to CPU-2.\n", prList[i].process_number);
                fprintf(file, "%s", buffer);

                // Update the total RAM and CPU rate
                total_ram += prList[i].ram;
                total_cpu_rate += prList[i].cpu_rate;

                // Execute the process for the quantum time or until it's done
                int quantum = prList[i].burst_time < QUANTUM ? prList[i].burst_time : QUANTUM;
                prList[i].burst_time -= quantum;

                // Check if the process is done
                if (prList[i].burst_time > 0) {
                    sprintf(buffer, "Process %s run until the defined quantum time and is queued again because the process is not completed.\n", prList[i].process_number);
                    fprintf(file, "%s", buffer);
                    done = 0;  // Not all processes are done
                } else {
                    sprintf(buffer, "Process %s is assigned to CPU-2, its operation is completed and terminated.\n", prList[i].process_number);
                    terminatedQueue[i] = prList[i].process_number;
                    fprintf(file, "%s\n", buffer);
                }

                // Update the total RAM and CPU rate
                total_ram -= prList[i].ram;
                total_cpu_rate -= prList[i].cpu_rate;
            }
        }
    }
    printf("CPU-2 que%d(priority-%d) (FCFS)->", queueNumber+1, queueNumber);
    for (int i = 0; i < n; i++) {
        printf("%s-", terminatedQueue[i]);
    }
    printf("\n");
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
        processes[index].process_number = malloc(strlen(token) + 1); // Allocate memory for the process number
        if (sscanf(token, "%[^,],%d,%d,%d,%d,%d", processes[index].process_number, 
        &processes[index].arrival_time, &processes[index].priority, &processes[index].burst_time, 
        &processes[index].ram, &processes[index].cpu_rate) == 6) {
            switch (processes[index].priority) {
                case 0:
                    PriorityCounts[0]++; // Increment the count of priority 0 processes
                    break;
                case 1:
                    PriorityCounts[1]++; // Increment the count of priority 1 processes
                    break;
                case 2:
                    PriorityCounts[2]++; // Increment the count of priority 2 processes
                    break;
                case 3:
                    PriorityCounts[3]++; // Increment the count of priority 3 processes
                    break;
                default:
                    printf("Invalid priority number.\n"); // Print an error message for invalid priority number
                    break;
            }
            index++;
        }
        token = strtok(NULL, "\n");
    }
    *numProcesses = index; // Update the number of processes
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
    rewind(file);
    // Allocate memory for the array of ProcessInfo structs
    ProcessInfo* processes = malloc(processCount * sizeof(ProcessInfo));
    if (processes == NULL) {
        printf("Memory allocation failed.\n");
        fclose(file);
	exit(1);
    }
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


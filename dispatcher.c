#include "queueAlgorithms.h"
#define CPU1_RAM 512 
#define CPU2_RAM 1536 
#define MAX_CPU_RATE 100
#define QUANTUM8 8
#define QUANTUM16 16

int main(int argc, char *argv[]) {
    if (argc != 2) {				//Usage check 
        printf("Usage:\n");
        printf("To dispatch processes to system:\n");
        printf("%s <input.txt>:\n",argv[0]);
        exit(1);
    }
    int numProcesses;
    int PriorityCounts[4] = {0, 0, 0, 0};

    ProcessInfo* processes = extractProcesses(argv[1], &numProcesses, PriorityCounts);

    if (processes == NULL) {
        return 1; // Extraction failed
    }

    //printing the processes
    /*
    for(int i =0 ; i< numProcesses;i++)
    {
        printf("Process: %s\n", processes[i].process_number);
        printf("Arrival Time: %d\n", processes[i].arrival_time);
        printf("Priority: %d\n", processes[i].priority);
        printf("Burst Time: %d\n", processes[i].burst_time);
        printf("RAM: %d\n", processes[i].ram);
        printf("CPU Rate: %d\n", processes[i].cpu_rate);
        printf("\n");
    }
    for(int i = 0; i < 4; i++) {
        printf("Priority %d: %d\n", i, PriorityCounts[i]);
    }*/


    // Sort the processes based on their priority
    ProcessInfo pr0[PriorityCounts[0]];
    ProcessInfo pr1[PriorityCounts[1]];
    ProcessInfo pr2[PriorityCounts[2]];
    ProcessInfo pr3[PriorityCounts[3]];
    int count0 = 0, count1 = 0, count2 = 0, count3 = 0;
    for(int i = 0; i < numProcesses; i++) {
        switch (processes[i].priority) {
            case 0:
                pr0[count0] = processes[i];
                count0++;
                break;
            case 1:
                pr1[count1] = processes[i];
                count1++;
                break;
            case 2:
                pr2[count2] = processes[i];
                count2++;
                break;
            case 3:
                pr3[count3] = processes[i];
                count3++;
                break;
            default:
                printf("Invalid priority number.\n");
                break;
        }
    }

    // Run the algorithms

    FILE *file = fopen("output.txt", "w");  // Open the file in write mode
    
    fcfs(pr0, PriorityCounts[0], CPU1_RAM, MAX_CPU_RATE, file);
    sjf(pr1, PriorityCounts[1], CPU2_RAM, MAX_CPU_RATE, file);
    rr_algorithm(pr2, PriorityCounts[2], CPU2_RAM, MAX_CPU_RATE, file, QUANTUM8);
    rr_algorithm(pr3, PriorityCounts[3], CPU2_RAM, MAX_CPU_RATE, file, QUANTUM16);


    // Free the memory allocated for the processArr
    for (int i = 0; i < numProcesses; i++) {
        free(processes[i].process_number);
    }
    free(processes);

    return 0;
}


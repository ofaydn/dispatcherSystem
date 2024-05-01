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

void extractInput
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
    create_archive(argc, argv);
    
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

void extract_archive(char *input_file) {
    FILE *file = fopen(input_file, "r");
    if (file == NULL) {
        printf("Failed to open archive %s.\n",input_file);
        exit(1);
    }
    char buffer[11]; // Buffer to store the first 10 characters (+1 for null terminator)
    if (fscanf(file, "%10s", buffer) != 1) {
        printf("Failed to read archive  section size.\n");
        fclose(file);
        exit(1);
    }
    int archiveSize = atoi(buffer);
    int num_files = 0;
    ProcessInfo processes[MAX_FILES];
    if (fseek(file, 10, SEEK_SET) != 0) {
        printf("Failed to seek to the 11th character.\n");
        fclose(file);
        exit(1);
    }
    fseek(file,10,SEEK_SET);
    char buff[archiveSize + 1 -10];
    size_t sectionLength = archiveSize -10;
    size_t bytes_read = fread(buff,sizeof(char),sectionLength,file);
    buff[bytes_read] = '\0'; //null terminate
    //printf("%s\n",buff);
    if (buff != NULL && num_files < MAX_FILES) {
        char *token = strtok(buff, "!|");
        while(token !=NULL){
        	files[num_files].name = malloc(strlen(token) + 1);
		if (sscanf(token, "%[^,],%d,%d", files[num_files].name, &files[num_files].perms, &files[num_files].size) == 3) {
		        num_files++;        
		    }
        token = strtok(NULL, "!|");
        }  
    
    fseek(file,archiveSize,SEEK_SET);
    int i;
    for (i = 0; i < num_files; ++i) {
        fscanf(file, "%d\n", &files[i].size);
        files[i].content = malloc(files[i].size + 1); // Allocate memory
        if (files[i].content != NULL) {
            if (fgets(files[i].content, files[i].size + 1, file) != NULL) {
                // Remove newline character if present
                size_t len = strlen(files[i].content);
                if (len > 0 && files[i].content[len - 1] == '\n') {
                    files[i].content[len - 1] = '\0';
                }
            } else {
                fprintf(stderr, "Failed to read content for file %d.\n", i + 1);
                free(files[i].content); // Free memory if content reading fails
                break;
            }
        } else {
            fprintf(stderr, "Memory allocation failed for file %d.\n", i + 1);
            brex

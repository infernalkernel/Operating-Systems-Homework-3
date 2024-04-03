#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

#define FRAME_SIZE 20

//Define a structure with integer and character members: pageNumber and accessType respectively
typedef struct PageReference{
    int pageNumber;
    char accessType;
} PageReference;

int countLinesInFile(const char *filePath) {
    //Open file for read access
    FILE *file = fopen(filePath, "r");
    if (!file) {
        perror("Error opening file");
        return -1;
    }

    //Count Lines
    int lines = 0;
    char ch;
    while (!feof(file)) {
        ch = fgetc(file);
        if (ch == '\n') {
            lines++;
        }
    }

    // If the last line doesn't end with a newline but contains data, it should still count
    if (ch != '\n' && lines > 0) {
        lines++;
    }

    fclose(file);
    return lines;
}

//Method to parse input file into PageReference structure for easy access
int readPageReferences(const char* filePath, PageReference **references) {
    //Count number of lines
    int numLines = countLinesInFile(filePath);
    if (numLines < 0) {
        return -1; // File error or no lines read
    }

    //Open file for read access
    FILE *file = fopen(filePath, "r");
    if (!file) {
        perror("Error opening file");
        return -1;
    }

    //Allocate sufficient memory for all references
    *references = (PageReference *)malloc(numLines * sizeof(PageReference));
    if (!*references) {
        perror("Memory allocation failed");
        fclose(file);
        return -1;
    }

    //Read all references into PageReference structure
    int count = 0;
    while (fscanf(file, "%d %c", &((*references)[count].pageNumber), &((*references)[count].accessType)) == 2) {
        count++;
    }

    fclose(file);
    return count;
}

int fifo(PageReference *references, int refCount){
    int frames[FRAME_SIZE];
    int front = 0;
    int pageFaults = 0;
    bool pageInMemory;

    //Initialize frames empty to simulate cold start
    for(int i = 0; i < FRAME_SIZE; i++){
        frames[i] = -1;
    }

    for (int i = 0; i < refCount; i++){
        //Check if page is in memory
        pageInMemory = false;
        for(int j = 0; j < FRAME_SIZE; j++){
            if(frames[j] == references[i].pageNumber){
                pageInMemory = true;
                break;

            }

        }

        //Replace page first-in and increment pageFaults
        if(!pageInMemory){
            frames[front] = references[i].pageNumber;
            front = (front + 1) % FRAME_SIZE;
            pageFaults++;
        }
    }

    return pageFaults;
}

int lru(PageReference *references, int refCount) {
    int frames[FRAME_SIZE];
    int lastUsed[FRAME_SIZE];
    int pageFaults = 0;
    int leastRecentlyUsed;
    int time = 0;
    bool pageInMemory;

    //Initialize frame and last used time
    for (int i = 0; i < FRAME_SIZE; i++) {
        frames[i] = -1;
        lastUsed[i] = -1;
    }

    for (int i = 0; i < refCount; i++) {
        pageInMemory = false;
        leastRecentlyUsed = time;

        //Check if page is in memory and last used
        for (int j = 0; j < FRAME_SIZE; j++) {
            if (frames[j] == references[i].pageNumber) {
                pageInMemory = true;
                lastUsed[j] = time++;
                break;
            }

            if (lastUsed[j] < leastRecentlyUsed) {
                leastRecentlyUsed = lastUsed[j];
            }
        }

        //Handle page fault and update LRU
        if (!pageInMemory) {
            for (int j = 0; j < FRAME_SIZE; j++) {
                if (lastUsed[j] == leastRecentlyUsed) {
                    frames[j] = references[i].pageNumber;
                    lastUsed[j] = time++;
                    break;
                }
            }
            pageFaults++;
        }
    }

    return pageFaults;
}

int main() {
    //Initialize references structure, and load with parsed data
    PageReference *references;
    int refCount = readPageReferences("page_references.txt", &references);

    //Run fifo method and calculate fault rate
    int fifoFaults = fifo(references, refCount);
    double fifoFaultRate = (double)fifoFaults / refCount * 100;

    int lruFaults = lru(references, refCount);
    double lruFaultRate = (double)lruFaults / refCount * 100; 

    printf("Num FIFO faults: %d (%.2f%% of references)\n", fifoFaults, fifoFaultRate);
    printf("Num LRU faults: %d (%.2f%% of references)\n", lruFaults, lruFaultRate);



    free(references);
    return 0;
}
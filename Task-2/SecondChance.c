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

int secondChance(PageReference *references, int refCount) {
    int frames[FRAME_SIZE];
    int refBits[FRAME_SIZE];
    int front = 0;
    int pageFaults = 0;
    bool pageInMemory;

    //Initilize frame to empty and initialize ref bit
    for (int i = 0; i < FRAME_SIZE; i++) {
        frames[i] = -1;
        refBits[i] = 0;
    }

    for (int i = 0; i < refCount; i++) {
        pageInMemory = false;

        //Check if reference is in memory
        for (int j = 0; j < FRAME_SIZE; j++) {
            if (frames[j] == references[i].pageNumber) {
                pageInMemory = true;
                refBits[j] = 1;
                break;
            }
        }

        //Handle page fault
        if (!pageInMemory) {
            while (refBits[front]) {
                refBits[front] = 0; // Clear the reference bit
                front = (front + 1) % FRAME_SIZE;
            }

            frames[front] = references[i].pageNumber;
            front = (front + 1) % FRAME_SIZE;
            pageFaults++;
        }
    }

    return pageFaults;
}

int main() {
    //Initialize references structure, and load with parsed data
    PageReference *references;
    int refCount = readPageReferences("page_references.txt", &references);

    //Run secondChance method and calculate fault rate
    int secondChanceFaults = secondChance(references, refCount);
    double secondChanceFaultRate = (double)secondChanceFaults / refCount * 100;

    printf("Num secondChance faults: %d (%.2f%% of references)\n", secondChanceFaults, secondChanceFaultRate);

    free(references);
    return 0;
}
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

#define FRAME_SIZE 200

// Define a structure with integer and character members: pageNumber and accessType respectively
typedef struct PageReference{
    int pageNumber;
    char accessType; // 'r' for read, 'w' for write
} PageReference;

// Define a structure with integer, and 2 boolean members: pageNumber, referenceBit, and modifyBit respectively
typedef struct {
    int pageNumber;
    bool referenceBit;
    bool modifyBit;
} Frame;

// Method to count number of references in order to properly allocate memory
int countLinesInFile(const char *filePath) {
    // Open file for read access
    FILE *file = fopen(filePath, "r");
    if (!file) {
        perror("Error opening file");
        return -1;
    }

    // Count Lines
    int lines = 0;
    int ch; // int to handle EOF properly
    while ((ch = fgetc(file)) != EOF) {
        if (ch == '\n') {
            lines++;
        }
    }

    fclose(file);
    // If the file doesn't end with a newline, add the last line if it exists
    if (ch != '\n' && lines > 0) lines++;
    return lines;
}

// Method to parse input file into PageReference structure for easy access
int readPageReferences(const char* filePath, PageReference **references) {
    // Count number of lines
    int numLines = countLinesInFile(filePath);
    if (numLines < 0) {
        return -1; // File error or no lines read
    }

    // Open file for read access
    FILE *file = fopen(filePath, "r");
    if (!file) {
        perror("Error opening file");
        return -1;
    }

    // Allocate sufficient memory for all references
    *references = (PageReference *)malloc(numLines * sizeof(PageReference));
    if (!*references) {
        perror("Memory allocation failed");
        fclose(file);
        return -1;
    }

    // Read all references into PageReference structure
    int count = 0;
    while (fscanf(file, "%d %c", &((*references)[count].pageNumber), &((*references)[count].accessType)) == 2) {
        count++;
    }

    fclose(file);
    return count;
}

// Implement the Enhanced Second Chance Algorithm
int enhancedSecondChance(PageReference *references, int refCount) {
    Frame frames[FRAME_SIZE];
    int pointer = 0;
    int pageFaults = 0;
    int diskWrites = 0;

    for (int i = 0; i < refCount; i++) {
        bool pageFound = false;

        // Check if the page is already loaded
        for (int j = 0; j < FRAME_SIZE; j++) {
            if (frames[j].pageNumber == references[i].pageNumber) {
                pageFound = true;
                frames[j].referenceBit = true; // Mark as recently accessed
                frames[j].modifyBit = frames[j].modifyBit || (references[i].accessType == 'w'); // Update modify bit if it's a write access
                break;
            }
        }

        //Handle page faults
        if (!pageFound) {
            bool replaced = false;
            int pass = 0;
            while (!replaced) {
                for (int j = 0; j < FRAME_SIZE && !replaced; j++) {
                    pointer = (pointer + 1) % FRAME_SIZE;
                    if (!frames[pointer].referenceBit && !frames[pointer].modifyBit) {
                        // Replace the page
                        if (frames[pointer].pageNumber != -1 && frames[pointer].modifyBit) {
                            diskWrites++; // If the page was modified, it needs to be written to disk
                        }
                        frames[pointer].pageNumber = references[i].pageNumber;
                        frames[pointer].referenceBit = true; // Mark it as recently used
                        frames[pointer].modifyBit = (references[i].accessType == 'w'); // Set modify bit if write access
                        pageFaults++;
                        replaced = true;
                    }
                }

                if (!replaced && pass == 0) {
                    // On the first pass, if no suitable frame is found, reset the reference bits for the next pass
                    for (int j = 0; j < FRAME_SIZE; j++) {
                        frames[j].referenceBit = false;
                    }
                    pass = 1; // Move to the second pass
                } else if (!replaced && pass == 1) {
                    // On the second pass, we force replace even if it's modified since no ideal page (0,0) was found
                    pointer = (pointer + 1) % FRAME_SIZE; // Ensure we move the pointer forward
                    if (frames[pointer].pageNumber != -1 && frames[pointer].modifyBit) {
                        diskWrites++;
                    }
                    frames[pointer].pageNumber = references[i].pageNumber;
                    frames[pointer].referenceBit = true;
                    frames[pointer].modifyBit = (references[i].accessType == 'w');
                    pageFaults++;
                    replaced = true;
                }
            }
        }
    }
    

    printf("Disk writes: %d\n", diskWrites);
    return pageFaults;
}

int main() {
    // Initialize references structure, and load with parsed data
    PageReference *references;
    int refCount = readPageReferences("page_references.txt", &references);

    // Run enhancedSecondChance method and calculate fault rate
    int enhancedSecondChanceFaults = enhancedSecondChance(references, refCount);
    double enhancedSecondChanceFaultRate = (double)enhancedSecondChanceFaults / refCount * 100;

    printf("Num enhancedSecondChance faults: %d (%.2f%% of references)\n", enhancedSecondChanceFaults, enhancedSecondChanceFaultRate);

    free(references);
    return 0;
}
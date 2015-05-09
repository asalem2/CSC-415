//Homework 6
//Created by Eric Gonzalez

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>

int pageSize;
int memSize;
int pagingMode;
int frameNumber;

int isInArray(int findme, int array[], int size){
    int index = 0;
    do{
        if (array[index] == findme)
            return index;
        index++;
    }while (index < size);
    
    return -1;
}


int searchArrayBackwardsFrom(int findme, int start, int array[], int size){
    int index = start;
    do{
        if (array[index] == findme)
            return index;
        index--;
    } while (index >= 0);
    
    return -1;
}

int fifo(int prefs[], int size){
    int replace = 0, numOfPageFaults = 0, i;
    int *frames = (int *)malloc(frameNumber * sizeof(int));
    for (i = 0; i < 3; i++){
        frames[i] = -1;
    }
    for (i = 0; i < size; i++){ 
        if (isInArray(prefs[i], frames, frameNumber) < 0){
            frames[replace % frameNumber] = prefs[i];
            replace++;
            numOfPageFaults++;
        }
    }
    
    return numOfPageFaults;
}

int lru (int prefs[], int size){
    int numOfPageFaults = 0, index, i, j, innerIndex = 0, referece, t;
    int *frames = (int *)malloc(frameNumber * sizeof(int));
    for (i = 0; i < 3; i++){
        frames[i] = -1;
    }
    int *b = (int *)malloc(frameNumber * sizeof(int));
    int *c2 = (int *)malloc(frameNumber * sizeof(int));
    frames[innerIndex] = prefs[innerIndex];
    numOfPageFaults++;
    innerIndex++;
    for (i = 1; i< size; i++){ 
        index = 0;
        if (isInArray(prefs[i], frames, frameNumber) < 0){
            index = frameNumber;
            numOfPageFaults++;
            if (innerIndex < frameNumber){
                frames[innerIndex] = prefs[i];
                innerIndex++;
            }
            else{
                for (referece = 0; referece < frameNumber; referece++){
                    c2[referece] = 0;
                    for (j = i - 1; j < pageSize; j--){
                        if (frames[referece] != prefs[j])
                            c2[referece]++;
                        else
                            break;
                    }
                }
                for (referece = 0; referece < frameNumber; referece++)
                    b[referece] = c2[referece];
                for (referece = 0; referece < frameNumber; referece++){
                    for (j = referece; j < frameNumber; j++){
                        if (b[referece]<b[j]){
                            t = b[referece];
                            b[referece] = b[j];
                            b[j] = t;
                        }
                    }
                }
                for (referece = 0; referece < frameNumber; referece++){
                    if (c2[referece] == b[0])
                        frames[referece] = prefs[i];
                }
            }
        }
    }
    
    return numOfPageFaults;
}

int main(int argc, char *argv[]){
    pageSize = atoi(argv[1])/10;
    memSize = atoi(argv[2])*4;
    if (strcmp(argv[3], "FIFO") == 0){
        pagingMode = 0;
    }
    else{
        pagingMode = 1;
    }
    
    frameNumber = atoi(argv[4]);
    if (pagingMode == 0){
        printf("Page Size: %dkb\nMemory Size: %dkb\nAlgorithm: FIFO\nFrames Allocated: %d\n",pageSize,memSize,frameNumber);
    }
    else{
        printf("Page Size: %dkb\nMemory Size: %dkb\nAlgorithm: LRU\nFrames Allocated: %d\n",pageSize,memSize,frameNumber);
    }
    int prefs[100];
    int g = 0;
    while (g < 100){
        prefs[g] = 0;
        g++;
    }
    int check = 0;
    char exit[8] = " ";
    int number;
    printf("Enter the addresses (eg: 0xFC68):\n");
    while (fgets(exit, sizeof(exit), stdin)){
        exit[strlen(exit) - 1] = '\0';
        if (strcmp(exit, "") == 0)
            break;
        for (int z = 0; z < 8; z++){
            if (exit[z] == 'A' || exit[z] == 'a')
                exit[z] = '3';
            else if(exit[z] == 'B' || exit[z] == 'b')
                exit[z] = '5';
            else if(exit[z] == 'C' || exit[z] == 'c')
                exit[z] = '7';
            else if(exit[z] == 'D' || exit[z] == 'd')
                exit[z] = '9';
            else if(exit[z] == 'E' || exit[z] == 'e')
                exit[z] = '0';
            else if(exit[z] == 'F' || exit[z] == 'f')
                exit[z] = '1';
            else if(exit[z] == 'X' || exit[z] == 'x')
                exit[z] = '4';
        }
        prefs[check] = atoi(exit);
        check++;
    }
    if (pagingMode == 0){
        printf("Number of faults: %d\n\n", fifo(prefs, check));
    }
    else if (pagingMode == 1){
        printf("Number of faults: %d\n\n", lru(prefs, check));
    }
    
    return 0;
}
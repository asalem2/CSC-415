//  Homework 5
//  Created by Eric Gonzalez

#include <stdio.h>
#include <pthread.h>
#include <time.h>
#include <unistd.h>
#include <fcntl.h>
#define NUM_THREADS 8
#define BUFFERLENGTH 65536

int asciiCharacters[128];
char buffer[BUFFERLENGTH];


typedef struct{
    int partitionBounds;
    int threadNumber;
    int max_length;
} thread_information;

void *ascii_checker(void *t_info) {
    int i, threadLocation;
    thread_information *threadinfo = (thread_information*) t_info;
    threadLocation = threadinfo->threadNumber * threadinfo->partitionBounds;
    for(i = 0; threadLocation + i < threadinfo->max_length && i < threadinfo->partitionBounds; i++) {

        asciiCharacters[buffer[threadLocation + i]]++;

    } 
    
    pthread_exit(NULL);
} 

double Seconds(void){
    return (double)clock()/CLOCKS_PER_SEC;
} 
//Main
int main(int argc, char* argv[]){
    pthread_t threadIds[NUM_THREADS];
    thread_information t_info[NUM_THREADS];
    int i, j, sourceFile, partitions, charTotal, aThread;
    
    sourceFile = open(argv[1], O_RDONLY);
    
    if (sourceFile < 0){
        fprintf(stderr, "Error opening file!\n");
        return -1;
    } 
    sourceFile = read(sourceFile, buffer, BUFFERLENGTH);
    partitions = (double) sourceFile / (double) NUM_THREADS;
    
    for(i = 0; i < NUM_THREADS; i++) {
        t_info[i].threadNumber = i;
        t_info[i].partitionBounds = partitions;
        t_info[i].max_length = sourceFile;
        
        aThread = pthread_create(&threadIds[i], NULL, &ascii_checker, &t_info[i]);
        
        if(aThread != 0)
            printf("Error creating thread! %i\n", i);
    }
    i = 0;
    do{
        pthread_join(threadIds[i], NULL);
        i++;
    } while (i < NUM_THREADS);
    
    for(i = 0; i < 128; i++) {
        printf("%i occurrences of ", asciiCharacters[i]);
        if(i < 33 || i == 127){
            printf("%#0x\n", i);
        } 
        else{
            printf("%c\n", i);
        } 
    }
    close(sourceFile);
    return 0;
}


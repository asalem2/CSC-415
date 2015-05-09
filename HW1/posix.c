//Eric Gonzalez
//CSC 415 - HW #4

#include <stdio.h>
#include <pthread.h>
#include <time.h>
#include <unistd.h>
#include <sys/stat.h>
#include <fcntl.h>


#define NUM_THREADS 1
#define BUFFERLENGTH 65536

int ascii_characters[NUM_THREADS][128];
char buffer[BUFFERLENGTH];

typedef struct{
    int partition_bounds;
    int thread_number;
} thread_information;

void *runner(void *t_info) {
    int i, threadLocation;
    char current_char;

    
    thread_information threadinfo = *(thread_information*) t_info;
    threadLocation = threadinfo.thread_number * threadinfo.partition_bounds;
    current_char = buffer[threadLocation];

    
    for(i = 0; threadLocation < BUFFERLENGTH && i < threadinfo.partition_bounds; i++) {
        current_char = buffer[threadLocation++];
        ascii_characters[threadinfo.thread_number][current_char]++;
    }

    
    pthread_exit(NULL);
}

double Seconds(void){
    return (double)clock()/CLOCKS_PER_SEC;
}

int main(int argc, char* argv[]){

    
    pthread_t threads[NUM_THREADS];
    thread_information t_info[NUM_THREADS];
    int i, j, sourceFile, partitions, charTotal, aThread;
    double t;

    
    sourceFile = open(argv[1], O_RDONLY);

    
    if (sourceFile < 0){
        fprintf(stderr, "Error opening file!\n");
        return -1;
    } 

    
    sourceFile = read(sourceFile, buffer, BUFFERLENGTH);

    
    t = Seconds();

    
    partitions = (double) sourceFile / (double) NUM_THREADS;
    close(sourceFile);

    
    for(i = 0; i < NUM_THREADS; i++) {
        t_info[i].thread_number = i;
        t_info[i].partition_bounds = partitions;
        aThread = pthread_create(&threads[i], NULL, &runner, &t_info[i]);

        
        if(aThread < 0)
            printf("Error creating thread! %i\n", i);
    } 

    
    i = 0;
    do{
        pthread_join(threads[i], NULL);
        i++;
    } while (i < NUM_THREADS);

    
    for(i = 0; i < 128; i++) {
        charTotal = 0;
        for(j = 0; j < NUM_THREADS; j++) {
            charTotal += ascii_characters[j][i];
        } 

        
        printf("%i occurrences of ", charTotal);

        
        if(i < 33 || i == 127){
            printf("%#0x\n", i);
        }
        else{
            printf("%c\n", i);
        } 
    } 

    
    t = Seconds() - t;
    

    
    return 0;
} 
//Eric Gonzalez
//CSC 415 - Hw #4

#include <stdio.h>
#include <Windows.h>
#include <time.h>


#define NUM_THREADS 1
#define BUFFERLENGTH 65536

int ascii_characters[NUM_THREADS][128];
char buffer[BUFFERLENGTH];

typedef struct{
int partition_bounds;
int thread_number;
} thread_information;

DWORD WINAPI runner(LPVOID t_info) {
int i, threadLocation;
char current_char;

thread_information threadinfo = *(thread_information*)t_info;
threadLocation = threadinfo.thread_number * threadinfo.partition_bounds;
current_char = buffer[threadLocation];

for (i = 0; threadLocation < BUFFERLENGTH && i < threadinfo.partition_bounds; i++) {
current_char = buffer[threadLocation++];
ascii_characters[threadinfo.thread_number][current_char]++;
}
}

double Seconds(void){
return (double)clock() / CLOCKS_PER_SEC;
}

int main(int argc, char* argv[]){
HANDLE sourceFile;
DWORD sourceFileLength;
HANDLE threads[NUM_THREADS];
thread_information t_info[NUM_THREADS];
int partitions, charTotal, i, j;
double t;


sourceFile = CreateFile(argv[1], GENERIC_READ, 0, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);

if (sourceFile == INVALID_HANDLE_VALUE){
fprintf(stderr, "Error opening file\n");
return -1;
}

t = Seconds();

ReadFile(sourceFile, buffer, BUFFERLENGTH, &sourceFileLength, NULL);
CloseHandle(sourceFile);
partitions = (double)sourceFileLength / (double)NUM_THREADS;
i = 0;
do{
t_info[i].partition_bounds = partitions;
t_info[i].thread_number = i;

threads[i] = CreateThread(NULL, 0, runner, &t_info[i], 0, NULL);
if (threads[i] == NULL){
fprintf(stderr, "Error creating thread #%i\n", i);
return -1;
}
i++;
} while (i < NUM_THREADS);

/* waiting for the threads to finish execution */
WaitForMultipleObjects(NUM_THREADS, threads, TRUE, INFINITE);

/* printing out the data */
for (i = 0; i < 128; i++) {
charTotal = 0;
for (j = 0; j < NUM_THREADS; j++) {
charTotal += ascii_characters[j][i];
}
printf("%i occurrences of ", charTotal);
if (i <= 32 || i == 127)
printf("%#02x\n", i);
else
printf("%c\n", i);
}


return 0;
}

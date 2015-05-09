//  Homework 5
//  Created by Eric Gonzalez

#include <stdio.h>
#include <Windows.h>
#include <time.h>
#define NUM_THREADS 4
#define BUFFERLENGTH 65536

char buffer[BUFFERLENGTH];
int ascii_characters[128];

typedef struct{
	int partition_bounds;
	int thread_number;
	int max_length;
} thread_information;
//Function it uses to count the ASCII characters
DWORD WINAPI ascii_checker(LPVOID t_info){
	int i, threadLocation;
	thread_information threadinfo = *(thread_information*)t_info;
	threadLocation = threadinfo.thread_number * threadinfo.partition_bounds;
	for (i = 0; threadLocation + i < threadinfo.max_length && i < threadinfo.partition_bounds; i++) {
		ascii_characters[buffer[threadLocation + i]]++;
	}
}
//Main
int main(int argc, char* argv[]){
	HANDLE sourceFile;
	DWORD sourceFileLength;
	HANDLE threads[NUM_THREADS];
	thread_information t_info[NUM_THREADS];
	int partitions, charTotal, i, j;
	sourceFile = CreateFile(argv[1], GENERIC_READ, 0, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
	if (sourceFile == INVALID_HANDLE_VALUE){
		printf("\nError opening file\n");
		return 1;
	}
	ReadFile(sourceFile, buffer, BUFFERLENGTH, &sourceFileLength, NULL);
	partitions = (double)sourceFileLength / (double)NUM_THREADS;
	i = 0;
	do{
		t_info[i].partition_bounds = partitions;
		t_info[i].thread_number = i;
		t_info[i].max_length = sourceFileLength;

		threads[i] = CreateThread(NULL, 0, ascii_checker, &t_info[i], 0, NULL);
		if (threads[i] == NULL){
			fprintf(stderr, "Error creating thread #%i\n", i);
			return 1;
		}
		i++;
	} while (i < NUM_THREADS);

	WaitForMultipleObjects(NUM_THREADS, threads, TRUE, INFINITE);
	for (i = 0; i < 128; i++) {
		printf("%i occurrences of ", ascii_characters[i]);
		if (i <= 32 || i == 127){
			printf("%#02x\n", i);
		}
		else{
			printf("%c\n", i);
		}
	}
	CloseHandle(sourceFile);
	return 0;
}

//Homework 5
//Created by Eric Gonzalez

#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <windows.h>

#define BUFFERLENGTH 16


struct p_data {
    int thread_number;
    int num_produced;
};

typedef struct stack {
    int *elements;
    int capacity;
    int size;
} stack;

stack buffer;
HANDLE full;
HANDLE empty;
HANDLE lock;

void push(stack *stack, int element) {
    if (stack->size != stack->capacity) {
        stack->elements[stack->size] = element;
        stack->size++;
    }
    else {
        exit(1);
    }
}

int pop(stack *stack) {
    int top;
    if (stack->size != 0) {
        stack->size--;
        top = stack->elements[stack->size];
    }
    else {
        exit(1);
    }
    return top;
}

DWORD WINAPI produce(LPVOID param) {
    struct p_data *data = (struct p_data *)param;
    int item;
    int counter = 0;
    while (counter < data->num_produced) {
            item = data->thread_number * data->num_produced + counter;
            WaitForSingleObject(empty, INFINITE);
            WaitForSingleObject(lock, INFINITE);
            push(&buffer, item);
            ReleaseSemaphore(lock, 1, NULL);
            ReleaseSemaphore(full, 1, NULL);
            counter++;
    }
    return 0;
}

DWORD WINAPI consume(LPVOID param) {
    int num_consumed = *(int *) param;
    int item;
    int i = 0;
    while (i < num_consumed) {
            WaitForSingleObject(full, INFINITE);
            WaitForSingleObject(lock, INFINITE);
            item = pop(&buffer);
            ReleaseSemaphore(lock, 1, NULL);
            ReleaseSemaphore(empty, 1, NULL);
            printf("%i\n", item);
            i++;
    }
    return 0;
}

//Main
int main(int argc, char *argv[]){
    int num_producers, num_consumers, num_total_produced, num_threads, produced_per_thread, consumed_per_thread, i;
    HANDLE *producers;
    HANDLE *consumers;
    struct p_data *producer_data;
    if (argc != 4) {
            fprintf(stderr, "usage: a.out p c i\n");
            return 1;
    }
    buffer.elements = (int *) malloc(BUFFERLENGTH * sizeof(int));
    buffer.capacity = BUFFERLENGTH;
    buffer.size = 0;
    full = CreateSemaphore(NULL, 0, BUFFERLENGTH, NULL);
    empty = CreateSemaphore(NULL, BUFFERLENGTH, BUFFERLENGTH, NULL);
    lock = CreateSemaphore(NULL, 1, 1, NULL);
    num_producers = pow(2, atoi(argv[1]));
    num_consumers = pow(2, atoi(argv[2]));
    num_total_produced = pow(2, atoi(argv[3]));
    num_threads = num_producers + num_consumers;
    printf("Number of producer threads: %i\nNumber of consumer threads: %i\nItems to produce: %i\n", num_producers, num_consumers, num_total_produced);
    produced_per_thread = num_total_produced / num_producers;
    consumed_per_thread = num_total_produced / num_consumers;
    producers = (HANDLE *) malloc(num_producers * sizeof(HANDLE));
    consumers = (HANDLE *) malloc(num_consumers * sizeof(HANDLE));
    producer_data = (struct p_data *) malloc(num_producers * sizeof(struct p_data));
    i = 0;
    while (i < num_producers) {
        producer_data[i].thread_number = i;
        producer_data[i].num_produced = produced_per_thread;
        producers[i] = CreateThread(NULL, 0, produce, &producer_data[i], 0, NULL);
        if (producers[i] == NULL) {
                fprintf(stderr, "error creating producer %i\n", i);
                return 1;
        }
        i++;
    }
    i = 0;
    while (i < num_consumers) {
        consumers[i] = CreateThread(NULL, 0, consume, &consumed_per_thread, 0, NULL);
        if (consumers[i] == NULL) {
                fprintf(stderr, "error creating consumer %i\n", i);
                return 1;
        }
        i++;
    }
    WaitForMultipleObjects(num_producers, producers, TRUE, INFINITE);
    WaitForMultipleObjects(num_consumers, consumers, TRUE, INFINITE);
    free(producers);
    free(consumers);
    free(producer_data);
    free(buffer.elements);
    CloseHandle(lock);
    CloseHandle(full);
    CloseHandle(empty);
    printf("Finished Successfully!\n");
    return 0;
}

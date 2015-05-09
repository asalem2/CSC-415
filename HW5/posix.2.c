//Homework 5
//Created by Eric Gonzalez	

     	#include <stdlib.h>
	#include <stdio.h>
	#include <pthread.h>
	#include <sys/types.h>
	#include <sys/stat.h>
	#include <fcntl.h>
	#include <math.h>
	#include <unistd.h>
	#include <semaphore.h>
	
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
	sem_t full;
	sem_t empty;
	pthread_mutex_t lock;
	
	
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
	void *produce(void *param) {
		struct p_data *data = (struct p_data *)param;
		int item;
		int counter = 0;
		while (counter < data->num_produced) {
			item = data->thread_number * data->num_produced + counter;
			sem_wait(&empty);
			pthread_mutex_lock(&lock);
			push(&buffer, item);
			pthread_mutex_unlock(&lock);
			sem_post(&full);
			counter++;
		}
		pthread_exit(NULL);
	}
	void *consume(void *param) {
		int num_consumed = *(int *) param;
		int item;
		int i = 0;
		while (i < num_consumed) {
			sem_wait(&full);
			pthread_mutex_lock(&lock);
			item = pop(&buffer);
			pthread_mutex_unlock(&lock);
			sem_post(&empty);
			printf("%i\n", item);
			i++;
		}
		pthread_exit(NULL);
	}
int main(int argc, char *argv[]){
	int num_producers;
	int num_consumers;
	int num_total_produced;
	int num_threads;
	int produced_per_thread;
	int consumed_per_thread;
	int i;
	pthread_t *producers;
	pthread_t *consumers;
	struct p_data *producer_data;
	
	if (argc != 4) {
	fprintf(stderr, "usage: a.out p c i\n");
	return 1;
	}
	
	buffer.elements = (int *) malloc(BUFFERLENGTH * sizeof(int));
	buffer.capacity = BUFFERLENGTH;
	buffer.size = 0;
	
	sem_init(&full, 0, 0);
	sem_init(&empty, 0, BUFFERLENGTH);
	
	if (pthread_mutex_init(&lock, NULL) != 0) {
	fprintf(stderr, "could not initialize mutex\n");
	return 1;
	}
	
	num_producers = pow(2, atoi(argv[1]));
	num_consumers = pow(2, atoi(argv[2]));
	num_total_produced = pow(2, atoi(argv[3]));
	num_threads = num_producers + num_consumers;
	
	printf("Number of producer threads: %i\nNumber of consumer threads: %i\nItems to produce: %i\n",
	num_producers, num_consumers, num_total_produced);
	
	produced_per_thread = num_total_produced / num_producers;
	consumed_per_thread = num_total_produced / num_consumers;
	
	producers = (pthread_t *) malloc(num_producers * sizeof(pthread_t));
	consumers = (pthread_t *) malloc(num_consumers * sizeof(pthread_t));
	producer_data = (struct p_data *) malloc(num_producers * sizeof(struct p_data));
	
	i = 0;
	while (i < num_producers) {
	producer_data[i].thread_number = i;
	producer_data[i].num_produced = produced_per_thread;
	if (pthread_create(&producers[i], NULL, &produce, &producer_data[i]) != 0) {
	fprintf(stderr, "error creating producer %i\n", i);
	return 1;
	}
	i++;
	}
	
	i = 0;
	while (i < num_consumers) {
	if (pthread_create(&consumers[i], NULL, &consume, &consumed_per_thread) != 0) {
	fprintf(stderr, "error creating consumer %i\n", i);
	return 1;
	}
	i++;
	}
	
	i = 0;
	while (i < num_producers) {
	pthread_join(producers[i], NULL);
	i++;
	}
	
	i = 0;
	while (i < num_consumers) {
	pthread_join(consumers[i], NULL);
	i++;
	}
	
	free(producers);
	free(consumers);
	free(producer_data);
	free(buffer.elements);
	pthread_mutex_destroy(&lock);
	sem_destroy(&full);
	sem_destroy(&empty);
	
	printf("Finished Successfully!\n");
	return 0;
	}

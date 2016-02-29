#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include "etime.h"


int thread_count;

void *Hello (void* rank);

int main(int argc, char* argv[]) {

	/*
		./kmeans K 			  P 		  inputFile outputFile
	    	     numClusters  Processors

	*/

	int K, processors;
	FILE *inputFile, *outputFile;

	if (argc != 5) {
		printf("%i\n", argc);
		printf("Incorrect parameters.\n");
		exit(0);
	} else {
		K = strtol(argv[1], NULL, 10);
		processors = strtol(argv[2], NULL, 10);
		inputFile = fopen(argv[3], "r");
		outputFile = fopen("output.txt", "w+");
	}
	
	printf("Threads: %i, Processors: %i\n", K, processors);


	//Reading in data to dynamic array
	int samples, dimensions;
	fscanf(inputFile, "%i", &samples);
	fscanf(inputFile, "%i", &dimensions);
	printf("Samples: %i, Dimensions: %i\n", samples, dimensions);

	//while(fgets(line, 80, fr) != NULL) {


	//}





	long thread;
	pthread_t* thread_handles;

	thread_count = strtol(argv[1], NULL, 10);

	thread_handles = malloc(thread_count*sizeof(pthread_t));

	for (thread = 0; thread < thread_count; thread++) {
		pthread_create(&thread_handles[thread], NULL, Hello, (void*) thread);
	}

	printf("Hello from main thread\n");

	for (thread = 0; thread < thread_count; thread++) {
		pthread_join(thread_handles[thread], NULL);
	}

	free(thread_handles);

	return 0;

}

void *Hello(void* rank) {
	long my_rank = (long) rank;

	printf("Thread %ld of %d\n", my_rank, thread_count);

	return NULL;

}
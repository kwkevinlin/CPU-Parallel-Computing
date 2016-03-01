#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <string.h> //memset
#include <math.h>
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

	//For all loops
	int i, j, y, z;

	//Creating data[samples][dimensions]
	double **data = (double **)malloc(samples * sizeof(double *));
    for (i = 0; i < samples; i++)
         data[i] = (double *)malloc(dimensions * sizeof(double));

    //Reading in coordinates to 2D array
    double buffer;
    for (i = 0; i < samples; i++) {
    	for (j = 0; j < dimensions; j++) {
    		fscanf(inputFile, "%lf", &buffer);
    		data[i][j] = buffer;
    		//printf("%lf ", data[i][j]);
    	}
    }

    //Creating clusterInfo to store coordinates of each cluster
    double **clusterInfo = (double **)malloc(K * sizeof(double *));
    for (i = 0; i < K; i++)
         clusterInfo[i] = (double *)malloc(dimensions * sizeof(double));

    //Creating dataClusterIndex to map samples to corresponding cluster
    int *dataClusterIndex = (int *)malloc(samples * sizeof(int));

    //2. Seting initial cluster coordinates as coordinates of sample 1, 2, .. K
    for (i = 0; i < K; i++) {
		for (j = 0; j < dimensions; j++) {
			clusterInfo[i][j] = data[i][j]; //Populate initial cluster with first K samples (ie. cluster 1 = sample 1)
			printf("Cluster %i:%i = %lf ", i, j, clusterInfo[i][j]);
		}
		printf("\n");
	}
	printf("\n");

	//Variable declarations for step 3 below
	double dist, minDist = 100000;
	int minCluster;
	//int clusterIndex[K] = {0}; //Index to imitate stack in array
	//For step 4
	int clusterElements[K];
	memset(clusterElements, 0, sizeof(clusterElements));

	/*==========================================================================
		Major loop starts here
	*/
	for (z = 0; z < 5; z++) { //Repeat process for a maximum of 100 iterations

		memset(clusterElements, 0, sizeof(clusterElements));

		//3. For each element in your data, assign it to the cluster it's closest to.
		for (i = 0; i < samples; i++) { //Per sample
			for (j = 0; j < K; j++) { // Per cluster
				double calcKMeansBuffer = 0;
				for (y = 0; y < dimensions; y++) { //Arbitrary dimensions

					calcKMeansBuffer = calcKMeansBuffer + pow((clusterInfo[j][y] - data[i][y]), 2);
				
				}
				dist = sqrt(calcKMeansBuffer);

				if (dist < minDist) {
					minDist = dist;
					dataClusterIndex[i] = j;
				}
			}
			printf("Sample %i closest cluster: %i\n", i, dataClusterIndex[i]);
			clusterElements[dataClusterIndex[i]]++;
			minDist = 100000;
		}

		
		//memset(clusterInfo, 0, sizeof(double) * K * dimensions);
		for (i = 0; i < K; i++) {
			printf("Cluster %i contains: %i elements\n", K, clusterElements[i]);
			for (j = 0; j < dimensions; j++) {
				clusterInfo[i][j] = 0;
			}
		}

		//4. Move the center of each cluster to be in the middle of the elements that are assigned to that cluster.
		//Get average of coordinates. x+x+x/n, y+y+y/n
		for (i = 0; i < K; i++) { //Per Cluster
			printf("Cluster %i (avg): ", i);
			for (j = 0; j < samples; j++) { //For each sample in each cluster
				for (y = 0; y < dimensions; y++) { //Per dimension

					//If data belongs in current cluster
					if (dataClusterIndex[j] == i) { //if belong in cluster 0
						clusterInfo[dataClusterIndex[j]][y] = clusterInfo[dataClusterIndex[j]][y] + data[j][y];
					}
					
				}
			}

			//After getting sum, divide by number of samples in the cluster
			for (y = 0; y < 2; y++) {
				clusterInfo[i][y] = clusterInfo[i][y] / (double) clusterElements[i];
				printf("%lf ", clusterInfo[i][y]);
			}
			printf("\n");
		}

		printf("//=========== Loop %i ===========//\n", z + 1);

	} //End major loop





	//Reference code for pThreads
	long thread;
	pthread_t* thread_handles;

	thread_count = strtol(argv[1], NULL, 10);

	thread_handles = malloc(thread_count*sizeof(pthread_t));

	for (thread = 0; thread < thread_count; thread++) {
		pthread_create(&thread_handles[thread], NULL, Hello, (void*) thread);
	}

	printf("\nHello from main thread\n");

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
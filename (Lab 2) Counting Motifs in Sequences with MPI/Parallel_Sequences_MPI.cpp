#include <iostream>
#include <cstring>
#include <fstream>
#include <cstdlib>
#include "etime.h"
#include <mpi.h> 

/*
	Compile:
	    mpic++ -g -Wall -o mpi1 mpi1.cpp etime.c
	Run:
	    mpiexec -n 4 ./mp1

	Distributing Sequences
		Every processor gets:
			Fair share of sequences
			Complete copy of motifs

	Processor 0 responsible input/output
		And distribution of data
*/

using namespace std;

int main(int argc, char* argv []) {
	int comm_sz;               /* Number of processes    */
	int my_rank;               /* My process rank        */		   
	char* localSequences; 	   /* Local buffer for scatter receive */
	char* motifs;              /* Each processor a copy */
	int* matchedCounter;	   /* Local copy */
	int motifsLength;		   /* Local copy */
	int numMotifs;			   /* Local copy */
	int numSequences;		   /* Local copy */
	int isMatch = 1;		   /* Local copy */

	/* Start up MPI */
	MPI_Init(NULL, NULL); 

	/* Get the number of processes */
	MPI_Comm_size(MPI_COMM_WORLD, &comm_sz); 

	/* Get my rank among all the processes */
	MPI_Comm_rank(MPI_COMM_WORLD, &my_rank); 

	if (my_rank == 0) {

		if (argc != 4) {
			if (my_rank == 0) { //Only Processor 0 printout, but all terminate
				cout << "Incorrect number of arguments. Terminating.\n";
			}
			exit(-1);
		}

		ifstream inMotif(argv[1]);
		ifstream inSequence(argv[2]);
		ofstream output(argv[3]);

		// ifstream inMotif("motifsSmall.txt");
		// ifstream inSequence("sequencesSmall.txt");
		// ofstream output("outputSmall.txt");

		// ifstream inMotif("motifsMedium.txt");
		// ifstream inSequence("sequencesMedium.txt");
		// ofstream output("outputMedium.txt");

		// ifstream inMotif("motifsLarge.txt");
		// ifstream inSequence("sequencesLarge.txt");
		// ofstream output("outputLarge.txt");

		//Reading in Motifs
		inMotif >> numMotifs >> motifsLength;

		motifs = (char*) malloc(sizeof(char) * ((numMotifs * motifsLength) + 1));
		memset(motifs, '\0', sizeof(char) * ((numMotifs * motifsLength) + 1));
		for (int i = 0; i < numMotifs; i++) {
			inMotif >> (motifs + strlen(motifs)); //Read in, append to motifs
			//strlen gives length to \0, so pointer arithmetic, starting from \0, append
		}

		//Checking Motifs Array
		// for (int i = 0; i < int(strlen(motifs)); i++) {
		// 	if (i % motifsLength == 0 && i != 0) {
		// 		cout << endl;
		// 	}
		// 	cout << motifs[i];
		// } cout << endl << endl;

		//Reading in Sequences
		inSequence >> numSequences >> motifsLength; //n can be discarded since n will == motifsLength
		char* sequences = (char *) malloc(sizeof(char) * ((numSequences * motifsLength) + 1)); //sequenceLength == motifsLength
		memset(sequences, '\0', sizeof(char) * ((numSequences * motifsLength) + 1));
		for (int i = 0; i < numSequences; i++) {
			inSequence >> (sequences + strlen(sequences));
		}

		//Checking Sequences Array
		// for (int i = 0; i < int(strlen(sequences)); i++) {
		// 	if (i % motifsLength == 0 && i != 0) {
		// 		cout << endl;
		// 	}
		// 	cout << sequences[i];
		// } cout << endl;

		//Closing ifstreams
		inMotif.close();
		inSequence.close();

		tic();

		//Broadcast motifsLength, numMotifs, numSequence
		MPI_Bcast(&motifsLength, 1, MPI_INT, 0, MPI_COMM_WORLD);
		MPI_Bcast(&numMotifs, 1, MPI_INT, 0, MPI_COMM_WORLD);
		MPI_Bcast(&numSequences, 1, MPI_INT, 0, MPI_COMM_WORLD);

		//Distribute sequences
		localSequences = (char *) malloc(sizeof(char) * ((numSequences/comm_sz) * motifsLength + 1));
		MPI_Scatter(sequences, (numSequences/comm_sz) * motifsLength, MPI_CHAR, localSequences, (numSequences/comm_sz) * motifsLength, MPI_CHAR, 0, MPI_COMM_WORLD);
		
		//Broadcast copy of complete motifs
		MPI_Bcast(motifs, (numMotifs * motifsLength) + 1, MPI_CHAR, 0, MPI_COMM_WORLD);

		//Array to keep track of motif match counts
		matchedCounter = (int*) malloc(sizeof(int) * numMotifs);
		memset(matchedCounter, 0, sizeof(int) * numMotifs);

		//Compare algorithm
		for (int i = 0; i < numSequences/comm_sz; i++) {
			for (int j = 0; j < numMotifs; j++) { //Compare to every motif

				//For each character
				for (int k = 0; k < motifsLength; k++) {
					if (localSequences[i * motifsLength + k] != motifs[j * motifsLength + k] && motifs[j * motifsLength + k] != 'X') { //i -> j
						isMatch = 0;
						break;
					}
				}

				if (isMatch == 1) {
					matchedCounter[j]++; //+1 to current motif's count
				} else {
					isMatch = 1;
				}

			}
		}

		int* histoCounter = (int*) malloc(sizeof(int) * (comm_sz * numMotifs + 1));
		memset(histoCounter, 0, sizeof(int) * (comm_sz * numMotifs + 1)); 
		MPI_Gather(matchedCounter, numMotifs, MPI_INT, histoCounter, numMotifs, MPI_INT, 0, MPI_COMM_WORLD);

		//Combine results of histoCounter, which is composed of comm_sz amounts of matchedCounter arrays linearly
		for (int i = numMotifs; i < comm_sz * numMotifs; i++) {
			histoCounter[i % numMotifs] = histoCounter[i % numMotifs] + histoCounter[i];
		}

		toc();

		//Output to ofstream file
		int index = 0;
		output << strlen(motifs) / motifsLength << endl;
		for (int i = 0; i <= strlen(motifs); i++) {
			if (i % motifsLength == 0 && i != 0) {
				output << "," << histoCounter[index] << endl;
				index++;
			}
			if (i != strlen(motifs)) {
				output << motifs[i];
			}
		}

		output.close();

		free(histoCounter);
		free(sequences);

		cout << "Elapsed Time: " << etime() << endl;
		//cout << etime() << endl;

	} else { //Other processes

		//Receive motifsLength, numMotifs, and numSequences
		MPI_Bcast(&motifsLength, 1, MPI_INT, 0, MPI_COMM_WORLD);
		MPI_Bcast(&numMotifs, 1, MPI_INT, 0, MPI_COMM_WORLD);
		MPI_Bcast(&numSequences, 1, MPI_INT, 0, MPI_COMM_WORLD);

		//Receive localSequences
		localSequences = (char *) malloc(sizeof(char) * ((numSequences/comm_sz) * motifsLength + 1));
		MPI_Scatter(NULL, (numSequences/comm_sz) * motifsLength, MPI_CHAR, localSequences, (numSequences/comm_sz) * motifsLength, MPI_CHAR, 0, MPI_COMM_WORLD);

		//Receive whole motifs
		motifs = (char *) malloc(sizeof(char) * ((numMotifs * motifsLength) + 1));
		MPI_Bcast(motifs, (numMotifs * motifsLength) + 1, MPI_CHAR, 0, MPI_COMM_WORLD);

		//Array to keep track of motif match counts
		matchedCounter = (int*) malloc(sizeof(int) * numMotifs);
		memset(matchedCounter, 0, sizeof(int) * numMotifs);

		//Compare algorithm
		for (int i = 0; i < numSequences/comm_sz; i++) {
			for (int j = 0; j < numMotifs; j++) { //For every motif

				//For each character in motif
				for (int k = 0; k < motifsLength; k++) {
					if (localSequences[i * motifsLength + k] != motifs[j * motifsLength + k] && motifs[j * motifsLength + k] != 'X') { // i -> j
						isMatch = 0;
						break;
					}
				}

				if (isMatch == 1) {
					matchedCounter[j]++; //+1 to current motif's count
				} else {
					isMatch = 1;
				}
			}
		}

		//Send results back to Process 0
		MPI_Gather(matchedCounter, numMotifs, MPI_INT, NULL, numMotifs, MPI_INT, 0, MPI_COMM_WORLD);

	}

	//Freeing mallocs
	free(localSequences);
	free(matchedCounter);
	free(motifs);

	/* Shut down MPI */
	MPI_Finalize(); 

}  
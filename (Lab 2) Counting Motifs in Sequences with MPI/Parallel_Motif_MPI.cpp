#include <iostream>
#include <cstring> //CString for C++
#include <string> //std::string
#include <fstream>
#include <cstdlib>
#include <vector>
#include <mpi.h> 

/*
	mpic++ -g -Wall -o prog prog.cpp
	mpiexec -n 4 ./prog

	Distributing Motifs
		Every processor gets:
			Fair share of motifs
			Complete sequence of sequences

	Processor 0 responsible input/output
		And distribution of data
*/

using namespace std;

int main(int argc, char* argv []) {
	int comm_sz;               /* Number of processes    */
	int my_rank;               /* My process rank        */
	char* localMotif;		   /* Local buffer for scatter receive */
	char* sequences;		   /* Local whole sequences array */
	int motifsLength;		   /* Local copy */
	int numMotifs;			   /* Local copy */
	int numSequences;		   /* Local copy */

	// if (argc != 4) {
	// 	if (my_rank == 0) { //Only Processor 0 printout, but all terminate
	// 		cout << "Incorrect number of arguments. Terminating.\n";
	// 	}
	// 	exit(-1);
	// }

	/* Start up MPI */
	MPI_Init(NULL, NULL); 

	/* Get the number of processes */
	MPI_Comm_size(MPI_COMM_WORLD, &comm_sz); 

	/* Get my rank among all the processes */
	MPI_Comm_rank(MPI_COMM_WORLD, &my_rank); 

	if (my_rank == 0) {

		// ifstream inMotif(argv[1]);
		// ifstream inSequence(argv[2]);
		// ofstream output(argv[3]);

		ifstream inMotif("classMotifs.txt");
		ifstream inSequence("classSequences.txt");
		ofstream output("outputSmall.txt");

		//Reading in Motifs
		inMotif >> numMotifs >> motifsLength;
		char motifs[(numMotifs * motifsLength) + 1]; //1 for \0
		memset(motifs, '\0', sizeof(char)*((numMotifs * motifsLength) + 1));
		for (int i = 0; i < numMotifs; i++) {
			inMotif >> motifs + strlen(motifs); //Read in, append to motifs
			//strlen gives length to \0, so pointer arithmetic, starting from \0, append
		}

		//Checking Motifs Array
		for (int i = 0; i < int(strlen(motifs)); i++) {
			if (i % 5 == 0 && i != 0) {
				cout << endl;
			}
			cout << motifs[i];
		} cout << endl;

		//Reading in Sequences
		inSequence >> numSequences >> motifsLength; //n can be discarded since n will == motifsLength
		//char sequences[(numSequences * motifsLength) + 1]; 
		sequences = (char *) malloc(sizeof(char) * (numSequences * motifsLength) + 1); //sequenceLength == motifsLength
		memset(sequences, '\0', sizeof(char)*((numSequences * motifsLength) + 1));
		for (int i = 0; i < numSequences; i++) {
			inSequence >> sequences + strlen(sequences);
		}

		//Checking Sequences Array
		for (int i = 0; i < int(strlen(sequences)); i++) {
			if (i % 5 == 0 && i != 0) {
				cout << endl;
			}
			cout << sequences[i];
		} cout << endl;

		//Broadcast motifLength, numMotifs, numSequence
		MPI_Bcast(&motifsLength, 1, MPI_INT, 0, MPI_COMM_WORLD);
		MPI_Bcast(&numMotifs, 1, MPI_INT, 0, MPI_COMM_WORLD);
		MPI_Bcast(&numSequences, 1, MPI_INT, 0, MPI_COMM_WORLD);

		//Distribute motifs
		localMotif = (char *) malloc(sizeof(char) * (numMotifs/comm_sz) * motifsLength + 1);
		MPI_Scatter(motifs, 5, MPI_CHAR, localMotif, 5, MPI_CHAR, 0, MPI_COMM_WORLD);
		
		//Broadcast whole sequence
		MPI_Bcast(sequences, (numSequences * motifsLength) + 1, MPI_CHAR, 0, MPI_COMM_WORLD);

		//Compare localMotifs against Sequences
		//localMotif, sequences
		for (int i = 0; i < numMotifs/comm_sz; i++) {
			for (int j = 0; j < numSequences/comm_sz; j++) {

				//For each character in motif
				for (int k = 0; k < motifsLength; k++) {
					if (localMotif[i * motifsLength + k] != sequences[j * motifsLength + k] && localMotif[i * motifsLength + k] != 'X') {
						cout << "Different!\n";
						for (int y = 0; y < motifsLength; y++) {
							cout << sequences[i * motifsLength + y];
						}
						cout << endl;
					}
				}
			}
		}

	} else { //Other processes

		//Receive motifsLength, numMotifs, and numSequences
		MPI_Bcast(&motifsLength, 1, MPI_INT, 0, MPI_COMM_WORLD);
		MPI_Bcast(&numMotifs, 1, MPI_INT, 0, MPI_COMM_WORLD);
		MPI_Bcast(&numSequences, 1, MPI_INT, 0, MPI_COMM_WORLD);

		//Receive motifs
		localMotif = (char *) malloc(sizeof(char) * (numMotifs/comm_sz) * motifsLength + 1);
		MPI_Scatter(NULL, 5, MPI_CHAR, localMotif, 5, MPI_CHAR, 0, MPI_COMM_WORLD);

		//Receive whole sequence
		sequences = (char *) malloc(sizeof(char) * (numSequences * motifsLength) + 1);
		MPI_Bcast(sequences, (numSequences * motifsLength) + 1, MPI_CHAR, 0, MPI_COMM_WORLD);

		// cout << "Processor " << my_rank << ": Motif: \n";
		// for (int i = 0; i < strlen(sequences); i++) {
		// 	if (i % 5 == 0 && i != 0) {
		// 		cout << endl;
		// 	}
		// 	cout << sequences[i];
		// }
		// cout << endl;


	}

	/* Shut down MPI */
	MPI_Finalize(); 

}  
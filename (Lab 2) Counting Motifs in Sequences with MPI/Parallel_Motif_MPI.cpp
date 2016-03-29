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
		Every processor gets fair share of motifs
		And complete sequence of sequences

	Processor 0 does input/output
		Also distributes data
*/

using namespace std;

int main(int argc, char* argv []) {
	int comm_sz;               /* Number of processes    */
	int my_rank;               /* My process rank        */

	/* Start up MPI */
	MPI_Init(NULL, NULL); 

	/* Get the number of processes */
	MPI_Comm_size(MPI_COMM_WORLD, &comm_sz); 

	/* Get my rank among all the processes */
	MPI_Comm_rank(MPI_COMM_WORLD, &my_rank); 

	// if (argc != 4) {
	// 	if (my_rank == 0) { //Only Processor 0 printout, but all terminate
	// 		cout << "Incorrect number of arguments. Terminating.\n";
	// 	}
	// 	exit(-1);
	// }

	if (my_rank == 0) {

		// ifstream inMotif(argv[1]);
		// ifstream inSequence(argv[2]);
		// ofstream output(argv[3]);

		ifstream inMotif("classMotifs.txt");
		ifstream inSequence("classSequences.txt");
		ofstream output("outputSmall.txt");

		int numMotifs, numSequences, motifsLength, distSize;
		string n;

		//Read in data
		inMotif >> numMotifs >> motifsLength;

		string motifs[numMotifs];
		for (int i = 0; i < numMotifs; i++) {
			inMotif >> n;
			motifs[i] = n;
		}

		inSequence >> numSequences >> n; //n can be discarded since n will == motifsLength

		string sequences[numSequences];
		for (int i = 0; i < numSequences; i++) {
			inSequence >> n;
			sequences[i] = n;
		}
		
		//Evenly distributing motifs
		distSize = numMotifs/comm_sz;
		cout << "Each processor getting " << distSize << " motifs\n\n";
		string distArr[distSize];
		int sendIndex = distSize;

		//MPI send distributed motifs to other processors
		for (int i = 1; i < comm_sz; i++) { //Other processors, thus i = 1
			cout << "Sending: ";
			for (int j = 0; j < distSize; j++) {
				distArr[j] = motifs[sendIndex];
				sendIndex++;
				cout << distArr[j] << " ";
			}
			cout << "to processor " << i << endl;
		}
		//MPI_broadcast whole sequence length to other processors
		//int MPI_Bcast( void *buffer, int count, MPI_Datatype datatype, int root, MPI_Comm comm )
	}
	else { //Other processes


	}

	/* Shut down MPI */
	MPI_Finalize(); 

}  
#include <iostream>
#include <cstring> //CString for C++
#include <string> //std::string
#include <fstream>
#include <cstdlib>
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

		int numMotifs, motifsLength, distSize;

		inMotif >> numMotifs >> motifsLength;

		//Evenly distributing motifs
		distSize = comm_sz/numMotifs;
		cout << "Each processor getting " << distSize << " motifs";

		//MPI send distributed motifs to other processors

		//MPI_broadcast whole sequence length to other processors

	}
	else { //Other processes


	}

	/* Shut down MPI */
	MPI_Finalize(); 

}  
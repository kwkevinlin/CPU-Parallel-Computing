#include <iostream>
#include <cstring> //CString for C++
#include <string> //std::string
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

int main() {
	int comm_sz;               /* Number of processes    */
	int my_rank;               /* My process rank        */


	/* Start up MPI */
	MPI_Init(NULL, NULL); 

	/* Get the number of processes */
	MPI_Comm_size(MPI_COMM_WORLD, &comm_sz); 

	/* Get my rank among all the processes */
	MPI_Comm_rank(MPI_COMM_WORLD, &my_rank); 


	if (my_rank == 0) {


	}
	else { //Other processes


	}

	//If not Process 0
	if (my_rank != 0) { 

	}


	/* Shut down MPI */
	MPI_Finalize(); 

}  
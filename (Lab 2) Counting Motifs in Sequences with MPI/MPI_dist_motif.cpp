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

const int MAX_STRING = 100;

using namespace std;

int main() {
   //char greeting[MAX_STRING];  /* String storing message */
	string greeting;
   int comm_sz;               /* Number of processes    */
   int my_rank;               /* My process rank        */
   int q;		     /* A counter variable */

   /* Start up MPI */
   MPI_Init(NULL, NULL); 

   /* Get the number of processes */
   MPI_Comm_size(MPI_COMM_WORLD, &comm_sz); 

   /* Get my rank among all the processes */
   MPI_Comm_rank(MPI_COMM_WORLD, &my_rank); 


   //Not 0
   if (my_rank != 0) { 
      /* Create message */
      //sprintf(greeting, "Greetings from process %d of %d!", my_rank, comm_sz);
	greeting = "Rank"; 
      /* Send message to process 0 */
	MPI_Send(greeting.c_str(), greeting.length(), MPI_CHAR, 0, 0, MPI_COMM_WORLD);
      //MPI_Send(greeting, strlen(greeting)+1, MPI_CHAR, 0, 0, MPI_COMM_WORLD); 
   } else {  
	  //Process 0
      /* Print my message */
   	char buff[5];
      printf("Greetings from process %d of %d!\n", my_rank, comm_sz);
      for (q = 1; q < comm_sz; q++) {
      	MPI_Recv(buff, 4, MPI_CHAR, q, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);

        //MPI_Recv(greeting, MAX_STRING, MPI_CHAR, q, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);

         cout << buff << endl;
      } 
   }

   /* Shut down MPI */
   MPI_Finalize(); 

}  
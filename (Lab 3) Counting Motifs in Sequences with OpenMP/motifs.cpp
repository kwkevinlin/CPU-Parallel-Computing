#include <iostream>
#include <cstdlib>
#include <fstream>
#include <omp.h>
#include "etime.h"

using namespace std;

/*
	Compile via:
		g++ -g -fopenmp -o motifs motifs.cpp

	Execute via:
		. /motifs (int)numThreads
 */

int main(int argc, char* argv[]) {

	// if (argc != 5) {
	// 	cout << "Incorrect number of arguments. Terminating.\n";
	// 	exit(-1);
	// }

	// ifstream inMotif(argv[2]);
	// ifstream inSequence(argv[3]);
	// ofstream output(argv[4]);

	ifstream inMotif("motifsSmall.txt");
	ifstream inSequence("sequencesSmall.txt");
	ofstream output("outputSmall.txt");

	// ifstream inMotif("motifsMedium.txt");
	// ifstream inSequence("sequencesMedium.txt");
	// ofstream output("outputMedium.txt");

	string n;
	int thread_count = stoi(argv[1]); //Number of threads
	int numMotifs, numSequences, motifsLength, index = 0; //IO variables

	//Reading in motifs
	inMotif >> numMotifs >> motifsLength;
	string *motifs = new string[numMotifs];
	while (inMotif >> n) {
		motifs[index] = n;
		index++;
	}

	//Reading in sequences
	inSequence >> numSequences >> n;
	string *sequences = new string[numSequences];
	index = 0;
	while (inSequence >> n) {
		sequences[index] = n;
		index++;
	}

//	cout << "Input:\n";
//	for (int i = 0; i < numMotifs; i++) {
//		cout << motifs[i] << " ";
//	} cout << endl;
//	for (int i = 0; i < numSequences; i++) {
//		cout << sequences[i] << " ";
//	} cout << endl << endl;

	int *histoCounter = new int[numMotifs];

	tic();

	/*
		Notes: reduction(+: varReduction) after pragma
		histoCounter array can be reduced
		Probably dont need pragma critical
	*/

	//OpenMP
	# pragma omp parallel for num_threads (thread_count)

	//Brute force method
	for (int i = 0; i < numMotifs; i++) { //For every motif string
		int isMatch = 1; //Private for each thread
		# pragma omp critical
		histoCounter[i] = 0;
		for (int j = 0; j < numSequences; j++) { //For every sequence string

			//Compare character by character
			for (int k = 0; k < motifsLength; k++) {

				if (motifs[i][k] != sequences[j][k] && motifs[i][k] != 'X') {
					isMatch = 0;
					break;
				}
			}

			if (isMatch == 1) {
				//Critical section
				# pragma omp critical
				histoCounter[i]++;
			} else {
				isMatch = 1;
			}

		}
	}

	toc();

	//cout << endl << matchedMotifs.size() << endl;
	output << numMotifs << endl;
	for (int i = 0; i < numMotifs; i++) {
		output << motifs[i] << "," << histoCounter[i] << endl;
	}

	//cout << "\nTime: " << etime() << endl;
	cout << etime() << endl;

	inMotif.close();
	inSequence.close();
	output.close();

}

#include <iostream>
#include <cstdlib>
#include <fstream>
#include <unordered_map>
//#include "etime.h"

using namespace std;

/*
	Currently compiling via:
	g++ -o main sequential.cpp etime.c -std=c++0x

 */

int main(int argc, char* argv[]) {

//	if (argc != 4) {
//		cout << "Incorrect number of arguments. Terminating.\n";
//		exit(-1);
//	}
//
//	ifstream inMotif(argv[1]);
//	ifstream inSequence(argv[2]);
//	ofstream output(argv[3]);

	ifstream inMotif("motifsSmall.txt");
	ifstream inSequence("sequencesSmall.txt");
	ofstream output("outputSmall.txt");


	string n;
	string motifsLength;
	int numMotifs, numSequences, index = 0;

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
	if (motifsLength != n) {
		cout << "Number of characters do not match!\n";
		exit(-1);
	}
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

	int isMatch = 1;
	int *histoCounter = new int[numMotifs]();

	//tic();

	//Brute force method
	for (int i = 0; i < numMotifs; i++) { //For every motif string
		cout << "MotifsLoop: " << i << endl;
		for (int j = 0; j < numSequences; j++) { //For every sequence string

			//Compare character by character
			for (int k = 0; k < numMotifs; k++) {

				if (motifs[i][k] != sequences[j][k] && motifs[i][k] != 'X') {
					isMatch = 0;
					break;
				}
			}

			if (isMatch == 1) {
				histoCounter[i]++;
			} else {
				isMatch = 1;
			}

		}
	}

	//toc();

	//cout << endl << matchedMotifs.size() << endl;
	output << numMotifs << endl;
	for (int i = 0; i < numMotifs; i++) {
		output << motifs[i] << "," << histoCounter[i] << endl;
	}

	//cout << "\nTime: " << etime() << endl;

	inMotif.close();
	inSequence.close();
	output.close();

}

#include <iostream>
#include <cstdlib>
#include <fstream>
#include <vector>
#include <unordered_map>
#include "etime.h"

using namespace std;

/*
	Currently compiling via:
	g++ -o main sequential.cpp etime.c -std=c++0x

 */

int main(int argc, char* argv[]) {

	if (argc != 4) {
		cout << "Incorrect number of arguments. Terminating.\n";
		exit(-1);
	}

	ifstream inMotif(argv[1]);
	ifstream inSequence(argv[2]);
	ofstream output(argv[3]);

	// ifstream inMotif("classMotifs.txt");
	// ifstream inSequence("classSequences.txt");
	// ofstream output("outputSmall.txt");

	vector<string> motifs;
	vector<string> sequences;

	string n;
	string numChars;
	inMotif >> n >> n;
	numChars = n;
	while (inMotif >> n) {
		motifs.push_back(n);
	}
	inSequence >> n >> n;
	if (numChars != n) {
		cout << "Number of characters do not match!\n";
		exit(-1);
	}
	while (inSequence >> n) {
		sequences.push_back(n);
	}

	cout << "Input:\n";
	for (int i = 0; i < motifs.size(); i++) {
		cout << motifs[i] << " ";
	} cout << endl;
	for (int i = 0; i < sequences.size(); i++) {
		cout << sequences[i] << " ";
	} cout << endl << endl;

	int isMatch = 1;
	unordered_map<string, int> matchedMotifs; //Stores matched motifs and their count in a hash

	tic();

	//Brute force method
	for (int i = 0; i < motifs.size(); i++) { //For every motif string
		for (int j = 0; j < sequences.size(); j++) { //For every sequence string

			//Compare character by character
			for (int k = 0; k < motifs[i].length(); k++) {

				if (motifs[i][k] != sequences[j][k] && motifs[i][k] != 'X') {
					isMatch = 0;
					break;
				}
			}

			if (isMatch == 1) {
				cout << "Match: " << motifs[i] << " and " << sequences[j] << endl;
				matchedMotifs[motifs[i]]++;
			} else {
				isMatch = 1;
			}

		}
	}

	toc();

	cout << endl << matchedMotifs.size() << endl;
	output << matchedMotifs.size() << endl;
	for (auto kv : matchedMotifs) {
		cout << kv.first << "," << kv.second << endl;
		output << kv.first << "," << kv.second << endl;
	}

	cout << "\nTime: " << etime() << endl;

	/*
	 * MingW Output:
	 * 	   3
	 *	   RSTXC,1
	 *	   TXCCX,2
	 *	   AXMLC,1
	 *
	 * 	   On Windows's MingW is REVERSED
	 *
	 *	BUT NOTE: Apparently MingW compiler's map insert is different
	 *            from Bally's GCC.
	 *		Do NOT need to use stack to reverse map in Bally
	 */

	inMotif.close();
	inSequence.close();
	output.close();

}

#include <iostream>
#include <cstdlib>
#include <fstream>
#include <vector>
#include <unordered_map>
#include <stack> //Get rid of this
//#include "etime.h"

using namespace std;

struct Motifs {
	string data;
	int count;

	Motifs(string d, int c) {
		data = d;
		count = c;
	}
};


int main(int argc, char* argv[]) {

	//	if (argc != 4) {
	//		cout << "Incorrect number of arguments. Terminating.\n";
	//		exit(-1);
	//	}
	//
	//	ifstream inMotif(argv[1]);
	//	ifstream inSequence(argv[2]);
	//	ofstream output(argv[3]);

	ifstream inMotif("classMotifs.txt");
	ifstream inSequence("classSequences.txt");
	ofstream output("outputSmall.txt");

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

	//tic();

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

	//toc();

	cout << endl << matchedMotifs.size() << endl;
	output << matchedMotifs.size() << endl;
	stack<Motifs> revStack;
	for (auto kv : matchedMotifs) {
		revStack.push(Motifs(kv.first, kv.second));
		//cout << kv.first << "," << kv.second << endl;
		//output << kv.first << "," << kv.second << endl;
	}

	//cout << "Time: " << etime() << endl;

	/*
	 * Current Output:
	 * 	   3
	 *	   RSTXC,1
	 *	   TXCCX,2
	 *	   AXMLC,1
	 *
	 * 	   ... is revered
	 *
	 */

	//To reverse output, UN-IDEALLY for now:
	while (revStack.size() > 0) {
		cout << revStack.top().data << ", " << revStack.top().count << endl;
		output << revStack.top().data << ", " << revStack.top().count << endl;
		revStack.pop();
	}

	inMotif.close();
	inSequence.close();
	output.close();

}

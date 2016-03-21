#include <iostream>
#include <cstdlib>
#include <fstream>
#include <vector>
#include <unordered_map>

using namespace std;

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
	cout << "Input:\n";
	inMotif >> n >> n;
	while (inMotif >> n) {
		motifs.push_back(n);
	}
	inSequence >> n >> n;
	while (inSequence >> n) {
		sequences.push_back(n);
	}

	for (int i = 0; i < motifs.size(); i++) {
		cout << motifs[i] << " ";
	} cout << endl;
	for (int i = 0; i < sequences.size(); i++) {
		cout << sequences[i] << " ";
	} cout << endl << endl;

	int isMatch = 1;
	unordered_map<string, int> matchedMotifs; //Stores matched motifs and their count in a hash

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

	cout << endl << matchedMotifs.size() << endl;
	output << matchedMotifs.size() << endl;
	for (auto kv : matchedMotifs) {
		cout << kv.first << "," << kv.second << endl;
		output << kv.first << "," << kv.second << endl;
	}

	/*
	 * Output:
	 * 	   3
	 *	   RSTXC,1
	 *	   TXCCX,2
	 *	   AXMLC,1
	 */

	inMotif.close();
	inSequence.close();
	output.close();

}

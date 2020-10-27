#include <vector>
#include <algorithm>
#include <iostream>
#include <fstream>
#include <string>
using namespace std;

int main(int argc, char* argv[]) {

	// declare vector 
	vector<char> bookVector;
	vector<char> messageVector;
	vector<int> codedVector;

	ifstream bookInput("bookfile.txt");
	if (!bookInput) {
		cerr << "Could not open the file" << endl;
		return EXIT_FAILURE;
	}

	// load the book into the the vector
	char book;
	while (!bookInput.eof()) {
		bookVector.push_back(bookInput.get());
	}

	ifstream messageInput("messagefile.txt");
	if (!messageInput) {
		cerr << "Could not open the file" << endl;
		return EXIT_FAILURE;
	}
	// load the message into the the vector
	char message;
	while (!messageInput.eof()) {
		messageVector.push_back(messageInput.get());
	}

	ofstream codedfileStream("codedfile.txt");
	if (!codedfileStream) {
		cout << "Could not open the output file" << endl;
		return EXIT_FAILURE;
	}

	vector<vector< unsigned> >indexLocation;

	for (vector<char>::size_type i = 0; i < messageVector.size(); ++i) {
		for (vector<char>::size_type k = 0; k < bookVector.size(); ++k) {
			if (bookVector[k] == messageVector[i]) {
				indexLocation[i].push_back(k);
				codedfileStream << k << " ";
				break;
			}
		}
	}
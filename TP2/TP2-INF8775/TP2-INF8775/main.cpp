#include <iostream>
#include <fstream>
#include <vector>
#include <filesystem>
#include <sstream>
#include "AlgoDyn.h"
#include "AlgoGloutonProba.h"
#include "AmeliorationLocale.h"

using namespace std;
typedef int r;
typedef int q;

vector<int> split(const string& s, char delim) {
	vector<int> result;
	stringstream ss(s);
	string item;
	while (getline(ss, item, delim)) {		
		if(!item.empty())
			result.push_back(stoi(item));
	}

	return result;
}


int main() {


	// data
	//vector<pair<int, int>> data;

	// Read files
	ifstream file;
	file.open(".\\exemplaires\\WC-100-10-01.txt",ios::in);
	if (!file.is_open()) {
		cout << "fopen failed\n";
		return 0;
	}
	
	string line;
	int nbElem = 0;
	bool first = true;
	int last = 0;
	vector<pair<r, q>> data;
	while (getline(file, line)) { //read data from file object and put it into string.
		cout << line << endl;
		vector<int> v = split(line,' ');		
		if (first) {
			nbElem = stoi(line);
			first = false;		
		}
		if (v.size() == 3)
			data.push_back(pair<r, q>(v[1], v[2]));
	}
	
	
	return 0;
}
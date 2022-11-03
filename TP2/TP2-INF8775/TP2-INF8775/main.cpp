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

// Split method used to split a string containing numbers
// and returns a vector of ints
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

// Restaurant Struct
// Contains ID and r and q values
struct Restaurant {
	int ID;
	pair<r, q> rq_values;

	// Restaurant Ctor
	Restaurant(int id, pair<r, q> rq_val)
		: ID(id), rq_values(rq_val){}
};

int main() {

	// Read files
	ifstream file;
	file.open(".\\exemplaires\\WC-100-10-01.txt",ios::in);
	if (!file.is_open()) {
		cout << "fopen failed\n";
		return 0;
	}
	
	string line;
	vector<Restaurant*> data; // All restaurants data.
	while (getline(file, line)) {	//read data from file object and put it into the string line.
		vector<int> v = split(line,' ');	// Split the string	
		if (v.size() == 3) {
			data.push_back(
				new Restaurant(v[0],make_pair(v[1],v[2]))
			);
		}			
	}

	// Print data
	for (auto v : data) {
		cout << v->ID<<" "<< v->rq_values.first <<" " << v->rq_values.second << endl;
	}
		
	return 0;
}
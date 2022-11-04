#include <iostream>
#include <fstream>
#include <vector>
#include <filesystem>
#include <sstream>
#include <map>
#include "AlgoDyn.h"
#include "AlgoGloutonProba.h"
#include "AmeliorationLocale.h"
#include "Structs.h"

using namespace std;

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


int main() {

	// Read files
	ifstream file;
	file.open(".\\exemplaires\\WC-100-10-01.txt",ios::in);
	if (!file.is_open()) {
		cout << "open file failed\n";
		return 0;
	}
	
	
	// Restaurant datas
	map<int, Restaurant> Restaurants;
	int nbRestaurants = 0;
	int capacite = 0;

	string line;
	while (getline(file, line)) {	//read data from file object and put it into the string line.
		vector<int> v = split(line,' ');	// Split the string	
		if (v.size() == 3) {
			Restaurants.insert(make_pair(v[0], Restaurant(v[1],v[2])));
		}
		if (v.size() == 1) {
			if (nbRestaurants == 0)
				nbRestaurants = v[0];
			else 
				capacite = v[0];
		}
	}
	
	// Print restaurants data
	for (auto &restaurant : Restaurants) {
		cout << restaurant.first << " "
			<< restaurant.second.revenue <<" "
			<< restaurant.second.quantity << endl;
	}
		
	return 0;
}
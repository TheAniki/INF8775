#include "Includes.h"
#include "AlgoGloutonProba.h"

#include "AlgoDyn.h"

// Split method used to split a string containing numbers
// and returns a vector of ints.
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

	// Read files.
	ifstream file;

	string fileName = ".\\exemplaires\\WC-100-100-01.txt";
	file.open(fileName, ios::in);
	if (!file.is_open()) {
		cout << "open file failed\n";
		return 0;
	}
	cout << "From exemplaire " + fileName << endl;

	// Restaurants data.
	vector<Restaurant> restaurants;
	int nbRestaurants = 0;
	int capacite = 0;

	string line;
	while (getline(file, line)) {	//read data from file object and put it into the string line.
		vector<int> v = split(line, ' ');	// Split the string.
		if (v.size() == 3) {
			restaurants.push_back(Restaurant(v[0], v[1], v[2]));
		}
		if (v.size() == 1) {
			if (nbRestaurants == 0)
				nbRestaurants = v[0];
			else
				capacite = v[0];
		}
	}	
		
	vector<Restaurant> chosenRestaurants;
	double totalRevenue = alggoGloutonProba(restaurants, capacite, chosenRestaurants);
	cout << "GLOUTON PROBA - TOTAL REVENUE  : " << totalRevenue << endl;
	cout << "Chosen restaurants : " << endl;
	for (const Restaurant restaurant : chosenRestaurants) {
		cout << "restaurant " << "number..." << " - revenue : " << restaurant.revenue << " - quantity :  " << restaurant.quantity <<endl;
	}
	
	
	
	// Algo Dynamique
	vector<Restaurant> sol = AlgoDyn(restaurants, capacite, nbRestaurants);

	for (auto s : sol) {
		cout << s.iD << " " << s.revenue<<" " << s.quantity << endl;
	}
	return 0;
}
#include "Includes.h"
#include "AlgoGloutonProba.h"
#include "AmeliorationLocale.h"
#include "AlgoDyn.h"
#include <filesystem>
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

void displayChosenRestaurants(vector<Restaurant> restaurant, int revenue, int capacity){
	cout << "============================================" << endl;
	cout << "TOTAL REVENUE  : " << revenue << " - MAX CAPACITY : " << capacity << endl;
	cout << "Chosen restaurants : " << endl;
	int rev = 0;
	for (const Restaurant restaurant : restaurant) {
		cout << "restaurant " << restaurant.iD << " - revenue : " << restaurant.revenue << " - quantity :  " << restaurant.quantity << endl;
		rev += restaurant.revenue;
	}
	cout << "============================================" << endl;
	cout << "rev: " << rev << endl;

}


int main() {

	// Read files.
	ifstream file;

	string fileName = ".\\exemplaires\\WC-100-10-01.txt";
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
	
	// Algo glouton proba
	/*cout << endl << "ALGO GLOUTON PROBA" << endl;
	vector<Restaurant> restaurantsGloutonProba;
	pair<int ,int> revenueQuantityPair= alggoGloutonProba(restaurants, capacite, restaurantsGloutonProba);
	displayChosenRestaurants(restaurantsGloutonProba, revenueQuantityPair.first, revenueQuantityPair.second);
	*/
	
	// Algo Dynamique
	cout << endl<< "ALGO DYNAMIQUE" << endl;
	int totalRevenueDyn = 0;
	vector<Restaurant> restaurantsDyn = AlgoDyn(restaurants, capacite, nbRestaurants, totalRevenueDyn);
	displayChosenRestaurants(restaurantsDyn, totalRevenueDyn, capacite); //TODO : faudrait qu'algoDyn ressorte le totalRevenue ? 


	 // AlgoLocal
	cout << endl << "algo local" << endl;
	vector<Restaurant> restaurantslocalsearch;
	pair<int, int> heurrevenuequantitypair = algoLocalHeuristic(restaurants, capacite, restaurantslocalsearch);
	displayChosenRestaurants(restaurantslocalsearch, heurrevenuequantitypair.first, heurrevenuequantitypair.second);
	
	return 0;
}
#include "./headers/Includes.h"
#include "./headers/AlgoGloutonProba.h"
#include "./headers/AlgoDyn.h"
#include "./headers/AmeliorationLocale.h"

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

// Display the chosen restaurants's iDs.
void displayChosenRestaurants(vector<Restaurant> restaurant){	
	for (const Restaurant restaurant : restaurant) {
		cout << restaurant.iD <<" ";
	}	
	cout << endl;
}


int main(int argc, const char *argv[]) {

	// Arguments parsing.
	string fileName;
    bool print = false;
	bool time = false;
	string algo;
    for (int i = 1; i < argc-1; i++)
    {
        if (!strcmp(argv[i], "-e"))
            fileName = argv[i + 1];
        if (!strcmp(argv[i+1], "-p"))
            print = true;
		if (!strcmp(argv[i+1],"-t"))
			time = true;
		if (!(strcmp(argv[i],"-a")))
			algo = argv[i+1];		
    }

	
	// Read files.
	ifstream file;

	file.open(fileName, ios::in);
	if (!file.is_open()) {
		cout << "open file failed\n";
		return 0;
	}

	// Restaurants data.
	vector<Restaurant> restaurants;
	int nbRestaurants = 0;
	int capacite = 0;

	string line;
	while (getline(file, line)) {	// Read data from file object and put it into the string line.
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

	chrono::duration<double,milli> duration;

	// Algo glouton proba.
	if(algo == "glouton"){
		vector<Restaurant> restaurantsGloutonProba;
		auto start = chrono::high_resolution_clock::now(); // Start timer.
		pair<int ,int> revenueQuantityPair= alggoGloutonProba(restaurants, capacite, restaurantsGloutonProba);
		auto stop = chrono::high_resolution_clock::now();  // Stop timer.
		duration = stop-start;

		if(print)
			displayChosenRestaurants(restaurantsGloutonProba);		
	}

	// Algo Dynamique.
	if(algo == "progdyn"){		
		auto start = chrono::high_resolution_clock::now(); // Start timer.
		vector<Restaurant> restaurantsDyn = dynamicAlgorithm(restaurants, capacite, nbRestaurants);
		auto stop = chrono::high_resolution_clock::now();  // Stop timer.
		duration = stop-start;
		
		if(print)
			displayChosenRestaurants(restaurantsDyn); 		
	}

	// Algo Local.
	if(algo == "local"){
		vector<Restaurant> restaurantsLocalSearch;
		auto start = chrono::high_resolution_clock::now(); // Start timer.
		pair<int, int> heurRevenueQuantityPair = algoLocalHeuristic(restaurants, capacite, restaurantsLocalSearch);		
		auto stop = chrono::high_resolution_clock::now(); // Stop timer.
		duration = stop-start;

		if(print)
			displayChosenRestaurants(restaurantsLocalSearch);		
	}

	if(time)
			cout << duration.count() << endl;

	return 0;
}

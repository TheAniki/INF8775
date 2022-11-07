#include "./headers/Includes.h"
#include "./headers/AlgoGloutonProba.h"

#include "./headers/AlgoDyn.h"

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
	for (const Restaurant restaurant : restaurant) {
		cout << "restaurant " << restaurant.iD << " - revenue : " << restaurant.revenue << " - quantity :  " << restaurant.quantity << endl;
	}
	cout << "============================================" << endl;

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
	//cout << "From exemplaire " + fileName << endl;

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
	if(algo == "glouton"){
		
		cout << endl << "ALGO GLOUTON PROBA" << endl;
		vector<Restaurant> restaurantsGloutonProba;
		int totalRevenue = alggoGloutonProba(restaurants, capacite, restaurantsGloutonProba);
		if(print)
			displayChosenRestaurants(restaurantsGloutonProba, totalRevenue, capacite);
		if(time)
			cout << time << endl; // TODO : Add exec time;
		return 0;
	}

	// Algo glouton proba
	if(algo == "progdyn"){
		// Algo Dynamique
		cout << endl<< "ALGO DYNAMIQUE" << endl;
		int totalRevenueDyn = 0;
		vector<Restaurant> restaurantsDyn = AlgoDyn(restaurants, capacite, nbRestaurants, totalRevenueDyn);
		if(print)
			displayChosenRestaurants(restaurantsDyn, totalRevenueDyn, capacite); //TODO : faudrait qu'algoDyn ressorte le totalRevenue ? 
		if(time)
			cout << time << endl; // TODO : Add exec time;
		return 0;
	}

	if(algo == "local"){
		cout << endl<< "Amelioration Locale" << endl;
		if(print)
			cout<<"print local"<<endl; //TODO : mettre l algo ici! ? 
		if(time)
			cout << time << endl; // TODO : Add exec time;
	}

	return 0;
}
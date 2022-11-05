#include "Includes.h"
#include "AlgoGloutonProba.h"

//returns the total rentability and fills the rentabilities vector
double computeRestaurantsRentability(map<int, Restaurant>& restaurants, map<int, double>& rentabilities) {
	double totalRentability = 0.0;
	for (int i = 0; i < restaurants.size(); i++) {
		double rentability = double(restaurants[i].revenue / restaurants[i].quantity);
		//rentabilities.insert(pair<int, double>(restaurants[i]., rentability)); //TODO : get key
		totalRentability += rentability;
	}

	return totalRentability;
}

//returns the index of the restaurant
int choseARestaurant(map<int, double>& rentabilities, const double totalRentability) {
	double randomNumber = double((rand() % 10000) / 10000); // number between 0.0000 and 1.0000
	double previousTreshhold = 0.0;
	for (pair<int, double> rentability : rentabilities) {
		double currentProba = rentability.second / totalRentability;
		double currentTreshhold = previousTreshhold + currentProba;

		if (randomNumber < currentProba)
			return rentability.first;
	}
}

map<int, Restaurant> generateRestaurantMap(const vector<int[2]>& data) {
	map<int, Restaurant> dataRestaurantMap;

	for (int i = 0; i < data.size(); i++) {
		Restaurant restaurant = {i+1, *data[0], *data[1] };
		auto myPair = make_pair(restaurant.iD, restaurant);
		dataRestaurantMap.insert(myPair);
	}

	return dataRestaurantMap;
}

void removeTooBigCapacities(map<int, Restaurant>& restaurants, int currentQuantity, int maxCapcity) {
	int maxRemainingCapacity = maxCapcity - currentQuantity;
	for (pair<int, Restaurant> restaurant : restaurants) {
		if (restaurant.second.quantity > maxRemainingCapacity) {
			restaurants.erase(restaurant.first);
		}
	}
}

//returns the totalRevenue and fills the chosenRestaurants vector
double generateProbaGloutonRestaurants(vector<int[2]>& data, int maxCapacity, vector<int>& chosenRestaurants) {
	map<int, Restaurant> remainingRestaurants = generateRestaurantMap(data);

	double totalRevenue = 0.0;
	double currentQuantity = 0.0;

	while (remainingRestaurants.size() > 0) {
		removeTooBigCapacities(remainingRestaurants, currentQuantity, maxCapacity);

		map<int, double> rentabilities;
		double totalRentability = computeRestaurantsRentability(remainingRestaurants, rentabilities);

		int restaurentIndex = choseARestaurant(rentabilities, totalRentability);

		totalRevenue += remainingRestaurants[restaurentIndex].revenue;
		currentQuantity += remainingRestaurants[restaurentIndex].quantity;
		remainingRestaurants.erase(restaurentIndex);
	}

	return totalRevenue;
}



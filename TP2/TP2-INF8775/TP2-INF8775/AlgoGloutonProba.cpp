#include "Includes.h"
#include "AlgoGloutonProba.h"
#include <vector> 
#include <utility>
#include <stdlib.h>     /* srand, rand */
#include <time.h>       /* time */
#include <iterator>
#include <map>

using namespace std;

//returns the total rentability and fills the rentability map
double computeRestaurantsRentability(const map<int, Restaurant>& restaurantMap, map<int, double> & rentabilityMap) {
	double totalRentability = 0.0;
	for (pair<int, Restaurant> restaurantPair : restaurantMap) {
		double restaurantRentability = double(restaurantPair.second.revenue)/ restaurantPair.second.quantity;
		rentabilityMap.insert(pair<int, double>(restaurantPair.first, restaurantRentability));
		totalRentability += restaurantRentability;

	}
	return totalRentability;

}

//returns the index of the restaurant chosen with a probability proportional to the rentability
int choseARestaurant(const map<int, double>& rentabilityMap, const double totalRentability) {
	srand((unsigned int)time(NULL));
	double randomNumber =double(rand() % 10000)/10000; // number between 0.0000 and 1.0000
	double previousTreshhold = 0.0;
	for (const pair<int, double> & rentability : rentabilityMap) {
			double restaurantProba = rentability.second/ totalRentability;
			double currentTreshhold = previousTreshhold + restaurantProba; 

			if( randomNumber >= previousTreshhold && randomNumber < currentTreshhold) {
				return rentability.first;
			}
			previousTreshhold = currentTreshhold;
		}
	return rentabilityMap.size() -1 ; // last restaurant chosen with randomNumber = 1 -> 1 chance /10000 for this scenario
}


map<int, Restaurant> generateRestaurantMap(vector<Restaurant> data){
	map<int, Restaurant> dataRestaurantMap;

	for(unsigned i = 0; i < data.size() ; i++ ){
		//pair<int, Restaurant> restaurantPair = make_pair(i+1, data[i]); //Restaurant ids start at 1
		dataRestaurantMap.emplace(i+1, data[i]);
	}

	return dataRestaurantMap;
}


void removeTooBigCapacities (map<int, Restaurant>& restaurants, int currentQuantity, int maxCapcity) {
	vector<int> idsToRemove;
	int maxRemainingCapacity = maxCapcity - currentQuantity;
	for (const pair<int, Restaurant> & restaurantPair : restaurants) {
		if (restaurantPair.second.quantity > maxRemainingCapacity) { // having the same capcity as the remaining one is ok
			idsToRemove.push_back(restaurantPair.first);
		}
	}

	for(const int id : idsToRemove){
		restaurants.erase(id);
	}


}

//returns the totalRevenue and fills the chosenRestaurants vector
double alggoGloutonProba(vector<Restaurant>& data, int maxCapacity, vector<Restaurant>& chosenRestaurants) {
	map<int, Restaurant> remainingRestaurants = generateRestaurantMap(data);

	int totalRevenue = 0.0;
	int currentQuantity = 0.0;



	while(remainingRestaurants.size() > 0){
		removeTooBigCapacities(remainingRestaurants, currentQuantity, maxCapacity);

		map<int, double> rentabilityMap;
		double totalRentability = computeRestaurantsRentability(remainingRestaurants, rentabilityMap);

		int restaurentId= choseARestaurant(rentabilityMap, totalRentability);

		totalRevenue += remainingRestaurants[restaurentId].revenue;
		currentQuantity += remainingRestaurants[restaurentId].quantity;
		chosenRestaurants.push_back(remainingRestaurants[restaurentId]);
		remainingRestaurants.erase(restaurentId);
	}

	return totalRevenue;

}
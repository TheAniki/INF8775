#ifndef ALGOGLOUTONPROBA_H
#define ALGOGLOUTONPROBA_H


double computeRestaurantsRentability(const map<int, Restaurant>& restaurantMap, map<int, double>& rentabilityMap);
int choseARestaurant(const map<int, double>& rentabilityMap, const double totalRentability);
map<int, Restaurant> generateRestaurantMap(vector<Restaurant> data);
void removeTooBigCapacities(map<int, Restaurant>& restaurants, int currentQuantity, int maxCapcity);
double alggoGloutonProba(vector<Restaurant>& data, int maxCapacity, vector<Restaurant>& chosenRestaurants);




#endif  // !ALGOGLOUTONPROBA_H

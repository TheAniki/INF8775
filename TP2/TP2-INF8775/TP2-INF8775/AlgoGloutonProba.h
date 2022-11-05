#ifndef ALGOGLOUTONPROBA_H
#define ALGOGLOUTONPROBA_H

double computeRestaurantsRentability(map<int, Restaurant>& restaurants, map<int, double>& rentabilities);
int choseARestaurant(map<int, double>& rentabilities, const double totalRentability);
void removeTooBigCapacities(map<int, Restaurant>& restaurants, int currentQuantity, int maxCapcity);
double generateProbaGloutonRestaurants(vector<int[2]>& data, int maxCapacity, vector<int>& chosenRestaurants);
map<int, Restaurant> generateRestaurantMap(const vector<int[2]>& data);

#endif  // !ALGOGLOUTONPROBA_H

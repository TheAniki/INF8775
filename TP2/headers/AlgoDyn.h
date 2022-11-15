#ifndef ALGODYN_H
#define ALGODYN_H

vector<Restaurant> dynamicAlgorithm(vector<Restaurant> restaurants, int capacite, int nbRestaurants);
vector<vector<int>> initializeTable(vector<Restaurant> restaurants, int capacite);
void addToSolution(vector<Restaurant>& solution, int& capacite, Restaurant resto);

#endif // !ALGODYN_H


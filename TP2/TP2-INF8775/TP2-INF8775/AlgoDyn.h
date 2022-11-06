#ifndef ALGODYN_H
#define ALGODYN_H


vector<Restaurant> AlgoDyn(vector<Restaurant> restaurants, int capacite, int nbRestaurants, int& revenue);
vector<vector<int>> InitializeTable(vector<Restaurant> restaurants, int capacite);
void Ajouter(vector<Restaurant>& solution, int& capacite, Restaurant resto, int& revenue);
#endif // !ALGODYN_H


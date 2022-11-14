#include "./headers/Includes.h"
#include "./headers/AlgoDyn.h"


vector<Restaurant> AlgoDyn(vector<Restaurant> restaurants, int capacite, int nbRestaurants, int& revenue) {

	vector<Restaurant> solution;
	vector<vector<int>> D = InitializeTable(restaurants, capacite);
	int j = capacite;
	// On veux maximiser en partant du dernier element.
	for (int i = D.size() - 1; i > 0; i--)
	{
		if (D[i][j] != D[i - 1][j]) {
			if (j - restaurants[i - 1].quantity >= 0) {
				Ajouter(solution, j, restaurants[i - 1], revenue);
			}
		}
	}
	return solution;
}

// Ajoute un restaurant a la solution.
void Ajouter(vector<Restaurant>& solution, int& capacite, Restaurant resto, int& revenue) {
	solution.push_back(resto);
	capacite -= resto.quantity;
	revenue += resto.revenue;
}

vector<vector<int>> InitializeTable(vector<Restaurant> restaurants, int capacite) {

	// Intialisation du tableau D de taille restaurants.size() X capacite remplis de 0.
	vector<vector<int>> D(restaurants.size() + 1, vector<int>(capacite + 1));

	// Initialise la premiere ligne du tableau.	
	for (int j = 0; j < capacite + 1; j++)
	{
		D[0][j] = 0;
	}
	// Remplisage du tableau.
	for (long unsigned int i = 0; i < restaurants.size(); i++) {
		for (int j = 0; j < capacite + 1; j++)
		{
			int first = 0;
			// D[i,j] = max(  r_i + D[i-1,j-q_i] ,   D[i-1,j]  )
			if (j >= restaurants[i].quantity) {
				first = restaurants[i].revenue + D[i][j - restaurants[i].quantity];
			}

			int second = D[i][j];
			D[i + 1][j] = max(first, second);
		}
	}

	return D;
}
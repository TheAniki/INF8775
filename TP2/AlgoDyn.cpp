#include "./headers/Includes.h"
#include "./headers/AlgoDyn.h"

// The main function of this algorithm.
// It Return's the final solution.
vector<Restaurant> dynamicAlgorithm(vector<Restaurant> restaurants, int capacite, int nbRestaurants) {

	vector<Restaurant> solution;
	vector<vector<int>> D = initializeTable(restaurants, capacite);
	int j = capacite;
	// We want to maximize so we start from the last element.
	for (int i = D.size() - 1; i > 0; i--)
	{
		if (D[i][j] != D[i - 1][j]) {
			if (j - restaurants[i - 1].quantity >= 0) {
				addToSolution(solution, j, restaurants[i - 1]);
			}
		}
	}
	return solution;
}

// Add a restaurant to the solution and update the total capacity.
void addToSolution(vector<Restaurant>& solution, int& capacite, Restaurant resto) {
	solution.push_back(resto);
	capacite -= resto.quantity;
}

// Initialize the D array.
vector<vector<int>> initializeTable(vector<Restaurant> restaurants, int capacite) {

	// Initialization of the D array of size restaurants.size() X capacity.
	vector<vector<int>> D(restaurants.size() + 1, vector<int>(capacite + 1));

	// Initializing the array's first row.	
	for (int j = 0; j < capacite + 1; j++)
	{
		D[0][j] = 0;
	}
	// Filling the array.
	for (int i = 0; i < restaurants.size(); i++) {
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
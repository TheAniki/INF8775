#include "Includes.h"
#include "AlgoDyn.h"


vector<Restaurant> AlgoDyn(vector<Restaurant> restaurants, int capacite , int nbRestaurants) {

	vector<Restaurant> solution;
	// TODO: start time
	vector<vector<int>> D = InitializeTable(restaurants, capacite);	
	
	// On veux maximiser en partant du dernier element.
	for (int i = nbRestaurants - 1; i >= 0; i--)
	{	
		if (capacite <= 0)
			break;
		if (i == 0 && capacite - restaurants[i].quantity >= 0) {
			Ajouter(solution, capacite, restaurants[i]);

			return solution;
		}

		if (D[i][capacite] != D[i - 1][capacite]) {
			if (capacite - restaurants[i].quantity >= 0) {
				Ajouter(solution, capacite, restaurants[i]);
			}			
		}
	}
	// cout << "capacite restante: " << capacite << endl;
	// TODO: end time.
	return solution;
}

// Ajoute un restaurant a la solution.
void Ajouter(vector<Restaurant>& solution, int& capacite, Restaurant resto) {
	solution.push_back(resto);
	capacite -= resto.quantity;
}

vector<vector<int>> InitializeTable(vector<Restaurant> restaurants, int capacite) {
	
	// Intialisation du tableau D de taille restaurants.size() X capacite remplis de 0.
	vector<vector<int>> D(restaurants.size()+1, vector<int>(capacite+1)); 
	
	// Initialise la premiere ligne du tableau.	
	for (int j = 0; j < capacite+1; j++)
	{
		if (restaurants[0].quantity <= j)
			D[0][j] = restaurants[0].revenue;
		//cout << " " << D[0][j];
	}
	//cout << endl;

	// Remplisage du tableau.
	for (int i = 1; i < restaurants.size(); i++) {
		for (int j = 0; j < capacite+1; j++)
		{	
			// Place un 0 comme premiere valeur de la ligne.
			if (j == 0)
				D[i - 1][j] = 0;

			if (j <= restaurants[i].quantity)
				D[i][j] = D[i - 1][j];
			else {
				// D[i,j] = max(  r_i + D[i-1,j-q_i] ,   D[i-1,j]  )
				D[i][j] = 
					max(restaurants[i].revenue + D[i - 1][j - restaurants[i].quantity],
						D[i - 1][j]);				
			}	
			//cout << " " << D[i][j];
		}
		//cout << endl;
	}
		
	return D;
}
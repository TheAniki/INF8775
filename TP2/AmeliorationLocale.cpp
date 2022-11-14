#include "./headers/Includes.h"
#include "./headers/AmeliorationLocale.h"
#include "./headers/AlgoGloutonProba.h"


// Returns the totalRevenue and fills the chosenRestaurants vector.
pair<int, int> algoLocalHeuristic(vector<Restaurant>& data, int maxCapacity, vector<Restaurant>& chosenRestaurants) {
	pair<int ,int> revenueQuantityPair= alggoGloutonProba(data, maxCapacity, chosenRestaurants); //For initial solution

	map<int, Restaurant> dataMap = generateRestaurantMap(data);
	map<int, Restaurant> currentSolution = generateRestaurantMap(chosenRestaurants);

	int currentRevenue = revenueQuantityPair.first;
	int currentQuantity = revenueQuantityPair.second;

	// Variables that will be updated at each iterations.
	int newRevenue = 0;
	int newQuantity = 0;
	map<int, Restaurant> newSolution = deepCopyMap(currentSolution);
	
	while (true) {
		pair<int, int> newRevenueQuantityPair = findBestSolutionInNeighborhood(newSolution, dataMap, currentQuantity, currentRevenue, maxCapacity);

		newRevenue = newRevenueQuantityPair.first;
		newQuantity = newRevenueQuantityPair.second;
		if (currentRevenue < newRevenue ) {
			currentSolution = deepCopyMap(newSolution);
			currentRevenue = newRevenue;
			currentQuantity = newQuantity;
		}
		else{
			break;
		}	
	}

	chosenRestaurants = convertMapToVector(currentSolution);
	return make_pair<int, int>(int(currentRevenue), int(currentQuantity));
}

vector<Restaurant> convertMapToVector(map<int, Restaurant> solution) {
	vector<Restaurant> restaurants;
	for (const pair<int, Restaurant> & restaurant : solution) {
		restaurants.push_back(restaurant.second);
	}
	return restaurants;

}


// Combinations of 1 or 2 elements present in te solution.
vector<vector<Restaurant>> generateInsideSolCombinations(const map<int, Restaurant>& solution) {
	vector<vector<Restaurant>> insideSolCombinations;
	addSoloCombinations(solution, insideSolCombinations);
	addDuoCombinations(solution, insideSolCombinations);
	return insideSolCombinations;
}

// Combinations of 1 or 2 elements that or NOT in te solution.
vector<vector<Restaurant>> generateOutsideSolCombinations(const map<int, Restaurant>& solution, const map<int, Restaurant> & dataMap) {
	map<int, Restaurant> outsideSolution;
	for(pair<int, Restaurant> restaurant : dataMap){
		if (solution.count(restaurant.first) ==  0) {// restaurant not in solution.
			outsideSolution.emplace(restaurant);
		}
	}

	vector<vector<Restaurant>> insideSolCombinations;
	addSoloCombinations(outsideSolution, insideSolCombinations);
	addDuoCombinations(outsideSolution, insideSolCombinations);
	return insideSolCombinations;
}


// Generate duo combinations (cardinality = 2 in elements.size() ).
 void addDuoCombinations(map<int, Restaurant> elements, vector<vector<Restaurant>>& combinations) {
	map<int, Restaurant>::iterator startOfSecondLoop = elements.begin();
	for (const pair<int, Restaurant> & element : elements) {
	 startOfSecondLoop++;
		for (map<int, Restaurant>::iterator iter = startOfSecondLoop; iter != elements.end(); ++iter) {
			vector<Restaurant> combination = {element.second, iter->second };
			combinations.push_back(combination);
		}
	}
}


// Generate solo combinaitions (cardinality = elements.size() ).
void addSoloCombinations(map<int, Restaurant> elements, vector<vector<Restaurant>>& combinations) {
	for(map<int, Restaurant>::iterator iter = elements.begin(); iter != elements.end();++iter ){
		vector<Restaurant> combination = {iter->second };
		combinations.push_back(combination);
	}
}


pair<int,int> findBestSolutionInNeighborhood(map<int, Restaurant>& currentSolution, map<int, Restaurant> dataMap, int currentQuantity, int currentRevenue , int maxQuantity) {

	// Generate combinations to iterate over.
	vector<vector<Restaurant>> outsideSolutionCombinations =generateOutsideSolCombinations(currentSolution, dataMap );
	vector<vector<Restaurant>> insideSolutionCombinations = generateInsideSolCombinations(currentSolution);

	// Best values.
	int bestRevenue = currentRevenue;
	int bestCapacity = currentQuantity;
	map<int, Restaurant> bestSolution = deepCopyMap(currentSolution);


	// Outer-loop is the combination of restaurants we can remove from the solution.
	for (vector<Restaurant> removeableCombination : insideSolutionCombinations) {
		int revenueDiff = 0;
		int quantityDiff = 0;
		// This loop has at most two restaurants to remove.
		for (const Restaurant& toRemoveElement : removeableCombination) {
			revenueDiff -= toRemoveElement.revenue;
			quantityDiff -= toRemoveElement.quantity;
		}
		
		// Best values for the inner-loop.
		int bestInnerLoopRevenue = currentRevenue;
		int bestInnerLoopQuantity = currentQuantity;
		vector<Restaurant> bestCombinationToAdd;
		int i = 0;
		// Inner-loop is for the combination of restaurants we can add to the solution.
		for (vector<Restaurant> addableCombination : outsideSolutionCombinations) {
			int innerLoopRevenueDiff = revenueDiff;
			int innerLoopQuantityDiff = quantityDiff;
			i++;
			// At most two resturants to add.
			for (const Restaurant & toAddElement : addableCombination) {
				innerLoopRevenueDiff += toAddElement.revenue;
				innerLoopQuantityDiff += toAddElement.quantity;
			}

			int newRevenue = currentRevenue + innerLoopRevenueDiff;
			int newQuantity = currentQuantity + innerLoopQuantityDiff;
			
			// Update bestInnerLoop solution variables. This is a combination to add considering the same combination to remove for all the inner-loop.
			if (newQuantity <= maxQuantity && newRevenue > bestInnerLoopRevenue) {
				bestCombinationToAdd = deepCopyVector(addableCombination);
				bestInnerLoopRevenue = newRevenue; 
				bestInnerLoopQuantity = newQuantity;
			}
		}

		// Update best solution variables after havin considered all the solution with a single combination to remove.
		if (bestInnerLoopRevenue > bestRevenue) {
			bestRevenue = bestInnerLoopRevenue;
			bestSolution = deepCopyMap(currentSolution);
			bestSolution = currentSolution;
			updateSolution(bestSolution, removeableCombination, bestCombinationToAdd);	
		}
	}

	currentSolution = deepCopyMap(bestSolution);
	return make_pair(int(bestRevenue), int(bestCapacity));
}

vector<Restaurant> deepCopyVector(vector<Restaurant> toCopy) {
	vector<Restaurant> clone;
	for (const Restaurant & element : toCopy) {
		clone.push_back(element);
	}
	return clone;
}

map<int, Restaurant> deepCopyMap(map<int, Restaurant> mapToCopy) {
	map<int, Restaurant> mapClone;
	for (pair<int, Restaurant> restaurantPair : mapToCopy) {
		mapClone.emplace(restaurantPair);
	}
	return mapClone;
}

void updateSolution(map<int, Restaurant> & currentSolution, const vector<Restaurant> toRemoveCombination, const vector<Restaurant> & toAddCombination) {
	for (const Restaurant& restaurant : toRemoveCombination) {
		currentSolution.erase(restaurant.iD);
	}

	for (Restaurant restaurant : toAddCombination) {
		currentSolution.emplace(restaurant.iD, restaurant);
	}
}
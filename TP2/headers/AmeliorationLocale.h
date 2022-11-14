#ifndef AMELIORATIONLOCALE_H
#define AMELIORATIONLOCALE_H

pair<int, int> algoLocalHeuristic(vector<Restaurant>& data, int maxCapacity, vector<Restaurant>& chosenRestaurants);
void addDuoCombinations(map<int, Restaurant> elements, vector<vector<Restaurant>>& combinations);

void addSoloCombinations(map<int, Restaurant> elements, vector<vector<Restaurant>>& combinations);
vector<vector<Restaurant>> generateOutsideSolCombinations(const map<int, Restaurant>& solution, const map<int, Restaurant>& dataMap);
vector<vector<Restaurant>> generateInsideSolCombinations(const map<int, Restaurant>& solution);
void updateSolution(map<int, Restaurant>& currentSolution, const vector<Restaurant> toRemoveCombination, const vector<Restaurant>& toAddCombination);

pair<int, int> findBestSolutionInNeighborhood(map<int, Restaurant>& currentSolution, map<int, Restaurant> dataMap, int currentQuantity, int currentRevenue, int maxQuantity);
vector<Restaurant> deepCopyVector(vector<Restaurant> toCopy);
map<int, Restaurant> deepCopyMap(map<int, Restaurant> mapToCopy);

vector<Restaurant> convertMapToVector(map<int, Restaurant> solution);

#endif  // !1AMELIORATIONLOCALE_H


#include "./headers/localSearch.h"

LocalSearch::LocalSearch(/* args */)
{
}

LocalSearch::LocalSearch(SingleSolution singleSolution)
:SingleSolution::SingleSolution(singleSolution)
{
}

LocalSearch::~LocalSearch()
{
}

Solution LocalSearch::getSolution(){
    return this->_solution;
}

void LocalSearch::upgradeSolution(int nbIterations){
    for(int i=0; i<nbIterations; i++){
        // TODO: upgrade current solution.
    }
}
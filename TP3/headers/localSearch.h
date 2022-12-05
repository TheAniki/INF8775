#ifndef LOCALSEARCH_H
#define LOCALSEARCH_H
#include "singleSolution.h"


class LocalSearch : protected virtual SingleSolution 
{
    public:
        LocalSearch();
        LocalSearch(SingleSolution singleSolution);
        
        void upgradeSolution(int nbIterations);

        // getter
        Solution getSolution();
        
        ~LocalSearch();

    private:
        /* data */  
};

#endif // !LOCALSEARCH_H
#ifndef LOCALSEARCH_H
#define LOCALSEARCH_H
#include "singleSolution.h"


class LocalSearch : protected virtual SingleSolution 
{
    public:
        LocalSearch();
        LocalSearch(SingleSolution singleSolution);
        
        void upgradeSolution(int nbIterations);
        map<int, shared_ptr<Circumscription>> findNeihborCircumscriptionsOfLowest(SharedCirc circ);
        SharedMun findLowestMunicipality(SharedCirc circ);

        vector<pair<SharedCirc,int>> orderLosingCirc();
        void increaseVotesClosestToTreshold(SharedCirc losingCirc);
        void increaseVotesofAllLosing(vector<pair<SharedCirc,int>>  losingCirc);

        // getter
        Solution getSolution();
        
        ~LocalSearch();

    private:
        /* data */  
};

SharedCirc findBestWinningCric(map<int, SharedCirc> neighbors);
#endif // !LOCALSEARCH_H
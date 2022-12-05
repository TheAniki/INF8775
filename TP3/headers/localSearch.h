#ifndef LOCALSEARCH_H
#define LOCALSEARCH_H
#include "singleSolution.h"


class LocalSearch : protected virtual SingleSolution 
{
    public:
        LocalSearch();
        LocalSearch(SingleSolution singleSolution);
        
        void upgradeSolution(int nbIterations);
        
        

        vector<pair<SharedCirc,int>> orderLosingCirc();
        void increaseVotesClosestToTreshold(SharedCirc losingCirc);
        void increaseVotesofAllLosing(vector<pair<SharedCirc,int>>  losingCirc);
        void TrySwappingMunicipalities(SharedCirc losingCirc, SharedCirc bestCirc);

        SharedMun findBestMunicipalityToRemove(SharedCirc circ);
        SharedMun findLowestMunicipality(SharedCirc circ);
        map<int,SharedCirc> findNeihborCircumscriptionsOfLowest(SharedCirc circ);

        // getter
        Solution getSolution();
        
        ~LocalSearch();

    private:
        /* data */  
};

SharedCirc findBestWinningCric(map<int, SharedCirc> neighbors);
#endif // !LOCALSEARCH_H
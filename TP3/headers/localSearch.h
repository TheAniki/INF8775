#ifndef LOCALSEARCH_H
#define LOCALSEARCH_H
#include "singleSolution.h"


class LocalSearch : protected virtual SingleSolution 
{
    public:
        LocalSearch();
        LocalSearch(SingleSolution singleSolution);
        
        bool upgradeSolution();
        
        

        vector<pair<SharedCirc,int>> orderLosingCirc();
        bool increaseVotesClosestToTreshold(SharedCirc losingCirc,int distanceToWinning);
        void increaseVotesofAllLosing(vector<pair<SharedCirc,int>>  losingCirc);
        bool TrySwappingMunicipalities(SharedCirc losingCirc, vector<SharedCirc> neighbors,int distanceToWinning);
        vector<SharedMun> findAllCircsThatFit(SharedCirc circ, SharedCirc inCirc);
        SharedMun findBestMunicipalityToRemove(SharedCirc circ);
        vector<SharedMun> findAllBestMunicipalities(SharedCirc circ);

        SharedMun findLowestMunicipality(SharedCirc circ);
        vector<SharedMun> findAllLowestMunicipalities(SharedCirc circ);
        

        map<int,SharedCirc> findNeihborCircumscriptionsOfLowest(SharedCirc circ);


        void swapMuns(int bestCircNb , SharedMun bestMunInNeighbor , int losingCircNb , SharedMun worstMunInLosing);
        // getter
        Solution getSolution();
        
        ~LocalSearch();

    private:
        /* data */  
};
bool isContainedInBoth(int circNumber, vector<pair<int,SharedMun>> neig, vector<pair<int,SharedMun>> loss);
bool isContainedIn(int circNumber, vector<pair<int,SharedMun>> list);
SharedCirc findBestWinningCric(map<int, SharedCirc> neighbors);
#endif // !LOCALSEARCH_H
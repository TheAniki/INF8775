#ifndef QUICKSOLUTION_H
#define QUICKSOLUTION_H
#include "singleSolution.h"
using SharedCirc = shared_ptr<Circumscription>;

class QuickSolution : protected virtual SingleSolution
{
    public:
        QuickSolution(/* args */);
        QuickSolution(SingleSolution SingleSolution);
        ~QuickSolution();
        
        // create solution
        bool create();

        // methods
        bool addMunicipalityWithProbaHeur(int i, int j);
        vector<pair<SharedCirc, double>> findCircsInRange(int i, int j);
        SharedCirc choseCircumscriptionFrom(vector<pair<SharedCirc, double>> circsInRange, int,int);
        vector<int> calculateDistance(vector<pair<SharedCirc, double>> circsInRange,int i, int j);
        bool addMunicipalityToChosenCirc(SharedCirc circChosen,int i,int j);
        bool forceAddMunicipality(shared_ptr<Municipality> municipalityToForce, vector<Coord> historyOfForcedMun);
        int findSmallestTotalDistanceToAnIncomplete(shared_ptr<Municipality> municipalityToCheck , map<int, shared_ptr<Circumscription>> incompleteCircs);

        //getter
        Solution getSolution();
    
    private:
        /* data */    
};




#endif // !QUICKSOLUTION_H
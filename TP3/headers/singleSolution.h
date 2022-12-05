#ifndef SINGLESOLUTION_H
#define SINGLESOLUTION_H
#include "Includes.h"
using SharedCirc = shared_ptr<Circumscription>;

class SingleSolution 
{
    public:
        SingleSolution();
        SingleSolution(vector<vector<shared_ptr<Municipality>>> municipalities, int nbCircumscription);
        ~SingleSolution();
        Solution initializeSolution(int nbCircumscription);
        vector<vector<bool>> createAssignedMun(vector<vector<shared_ptr<Municipality>>> municipalities);        
        void computeCircBounds();
        void computeRepartition();
        bool validateMunFitsInCirc(SharedCirc circumscription, shared_ptr<Municipality> municipalityToValidate);
        void addMunicipalityToCirc(SharedCirc circumscription, shared_ptr<Municipality> municipality);
        vector<SharedCirc> findIncompleteCircs(vector<SharedCirc> circumscriptions);
        vector<SharedCirc> findPossibleCircumscriptionsToContainMun(
        shared_ptr<Municipality> municipalityToInclude, vector<SharedCirc> circumscriptionsConsidered);
        SharedCirc findClosestCircumscription();
        vector<int> findNeighbourCircumscriptions(Coord coord);     
        void removeMunicipalityFromCirc(shared_ptr<Municipality> municipalityToRemove, SharedCirc circumscription);
        shared_ptr<Municipality> choseMunicipalityToRemoveFromCirc( SharedCirc circumscriptionToRemoveIn, SharedCirc incompleteCirc);
        int computeTotalDistanceToCirc(shared_ptr<Municipality> municipality, SharedCirc circumscription);
        
        // Getters
        Solution getSolution();

    protected:
        int _nbMunicipalities;
        int _votesToWin;
        int _maxDist;
        int _nbCircumscriptions;
        CircBound _minCirc;
        CircBound _maxCirc; 
        CircBound _currentBound;    
        Solution _solution;
        vector<vector<bool>> _assignedMunicipalities;
        vector<vector<shared_ptr<Municipality>>> _municipalities;
        queue<shared_ptr<Municipality>> _unassignedMunicipalities;
};





#endif // !SINGLESOLUTION_H
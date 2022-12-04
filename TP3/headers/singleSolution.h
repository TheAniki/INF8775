#ifndef SINGLESOLUTION_H
#define SINGLESOLUTION_H
#include "Includes.h"


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
        bool validateMunFitsInCirc(shared_ptr<Circumscription> circumscription, shared_ptr<Municipality> municipalityToValidate);
        void addMunicipalityToCirc(shared_ptr<Circumscription> circumscription, shared_ptr<Municipality> municipality);
        vector<shared_ptr<Circumscription>> findIncompleteCircs(vector<shared_ptr<Circumscription>> circumscriptions);
        vector<shared_ptr<Circumscription>> findPossibleCircumscriptionsToContainMun(
        shared_ptr<Municipality> municipalityToInclude, vector<shared_ptr<Circumscription>> circumscriptionsConsidered);
        shared_ptr<Circumscription> findClosestCircumscription();
        vector<int> findNeighbourCircumscriptions(Coord coord);     
        void removeMunicipalityFromCirc(shared_ptr<Municipality> municipalityToRemove, shared_ptr<Circumscription> circumscription);
        shared_ptr<Municipality> choseMunicipalityToRemoveFromCirc( shared_ptr<Circumscription> circumscriptionToRemoveIn, shared_ptr<Circumscription> incompleteCirc);
        
        // Getters
        Solution getSolution();

    protected:
        int _nbMunicipalities;
        int _votesToWin;
        int _maxDist;
        int _nbCircumscriptions;
        CircBound _minCirc;
        CircBound _maxCirc; 
        CircBound _currentCirc;    
        Solution _solution;
        vector<vector<bool>> _assignedMunicipalities;
        vector<vector<shared_ptr<Municipality>>> _municipalities;
        queue<shared_ptr<Municipality>> _unassignedMunicipalities;
};





#endif // !SINGLESOLUTION_H
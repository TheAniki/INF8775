#ifndef SINGLESOLUTION_H
#define SINGLESOLUTION_H
#include "Includes.h"
using SharedCirc = shared_ptr<Circumscription>;
using SharedMun = shared_ptr<Municipality>;

class SingleSolution 
{
    public:
        SingleSolution();
        SingleSolution(vector<vector<SharedMun>> municipalities, int nbCircumscription);
        ~SingleSolution();
        Solution initializeSolution(int nbCircumscription);
        vector<vector<bool>> createAssignedMun(vector<vector<SharedMun>> municipalities);        
        void computeCircBounds();
        void computeRepartition();
        bool validateMunFitsInCirc(SharedCirc circumscription, SharedMun municipalityToValidate);
        void addMunicipalityToCirc(SharedCirc circumscription, SharedMun municipality);
        map<int, SharedCirc> findIncompleteCircs(vector<SharedCirc> circumscriptions);
        vector<SharedCirc> findPossibleCircumscriptionsToContainMun(
        SharedMun municipalityToInclude, vector<SharedCirc> circumscriptionsConsidered);
        //SharedCirc findClosestCircumscription();
        map<int, SharedCirc> findNeighbourCircumscriptions(Coord coord, bool inManhattan); 
        bool isMunInVector(SharedMun municipality, const vector<Coord>& munCoordList);    
        void removeMunicipalityFromCirc(SharedMun municipalityToRemove, SharedCirc circumscription);
        SharedMun choseMunicipalityToRemoveFromCirc( SharedCirc circumscriptionToRemoveIn, SharedCirc incompleteCirc);
        int computeTotalDistanceToCirc(SharedMun municipality, SharedCirc circumscription);
        vector<SharedMun> getTooFarMunsInCirc(SharedMun referenceMunicipality, SharedCirc circumscription);

        bool validateMunFits(SharedCirc circumscription, SharedMun municipalityToValidate);
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
        vector<vector<SharedMun>> _municipalities;
        queue<SharedMun> _unassignedMunicipalities;
};





#endif // !SINGLESOLUTION_H
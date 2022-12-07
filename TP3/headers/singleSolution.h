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
        map<int, SharedCirc> findIncompleteCircs();
        vector<SharedCirc> findPossibleCircumscriptionsToContainMun(
        SharedMun municipalityToInclude, vector<SharedCirc> circumscriptionsConsidered);
        //SharedCirc findClosestCircumscription();
        map<int, SharedCirc> findNeighbourCircumscriptions(Coord coord); 
        bool isMunInVector(shared_ptr<Municipality> municipality, const vector<Coord>& munCoordList);    
        void removeMunicipalityFromCirc(shared_ptr<Municipality> municipalityToRemove, SharedCirc circumscription);
        shared_ptr<Municipality> choseMunicipalityToRemoveFromCirc( SharedCirc circumscriptionToRemoveIn, SharedCirc incompleteCirc);
        int computeTotalDistanceToCirc(shared_ptr<Municipality> municipality, SharedCirc circumscription);
        vector<shared_ptr<Municipality>> getTooFarMunsInCirc(shared_ptr<Municipality> referenceMunicipality, SharedCirc circumscription);
      
        Coord findFurthestMunicipalityInCirc(shared_ptr<Municipality> municipality, SharedCirc circumscription );

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
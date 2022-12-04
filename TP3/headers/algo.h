#ifndef ALGO_H
#define ALGO_H

// ==================== Struct ==================== //
struct CircBound{
    int circSize;
    int maxAmount;

    CircBound():circSize(0), maxAmount(0){};
    CircBound(int cSize, int mAmount ):
        circSize(cSize), maxAmount(mAmount){};
};

// ================================================= //

class Algo
{        
   public:
    Algo();
    Algo(vector<vector<shared_ptr<Municipality>>> municipalities, int nbCircumscription);
    ~Algo();
    bool quickSolution();
    vector<vector<bool>> createAssignedMun(vector<vector<shared_ptr<Municipality>>> municipalities);
    Solution initializeSolution(int nbCircumscription);
    void computeCircBounds();
    void computeRepartition();
    bool addMunicipalityToFirstAvailableCirc(int i, int j);
    bool forceAddMunicipality(shared_ptr<Municipality> municipalityToForce, vector<Coord> historyOfForcedMun);
    bool validateMunFitsInCirc(shared_ptr<Circumscription> circumscription, shared_ptr<Municipality> municipalityToValidate);
    int computeTotalDistanceToCirc(shared_ptr<Municipality> municipality, shared_ptr<Circumscription> circumscription);
    void addMunicipalityToCirc(shared_ptr<Circumscription> circumscription, shared_ptr<Municipality> municipality);
    map<int, shared_ptr<Circumscription>> findIncompleteCircs(vector<shared_ptr<Circumscription>> circumscriptions);
    vector<shared_ptr<Circumscription>> findPossibleCircumscriptionsToContainMun(
    shared_ptr<Municipality> municipalityToInclude, vector<shared_ptr<Circumscription>> circumscriptionsConsidered);
    vector<shared_ptr<Circumscription>> findClosestCircumscription(shared_ptr<Municipality> municipality);
    map<int, shared_ptr<Circumscription>> findNeighbourCircumscriptions(Coord coord);
    void removeMunicipalityFromCirc(shared_ptr<Municipality> municipalityToRemove, shared_ptr<Circumscription> circumscription);
    shared_ptr<Municipality> choseMunicipalityToRemoveFromCirc( shared_ptr<Circumscription> circumscriptionToRemoveIn, shared_ptr<Circumscription> incompleteCirc);
    
    // Getters
    Solution getSolution();

   private:
    int _nbMunicipalities;
    int _votesToWin;
    int _maxDist;
    int _nbCircumscriptions;
    CircBound _minCirc;
    CircBound _maxCirc; 
    CircBound _currentBound;
    Solution _solution;
    vector<vector<bool>> _assignedMunicipalities; //TODO : I dont think we need that
    vector<vector<shared_ptr<Municipality>>> _municipalities;
    queue<shared_ptr<Municipality>> _unassignedMunicipalities;
};

#endif // !ALGO_H
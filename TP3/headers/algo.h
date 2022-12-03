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
    void quickSolution();
    vector<vector<bool>> createAssignedMun(vector<vector<shared_ptr<Municipality>>> municipalities);
    Solution initializeSolution(int nbCircumscription);
    void computeCircBounds();
    void computeRepartition();
    bool addMunicipalityToFirstAvailableCirc(int i, int j);
    bool validateMunFitsInCirc(shared_ptr<Circumscription> circumscription, shared_ptr<Municipality> municipalityToValidate);
    void addMunicipalityToCirc(shared_ptr<Circumscription> circumscription, shared_ptr<Municipality> municipality);
    vector<shared_ptr<Circumscription>> findIncompleteCircs(vector<shared_ptr<Circumscription>> circumscriptions);
    vector<shared_ptr<Circumscription>> findPossibleCircumscriptionsToContainMun(
    shared_ptr<Municipality> municipalityToInclude, vector<shared_ptr<Circumscription>> circumscriptionsConsidered);
    shared_ptr<Circumscription> findClosestCircumscription();
    vector<shared_ptr<Circumscription>> findNeighbourCircumscriptions(Coord coord);     
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
    CircBound _currentCirc;    
    Solution _solution;
    vector<vector<bool>> _assignedMunicipalities;
    vector<vector<shared_ptr<Municipality>>> _municipalities;
    queue<shared_ptr<Municipality>> _unassignedMunicipalities;
};

#endif // !ALGO_H
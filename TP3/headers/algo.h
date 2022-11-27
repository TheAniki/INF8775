#ifndef ALGO_H
#define ALGO_H

void algo();
vector<vector<bool>> createAssignedMun(vector<vector<shared_ptr<Municipality>>> municipalities);
Solution quickSolution(vector<vector<shared_ptr<Municipality>>> municipalities, int nbCircumscription );
Solution initializeSolution(int nbCircumscription);
void addMunicipalityToCirc(shared_ptr<Circumscription> circumscription, shared_ptr<Municipality> municipality );
void computeCircBounds(int nbMunicipalities,int nbCircumscription, int& minCirc, int& maxCirc);
int computeManhattanDist(const Coord& coord1, const Coord& coord2);
bool validateMunFitsInCirc(shared_ptr<Circumscription> circumscription, shared_ptr<Municipality> municipalityToValidate, int maxDist);
void addMunicipalityToFirstAvailableCirc(shared_ptr<Municipality> municipality, vector<shared_ptr<Circumscription>> circumscription, bool& isAddedSuccess );
bool addMunicipalityToFirstAvailableCirc(shared_ptr<Municipality> municipality, 
        vector<shared_ptr<Circumscription>> circumscriptions, 
        int maxDist, 
        int votesToWin,
        int maxCirc );
vector<shared_ptr<Circumscription>> findIncompleteCircs(vector<shared_ptr<Circumscription>> circumscriptions, int minCirc );
vector<shared_ptr<Circumscription>> findPossibleCircumscriptionToContain(shared_ptr<Municipality> municipalityToInclude, vector<shared_ptr<Circumscription>> circumscriptionsConsidered, int maxDist);
#endif // !ALGO_H
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

#endif // !ALGO_H
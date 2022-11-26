#ifndef ALGO_H
#define ALGO_H

void algo();
vector<vector<bool>> createAssignedMun(vector<vector<shared_ptr<Municipality>>> municipalities);
Solution quickSolution(vector<vector<shared_ptr<Municipality>>> municipalities, int nbCircumscription );
Solution initializeSolution(int nbCircumscription);
void addMunicipalityToCirc(shared_ptr<Circumscription> circumscription, shared_ptr<Municipality> municipality );
void computeCircBounds(int nbMunicipalities,int nbCircumscription, int& minCirc, int& maxCirc);

#endif // !ALGO_H
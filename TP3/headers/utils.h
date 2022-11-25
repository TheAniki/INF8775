#ifndef UTILS_H
#define UTILS_H

vector<int> split(const string& s, char delim);
void parseTxtArguments(int argc, const char* argv[], string& filename,unsigned int& nbCircumscription, bool& print);
vector<vector<shared_ptr<Municipality>>> createMunicipalityMatrix(ifstream& file, int& nColumn, int& nLine);
void displaySolution();
void calculateScores(vector<vector<shared_ptr<Municipality>>> municipalities, int nColumn, int nLine);
float scoreFromNeighbors(Coord coord,vector<vector<shared_ptr<Municipality>>> municipalities ,int& nbNeighbors, int nColumn, int nLine);
bool CoordinateIsValid(int x_coord, int y_coord ,int nColumn, int nLine);
#endif // !UTILS_H
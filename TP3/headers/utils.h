#ifndef UTILS_H
#define UTILS_H

vector<int> split(const string& s, char delim);
void parseTxtArguments(int argc, const char* argv[], string& filename,unsigned int& nbCircumscription, bool& print);
vector<vector<shared_ptr<Municipality>>> createMunicipalityMatrix(ifstream& file, int& nColumn, int& nRows);
void displaySolution();
void calculateScores(vector<vector<shared_ptr<Municipality>>> municipalities, int nColumn, int nRows);
float scoreFromNeighbors(Coord coord,vector<vector<shared_ptr<Municipality>>> municipalities ,int& nbNeighbors, int nColumn, int nRows);
bool coordinateIsValid(int column_coord, int row_coord ,int nColumn, int nRows);
#endif // !UTILS_H
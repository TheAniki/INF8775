#ifndef UTILS_H
#define UTILS_H

vector<int> split(const string& s, char delim);
void argumentsParsing(int argc, const char* argv[], string& filename,unsigned int& nbCirconscription, bool& print);
vector<Municipality> getMunicipalities(ifstream& file, int& x_mun, int& y_mun);
void displaySolution();
void CalculateScore(vector<Municipality>& municipalities, vector<vector<int>> voteMatrix, int x_mun, int y_mun);
float scoreFromNeighbors(Coord coord,vector<vector<int>> voteMatrix,int& nbNeighbor, int x_mun, int y_mun);
bool CoordinateIsValid(int x_coord, int y_coord ,int x_mun, int y_mun);
#endif // !UTILS_H
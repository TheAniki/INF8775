#ifndef UTILS_H
#define UTILS_H

vector<int> split(const string& s, char delim);
void argumentsParsing(int argc, const char* argv[], string& filename,unsigned int& nbCirconscription, bool& print);
vector<Municipality> getMunicipalities(ifstream& file, int& x_mun, int& y_mun);
void displaySolution();

#endif // !UTILS_H
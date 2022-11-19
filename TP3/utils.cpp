#include "./headers/Includes.h"
#include "./headers/utils.h"

// Split method used to split a string containing numbers
// and returns a vector of ints.
vector<int> split(const string& s, char delim) {
	vector<int> result;
	stringstream ss(s);
	string item;
	while (getline(ss, item, delim)) {		
		if(!item.empty())
			result.push_back(stoi(item));
	}

	return result;
}

// This method is used to parse the arguments.
void argumentsParsing(int argc, const char* argv[], 
string& filename,unsigned int& nbCirconscription, bool& print){

    for (int i = 1; i < argc-1; i++)
    {
        if (!strcmp(argv[i], "-e"))
            filename = argv[i + 1];
        if (!strcmp(argv[i+1], "-p"))
            print = true;
		if (!(strcmp(argv[i],"-c"))){
            stringstream strValue;
            strValue << argv[i+1];
            strValue>>nbCirconscription;
        }	
    }
}

// ========================================================================== //
// Reads the file's content and extract the municipality's data.
// Then it returns a list of municipalities containing all the municipalities.
//
// Also :
//  x_mun = x number of municipalities.
//  y_mun = y number of municipalities.
vector<Municipality> getMunicipalities(ifstream& file, int& x_mun, int& y_mun){    
    // List of municipalities
	vector<Municipality> municipalities;  
	string line;
    int x = 0;
    int y = 0;
    bool once = true;
	while (getline(file, line)) {	// Read data from file object and put it into the string line.
		vector<int> v = split(line, ' ');	// Split the string.
		if (once) {
			x_mun = v[0];
            y_mun = v[1];
            once = false;
		}
        else{
            for(auto vote : v){
                municipalities.push_back(Municipality(x,y,vote));
                y++;
            }              
            y=0;
            x++;          
        }        
	}	

    return municipalities;
}

/*  Pour une même solution, il faut afficher les différentes municipalités qui se trouvent dans une 
    même circonscription sur la même ligne, une à la suite de l’autre, séparées par un espace, 
    puis lorsqu’on passe à une prochaine circonscription, on change de ligne. Par la suite, 
    lorsqu’une meilleure solution est trouvée, on saute deux fois la ligne.
*/
void displaySolution(){
    // TODO : Display solution.
}
#include "./headers/Includes.h"
#include "./headers/utils.h"

// Split method used to split a string containing numbers
// and returns a vector of ints.
vector<int> split(const string& s, char delim) {
	vector<int> result;
	stringstream ss(s);
	string item;
	while (getline(ss, item, delim)) {		
		if(!item.empty()){
			result.push_back(stoi(item));
        }
        
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
    vector<vector<int>> voteMatrix;    
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
            voteMatrix.push_back(v);              
            y=0;
            x++;          
        }        
	}	

    CalculateScore(municipalities, voteMatrix,x_mun,y_mun);

    return municipalities;
}

// Calculate the Score for each municipality.
void CalculateScore(vector<Municipality>& municipalities, vector<vector<int>> voteMatrix, int x_mun, int y_mun){
    
    for(int i =0 ; i<municipalities.size(); i++){
        int nbNeighbor = 0;
        municipalities[i].score = (float) scoreFromNeighbors(municipalities[i].coordinates,voteMatrix, nbNeighbor, x_mun, y_mun);
        municipalities[i].nbVoisins = nbNeighbor;
    }
}

// Calculate the score using the neighbors.
float scoreFromNeighbors(Coord coord,vector<vector<int>> voteMatrix,int& nbNeighbor, int x_mun, int y_mun){
    
    // list of all possible neighbors offsets. 
    Coord possibleNeighbors[8] = { Coord(-1,-1), Coord(-1,0), Coord(-1,1),
                                   Coord(0,-1),               Coord(0,1),
                                   Coord(1,-1),  Coord(1,0),  Coord(1,1)};
    int sum = 0;
    int counter =0;
    for(int i =0; i<8;i++){
        int x = coord.x + possibleNeighbors[i].x;
        int y = coord.y + possibleNeighbors[i].y;

        if(CoordinateIsValid(x,y, x_mun, y_mun)){
            sum += voteMatrix[x][y];
            counter++;
        }
    }
    float score = (float)sum /(float)counter;
    nbNeighbor = counter;

    return score;
}

// Verify if the coordinate is valid.
bool CoordinateIsValid(int x_coord, int y_coord ,int x_mun, int y_mun){
    bool isValid = !(
    x_coord < 0 || y_coord < 0 ||
    x_coord >= y_mun || y_coord >= x_mun );

    return isValid;
}

/*  Pour une même solution, il faut afficher les différentes municipalités qui se trouvent dans une 
    même circonscription sur la même ligne, une à la suite de l’autre, séparées par un espace, 
    puis lorsqu’on passe à une prochaine circonscription, on change de ligne. Par la suite, 
    lorsqu’une meilleure solution est trouvée, on saute deux fois la ligne.
*/
void displaySolution(){
    // TODO : Display solution.
}
#include "./headers/Includes.h"
#include "./headers/utils.h"
#include <memory>

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
void parseTxtArguments(int argc, const char* argv[], 
string& filename,unsigned int& nbCircumscription, bool& print){

    for (int i = 1; i < argc-1; i++)
    {
        if (!strcmp(argv[i], "-e"))
            filename = argv[i + 1];
        if (!strcmp(argv[i+1], "-p"))
            print = true;
		if (!(strcmp(argv[i],"-c"))){
            stringstream strValue;
            strValue << argv[i+1];
            strValue>>nbCircumscription;
        }	
    }
}

// Reads the file's content and extract the municipality's data.
// Then it returns a list of municipalities containing all the municipalities.
//
// Also :
//  nColumn = column number of municipalities.
//  nLine = y number of municipalities.
vector<vector<shared_ptr<Municipality>>> createMunicipalityMatrix(ifstream& file, int& nColumn, int& nLine){    
    // List of municipalities
	vector<vector<shared_ptr<Municipality>>> municipalities;  
    vector<vector<int>> voteMatrix;    
	string line;
    int column = 0;
    int y = 0;
    bool once = true;

	while (getline(file, line)) {	// Read data from file object and put it into the string line.
		vector<int> txtVote = split(line, ' ');	// Split the string.
		if (once) { //For first line
			nColumn = txtVote[0];
            nLine = txtVote[1];
            once = false;
		}
        else{ //Every other line
            municipalities.push_back(vector<shared_ptr<Municipality>>() );

            for(auto vote : txtVote){
                municipalities[y].push_back(make_unique<Municipality>(column,y,vote));
                cout << municipalities[y][column] ->nbVotes << " ";
                column++;
            }
            cout << endl;
            // voteMatrix.push_back(txtVote);              
            column=0;
            y++;          
        }        
	}	
    cout << "ENDED READ" << endl;
    calculateScores(municipalities, nColumn,nLine);

    return municipalities;
}

// Calculate the Score for each municipality.
void calculateScores(vector<vector<shared_ptr<Municipality>>> municipalities, int nColumn, int nLine){
    cout << "x_num : " << nColumn << endl;
    cout << "y_num : " << nLine << endl;

    for(int i =0 ; i<nLine; i++){
        for(int j = 0 ; j < nColumn ; j++ ){
            cout << "COMPUTING ... i=" << i << ", j ="<< j << endl;
            int nbNeighbors = 0;
            municipalities[i][j]->score = (float) scoreFromNeighbors(Coord(j, i), municipalities,nbNeighbors, nColumn, nLine);
            cout << "SCORE OF " << municipalities[i][j]->coordinates.column << ", " <<municipalities[i][j]->coordinates.column << " : "  << municipalities[i][j]->score << endl;
            municipalities[i][j]->nbNeighbors = nbNeighbors - 1;
        }
    cout << endl;
    }

}

// Calculate the score using the neighbors.
float scoreFromNeighbors(Coord coord,vector<vector<shared_ptr<Municipality>>> municipalities ,int& nbNeighbors, int nColumn, int nLine){
    // list of all possible neighbors offsets. 
    Coord possibleNeighbors[9] = { Coord(-1,-1), Coord(-1,0), Coord(-1,1),
                                   Coord(0,-1),  Coord(0,0) ,  Coord(0,1),
                                   Coord(1,-1),  Coord(1,0) ,  Coord(1,1)};
    int sum = 0;
    int counter =0;
    for(int i =0; i<9;i++){
        int column = coord.column + possibleNeighbors[i].column;
        int y = coord.y + possibleNeighbors[i].y;

        if(CoordinateIsValid(column,y, nColumn, nLine)){
            sum += municipalities[y][column]->nbVotes;
            counter++;
        }
    }
    float score = (float)sum /(float)counter;
    nbNeighbors = counter;

    return score;
}

// Verify if the coordinate is valid.
bool CoordinateIsValid(int x_coord, int y_coord ,int nColumn, int nLine){
    bool isValid = !(
    x_coord < 0 || y_coord < 0 ||
    x_coord >= nColumn || y_coord >= nLine );

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
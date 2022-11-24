#include "./headers/Includes.h"
#include "./headers/utils.h" 

int main(int argc, const char*argv[]){

    // Arguments parsing.
	string filename;
    bool print = false;
    unsigned int nbCirconscription;
    argumentsParsing(argc, argv, filename, nbCirconscription, print);

    // file reading
   
	ifstream file;  // file initialization.
    file.open(filename, ios::in); // Opening of the file.
	if (!file.is_open()) { // Verify if the file is openned.
		cout << "open file failed\n";
		return 0;
	}

    int x_num; // x number of municipalities.
    int y_num; // y number of minicipalities.

    // List of municipalities
    vector<Municipality> municipalities = getMunicipalities(file, x_num, y_num);

    // Display each municipality's data.
    for(Municipality municipality : municipalities){
        cout<<"x: "<<municipality.coordinates.x
            <<" | y: "<< municipality.coordinates.y 
            << " | votes for green party : "<<municipality.nbVotes
            << " | score : "<<municipality.score
            << " | nbVoisin: "<<municipality.nbVoisins
            <<endl;
    }

    return 0;
}
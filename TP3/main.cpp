#include "./headers/Includes.h"
#include "./headers/utils.h" 
#include "./headers/algo.h"

int main(int argc, const char*argv[]){

    // Arguments parsing.
	string filename;
    bool print = false;
    unsigned int nbCircumscription;
    parseTxtArguments(argc, argv, filename, nbCircumscription, print);
    
    // file reading
	ifstream file;  // file initialization.
    file.open(filename, ios::in); // Opening of the file.
	if (!file.is_open()) { // Verify if the file is openned.
		cout << "open file failed\n";
		return 0;
	}

    int nColumn; // column number of municipalities.
    int nRows; // row number of minicipalities.

    // List of municipalities
    vector<vector<shared_ptr<Municipality>>> municipalities = createMunicipalityMatrix(file, nColumn, nRows);
    if(nbCircumscription > nColumn*nRows){
        cout<<"le nombre de circonscription ne peut pas depacer le nombre de municipalite"<<endl;
        return 0;
    }

    int nbVotesTotal = 0;
    // Display each municipality's data.
    for(int i = 0 ; i < nRows ; i++){
        vector<shared_ptr<Municipality>> readLine = municipalities[i];
        for(auto&& municipality : readLine){
            cout<<"column: "<< municipality->coordinates.column
                <<" | row: "<< municipality->coordinates.row 
                << " | votes for green party : "<< municipality->nbVotes
                << " | score : "<< municipality->score
                << " | nbVoisin: "<< municipality->nbNeighbors
                << endl;
            nbVotesTotal += municipality -> nbVotes;
        }
    }
    

    // Calculate the number of possible winning district.
    int votesToWin = ((50*(nColumn*nRows))/nbCircumscription)+1;
    cout<<"Nb de votes total pour gagner par circonstription: "<< votesToWin << endl;
    cout<<"Nb de votes total: "<<nbVotesTotal<<endl;
    cout<<"Nb circonscription gagante possible: "<< nbVotesTotal/votesToWin;
    cout<<" sur "<< nbCircumscription<<endl;
    

    Solution solution = quickSolution(municipalities, nbCircumscription);
    displaySolution(solution);

    return 0;
}
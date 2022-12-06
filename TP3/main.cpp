#include "./headers/Includes.h"
#include "./headers/utils.h" 
#include "./headers/quickSolution.h"
#include "./headers/localSearch.h"


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
    unsigned int size = nColumn*nRows;
    if(nbCircumscription > size && nbCircumscription == 0){
        cout<<"le nombre de circonscription ne peut pas depacer le nombre de municipalite"<<endl;
        return 0;
    }

    int nbVotesTotal = 0;
    // Display each municipality's data.
    for(int i = 0 ; i < nRows ; i++){
        vector<shared_ptr<Municipality>> readLine = municipalities[i];
        for(auto&& municipality : readLine){
            // cout<<"column: "<< municipality->coordinates.column
            //     <<" | row: "<< municipality->coordinates.row 
            //     << " | votes for green party : "<< municipality->nbVotes
            //     << " | score : "<< municipality->score
            //     << " | nbVoisin: "<< municipality->nbNeighbors
            //     << endl;
            nbVotesTotal += municipality -> nbVotes;
        }
    }
    

    // Calculate the number of possible winning district.
    int votesToWin = ((50*(nColumn*nRows))/nbCircumscription)+1;
    cout<<"Nb de votes total pour gagner par circonstription: "<< votesToWin << endl;
    cout<<"Nb de votes total: "<<nbVotesTotal<<endl;
    cout<<"Nb circonscription gagante possible: "<< nbVotesTotal/votesToWin;
    cout<<" sur "<< nbCircumscription<<endl;
    

    // Algo algo = Algo(municipalities, nbCircumscription);
    // bool worked = algo.quickSolution();
    // displaySolution(algo.getSolution());

    
    
    bool worked = false;
    int it = 0;
    int oldBest = 0;
    SingleSolution singleSolution = SingleSolution(municipalities,nbCircumscription);
    QuickSolution quickSolution = QuickSolution(singleSolution);
    LocalSearch localSearch = LocalSearch(singleSolution);
    worked = quickSolution.create(); 
    int i=0;
    for(auto&& circ : localSearch.getSolution().circumscriptions){
        if(circ->totalVotes >= votesToWin)
            i++;
    }
    cout<<"Number of circs won "<<i<<" of "<<nbCircumscription<<endl;
    displaySolution(quickSolution.getSolution());

    int best = 0;
    i=0;
    int lastBest = 0;
    while(i<13){
        i=0;
        localSearch.upgradeSolution(1);
        
        for(auto&& circ : localSearch.getSolution().circumscriptions){
            if(circ->totalVotes >= votesToWin)
                i++;
        }
        if(i>lastBest){
            lastBest = i;
            cout<<"Number of circs won "<<i<<" of "<<nbCircumscription<<endl;
            displaySolution(localSearch.getSolution());
        }        
    }    
    return 0;
}
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

    // Calculate the number of possible winning district.
    int votesToWin = ((50*(nColumn*nRows))/nbCircumscription)+1;
    

    SingleSolution singleSolution = SingleSolution(municipalities,nbCircumscription);
    QuickSolution quickSolution = QuickSolution(singleSolution);
    LocalSearch localSearch = LocalSearch(singleSolution);

    // for(double ratio = 20 ; ratio <= 30  ; ratio+=0.5){
    //     cout <<"RATIO : " << ratio << " - ";
    srand((unsigned) time(0)); 
    bool foundInitialSolution=false;
    while(!foundInitialSolution){
        foundInitialSolution = quickSolution.create();    
       
    } 
    //cout << "FOUND FIRST SOLUTION " << endl;
     
    int amountWon = 0; 
    for(auto&& circ : quickSolution.getSolution().circumscriptions){
    if(circ->totalVotes >= votesToWin)
        amountWon++;
    }
    if(print)
        displaySolution(quickSolution.getSolution());
    //cout << "WON : " << amountWon << endl;

    int lastBest = amountWon;
    bool solutionChanged = true;
    int currentAmountWon = 0;
    while(solutionChanged){
        solutionChanged = localSearch.upgradeSolution();
        currentAmountWon = 0;
        for(auto&& circ : localSearch.getSolution().circumscriptions){
            if(circ->totalVotes >= votesToWin)
                currentAmountWon++;
        }
        if(currentAmountWon>lastBest){
            lastBest  = currentAmountWon;  
            //cout<<"Number of circs won "<<currentAmountWon<<" of "<<nbCircumscription<<endl;
            if(print)
                displaySolution(localSearch.getSolution());
        }        
    }    

    return 0;
}
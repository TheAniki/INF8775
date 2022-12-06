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
    SingleSolution singleSolution = SingleSolution(municipalities,nbCircumscription);
    QuickSolution quickSolution = QuickSolution(singleSolution);
    LocalSearch localSearch = LocalSearch(singleSolution);
    worked = quickSolution.create();
    cout<<" worked? "<<worked<<endl; 

    int i=0;
    for(auto&& circ : localSearch.getSolution().circumscriptions){
        if(circ->totalVotes >= votesToWin)
            i++;
    }
    cout<<"Number of circs won "<<i<<" of "<<nbCircumscription<<endl;
    displaySolution(quickSolution.getSolution());

    i=0;
    int lastBest = 0;
    while(i<13){
        i=0;
        localSearch.upgradeSolution(10);
        
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










    // bool worked = false;
    // // test:
    // // while(!worked){
    // // cout<<"in local: "<<endl;
    // // displaySolution(localSearch.getSolution());
    // // }
    //  srand((unsigned) time(0)); 

    // SingleSolution singleSolution = SingleSolution(municipalities,nbCircumscription);
    // QuickSolution quickSolution = QuickSolution(singleSolution);
    // LocalSearch localSearch = LocalSearch(singleSolution);
        
    // // quickSolution.getSolution();
    // // worked = quickSolution.create(100);    
    // // quickSolution.getSolution();
    // // cout << "MARCHÉ ? .... " << worked << endl;
    // //   cout<<"--------------------------"<<endl;









    // // for(double ratio = 20 ; ratio <= 30  ; ratio+=0.5){
    // //     cout <<"RATIO : " << ratio << " - ";
    //     int counter =0;
    //     for(int i = 0 ; i < 500 ; i++){
    //         SingleSolution singleSolution = SingleSolution(municipalities,nbCircumscription);
    //         QuickSolution quickSolution = QuickSolution(singleSolution);
    //         worked = quickSolution.create(1);    
    //         if(worked)     counter++;
    //     } 
    //     cout << "WORKED : " << counter << "/500" <<endl;
    //     cout << endl;
    // // }



    return 0;
}
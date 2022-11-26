#include "./headers/Includes.h"
#include "./headers/algo.h"

void algo(){
    cout<<"Bonjour je suis un algo qui marche!"<<endl;
    
}

Solution quickSolution(vector<vector<shared_ptr<Municipality>>> municipalities, int nbCircumscription ){
    Solution solution = initializeSolution(nbCircumscription);

    vector<vector<bool>> assignedMunicipalities = createAssignedMun(municipalities);
    int currentCircIndex = 0;
    int currentCircVotes = 0;
    int counter = 0;

    int minCirc = 0;
    int maxCirc = 0;
    int nbMunicipalities = municipalities.size()*municipalities[0].size();
    int votesToWin = ((50*(nbMunicipalities))/nbCircumscription)+1;

    computeCircBounds(nbMunicipalities, nbCircumscription, minCirc, maxCirc);
    cout << "MIN CIRC : " << minCirc << endl;
    cout << "MAX CIRC : " << maxCirc << endl;

    for(long unsigned int i = 0 ; i < municipalities.size(); i++){
        for(long unsigned int j= 0 ; j < municipalities[i].size(); j++){
            if(assignedMunicipalities[i][j]) continue;
            if(solution.circumscriptions[currentCircIndex]->municipalities.size()>=maxCirc){
                if(currentCircVotes >= votesToWin){
                    solution.circumscriptions[currentCircIndex]->isWon = true;
                }
                cout << "CIRC  " << currentCircIndex  << " WINS..... ? : " << solution.circumscriptions[currentCircIndex]->isWon<< endl << endl;
                currentCircIndex++;
                currentCircVotes = 0;
                counter = 0;

            }
        

                cout << "i, j : " << i << " , " << j  << "    ";
    
                assignedMunicipalities[i][j] = true;
                addMunicipalityToCirc(solution.circumscriptions[currentCircIndex], municipalities[i][j]);
                currentCircVotes+=municipalities[i][j]->nbVotes;


                cout << "MUNICIPALITY IN CIRC  " << currentCircIndex  << " : " << solution.circumscriptions[currentCircIndex]->municipalities[counter]->nbVotes<< endl;
                // ->municipalities.push_back(municipalities[i][j]);
                counter++;



        }
   
    }
     if(currentCircVotes >= votesToWin){
                    solution.circumscriptions[currentCircIndex]->isWon = true;
                }
                cout << "CIRC  " << currentCircIndex  << " WINS..... ? : " << solution.circumscriptions[currentCircIndex]->isWon<< endl << endl;
                currentCircIndex++;
                currentCircVotes = 0;
                counter = 0;

    return solution;
}


void addMunicipalityToCirc(shared_ptr<Circumscription> circumscription, shared_ptr<Municipality> municipality ){
    circumscription->municipalities.push_back(municipality);
}

void computeCircBounds(int nbMunicipalities, int nbCircumscription, int& minCirc, int& maxCirc){
    float fraction = float(nbMunicipalities) /nbCircumscription;
    minCirc = floor(fraction);
    maxCirc = ceil(fraction);
}

//Generates a matrix that tells if a municipality is assigned to a 
vector<vector<bool>> createAssignedMun(vector<vector<shared_ptr<Municipality>>> municipalities){
    vector<vector<bool>> assignedMunicipalities(municipalities.size());

    for(long unsigned int i = 0 ; i < municipalities.size(); i++){
        for(long unsigned int j= 0 ; j < municipalities[i].size(); j++){
            assignedMunicipalities[i].push_back(false);
        }
    }

    return assignedMunicipalities;
}


Solution initializeSolution(int nbCircumscription){

    Solution initialSolution;
    initialSolution.circumscriptions = vector<shared_ptr<Circumscription>>(nbCircumscription);
        for(int i = 0 ; i < nbCircumscription ; i++ ){
            vector<shared_ptr<Municipality>> emptyMunicipality;
            initialSolution.nbCircWon = 0;
            initialSolution.circumscriptions[i] = make_shared<Circumscription>(false, emptyMunicipality  ); ;
       }

       return initialSolution;
}

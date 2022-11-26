#include "./headers/Includes.h"
#include "./headers/algo.h"

void algo(){
    cout<<"Bonjour je suis un algo qui marche!"<<endl;
    
}

Solution quickSolution(vector<vector<shared_ptr<Municipality>>> municipalities, int nbCircumscription ){
    Solution solution = initializeSolution(nbCircumscription);

    vector<vector<bool>> assignedMunicipalities = createAssignedMun(municipalities);
    int currentCircIndex = 0;
    int counter = 0;

    for(long unsigned int i = 0 ; i < municipalities.size(); i++){
        for(long unsigned int j= 0 ; j < municipalities[i].size(); j++){
            if(assignedMunicipalities[i][j]) continue;
            

                cout << "i, j : " << i << " , " << j  << "    ";
    
                assignedMunicipalities[i][j] = true;
                addMunicipalityToCirc(solution.circumscriptions[currentCircIndex], municipalities[i][j] );
                cout << "MUNICIPALITY IN CIRC 0  -- ROW:  " << solution.circumscriptions[currentCircIndex]->municipalities[counter]->nbVotes<< endl;
                // ->municipalities.push_back(municipalities[i][j]);
                counter++;



        }
        cout << endl;
    }
    return solution;
}

void addMunicipalityToCirc(shared_ptr<Circumscription> circumscription, shared_ptr<Municipality> municipality ){

    circumscription->municipalities.push_back(municipality);
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

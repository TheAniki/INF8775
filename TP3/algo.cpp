#include "./headers/Includes.h"
#include "./headers/algo.h"
#include <queue>

void algo(){
    cout<<"Bonjour je suis un algo qui marche!"<<endl;
    
}

Solution quickSolution(vector<vector<shared_ptr<Municipality>>> municipalities, int nbCircumscription ){
    Solution solution = initializeSolution(nbCircumscription);
    vector<vector<bool>> assignedMunicipalities = createAssignedMun(municipalities);

    // constant parameters... -> TODO : put as attribute of a class ? ...
    const int nbMunicipalities = municipalities.size()*municipalities[0].size(); //n
    const int votesToWin = ((50*(nbMunicipalities))/nbCircumscription)+1; 
    const int maxDist = ceil(float(nbMunicipalities)/(2*nbCircumscription)); //   ceil(n/2m). m = nbCircumscription 

    
    int minCirc = 0; //k_min
    int maxCirc = 0; //k_max
    computeCircBounds(nbMunicipalities, nbCircumscription, minCirc, maxCirc);

    queue<shared_ptr<Municipality>> unassignedMunicipality;

    // queue<shared_ptr<Circumscription>> incompleteCircumscriptions(deque<shared_ptr<Circumscription>>(v.begin(),v.end()));

    for(long unsigned int i = 0 ; i < municipalities.size(); i++){
        for(long unsigned int j= 0 ; j < municipalities[i].size(); j++){
            if(assignedMunicipalities[i][j]) continue;
            
       
            for(auto&& circumscription : solution.circumscriptions){
                if(circumscription->municipalities.size()>=maxCirc) continue; //no more space in circumscription

                if(validateMunFitsInCirc(circumscription, municipalities[i][j], maxDist ) ){
                    addMunicipalityToCirc(circumscription, municipalities[i][j]);
                    assignedMunicipalities[i][j] = true; 

                    circumscription->totalVotes+=municipalities[i][j]->nbVotes;
                    if(circumscription->totalVotes >= votesToWin){
                        circumscription->isWon = true;
                    }
                    break;
                }
            }  
            if(!assignedMunicipalities[i][j]){
                //Was impossible to add municipality to a circumscription 
                unassignedMunicipality.push(municipalities[i][j]);
                cout << "unassignedMunicipality : " << municipalities[i][j]->coordinates.row<<"   ,   "<<municipalities[i][j]->coordinates.column << endl;
            }
        }
   
    }

    int i = 0 ;
    vector<shared_ptr<Circumscription>> incompleteCircs;
    for(auto&& circ : solution.circumscriptions){
        if(circ->municipalities.size() <minCirc){
            incompleteCircs.push_back(circ);
            cout << "INCOMPLETE CIRC : " << i << endl;
        }
        i++;
    }
    


    cout << "MAX DISTS : " << maxDist<< endl;
    cout << "MIN CIRC : " << minCirc << endl;
    cout << "MAX CIRC : " << maxCirc << endl;
    cout << "NB CIRC : "<< nbCircumscription << endl;
    

    return solution;
}




int computeManhattanDist(const Coord& coord1, const Coord& coord2){
    return abs(coord2.row - coord1.row) + abs(coord2.column- coord1.column );
}

bool validateMunFitsInCirc(shared_ptr<Circumscription> circumscription, shared_ptr<Municipality> municipalityToValidate, int maxDist){
    for(auto&& municipality : circumscription->municipalities){
        if(computeManhattanDist(municipalityToValidate->coordinates, municipality->coordinates) > maxDist){
            return false;
        } 
    }
    return true;
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
// Necessary, because there will be multiple solutions sharing the same municipalities
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
            initialSolution.circumscriptions[i] = make_shared<Circumscription>(false,  0, emptyMunicipality); ;
       }

       return initialSolution;
}

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
    int votesToWin = ((50*(nbMunicipalities))/nbCircumscription)+1; 
    const int maxDist = ceil(float(nbMunicipalities)/(2*nbCircumscription)); //  ceil(n/2m). m = nbCircumscription 

    int minCirc = 0; //k_min
    int maxCirc = 0; //k_max


    
    
    computeCircBounds(nbMunicipalities, nbCircumscription, minCirc, maxCirc);
    cout << "MAX DISTS : " << maxDist<< endl;
    cout << "MIN CIRC : " << minCirc << endl;
    cout << "MAX CIRC : " << maxCirc << endl;
    cout << "NB CIRC : "<< nbCircumscription << endl;

    queue<shared_ptr<Municipality>> unassignedMunicipality;


    // Loops over all the municipalities to assign them to a circumscription 
    for(long unsigned int i = 0 ; i < municipalities.size(); i++){
        for(long unsigned int j= 0 ; j < municipalities[i].size(); j++){
            if(assignedMunicipalities[i][j]) continue;

            assignedMunicipalities[i][j] = addMunicipalityToFirstAvailableCirc(municipalities[i][j], solution, maxDist, votesToWin,maxCirc); //Returns true if successfully added


            //Was impossible to add municipality to a circumscription 
            if(!assignedMunicipalities[i][j]){
                unassignedMunicipality.push(municipalities[i][j]);
                cout << "unassignedMunicipality : " << municipalities[i][j]->coordinates.row<<"   ,   "<<municipalities[i][j]->coordinates.column << endl;
            }
        }
   
    }

    //Creating incomplete circ vect
    vector<shared_ptr<Circumscription>> incompleteCircs = findIncompleteCircs(solution.circumscriptions, minCirc);
    
    if(unassignedMunicipality.empty()){
        cout << "EMPTY QUEUE " << endl;
    }

    //Trying to repare solution
    // while(!unassignedMunicipality.empty()){
        vector<shared_ptr<Circumscription>> possibleCircumscriptions = findPossibleCircumscriptionsToContainMun(unassignedMunicipality.front(),  solution.circumscriptions, maxDist);

        //chose municipality to remove from a possible circ 
        if(possibleCircumscriptions.size()==0) return solution; // FAILED

     
        shared_ptr<Municipality> municipalityToRemove =  choseMunicipalityToRemoveFromCirc( possibleCircumscriptions[0], incompleteCircs[0], nbMunicipalities,  maxCirc);

        removeMunicipalityFromCirc(municipalityToRemove,  possibleCircumscriptions[0]);

        addMunicipalityToCirc(possibleCircumscriptions[0],unassignedMunicipality.front() );
        addMunicipalityToCirc(incompleteCircs[0],municipalityToRemove );
       
       
        // NOT GOOD----------------------
        // add  possibleCircumscriptions[0]->municipalities unassignedMunicipality.front()

        // remove_if(possibleCircumscriptions[0]->municipalities.begin(), possibleCircumscriptions[0]->municipalities.end(), []()->bool{


        // })
        // for(auto&& possibleMunicipality : possibleCircumscriptions[0]->municipalities){
        //     if(possibleMunicipality->coordinates.row == municipalityToRemove )
        //     swap(possibleCircumscriptions[0]->municipalities[municipalityToRemove->coordinates.row][municipalityToRemove->coordinates.column]



        // municipalityToRemove->coordinates
        // vector<shared_ptr<Municipality>> ints = possibleCircumscriptions[0]->municipalities;
        // ints.erase(std::remove_if(begin(ints), end(ints), [](int i) { municipalityToRemove->  ints[i]->    }), end(ints));
        // ------------------
    


    cout << "MAX DISTS : " << maxDist<< endl;
    cout << "MIN CIRC : " << minCirc << endl;
    cout << "MAX CIRC : " << maxCirc << endl;
    cout << "NB CIRC : "<< nbCircumscription << endl;
    

    return solution;
}

void removeMunicipalityFromCirc(shared_ptr<Municipality> municipalityToRemove, shared_ptr<Circumscription> circumscription){
    //TODO : there are more efficient algorithms to remove an element from a vector
   int position =0;
    for(auto&& possibleMunicipality : circumscription->municipalities){
        if(possibleMunicipality->coordinates.row == municipalityToRemove->coordinates.row && possibleMunicipality->coordinates.column == municipalityToRemove->coordinates.column){
            circumscription->municipalities.erase(circumscription->municipalities.begin()+ position);
            break;
        }
        position++;
    }

}



// Choses a municipality to remove from a circ based on its total distance to an incomplete circ passed in parameter
shared_ptr<Municipality> choseMunicipalityToRemoveFromCirc( shared_ptr<Circumscription> circumscriptionToRemoveIn, shared_ptr<Circumscription> incompleteCirc ,int nbMunicipalities, int maxCirc){
    int smallestDistToIncompleteCirc = maxCirc*nbMunicipalities; // INFINITY
    shared_ptr<Municipality> municipalityToRemove;

    for(auto&& possibleMunicipalityToRemove :  circumscriptionToRemoveIn->municipalities){
        int totalDistToIncompleteCirc = 0;
        for(auto&& municipalityFromIncompleteCirc :  incompleteCirc->municipalities){
            totalDistToIncompleteCirc += computeManhattanDist(possibleMunicipalityToRemove->coordinates, municipalityFromIncompleteCirc->coordinates);
        }
        if(totalDistToIncompleteCirc < smallestDistToIncompleteCirc){
            smallestDistToIncompleteCirc = totalDistToIncompleteCirc;
            municipalityToRemove = possibleMunicipalityToRemove;
        }
    }
    return municipalityToRemove;

}




vector<shared_ptr<Circumscription>> findPossibleCircumscriptionsToContainMun(shared_ptr<Municipality> municipalityToInclude, vector<shared_ptr<Circumscription>> circumscriptionsConsidered, int maxDist){
    vector<shared_ptr<Circumscription>> possibleCircumscriptions;
    
    int i = 0 ; //TODO : remove
    // find possible solutions
    for(auto&& circ : circumscriptionsConsidered){
        if(validateMunFitsInCirc(circ, municipalityToInclude, maxDist)){
            possibleCircumscriptions.push_back(circ);
            cout<<"POSSIBLE CIRC FOR MUNICIPALITY : " << i << endl;
        }
        i++;
    }

    return possibleCircumscriptions;

}



vector<shared_ptr<Circumscription>> findIncompleteCircs(vector<shared_ptr<Circumscription>> circumscriptions, int minCirc ){
    vector<shared_ptr<Circumscription>> incompleteCircs;
    for(auto&& circ : circumscriptions){
        if(circ->municipalities.size() <minCirc){
            incompleteCircs.push_back(circ);
        }
    }
    return incompleteCircs;

}



bool addMunicipalityToFirstAvailableCirc(shared_ptr<Municipality> municipality, 
        Solution& solution, 
        int maxDist, 
        int votesToWin,
        int maxCirc ){
    for(auto&& circumscription : solution.circumscriptions){
        if(circumscription->municipalities.size()>=maxCirc) continue; //no more space in circumscription

            if(validateMunFitsInCirc(circumscription, municipality, maxDist ) ){
            addMunicipalityToCirc(circumscription, municipality); 

            if(circumscription->totalVotes >= votesToWin){
                circumscription->isWon = true;
               solution.nbCircWon++;

            }
            return true; //successfully added 
        }
    }
    return false;

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
    circumscription->totalVotes+=municipality->nbVotes;

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

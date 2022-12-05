#include "./headers/quickSolution.h"
#include "./headers/Includes.h"
#include "./headers/utils.h"


QuickSolution::QuickSolution(/* args */)
{
}

QuickSolution::QuickSolution(SingleSolution singleSolution)
: SingleSolution::SingleSolution(singleSolution){

}

QuickSolution::~QuickSolution()
{
}

Solution QuickSolution::getSolution(){
    return this->_solution;
}

// Create the initial solution.
bool QuickSolution::create(){      
    cout << "MAX DISTS : " << this->_maxDist<< endl;
    cout << "MIN CIRC : " << this->_minCirc.circSize << endl;
 
    cout << "MAX CIRC : " << this->_maxCirc.circSize << endl;

    cout << "NB CIRC : "<< this->_nbCircumscriptions << endl;
    // Loops over all the municipalities to assign them to a circumscription     
    for(long unsigned int i = 0 ; i < this->_municipalities.size(); i++){
        for(long unsigned int j= 0 ; j < this->_municipalities[i].size(); j++){

            // TODO : Replace with this
            // bool added = addMunicipalityWithProbaHeur(i,j); // Returns true if successfully added

            bool added = false;
            // TODO: OLD FUNCTION -> adds in order.        
            for(auto&& circumscription : this->_solution.circumscriptions){
                if((int) circumscription->municipalities.size()>=this->_maxCirc.circSize) continue; //no more space in circumscription

                if(validateMunFitsInCirc(circumscription, this->_municipalities[i][j]) ){
                
                    addMunicipalityToCirc(circumscription, this->_municipalities[i][j]); 
                    if(circumscription->totalVotes >= this->_votesToWin){
                        circumscription->isWon = true;
                        this->_solution.nbCircWon++;
                    }
                    added  =  true; //successfully added 
                    break;
                    
                }
                
                // Was impossible to add municipality to a circumscription 
            }

            if(!added){
                vector<Coord> emptyHistory;
                bool isForceable = this->forceAddMunicipality(this->_municipalities[i][j], emptyHistory);
                cout << "IS FORCEABLE ? " << isForceable << endl;
                if(!isForceable) {
                    cout << "AMOUNT OF INCOMPLETES :  " << findIncompleteCircs(this->_solution.circumscriptions).size() << endl;
                    return false;
                }
            }
        }
       
    }           
    
    return true;
}

bool QuickSolution::forceAddMunicipality(shared_ptr<Municipality> municipalityToForce, vector<Coord> historyOfForcedMun){
    historyOfForcedMun.push_back(municipalityToForce->coordinates);
    if(historyOfForcedMun.size() > 5) return false;

    cout << "**problematic mun " << municipalityToForce->coordinates.row << " , " << municipalityToForce->coordinates.column<< endl;
    
    map<int, shared_ptr<Circumscription>> incompleteCircs = findIncompleteCircs(this->_solution.circumscriptions);
    // Finds the less problematic circ to force solution in
    map<int, shared_ptr<Circumscription>> neighborCircs =  findNeighbourCircumscriptions(municipalityToForce->coordinates);
    shared_ptr<Circumscription> bestCircumscriptionToBreak;
    shared_ptr<Municipality> bestMunicipalityToRemove;
    vector<shared_ptr<Municipality>> bestTooFarMunicipalitiesToRemove;
    int totalDistanceToIncompleteCircOfBestMunToRemove = 100000;
    
   for(auto&& neighborCirc : neighborCircs){
        //If neighborCirc is an incomplete circ -> if possible, municipalityToForce is added here
        if((int) neighborCirc.second->municipalities.size() < this->_maxCirc.circSize){ 
            //If possible to add in this incomplete circ
            if(validateMunFitsInCirc(neighborCirc.second, municipalityToForce)){
                addMunicipalityToCirc(neighborCirc.second, municipalityToForce);
                return true;
            }
            //No fit => at least one municipality too far from municipalityToForce
            vector<shared_ptr<Municipality>> tooFarMuns;
            for(auto&& municipality : neighborCirc.second->municipalities){
                if(computeManhattanDist(municipality->coordinates, municipalityToForce->coordinates) > this->_maxDist){
                    tooFarMuns.push_back(municipality);
                    if(isMunInVector(municipality, historyOfForcedMun)){ //If mun too far and in history, then it's impossible to put municipalityToPush in that circ
                        tooFarMuns.clear(); // clearing vector -> size = 0
                        break;
                    }
                }
                if(tooFarMuns.size() > 1) break; //The algorithms kicks out at most one municipality 
            }
            
            //If and only if 1 tooFarMun, removes it , add municipalityToForce, calls forceAddMunicipality with the removed mun
            if(tooFarMuns.size() == 1){
                removeMunicipalityFromCirc(tooFarMuns[0], neighborCirc.second);
                addMunicipalityToCirc(neighborCirc.second, municipalityToForce );
                return forceAddMunicipality(tooFarMuns[0], historyOfForcedMun);
            }
        }

        //neighborCirc is complete
        else{
            vector<shared_ptr<Municipality>> tooFarMuns;
            int worseDistanceInTooFarMuns = 0;
            for(auto&& candidateMunicipality : neighborCirc.second->municipalities){     
                if(computeManhattanDist(municipalityToForce->coordinates, candidateMunicipality->coordinates) > this->_maxDist){ //Has to be removed for municipalityToForce because tooFar
                    if(isMunInVector(candidateMunicipality, historyOfForcedMun)){ //If mun too far and in history, then it's impossible to put municipalityToPush in that circ
                        tooFarMuns.clear(); // clearing vector -> size = 0
                        break;
                    }
                    // tooFarMuns.push_back(candidateMunicipality);
                    // int munSmallestDistToAnIncompleteCirc = findSmallestTotalDistanceToAnIncomplete(candidateMunicipality, incompleteCircs);
                    // if(munSmallestDistToAnIncompleteCirc >worseDistanceInTooFarMuns ){
                    //     worseDistanceInTooFarMuns = munSmallestDistToAnIncompleteCirc;
                    // }


                    // USE THIS CODE TO REMOVE MAX ONE MUN
                    if(tooFarMuns.size() > 1 ) {
                        break ; // TODO : deal with the case where the other tooFar was the bestMun
                    };   
                    int munSmallestDistToAnIncompleteCirc2 = findSmallestTotalDistanceToAnIncomplete(candidateMunicipality, incompleteCircs);
   
                    if(munSmallestDistToAnIncompleteCirc2 < totalDistanceToIncompleteCircOfBestMunToRemove){ // Adjust bestToRemove
                        totalDistanceToIncompleteCircOfBestMunToRemove = munSmallestDistToAnIncompleteCirc2;
                        bestMunicipalityToRemove = candidateMunicipality;
                        bestCircumscriptionToBreak = neighborCirc.second;
                    }

                }

                else if(tooFarMuns.size() >=1 ) {
                    continue; //We will remove the tooFarMuns regardless of the others
                }
                else{//No tooFar yet detected
                    int munSmallestDistToAnIncompleteCirc = findSmallestTotalDistanceToAnIncomplete(candidateMunicipality, incompleteCircs);
                    if(munSmallestDistToAnIncompleteCirc < totalDistanceToIncompleteCircOfBestMunToRemove){ // Adjust bestToRemove
                        // bestTooFarMunicipalitiesToRemove.clear(); // The best option is not a vector of tooFarMuns
                        totalDistanceToIncompleteCircOfBestMunToRemove = munSmallestDistToAnIncompleteCirc;
                        bestMunicipalityToRemove = candidateMunicipality;
                        bestCircumscriptionToBreak = neighborCirc.second;
                    }
                }
            }

            // The tooFarMuns of this circ are the best option from all neighbors
            if(tooFarMuns.size()>0 && worseDistanceInTooFarMuns < totalDistanceToIncompleteCircOfBestMunToRemove){ 
                bestTooFarMunicipalitiesToRemove = tooFarMuns;
                bestMunicipalityToRemove = nullptr;
                bestCircumscriptionToBreak = neighborCirc.second;
            }
        }

    }

    // if(bestTooFarMunicipalitiesToRemove.size() >0){
    //     bool allForceAddWorked = true;
    //     for(auto&& municipalityToRemove : bestTooFarMunicipalitiesToRemove){
    //         removeMunicipalityFromCirc(municipalityToRemove, bestCircumscriptionToBreak);
    //         addMunicipalityToCirc( bestCircumscriptionToBreak, municipalityToForce);
    //         allForceAddWorked = allForceAddWorked && forceAddMunicipality(municipalityToRemove, historyOfForcedMun);
    //     }
    //     return allForceAddWorked;
    // }

    removeMunicipalityFromCirc(bestMunicipalityToRemove, bestCircumscriptionToBreak);
    addMunicipalityToCirc( bestCircumscriptionToBreak, municipalityToForce);
    return forceAddMunicipality(bestMunicipalityToRemove, historyOfForcedMun);
        
    return false;

}

int QuickSolution::findSmallestTotalDistanceToAnIncomplete(shared_ptr<Municipality> municipalityToCheck , map<int, shared_ptr<Circumscription>> incompleteCircs){
    int munSmallestDistToAnIncompleteCirc = 1000000;
    for(auto const& pair : incompleteCircs){
        int totalDistance = computeTotalDistanceToCirc(municipalityToCheck, pair.second);
        if(totalDistance < munSmallestDistToAnIncompleteCirc){
            munSmallestDistToAnIncompleteCirc = totalDistance;
        }   
    }
    return munSmallestDistToAnIncompleteCirc;
}

// Add municipality to corcumscription using proba heur.
bool QuickSolution::addMunicipalityWithProbaHeur(int i, int j){
    double nbEmpty = 0.0;
    
    vector<pair<SharedCirc, double>> circsInRange = findCircsInRange(i,j);
    
    if(circsInRange.size() == 0){
        for(auto&& circumscription : this->_solution.circumscriptions){
            if(circumscription->municipalities.size()==0){
                addMunicipalityToCirc(circumscription, this->_municipalities[i][j]);
                return true;
            }
        } 
        return false;       
    }  
    
    if(circsInRange.size() == 1){// if it can only fit in one:
        // can be added at random to a new circ or to the circ in range. 
        double randomNumber = double(rand() & 10000)/10000;
        double treshold = 0.4*(double(nbEmpty)/this->_solution.circumscriptions.size()); // x 40%
        if(randomNumber >= treshold)
            addMunicipalityToCirc(circsInRange[0].first, this->_municipalities[i][j]);
        else{
            for(auto&& circumscription : this->_solution.circumscriptions){
                if(circumscription->municipalities.size()==0){
                    addMunicipalityToCirc(circumscription, this->_municipalities[i][j]);
                    return true;
                }
            }
            return false;
        }

        return true;              
    }
 
    // chose circumscription.
    SharedCirc circChosen = choseCircumscriptionFrom(circsInRange,i,j);    

    return addMunicipalityToChosenCirc(circChosen,i,j);    
}

// Add mun to chosen circ in solution.
bool QuickSolution::addMunicipalityToChosenCirc(SharedCirc circChosen,int i,int j){
    for(auto&& circumscription : this->_solution.circumscriptions){
        if((int) circumscription->municipalities.size()>=this->_minCirc.circSize) continue;
        if(circChosen->circumscriptionNumber != circumscription->circumscriptionNumber) continue;       
        addMunicipalityToCirc(circumscription, this->_municipalities[i][j]); 
        
        return true; //successfully added 
    }        
      
    return false;
}

// Find all circs in manhattan range of i,j and that can be fitted 
vector<pair<SharedCirc, double>> QuickSolution::findCircsInRange(int i, int j){
    vector<pair<SharedCirc, double>> circsInRange;
    double nbEmpty = 0.0;
    for(auto&& circumscription : this->_solution.circumscriptions){
        if(circumscription->municipalities.size()==0) nbEmpty++;
        if(circumscription->municipalities.size()>0 && validateMunFitsInCirc(circumscription, this->_municipalities[i][j])){
            circsInRange.push_back(make_pair(circumscription,0.0));
        }
    }

    return circsInRange;
}

// Calculate the distance of each circumscription from the number of vote requierd to win the circumscription.
vector<int> QuickSolution::calculateDistance(vector<pair<SharedCirc, double>> circsInRange,int i, int j){
    vector<int> distanceCirc;
    //Calculate distance from winning for each circs.    
    for(auto&& circ : circsInRange){

        //calculate num of vote required to win circ.
        int nbVotesToWin = (circ.first->municipalities.size()+1)*50+1;   

        //calculate absolute distance between circ.totalVote + mun.votes and the number required to win.
        distanceCirc.push_back(abs(circ.first->totalVotes + this->_municipalities[i][j]->nbVotes - nbVotesToWin));   
    }
    return distanceCirc;
}

// Chose a circumscription From circumscriptions in range.
SharedCirc QuickSolution::choseCircumscriptionFrom(vector<pair<SharedCirc, double>> circsInRange,int i, int j){
    
    vector<int> distanceCirc = calculateDistance(circsInRange,i,j);
    int totalDistance = reduce(distanceCirc.begin(),distanceCirc.end()); 
    
    SharedCirc circChosen;    

    // Calculate probability for each circs
    double totalWeight=0.0;
    int it =0;
    for(auto&& circ : circsInRange){ 
        circ.second = double(distanceCirc[it])/totalDistance;
        totalWeight += double(1)/circ.second;
        it++;
    }

    // Invert probabilities.
    for(auto&& circ : circsInRange){
        if(circ.first->municipalities.size() == 0) continue; 
        circ.second = (double(1)/circ.second)/totalWeight;
    }

    //Chose circ to add municipality:
    double previousTreshold = 0.0;
    double currentTreshold = 0.0;
    double randomNumber = double(rand() & 10000)/10000; // number between 0.0000 and 1.0000 
    
    for(auto&& circ : circsInRange){               
        currentTreshold = previousTreshold + circ.second;
        if(randomNumber >= previousTreshold && randomNumber <currentTreshold){
           circChosen = circ.first; 
           break;         
        }
        previousTreshold = currentTreshold;
    }

    // If none is assign, assign last circs in range.
    if(circChosen == SharedCirc()){
        circChosen = circsInRange[circsInRange.size()-1].first;
    }

    return circChosen;
}

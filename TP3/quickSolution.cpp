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
    cout << "MIN CIRC maxAmount: " <<this->_minCirc.maxAmount << endl;
    cout << "MAX CIRC : " << this->_maxCirc.circSize << endl;
    cout << "MAX CIRC maxAmount: "<< this->_maxCirc.maxAmount << endl;
    cout << "NB CIRC : "<< this->_nbCircumscriptions << endl;
    // Loops over all the municipalities to assign them to a circumscription  

    for(long unsigned int i = 0 ; i < this->_municipalities.size(); i++){
        for(long unsigned int j= 0 ; j < this->_municipalities[i].size(); j++){

            bool added = addMunicipalityWithProbaHeur(i,j); // Returns true if successfully added
            // Was impossible to add municipality to a circumscription 
            if(!added){
                this->_unassignedMunicipalities.push(this->_municipalities[i][j]);
                //cout << " UnassignedMunicipality : " << this->_municipalities[i][j]->coordinates.row<<" , "
                //<<this->_municipalities[i][j]->coordinates.column << endl;

                vector<Coord> emptyHistory;
             
                bool isForceable = this->forceAddMunicipality(this->_municipalities[i][j], emptyHistory);
                //cout << "IS FORCEABLE ? " << isForceable << endl;
                if(!isForceable) return false;
            }
        }            
    }           
    
    return true;
}

bool QuickSolution::forceAddMunicipality(shared_ptr<Municipality> municipalityToForce, vector<Coord> historyOfForcedMun){
    historyOfForcedMun.push_back(municipalityToForce->coordinates);
    if(historyOfForcedMun.size() > 5) return false;

    //cout << "**problematic mun " << municipalityToForce->coordinates.row << " , " << municipalityToForce->coordinates.column<< endl;
    
    map<int, shared_ptr<Circumscription>> incompleteCircs = findIncompleteCircs(this->_solution.circumscriptions);
    // Finds the less problematic circ to force solution in
    map<int, shared_ptr<Circumscription>> neighborCircs =  findNeighbourCircumscriptions(municipalityToForce->coordinates);
    shared_ptr<Circumscription> bestCircumscriptionToBreak;
    shared_ptr<Municipality> bestMunicipalityToRemove;
    int totalDistanceToIncompleteCircOfBestMunToRemove = 100000;

    

   for(auto&& neighborCirc : neighborCircs){
        //If we neighbour an incomplete circ
        if((int) neighborCirc.second->municipalities.size() < this->_maxCirc.circSize){ 
           // cout << "A NEIGHBOUR IS AN INCOMPLETE CIRC --------- " << neighborCirc.second->circumscriptionNumber << endl;
            if(validateMunFitsInCirc(neighborCirc.second, municipalityToForce)){
               // cout << "WE FIT IN INCOMPLETE :) :) :) :) " << endl;
                addMunicipalityToCirc(neighborCirc.second, municipalityToForce);
                return true;
            }

            vector<shared_ptr<Municipality>> tooFarMuns;
            for(auto&& municipality : neighborCirc.second->municipalities){
                if(isMunInVector(municipality, historyOfForcedMun))continue;
                if(computeManhattanDist(municipality->coordinates, municipalityToForce->coordinates) > this->_maxDist){
                    tooFarMuns.push_back( municipality);
                }
                if(tooFarMuns.size() > 1) break;
            }
            // Cause a seg fault here when size of too Far muns is over 1.
            if(tooFarMuns.size() == 1){
                // cout<<"maybe segfault? "<<tooFarMuns[0]->nbVotes<<endl;
                // cout << "AMOUNT OF TOO FAR "<< tooFarMuns.size() <<endl;
                removeMunicipalityFromCirc(tooFarMuns[0], neighborCirc.second);
                addMunicipalityToCirc(neighborCirc.second, municipalityToForce );
                // displaySolution(this->_solution);
                return forceAddMunicipality(tooFarMuns[0], historyOfForcedMun);
            }
        }
        else{
        //cout << "NOT STOPPED IN NEIGHBOUR INCOMPLETE "<< endl;

        //If we don't neighbour an incomplete circ
        int amountOfMunTooFarFromMunicipalityToForce = 0;
        for(auto&& municipality : neighborCirc.second->municipalities){     
            if(isMunInVector(municipality, historyOfForcedMun))continue;
            if(computeManhattanDist(municipalityToForce->coordinates, municipality->coordinates) > this->_maxDist){ //Has to be removed for municipalityToForce because tooFar
                //cout<< " ................. Presence of too far.... " << municipality->coordinates.row << " , " << municipality->coordinates.column << endl;
                //We deal with at tooFar
                if(++amountOfMunTooFarFromMunicipalityToForce > 1 ) {
                    continue ; // TODO : deal with the case where the other tooFar was the bestMun
                };   
                int munSmallestDistToAnIncompleteCirc = 100000;
                for(auto const& pair : incompleteCircs){
                    //cout << ".............................. munSmallestDistToAnIncompleteCirc : " << munSmallestDistToAnIncompleteCirc << endl ;
                    int totalDistance = computeTotalDistanceToCirc(municipality, pair.second);
                    if(totalDistance < munSmallestDistToAnIncompleteCirc){
                        munSmallestDistToAnIncompleteCirc = totalDistance;
                    }   
                }
                if(munSmallestDistToAnIncompleteCirc < totalDistanceToIncompleteCircOfBestMunToRemove){ // Adjust bestToRemove
                    totalDistanceToIncompleteCircOfBestMunToRemove = munSmallestDistToAnIncompleteCirc;
                    bestMunicipalityToRemove = municipality;
                    bestCircumscriptionToBreak = neighborCirc.second;

                    // cout << "...............................................new totalDistanceToIncompleteCircOfBestMunToRemove " << totalDistanceToIncompleteCircOfBestMunToRemove <<endl;
                    // cout << "...............................................new bestMunicipalityToRemove " << bestMunicipalityToRemove->nbVotes <<endl;
                    // cout << "...............................................new bestCircumscriptionToBreak " << bestCircumscriptionToBreak->circumscriptionNumber <<endl;
                    

                }
            }
            else if(amountOfMunTooFarFromMunicipalityToForce >=1 ) {
                //cout << " ................ TOO MANY TO FAR  ...." << endl;
                continue; //We will remove the munTooFar regardless of the others
            }
            else{//No tooFar yet detected
                int munSmallestDistToAnIncompleteCirc = 1000000;
                for(auto const& pair : incompleteCircs){
                    int totalDistance = computeTotalDistanceToCirc(municipality, pair.second);
                    if(totalDistance < munSmallestDistToAnIncompleteCirc){
                        munSmallestDistToAnIncompleteCirc = totalDistance;
                    }   
                }
                if(munSmallestDistToAnIncompleteCirc < totalDistanceToIncompleteCircOfBestMunToRemove){ // Adjust bestToRemove
                    totalDistanceToIncompleteCircOfBestMunToRemove = munSmallestDistToAnIncompleteCirc;
                    bestMunicipalityToRemove = municipality;
                    bestCircumscriptionToBreak = neighborCirc.second;
                }
            }
         }


        }

    }
    removeMunicipalityFromCirc(bestMunicipalityToRemove, bestCircumscriptionToBreak);
    addMunicipalityToCirc( bestCircumscriptionToBreak, municipalityToForce);
    return forceAddMunicipality(bestMunicipalityToRemove, historyOfForcedMun);
        
    return false;

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

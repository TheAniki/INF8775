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
void QuickSolution::create(){      

    // Loops over all the municipalities to assign them to a circumscription     
    for(long unsigned int i = 0 ; i < this->_municipalities.size(); i++){
        for(long unsigned int j= 0 ; j < this->_municipalities[i].size(); j++){

            bool added = addMunicipalityWithProbaHeur(i,j); // Returns true if successfully added
            // Was impossible to add municipality to a circumscription 
            if(!added){
                this->_unassignedMunicipalities.push(this->_municipalities[i][j]);
                cout << " UnassignedMunicipality : " << this->_municipalities[i][j]->coordinates.row<<" , "
                <<this->_municipalities[i][j]->coordinates.column << endl;
            }
        }            
    }    
        
    cout << "MAX DISTS : " << this->_maxDist<< endl;
    cout << "MIN CIRC : " << this->_minCirc.circSize << endl;
    cout << "MIN CIRC maxAmount: " <<this->_minCirc.maxAmount << endl;
    cout << "MAX CIRC : " << this->_maxCirc.circSize << endl;
    cout << "MAX CIRC maxAmount: "<< this->_maxCirc.maxAmount << endl;
    cout << "NB CIRC : "<< this->_nbCircumscriptions << endl;
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
        if((int) circumscription->municipalities.size()>=this->_currentBound.circSize) continue;
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

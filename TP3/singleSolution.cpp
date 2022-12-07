#include "./headers/Includes.h"
#include "./headers/singleSolution.h"
#include "./headers/utils.h"
//#include <ra> 
// Default Ctor. 
SingleSolution::SingleSolution()
{
}

// SingleSolution Ctor using municipalities and the number of circumscriptions.
SingleSolution::SingleSolution(vector<vector<SharedMun>> municipalities, int nbCircumscriptions):
    _nbCircumscriptions(nbCircumscriptions),
    _municipalities(municipalities)
{
    _solution = initializeSolution(nbCircumscriptions);
    _assignedMunicipalities = createAssignedMun(municipalities);
    _nbMunicipalities = municipalities.size()*municipalities[0].size();
    _votesToWin = ((50*(_nbMunicipalities))/nbCircumscriptions)+1;
    _maxDist = ceil(float(_nbMunicipalities)/(2*nbCircumscriptions));
    computeCircBounds();
}

SingleSolution::~SingleSolution()
{
}

// SharedCirc SingleSolution::findClosestCircumscription(){
//     SharedMun unassignedMun = this->_unassignedMunicipalities.front();
//     map<int, SharedCirc> closestCircsIDs = findNeighbourCircumscriptions(unassignedMun->coordinates);
//     int distance = 10000; // set max distance.
//     int closestId = closestCircsIDs[0];
//     for(int id : closestCircsIDs){
//         cout<<"considering id: "<< id <<endl;
//         for(auto&& mun: this->_solution.circumscriptions[id]->municipalities){
//             if(distance > computeManhattanDist(mun->coordinates, unassignedMun->coordinates)){
//                 distance = computeManhattanDist(mun->coordinates, unassignedMun->coordinates);
//             }
//         }       
//     }


//     return this->_solution.circumscriptions[closestId]; // dummy return;
// }

Coord SingleSolution::findFurthestMunicipalityInCirc(shared_ptr<Municipality> municipality, SharedCirc circumscription ){
    int maxDistance = 0;
    Coord furthestCoord; 
    for(auto&& munInTargetCirc:circumscription->municipalities){
        int distance =computeManhattanDist(municipality->coordinates, munInTargetCirc->coordinates);
        if(distance > maxDistance){
            maxDistance = distance;
            furthestCoord = munInTargetCirc->coordinates;
        }
    }

    return furthestCoord;

}

int SingleSolution::computeTotalDistanceToCirc(shared_ptr<Municipality> municipality, SharedCirc circumscription ){
    int totalDistance = 0;
    for(auto&& munInTargetCirc:circumscription->municipalities){
        totalDistance += computeManhattanDist(municipality->coordinates, munInTargetCirc->coordinates);
    }

    return totalDistance;

}

vector<SharedMun> SingleSolution::getTooFarMunsInCirc(SharedMun referenceMunicipality, SharedCirc circumscription){
    vector<SharedMun> tooFarMuns;
    for(auto&& municipalityInCirc : circumscription->municipalities){
        if(computeManhattanDist(referenceMunicipality->coordinates, municipalityInCirc->coordinates) > this->_maxDist){
            tooFarMuns.push_back(municipalityInCirc);
        }
    }
    return tooFarMuns;
}




map<int, SharedCirc> SingleSolution::findNeighbourCircumscriptions(Coord coord){
    map<int, SharedCirc> neighbourCircs;
    int surroundCoords[3] = {-1, 0, 1};
    for(int i : surroundCoords){
        for(int j : surroundCoords){
            if( i == 0 && j == 0) continue;
            if( i+coord.row < 0 || j +coord.column < 0) continue;
            for(auto&& circ : this->_solution.circumscriptions){
                for(auto&& mun : circ->municipalities){
                    if(mun->coordinates.row ==i+coord.row && mun->coordinates.column == j + coord.column
                         &&  neighbourCircs.count(circ->circumscriptionNumber)==0  ){
                        neighbourCircs.emplace(circ->circumscriptionNumber, circ);
                    
                    }
                }
            }
                
        }
        
    }

    return neighbourCircs; 
}


void SingleSolution::removeMunicipalityFromCirc(shared_ptr<Municipality> municipalityToRemove, SharedCirc circumscription){
    // cout<<"removing "<< municipalityToRemove->nbVotes 
    // << " ("<<municipalityToRemove->coordinates.row <<" , "<< municipalityToRemove->coordinates.column<<") from "<<circumscription->circumscriptionNumber<<endl;
    if(!circumscription || !municipalityToRemove) { //if parameter undefined
        return;
    } 
 
   int position =0;
    for(auto&& possibleMunicipality : circumscription->municipalities){
        if(possibleMunicipality->coordinates.row == municipalityToRemove->coordinates.row && possibleMunicipality->coordinates.column == municipalityToRemove->coordinates.column){
            circumscription->municipalities.erase(circumscription->municipalities.begin()+ position);
            circumscription->totalVotes -= municipalityToRemove->nbVotes;
            if(circumscription->totalVotes < this->_votesToWin && circumscription->isWon){
                circumscription->isWon = false;
                this->_solution.nbCircWon--;
            }   
            
            break;
        }
        position++;
    }
    
    // TODO : UPDATE SOLUTION nbWon...  AND CIRCUMSCRIPTION->isWon



}

// Choses a municipality to remove from a circ based on its total distance to an incomplete circ passed in parameter
SharedMun SingleSolution::choseMunicipalityToRemoveFromCirc( SharedCirc circumscriptionToRemoveIn, SharedCirc incompleteCirc){
    int smallestDistToIncompleteCirc = this->_maxCirc.circSize*this->_nbMunicipalities; // INFINITY
    SharedMun municipalityToRemove;

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


vector<SharedCirc> SingleSolution::findPossibleCircumscriptionsToContainMun(
    SharedMun municipalityToInclude, vector<SharedCirc> circumscriptionsConsidered){

    vector<SharedCirc> possibleCircumscriptions;
    for(auto&& circ : circumscriptionsConsidered){        
            
        if(validateMunFitsInCirc(circ, municipalityToInclude)){
            possibleCircumscriptions.push_back(circ);
        }
    }

    return possibleCircumscriptions;

}

// TODO : Repair it. wrong number of municipality.
map<int, SharedCirc> SingleSolution::findIncompleteCircs(){
    vector<shared_ptr<Circumscription>> circumscriptions = this->_solution.circumscriptions;
    map<int, SharedCirc> incompleteCircs;
    for(auto&& circ : circumscriptions){
        if((int) circ->municipalities.size() < this->_currentBound.circSize){
            incompleteCircs.emplace(circ->circumscriptionNumber, circ);
        }
    }
    return incompleteCircs;
}

bool SingleSolution::validateMunFitsInCirc(SharedCirc circumscription, SharedMun municipalityToValidate){
    if((int) circumscription->municipalities.size() >= this->_currentBound.circSize) return false;
    for(auto&& municipality : circumscription->municipalities){
        if(computeManhattanDist(municipalityToValidate->coordinates, municipality->coordinates) > this->_maxDist ){
            return false;
        } 
    }


    return true;
}

bool SingleSolution::validateMunFits(SharedCirc circumscription, SharedMun municipalityToValidate){
    for(auto&& municipality : circumscription->municipalities){
        if(computeManhattanDist(municipalityToValidate->coordinates, municipality->coordinates) > this->_maxDist ){
            return false;
        } 
    }
    return true;
}

bool SingleSolution::isMunInVector(SharedMun municipality, const vector<Coord>& munCoordList){
    for(Coord coord : munCoordList){
        if(municipality->coordinates.row == coord.row && municipality->coordinates.column == coord.column ){
            return true;
        }
    }
    return false;
}

void SingleSolution::addMunicipalityToCirc(SharedCirc circumscription, shared_ptr<Municipality> municipality ){
    // cout<<"adding "<< municipality->nbVotes 
    // << " ("<<municipality->coordinates.row <<" , "<< municipality->coordinates.column<<") to "<<circumscription->circumscriptionNumber<<endl;
    if(!circumscription || !municipality) { //if parameter undefined
        return;
    } 

    circumscription->addMun(municipality);
    circumscription->totalVotes+=municipality->nbVotes;
    if(circumscription->totalVotes >= this->_votesToWin && !circumscription->isWon){
            circumscription->isWon = true;
            this->_solution.nbCircWon++;
    }    

}

void SingleSolution::computeCircBounds(){
    float fraction = float(this->_nbMunicipalities)/(this->_nbCircumscriptions);

    this->_minCirc = CircBound(floor(fraction), 0);
    this->_maxCirc = CircBound(ceil(fraction), 0);   

    computeRepartition();    
}

//Calculate repartition.
void SingleSolution::computeRepartition(){
    // repartition values;
    int x; // x amount of minC.
    int y; // y amount of maxC.

    if(float(this->_maxCirc.circSize)/this->_minCirc.circSize == 1)
        this->_maxCirc.maxAmount = this->_nbMunicipalities/this->_maxCirc.circSize;
    else{
        // finds the repartition of maxC elements.
        y = (this->_nbCircumscriptions - 
        float(this->_nbMunicipalities)/this->_minCirc.circSize)/(1-float(this->_maxCirc.circSize)/this->_minCirc.circSize);        
        x = this->_nbCircumscriptions - y;
        int total = x*this->_minCirc.circSize + y*this->_maxCirc.circSize;

        // In case the total is one short or over. the distance between x and y is always 1.
        if(this->_nbMunicipalities-total == 1){
            y++;
            x--;
        }
        if(this->_nbMunicipalities-total == -1){
            y--;
            x++;
        }
        this->_maxCirc.maxAmount = y;
        this->_minCirc.maxAmount = x;
    }    
    this->_currentBound = this->_minCirc;   
}
//Generates a matrix that tells if a municipality is assigned to a 
// Necessary, because there will be multiple solutions sharing the same municipalities
vector<vector<bool>> SingleSolution::createAssignedMun(vector<vector<SharedMun>> municipalities){
    vector<vector<bool>> assignedMunicipalities(municipalities.size());

    for(long unsigned int i = 0 ; i < municipalities.size(); i++){
        for(long unsigned int j= 0 ; j < municipalities[i].size(); j++){
            assignedMunicipalities[i].push_back(false);
        }
    }

    return assignedMunicipalities;
}


Solution SingleSolution::initializeSolution(int nbCircumscription){

    Solution initialSolution;
    initialSolution.circumscriptions = vector<SharedCirc>(nbCircumscription);
        for(int i = 0 ; i < nbCircumscription ; i++ ){
            vector<SharedMun> emptyMunicipality;
            initialSolution.nbCircWon = 0;
            initialSolution.circumscriptions[i] = make_shared<Circumscription>(i+1, false,  0, emptyMunicipality);
       }

    return initialSolution;
}

Solution SingleSolution::getSolution(){
    return this->_solution;
}
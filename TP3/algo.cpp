#include "./headers/Includes.h"
#include "./headers/algo.h"
#include "./headers/utils.h"

// Default Ctor. 
Algo::Algo()
{
}

// Algo Ctor using municipalities and the number of circumscriptions.
Algo::Algo(vector<vector<shared_ptr<Municipality>>> municipalities, int nbCircumscriptions):
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

Algo::~Algo()
{
}

void Algo::quickSolution(){      

    cout << "MAX DISTS : " << this->_maxDist<< endl;
    cout << "MIN CIRC : " << this->_minCirc.circSize << endl;
    cout << "MIN CIRC maxAmount: " <<this->_minCirc.maxAmount << endl;
    cout << "MAX CIRC : " << this->_maxCirc.circSize << endl;
    cout << "MAX CIRC maxAmount: "<< this->_maxCirc.maxAmount << endl;
    cout << "NB CIRC : "<< this->_nbCircumscriptions << endl;

    // Loops over all the municipalities to assign them to a circumscription 
    for(long unsigned int i = 0 ; i < this->_municipalities.size(); i++){
        for(long unsigned int j= 0 ; j < this->_municipalities[i].size(); j++){
            if(this->_assignedMunicipalities[i][j]) continue;

            this->_assignedMunicipalities[i][j] = addMunicipalityToFirstAvailableCirc(i,j); // Returns true if successfully added

            // Was impossible to add municipality to a circumscription 
            if(!this->_assignedMunicipalities[i][j]){
                this->_unassignedMunicipalities.push(this->_municipalities[i][j]);
                cout << "unassignedMunicipality : " << this->_municipalities[i][j]->coordinates.row<<"   ,   "
                <<this->_municipalities[i][j]->coordinates.column << endl;
            }
        }   
    }


    //Creating incomplete circ vect
    vector<shared_ptr<Circumscription>> incompleteCircs = findIncompleteCircs(this->_solution.circumscriptions);
    
    if(this->_unassignedMunicipalities.empty()){
        cout << "EMPTY QUEUE " << endl;
    }

    //Trying to repare solution
    int counterOfMun = 0; // 
    while(!this->_unassignedMunicipalities.empty() && counterOfMun < (int)(this->_municipalities.size()*this->_municipalities[0].size())){
        // cout << "FRONT OF QUEUE : " <<this->_unassignedMunicipalities.front()->nbVotes<< endl;
        vector<shared_ptr<Circumscription>> possibleCircumscriptions = findPossibleCircumscriptionsToContainMun(this->_unassignedMunicipalities.front(),  this->_solution.circumscriptions);
        //chose municipality to remove from a possible circ 
        if(possibleCircumscriptions.size()==0) {
            shared_ptr<Circumscription> closestCirc = findClosestCircumscription();
        }
        else{
            for(auto&& circ : possibleCircumscriptions)
                cout << "POSSIBLE CIRCUMSCRIPTION TO CHOP : "<< circ->circumscriptionNumber <<endl;
        
            shared_ptr<Municipality> municipalityToRemove =  choseMunicipalityToRemoveFromCirc( possibleCircumscriptions[0], incompleteCircs[0]);

            removeMunicipalityFromCirc(municipalityToRemove,  possibleCircumscriptions[0]); 
            addMunicipalityToCirc(possibleCircumscriptions[0],this->_unassignedMunicipalities.front() ); //Adds the unassignedMunicipality to the possibleCirc
        

            this->_unassignedMunicipalities.pop(); //municipality is now assigned
            this->_unassignedMunicipalities.push(municipalityToRemove); // new unassigned municipality

            if(validateMunFitsInCirc(incompleteCircs[0], this->_unassignedMunicipalities.front())){
                addMunicipalityToCirc(incompleteCircs[0], municipalityToRemove );
                this->_unassignedMunicipalities.pop();
            }       
        }
        counterOfMun++;
        cout<<"cnt: "<<counterOfMun<<endl;
    }  
}

shared_ptr<Circumscription> Algo::findClosestCircumscription(){
    shared_ptr<Municipality> unassignedMun = this->_unassignedMunicipalities.front();
    vector<int> closestCircsIDs = findNeighbourCircumscriptions(unassignedMun->coordinates);
    int distance = 10000; // set max distance.
    int closestId = closestCircsIDs[0];
    for(int id : closestCircsIDs){
        cout<<"considering id: "<< id <<endl;
        for(auto&& mun: this->_solution.circumscriptions[id]->municipalities){
            if(distance > computeManhattanDist(mun->coordinates, unassignedMun->coordinates)){
                distance = computeManhattanDist(mun->coordinates, unassignedMun->coordinates);
            }
        }        
    }

    return this->_solution.circumscriptions[closestId];
}

vector<int> Algo::findNeighbourCircumscriptions(Coord coord){
    cout<<"finding neighbour of : "<<coord.column <<", " <<coord.row<<endl;
    vector<int> closestCircsIDs;
    int surroundCoords[3] = {-1, 0, 1};
    for(int i : surroundCoords){
        for(int j : surroundCoords){
            if( i == 0 && j == 0) continue;
            if( i+coord.column < 0 || j +coord.row < 0) continue;
            if( i+coord.column >=(int) this->_municipalities[0].size()  
                || j +coord.row >= (int) this->_municipalities.size()) continue;
                          
        }
    }

    return closestCircsIDs; // TODO: Remove.
}

void Algo::removeMunicipalityFromCirc(shared_ptr<Municipality> municipalityToRemove, shared_ptr<Circumscription> circumscription){
    //TODO : there are more efficient algorithms to remove an element from a vector
   int position =0;
    for(auto&& possibleMunicipality : circumscription->municipalities){
        if(possibleMunicipality->coordinates.row == municipalityToRemove->coordinates.row && possibleMunicipality->coordinates.column == municipalityToRemove->coordinates.column){
            circumscription->municipalities.erase(circumscription->municipalities.begin()+ position);
            break;
        }
        position++;
    }

    circumscription->totalVotes-= municipalityToRemove->nbVotes;
    // TODO : UPDATE SOLUTION nbWon...  AND CIRCUMSCRIPTION->isWon



}

// Choses a municipality to remove from a circ based on its total distance to an incomplete circ passed in parameter
shared_ptr<Municipality> Algo::choseMunicipalityToRemoveFromCirc( shared_ptr<Circumscription> circumscriptionToRemoveIn, shared_ptr<Circumscription> incompleteCirc){
    int smallestDistToIncompleteCirc = this->_maxCirc.circSize*this->_nbMunicipalities; // INFINITY
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


vector<shared_ptr<Circumscription>> Algo::findPossibleCircumscriptionsToContainMun(
    shared_ptr<Municipality> municipalityToInclude, vector<shared_ptr<Circumscription>> circumscriptionsConsidered){

    vector<shared_ptr<Circumscription>> possibleCircumscriptions;
    // int i = 0 ; //TODO : remove
    // find possible solutions
    for(auto&& circ : circumscriptionsConsidered){        
            
        if(validateMunFitsInCirc(circ, municipalityToInclude)){
            possibleCircumscriptions.push_back(circ);
            // cout<<"POSSIBLE CIRC FOR MUNICIPALITY : " << i << endl;
        }
        // i++;
    }

    return possibleCircumscriptions;

}

// TODO : Repair it. wrong number of municipality.
vector<shared_ptr<Circumscription>> Algo::findIncompleteCircs(vector<shared_ptr<Circumscription>> circumscriptions){
    vector<shared_ptr<Circumscription>> incompleteCircs;
    for(auto&& circ : circumscriptions){
        if((int) circ->municipalities.size() < this->_currentCirc.circSize){
            incompleteCircs.push_back(circ);
        }
    }
    return incompleteCircs;
}

bool Algo::addMunicipalityToFirstAvailableCirc(int i, int j){{
    // TODO: maxCirc while possible else minCirc.        
    for(auto&& circumscription : this->_solution.circumscriptions){
        if((int) circumscription->municipalities.size()>=this->_currentCirc.circSize) continue; //no more space in circumscription

            if(validateMunFitsInCirc(circumscription, this->_municipalities[i][j]) ){
            
                addMunicipalityToCirc(circumscription, this->_municipalities[i][j]); 
                if(circumscription->totalVotes >= this->_votesToWin){
                    circumscription->isWon = true;
                    this->_solution.nbCircWon++;
                }
                return true; //successfully added 
            }
        }
    }
    return false;
}

bool Algo::validateMunFitsInCirc(shared_ptr<Circumscription> circumscription, shared_ptr<Municipality> municipalityToValidate){
    for(auto&& municipality : circumscription->municipalities){
        if(computeManhattanDist(municipalityToValidate->coordinates, municipality->coordinates) > this->_maxDist){
            return false;
        } 
    }
    return true;
}

void Algo::addMunicipalityToCirc(shared_ptr<Circumscription> circumscription, shared_ptr<Municipality> municipality ){
    circumscription->addMun(municipality);
    circumscription->totalVotes+=municipality->nbVotes;
    if((int) circumscription->municipalities.size() >= this->_currentCirc.circSize)
        this->_currentCirc.maxAmount--;
    if(this->_currentCirc.maxAmount <= 0)
        this->_currentCirc = this->_minCirc;
}

void Algo::computeCircBounds(){
    float fraction = float(this->_nbMunicipalities)/(this->_nbCircumscriptions);

    this->_minCirc = CircBound(floor(fraction), 0);
    this->_maxCirc = CircBound(ceil(fraction), 0);   

    computeRepartition();    
}

//Calculate repartition.
void Algo::computeRepartition(){
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
    this->_currentCirc = this->_maxCirc;   
}
//Generates a matrix that tells if a municipality is assigned to a 
// Necessary, because there will be multiple solutions sharing the same municipalities
vector<vector<bool>> Algo::createAssignedMun(vector<vector<shared_ptr<Municipality>>> municipalities){
    vector<vector<bool>> assignedMunicipalities(municipalities.size());

    for(long unsigned int i = 0 ; i < municipalities.size(); i++){
        for(long unsigned int j= 0 ; j < municipalities[i].size(); j++){
            assignedMunicipalities[i].push_back(false);
        }
    }

    return assignedMunicipalities;
}


Solution Algo::initializeSolution(int nbCircumscription){

    Solution initialSolution;
    initialSolution.circumscriptions = vector<shared_ptr<Circumscription>>(nbCircumscription);
        for(int i = 0 ; i < nbCircumscription ; i++ ){
            vector<shared_ptr<Municipality>> emptyMunicipality;
            initialSolution.nbCircWon = 0;
            initialSolution.circumscriptions[i] = make_shared<Circumscription>(i+1, false,  0, emptyMunicipality); ;
       }

    return initialSolution;
}

Solution Algo::getSolution(){
    return this->_solution;
}
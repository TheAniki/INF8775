#include "./headers/localSearch.h"
#include "./headers/utils.h"
#include "./headers/Includes.h"

LocalSearch::LocalSearch(/* args */)
{
}

LocalSearch::LocalSearch(SingleSolution singleSolution)
:SingleSolution::SingleSolution(singleSolution)
{
}

LocalSearch::~LocalSearch()
{
}

Solution LocalSearch::getSolution(){
    return this->_solution;
}

// algo 2-opt
// returns true if solution changed
bool LocalSearch::upgradeSolution(){       
   
        // cout<<"Upgrading solution"<<endl;


        //vector<SharedCirc> neighbors = findNeihborCircumscriptions(this->_solution.circumscriptions[i]);
        vector<pair<SharedCirc,int>> losingCircs = orderLosingCirc();      
         

        increaseVotesofAllLosing(losingCircs);

        // definir le voisinage.

        // on choisi le meilleur voisin.

        // tant qu il exite s elem de Voisinage[s] ou f(s)>f(s_i)
        //      i <- i + 1;
        //      s_i <- s
        
        // TODO: upgrade current solution.
        return false;
    
    // retourner la solution
}
void LocalSearch::increaseVotesofAllLosing(vector<pair<SharedCirc,int>> losingCircs){
    
    for(auto&& circ : losingCircs){
        // cout<<"increasing : "<<circ.first->circumscriptionNumber<<endl;
        bool success = increaseVotesClosestToTreshold(circ.first, circ.second);
        if(success) return;
    }    
}

bool LocalSearch::increaseVotesClosestToTreshold(SharedCirc losingCirc, int distanceToWinning){
    // find all circonscription around it.
    /*  prend en paramètre une circonscription (perdante) et considére toutes les circonscription 
        autour pour faire des échanges de municipalités de façon à la faire  gagner (en dépassant le moins possible le treshhold), 
        ou juste améliorer son nb de votes si on n'arrive pas a la faire gagner ,
        ET  sans faire perdre une circonscription qui était déjà gagnante*/
    map<int, SharedCirc> neighborCircs = findNeihborCircumscriptionsOfLowest(losingCirc);
    
    
    if(neighborCircs.size()==0) return false;

    vector<SharedCirc> neighbors;
    for(auto&& neigbor : neighborCircs){
        neighbors.push_back(neigbor.second);
    }

    bool success = TrySwappingMunicipalities(losingCirc, neighbors,distanceToWinning);

    return success;
}

bool LocalSearch::TrySwappingMunicipalities(SharedCirc losingCirc, vector<SharedCirc> neighbors, int distanceToWinning){
    // SharedMun lowestValueMunicipality = findLowestMunicipality(losingCirc);
    // SharedMun bestValueMunicipality = findBestMunicipalityToRemove(bestCirc);
    vector<SharedMun> lowestMuns= findAllLowestMunicipalities(losingCirc);
    //cout<<" distance to win "<<distanceToWinning<<endl;
    // if(distanceToWinning >= 51){
    //     lowestMuns = findAllBestMunicipalities(losingCirc);
    // }
    // else{
    //     lowestMuns = findAllLowestMunicipalities(losingCirc);        
    // }
    
    
    // find municipalities from neighbor that can be added to losingCirc.
    vector<pair<int,SharedMun>> circAndMun; // int: circ where mun can be added.
    for(auto&& neighbor : neighbors){
        if(neighbor->circumscriptionNumber == losingCirc->circumscriptionNumber) continue;
        for(auto& mun : neighbor->municipalities){
            if(validateMunFits(losingCirc, mun)){
                circAndMun.push_back(make_pair(neighbor->circumscriptionNumber, mun));
            }
        }        
    }    

    // find values municipalities that can be added to neigbor
    vector<pair<int,SharedMun>> munInNeig; // int: circ where mun can be added.
    for(auto&& neighbor : neighbors){
        if(neighbor->circumscriptionNumber == losingCirc->circumscriptionNumber) continue;
        for(auto& mun : lowestMuns){
            if(validateMunFits(neighbor, mun)){
                munInNeig.push_back(make_pair(neighbor->circumscriptionNumber, mun));
            }
        }        
    }



    // cout<<"municipalities from neighbor that can be added to losingCirc"<<endl;
    // for(auto& mun : circAndMun){
    //     cout<<"circ : "<< mun.first << " and mun that can be added: "<<mun.second->nbVotes
    //         <<"(" <<  mun.second->coordinates.row<<" , "<< mun.second->coordinates.column <<")"
    //         <<" distance: "<< abs(mun.second->nbVotes - distanceToWinning) <<endl;
    // }
    // cout<<"municipalities that can be added to neigbor"<<endl;
    // for(auto& mun : munInNeig){
    //     cout<<"circ : "<< mun.first << " and mun that can be added: "<<mun.second->nbVotes
    //         <<"(" <<  mun.second->coordinates.row<<" , "<< mun.second->coordinates.column <<")"
    //         <<" distance: "<< abs(mun.second->nbVotes - distanceToWinning) <<endl;
    // }
    // cout<<endl;   
    
    vector<SharedMun> emptyMunicipality;
    SharedCirc bestNeigbor = make_shared<Circumscription>(1, false,  0, emptyMunicipality); 

    for(auto&& neighbor : neighbors){
        //cout<<" checking neighbor : "<<neighbor->circumscriptionNumber<<endl;
        if(neighbor->circumscriptionNumber == losingCirc->circumscriptionNumber) continue;
        if(!isContainedInBoth(neighbor->circumscriptionNumber , munInNeig, circAndMun)) continue;
        //cout<<bestNeigbor->circumscriptionNumber<< " is contained in munNeig and circAndMun"<<endl;
        if(bestNeigbor->totalVotes < neighbor->totalVotes){
            bestNeigbor = neighbor;
        }
    }
    if(bestNeigbor->municipalities.size()==0) return false;

    //cout<<" best neigbor "<< bestNeigbor->circumscriptionNumber<< " with " << bestNeigbor->totalVotes <<" votes" <<endl;
    
    // find worst mun in circAndMun
    SharedMun worstMunInLosing = findLowestMunicipality(losingCirc); 
    int lastBest = 1000;
    for(auto& mun : munInNeig){
        if(mun.first == bestNeigbor->circumscriptionNumber && mun.second->nbVotes < lastBest ){
            worstMunInLosing = mun.second;
            lastBest = mun.second->nbVotes;
        }
    }
    
    // find best that fits without making the municipality lose;
    SharedMun bestMunInNeighbor = findLowestMunicipality(bestNeigbor);
    lastBest = 0;
    int nbVoteToWin = ((losingCirc->municipalities.size()*100)/2)+1;
    for(auto& mun : circAndMun){
        if(mun.first == bestNeigbor->circumscriptionNumber && mun.second->nbVotes > lastBest ){
            if(bestNeigbor->totalVotes < nbVoteToWin){
                bestMunInNeighbor = mun.second;
                lastBest = mun.second->nbVotes;
            }
            else if( (bestNeigbor->totalVotes - bestMunInNeighbor->nbVotes + worstMunInLosing->nbVotes) >= nbVoteToWin){
                bestMunInNeighbor = mun.second;
                lastBest = mun.second->nbVotes;
            }              
        }
    }
    
    // if none is found return.
    if(bestMunInNeighbor->nbVotes == worstMunInLosing->nbVotes) return false;

    // cout<<"worst in losing that can be swapped with neigbor "<< worstMunInLosing->nbVotes 
    // << " coords: "<<"(" <<  worstMunInLosing->coordinates.row<<" , "<< worstMunInLosing->coordinates.column <<")"<<endl;
    // cout<<"best in neigbor that can be swapped with losing "<< bestMunInNeighbor->nbVotes 
    // << " coords: "<<"(" <<  bestMunInNeighbor->coordinates.row<<" , "<< bestMunInNeighbor->coordinates.column <<")"<<endl;
    int oldLosing = losingCirc->totalVotes;
    
    

    swapMuns(bestNeigbor->circumscriptionNumber ,bestMunInNeighbor, losingCirc->circumscriptionNumber , worstMunInLosing);
       

    return (oldLosing < losingCirc->totalVotes);
    
    // TODO: verify combinations. to not make a circ lose. 
}

void LocalSearch::swapMuns(int bestCircNb , SharedMun bestMunInNeighbor , int losingCircNb , SharedMun worstMunInLosing){



    int initialLosing = this->_solution.circumscriptions[losingCircNb-1]->totalVotes;
    //int initialBest = this->_solution.circumscriptions[bestCircNb-1]->totalVotes;

    int newLosing = initialLosing - worstMunInLosing->nbVotes + bestMunInNeighbor->nbVotes;

    // only updateSolution if best value is found
    if(newLosing < initialLosing){

        return;
    } 
    //cout<<" ========================================================================== "<<endl;
    removeMunicipalityFromCirc(bestMunInNeighbor, this->_solution.circumscriptions[bestCircNb-1]);
    removeMunicipalityFromCirc(worstMunInLosing, this->_solution.circumscriptions[losingCircNb-1]);

    // cout<<" removed elements from bestCirc " << bestCircNb <<" and losingCirc "<<losingCircNb<<endl;
    // cout<<"         removed : "<< bestMunInNeighbor->nbVotes 
    // << " coords: "<<"(" <<  bestMunInNeighbor->coordinates.row<<" , "<< bestMunInNeighbor->coordinates.column <<")"<<endl;
    // cout<<"         removed : "<< worstMunInLosing->nbVotes 
    // << " coords: "<<"(" <<  worstMunInLosing->coordinates.row<<" , "<< worstMunInLosing->coordinates.column <<")"<<endl;


   addMunicipalityToCirc(this->_solution.circumscriptions[losingCircNb-1], bestMunInNeighbor);
   addMunicipalityToCirc(this->_solution.circumscriptions[bestCircNb-1], worstMunInLosing); 


    //    cout<<" swapped elements from bestCirc " << bestCircNb <<" and losingCirc "<<losingCircNb<<endl;
    //    cout<<"         swapped : "<< bestMunInNeighbor->nbVotes 
    //     << " coords: "<<"(" <<  bestMunInNeighbor->coordinates.row<<" , "<< bestMunInNeighbor->coordinates.column <<")"<<endl;
    //     cout<<"         swapped : "<< worstMunInLosing->nbVotes 
    //     << " coords: "<<"(" <<  worstMunInLosing->coordinates.row<<" , "<< worstMunInLosing->coordinates.column <<")"<<endl;
 
    //cout<<" ========================================================================== "<<endl;  
    // cout<<" new losing total vote : "<< this->_solution.circumscriptions[losingCircNb-1]->totalVotes<<endl;
    // cout<<" old best total vote : "<< this->_solution.circumscriptions[bestCircNb-1]->totalVotes<<endl;

}

bool isContainedInBoth(int circNumber, vector<pair<int,SharedMun>> neig, vector<pair<int,SharedMun>> loss){
    bool isContainedInNeig = isContainedIn(circNumber, neig);
    bool isContainedInLoss = isContainedIn(circNumber, loss);

    return isContainedInNeig && isContainedInLoss;
}

bool isContainedIn(int circNumber, vector<pair<int,SharedMun>> list){
    for(auto& n : list){
        if(n.first == circNumber){
            return true;
        }
    }
    return false;
}

// Find the Circ with the greater winning margin.
SharedCirc findBestWinningCric(map<int, SharedCirc> neighbors){
    SharedCirc bestCirc = neighbors.begin()->second;
    int lastBest = -1;
    for(auto&& neighbor : neighbors){
        int nbVoteToWin = ((neighbor.second->municipalities.size()*100)/2)+1;
        int difference = neighbor.second->totalVotes - nbVoteToWin;
        if(difference > lastBest){
            lastBest = difference;
            bestCirc = neighbor.second;
        }
    }

    return bestCirc;
}

map<int, SharedCirc> LocalSearch::findNeihborCircumscriptionsOfLowest(SharedCirc circ){
    
    // TODO: chose random municipality.
    SharedMun lowestValueMunicipality = findLowestMunicipality(circ);

    // cout<<" lowest value: "<< lowestValueMunicipality->nbVotes
    //     <<"(" <<  lowestValueMunicipality->coordinates.row<<" , "<< lowestValueMunicipality->coordinates.column <<")"
    //     << " in circ "<< circ->circumscriptionNumber<<endl;
    // find all neighbors in manhattan distance of lowest
    map<int, SharedCirc> neighborCircs =  findNeighbourCircumscriptions(lowestValueMunicipality->coordinates);
    

    return neighborCircs;
} 

SharedMun LocalSearch::findBestMunicipalityToRemove(SharedCirc circ){
    SharedMun bestMun = circ->municipalities[0];
    int nbVoteToWin = ((circ->municipalities.size()*100)/2)+1;
    int lastBest = 100;
    for(auto& mun : circ->municipalities){
        int maxAmount = circ->totalVotes - nbVoteToWin;
        int difference = maxAmount - mun->nbVotes;
        if( difference>0 &&  difference < lastBest){
            lastBest = difference;
            bestMun = mun;
        }
    }
    
    return bestMun;
}

vector<SharedMun> LocalSearch::findAllCircsThatFit(SharedCirc circ, SharedCirc inCirc){
     vector<SharedMun> munFits;
    for(auto& mun : circ->municipalities){
        if(validateMunFitsInCirc(inCirc,mun)){
            munFits.push_back(mun);
        }
    }
    
    return munFits;
}

// Finds all best municipalities that can be added to lowestCirc.
vector<SharedMun> LocalSearch::findAllBestMunicipalities(SharedCirc circ){
    vector<SharedMun> bestMuns;
    int winningTreshlod = 51;
    for(auto& mun : circ->municipalities){
        if( mun->nbVotes >= winningTreshlod){
            bestMuns.push_back(mun);
        }
    }
    
    return bestMuns;
}

// Finds all lowest municipalities that are below winning treshold.
vector<SharedMun> LocalSearch::findAllLowestMunicipalities(SharedCirc circ){
    vector<SharedMun> lowestMuns;
    int winningTreshlod = 51;
    for(auto& mun : circ->municipalities){
        if( mun->nbVotes < winningTreshlod){
            lowestMuns.push_back(mun);
        }
    }
    
    return lowestMuns;
}

SharedMun LocalSearch::findLowestMunicipality(SharedCirc circ){
    // srand(time(NULL));
    SharedMun lowest = circ->municipalities[0];
    
    // for(auto& mun : circ->municipalities){
    //     if(mun->nbVotes < lowest->nbVotes){
    //         lowest = mun;
    //     }
    // }
    
    return lowest;
}


vector<pair<SharedCirc,int>> LocalSearch::orderLosingCirc(){
    vector<pair<SharedCirc,int>> losingCircs;
    for(auto&& circ : this->_solution.circumscriptions){
        int nbVoteToWin = ((circ->municipalities.size()*100)/2)+1;
        if(circ->totalVotes < nbVoteToWin){
            losingCircs.push_back(make_pair(circ, abs(nbVoteToWin - circ->totalVotes)));
        }
    }
    
    sort(losingCircs.begin(), losingCircs.end(),
     [](const pair<SharedCirc,int>& a, const pair<SharedCirc,int>& b)-> bool{
        return a.second < b.second;
    });
     
    return losingCircs;
}
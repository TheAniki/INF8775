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
void LocalSearch::upgradeSolution(int nbIterations){    
    for(int i=0; i<nbIterations; i++){
        cout<<"Upgrading solution"<<endl;

        //vector<SharedCirc> neighbors = findNeihborCircumscriptions(this->_solution.circumscriptions[i]);
        vector<pair<SharedCirc,int>> losingCircs = orderLosingCirc();
        
        increaseVotesofAllLosing(losingCircs);

        // definir le voisinage.

        // on choisi le meilleur voisin.

        // tant qu il exite s elem de Voisinage[s] ou f(s)>f(s_i)
        //      i <- i + 1;
        //      s_i <- s
        
        // TODO: upgrade current solution.
    }
    // retourner la solution
}
void LocalSearch::increaseVotesofAllLosing(vector<pair<SharedCirc,int>> losingCircs){
    

    for(auto&& circ : losingCircs){
        cout<<"increasing : "<<circ.first->circumscriptionNumber<<endl;
        increaseVotesClosestToTreshold(circ.first, circ.second);
    }
    
}

void LocalSearch::increaseVotesClosestToTreshold(SharedCirc losingCirc, int distanceToWinning){
    // find all circonscription around it.
    /*  prend en paramètre une circonscription (perdante) et considére toutes les circonscription 
        autour pour faire des échanges de municipalités de façon à la faire  gagner (en dépassant le moins possible le treshhold), 
        ou juste améliorer son nb de votes si on n'arrive pas a la faire gagner ,
        ET  sans faire perdre une circonscription qui était déjà gagnante*/
    map<int, SharedCirc> neighborCircs = findNeihborCircumscriptionsOfLowest(losingCirc);
    vector<SharedCirc> neighbors;
    for(auto&& neigbor : neighborCircs){
        neighbors.push_back(neigbor.second);
    }
    
    TrySwappingMunicipalities(losingCirc, neighbors,distanceToWinning);

}

void LocalSearch::TrySwappingMunicipalities(SharedCirc losingCirc, vector<SharedCirc> neighbors,int distanceToWinning){
    // SharedMun lowestValueMunicipality = findLowestMunicipality(losingCirc);
    // SharedMun bestValueMunicipality = findBestMunicipalityToRemove(bestCirc);
    vector<SharedMun> lowestMuns;
    if(distanceToWinning >= 51){
        lowestMuns = findAllBestMunicipalities(losingCirc);
    }
    else{
        lowestMuns = findAllLowestMunicipalities(losingCirc);        
    }

    // find municipalities from neighbor that can be added to losingCirc.
    vector<pair<int,SharedMun>> circAndMun; // int: circ where mun can be added.
    for(auto&& neighbor : neighbors){
        if(neighbor == losingCirc) continue;
        for(auto& mun : neighbor->municipalities){
            if(validateMunFits(losingCirc, mun)){
                circAndMun.push_back(make_pair(neighbor->circumscriptionNumber, mun));
            }
        }        
    }

    // find values municipalities that can be added to neigbor
    vector<pair<int,SharedMun>> munInNeig; // int: circ where mun can be added.
    for(auto&& neighbor : neighbors){
        if(neighbor == losingCirc) continue;
        for(auto& mun : lowestMuns){
            if(validateMunFits(neighbor, mun)){
                munInNeig.push_back(make_pair(neighbor->circumscriptionNumber, mun));
            }
        }        
    }

    cout<<"municipalities from neighbor that can be added to losingCirc"<<endl;
    for(auto& mun : circAndMun){
        cout<<"circ : "<< mun.first << " and mun that can be added: "<<mun.second->nbVotes
            <<"(" <<  mun.second->coordinates.row<<" , "<< mun.second->coordinates.column <<")"
            <<" distance: "<< abs(mun.second->nbVotes - distanceToWinning) <<endl;
    }
    cout<<"municipalities that can be added to neigbor"<<endl;
    for(auto& mun : munInNeig){
        cout<<"circ : "<< mun.first << " and mun that can be added: "<<mun.second->nbVotes
            <<"(" <<  mun.second->coordinates.row<<" , "<< mun.second->coordinates.column <<")"
            <<" distance: "<< abs(mun.second->nbVotes - distanceToWinning) <<endl;
    }
    cout<<endl;   

    // TODO: verify combinations. to not make a circ lose. 
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
    
    SharedMun lowestValueMunicipality = findLowestMunicipality(circ);
    cout<<" lowest value: "<< lowestValueMunicipality->nbVotes
        <<"(" <<  lowestValueMunicipality->coordinates.row<<" , "<< lowestValueMunicipality->coordinates.column <<")"
        << " in circ "<< circ->circumscriptionNumber<<endl;
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
    SharedMun lowest = circ->municipalities[0];
    for(auto& mun : circ->municipalities){
        if(mun->nbVotes < lowest->nbVotes){
            lowest = mun;
        }
    }
    
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

    for(auto& circ : losingCircs){
        cout<<"# "<<circ.first->circumscriptionNumber<<": "<<circ.second<<" ";
    }    
    cout<<endl;
    return losingCircs;
}
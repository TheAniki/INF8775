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
        increaseVotesClosestToTreshold(circ.first);
    }
    
}
void LocalSearch::increaseVotesClosestToTreshold(SharedCirc losingCirc){
    // find all circonscription around it.
    /*  prend en paramètre une circonscription (perdante) et considére toutes les circonscription 
        autour pour faire des échanges de municipalités de façon à la faire  gagner (en dépassant le moins possible le treshhold), 
        ou juste améliorer son nb de votes si on n'arrive pas a la faire gagner ,
        ET  sans faire perdre une circonscription qui était déjà gagnante*/
    map<int, SharedCirc> neighborCircs = findNeihborCircumscriptionsOfLowest(losingCirc);
    SharedCirc bestCirc = findBestWinningCric(neighborCircs);
    
    TrySwappingMunicipalities(losingCirc, bestCirc);

}

void LocalSearch::TrySwappingMunicipalities(SharedCirc losingCirc, SharedCirc bestCirc){
    SharedMun lowestValueMunicipality = findLowestMunicipality(losingCirc);
    SharedMun bestValueMunicipality = findBestMunicipalityToRemove(bestCirc);

    cout<<"lowestVal : "<<lowestValueMunicipality->nbVotes <<" at "<<" ("
        <<lowestValueMunicipality->coordinates.row <<" , "<<lowestValueMunicipality->coordinates.row<<")"<<endl;

    cout<<"bestVal : "<<bestValueMunicipality->nbVotes <<" at "<<" ("
    <<bestValueMunicipality->coordinates.row <<" , "<<bestValueMunicipality->coordinates.row<<")"<<endl;
}

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
    
    return losingCircs;
}
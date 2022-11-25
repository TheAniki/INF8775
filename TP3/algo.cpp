#include "./headers/Includes.h"
#include "./headers/algo.h"

void algo(){
    cout<<"Bonjour je suis un algo qui marche!"<<endl;
    
}

Solution quickSolution(vector<vector<shared_ptr<Municipality>>> municipalities ){
    Solution solution;
    vector<vector<bool>> assignedMunicipalities = genAssignedMun(municipalities);


    for(long unsigned int i = 0 ; i < municipalities.size(); i++){
        for(long unsigned int j= 0 ; j < municipalities[i].size(); j++){
            if(assignedMunicipalities[i][j]) continue;
            


        }
        cout << endl;
    }
    return solution;
}

//Generates a matrix that tells if a municipality is assigned to a 
vector<vector<bool>> genAssignedMun(vector<vector<shared_ptr<Municipality>>> municipalities){
    vector<vector<bool>> assignedMunicipalities(municipalities.size());

    for(long unsigned int i = 0 ; i < municipalities.size(); i++){
        for(long unsigned int j= 0 ; j < municipalities[i].size(); j++){
            assignedMunicipalities[i].push_back(false);
        }
    }

    return assignedMunicipalities;
}
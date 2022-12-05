#ifndef INCLUDES_H
#define INCLUDES_H

#include <iostream>
#include <cstring>
#include <sstream>
#include <vector>
#include <map>
#include <fstream>
#include <string>
#include <memory>
#include <queue>
#include <set>
#include <algorithm>
#include <math.h>       /* floor */
#include <numeric>
using namespace std;

// ==================== Struct ==================== //
struct CircBound{
    int circSize;
    int maxAmount;

    CircBound():circSize(0), maxAmount(0){};
    CircBound(int cSize, int mAmount ):
        circSize(cSize), maxAmount(mAmount){};
};    


struct Coord{
    int row;
    int column;

    // Default Ctor.
	Coord():
		row(-1), column(-1){}

	// Coord Ctor.
	Coord(int row_coord, int column_coord)
		:row(row_coord), column(column_coord) {}
};

struct Municipality {
    Coord coordinates;
    int nbVotes; // Number of votes for the green party.
	float score; // score of the municipality : average of votes for the area of itself and euclidean neighbor.  
	int nbNeighbors = 0; // number of neighbors in euclidean distance of 1.

    // Default Ctor.
	Municipality():
		coordinates(Coord()),nbVotes(-1){}

	// Municipality Ctor.
	Municipality(int column_coord, int row_coord, int votes)
		:coordinates(Coord(column_coord,row_coord)),
         nbVotes(votes) {}
};

struct Circumscription{
	int circumscriptionNumber;
	bool isWon;
	int totalVotes;
	vector<shared_ptr<Municipality>> municipalities;

	Circumscription(){}

	Circumscription(int circumscriptionNumber, bool isWon, int totalVotes, 
	vector<shared_ptr<Municipality>> municipalities):
	circumscriptionNumber(circumscriptionNumber), isWon(isWon), totalVotes(totalVotes)  {
		for(auto&& mun : municipalities){
			municipalities.emplace_back(mun);
		}
	}

	void addMun(shared_ptr<Municipality> municipality){
		this->municipalities.emplace_back(municipality);
	}

	void removeMun(shared_ptr<Municipality> municipality){
		int position =0;
		for(auto&& mun : this->municipalities){
			if(mun == municipality){
				this->municipalities.erase(municipalities.begin()+ position);
				break;
			} 
			position++;
		}
	}
};

struct Solution{
	int nbCircWon; 
	vector<shared_ptr<Circumscription>> circumscriptions;

	Solution(){}

	Solution(int nbCircWon,vector<shared_ptr<Circumscription>> circumscriptions) : nbCircWon(nbCircWon) {
		for(auto&& circ : circumscriptions){
			circumscriptions.emplace_back(circ);
		}
	}

	~Solution(){}

};

# endif // !INCLUDES_H

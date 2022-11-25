#ifndef INCLUDES_H
#define INCLUDES_H

#include <iostream>
#include <cstring>
#include <sstream>
#include <vector>
#include <fstream>
#include <string>
#include <memory>

using namespace std;

struct Coord{
    int row;
    int column;

    // Default Ctor.
	Coord():
		row(-1), column(-1){}

	// Coord Ctor.
	Coord(int row_coord, int column_coord, )
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

// struct Circumscription{
// 	bool isWon;
// 	vector<shared_ptr<Municipality>> municipalities;

// 	Circumscription(){}

// 	Circumscription(bool isWon, const vector<shared_ptr<Municipality>> municipalities):isWon(isWon) {
// 		for(auto mun : municipalities){
// 			municipalities.push_back(mun);
// 		}
// 	}

// 	~Circumscription(){}
// };

// struct Solution{
// 	int nbCircWon; 
// 	vector<shared_ptr<Circumscription>> circumscriptions;

// 	Solution(){}

// 	Solution(int nbCircWon,vector<shared_ptr<Circumscription>> circumscriptions) : nbCircWon(nbCircWon) {
// 		for(auto circ : circumscriptions){
// 			circumscriptions.push_back(circ);
// 		}
// 	}

// 	~Solution(){}

// };

# endif // !INCLUDES_H

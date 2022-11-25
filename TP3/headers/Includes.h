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
    int column;
    int y;

    // Default Ctor.
	Coord():
		column(-1),y(-1){}

	// Coord Ctor.
	Coord(int x_coord, int y_coord)
		:column(x_coord), y(y_coord) {}
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
	Municipality(int x_coord, int y_coord, int votes)
		:coordinates(Coord(x_coord,y_coord)),
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

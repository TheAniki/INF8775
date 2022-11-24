#ifndef INCLUDES_H
#define INCLUDES_H

#include <iostream>
#include <cstring>
#include <sstream>
#include <vector>
#include <fstream>
#include <string>

using namespace std;

struct Coord{
    int x;
    int y;

    // Default Ctor.
	Coord():
		x(-1),y(-1){}

	// Coord Ctor.
	Coord(int x_coord, int y_coord)
		:x(x_coord), y(y_coord) {}
};

struct Municipality {
    Coord coordinates;
    int nbVotes; // Number of votes for the green party.
	float score; // score of the municipality.
	int nbVoisins = 0; // number of neighbors in eucli distance of 1.
    // Default Ctor.
	Municipality():
		coordinates(Coord()),nbVotes(-1){}

	// Municipality Ctor.
	Municipality(int x_coord, int y_coord, int votes)
		:coordinates(Coord(x_coord,y_coord)),
         nbVotes(votes) {}
};

# endif // !INCLUDES_H

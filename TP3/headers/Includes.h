#ifndef INCLUDES_H
#define INCLUDES_H

#include <iostream>
#include <cstring>
#include <sstream>
#include <vector>
#include <fstream>

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

    // Default Ctor.
	Municipality():
		coordinates(Coord()),nbVotes(-1){}

	// Municipality Ctor.
	Municipality(int x_coord, int y_coord, int votes)
		:coordinates(Coord(x_coord,y_coord)),
         nbVotes(votes) {}
};

# endif // !INCLUDES_H

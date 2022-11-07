#ifndef INCLUDES_H
#define INCLUDES_H

#include <map>
#include <vector> 
#include <utility>
#include <stdlib.h>     /* srand, rand */
#include <time.h>       /* time */
#include <iterator>
#include <iostream>
#include <fstream>
#include <filesystem>
#include <sstream>
#include <chrono>
#include <cstring>

using namespace std;

// Restaurant Struct
struct Restaurant {
	int iD;
	int revenue;
	int quantity;

	// Default Ctor
	Restaurant():
		iD(0),revenue(0),quantity(0){}

	// Restaurant Ctor
	Restaurant(int id, int r, int q)
		:iD(id), revenue(r), quantity(q) {}
};

#endif  // !INCLUDES_H

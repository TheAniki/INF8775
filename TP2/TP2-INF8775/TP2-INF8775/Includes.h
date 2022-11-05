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

using namespace std;

// Restaurant Struct
struct Restaurant {
	int revenue;
	int quantity;

	// Default Ctor
	Restaurant():
		revenue(0),quantity(0){}

	// Restaurant Ctor
	Restaurant(int r, int q)
		: revenue(r), quantity(q) {}
};


#endif  // !STRUCTS_H

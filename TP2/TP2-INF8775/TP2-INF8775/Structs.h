#ifndef STRUCTS_H
#define STRUCTS_H

// Restaurant Struct
struct Restaurant {
	int revenue;
	int quantity;

	// Restaurant Ctor
	Restaurant(int r, int q)
		: revenue(r), quantity(q) {}
};


#endif  // !STRUCTS_H

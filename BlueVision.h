#ifndef BLUEVISION_H
#define BLUEVISION_H

#include "BlueKnowledge.h"

class BlueVision
{

public:
	BlueVision();	// Default Constructor
        ~BlueVision();	// Destructor

	BlueKnowledge* lookAroundYou();	// Main Function

private:
	bool cardinalSearch(int, Piece_enum, int, int);	// board, side, search piece, x, y
	bool pawnSearch(int, int, int);	// side, x, y 

        BlueKnowledge *brain;
};
#endif // BLUEVISION_H


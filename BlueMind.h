#ifndef BLUEMIND_H
#define BLUEMIND_H

#include "BlueVision.h"
#include "BlueKnowledge.h"
#include "string.h"

class BlueMind
{

public:
	BlueMind();	// Default Constructor
	~BlueMind();	// Destructor

	Move* thinkItOver(int[8][8]);	// Main Function	
        void setBrain(BlueKnowledge);
        BlueKnowledge getBrain();

private:
        BlueVision vision;
	BlueKnowledge brain;
	Move the_move;
};
#endif // BLUEMIND_H

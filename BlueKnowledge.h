#ifndef BLUEKNOWLEDGE_H
#define BLUEKNOWLEDGE_H

#include "Move.h"
using namespace std;

class BlueKnowledge
{

public:
	BlueKnowledge();	// Default Constructor
	~BlueKnowledge();	// Destructor
 
        /* Getters & Setters */
	int getCheck();
        int getDebug();
        int getBoard(int, int);
        void setCheck(int);
	void setDebug(int);
	void setBoard(int, int, int);
        void printBoard();

private:
	int in_check;
	int debug;
	Move the_move;
	int board[8][8];
	int w_capturable[15];
	int b_capturable[15];
};
#endif // BLUEKNOWLEDGE_H

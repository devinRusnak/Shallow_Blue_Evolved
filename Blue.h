#ifndef BLUE_H
#define BLUE_H

#include <iostream>
#include "Move.h"
#include "Pieces.h"
using namespace std;

class Blue
{

public:
   
   Blue();	// default constructor
   ~Blue();	// destructor

   Move* decide(int[8][8], Pieces*);
   // getters and setters
   void setCheck(int);
   void setDebug(int);


private:

   void determineCapturable(int[8][8], Pieces*);// game board
   void determineKingVulrn(int[8][8]); 		// game board
   int cardinalSearch(int[8][8], Piece_enum, int, int);	// game board, search piece,x, y 
   int defenseKnightPawnSearch(int[8][8], int, int); // game board, x, y  

   Move the_move;
   bool in_check;
   bool debug;
   int array[16];
   int king_vulrn_w;
   int king_vulrn_b;

}; // end class
#endif // BLUE_H

#ifndef GAME_H
#define GAME_H

#include <string>
#include <iostream>
#include <stdio.h>
#include "Stats.h"
#include "Pieces.h"
#include "Updater.h"
#include "Move.h"
using namespace std;

class Game
{

public:
   // constructors
   Game();
   Game(int, int, const char*);

   // destructor
   ~Game();

   // public functions 
   Game* getGame();
   void start();
   void stop();
   void debugToggle();
   bool isActive();
   int validate(char*);			// input
   void printBoard();
   void printLogicalBoard();
   void recordMove(int, char*);		// turn, input
   void kingCheck(int);			// turn
   
private:
   // class instance (only ever one Game)
   Game *game_instance;

   // class variables
   int mode; 			// HvH, HvC, CvC
   int game_id;			// number
   int game_board[8][8];	// board representation
   int turn;			// count
   int debug;			// debug flag
   int in_check;		// check flag
   bool active;			// game is in progress?
   int cords[2];		// cordinate placeholder
   Stats recorder;		// statisitcs keeper
   Pieces the_pieces;		// the chess pieces
   Updater update;		// game logic
   Move *translated_move;	// filled out move object to check for validity

   // private class functions
   void initBoard();
   char itoaBoard(int);
   char itoaFile(int);
   int itoiLogical(int);
   void boardSwap(int,int,int,int,int);		// capture,x1,y1,x2,y2
   int cardinalSearch(Piece_enum,int,int,int); 	// piece being moved, turn, destX, destY
   int kingAround(int,int,int);			// turn, x_offset, y_offset
   int kingSafe(int,int,int,int);  		// mode, turn, x_offset, y_offset
   int moveSafe(int,int,int,int,int);		// turn, x1, y1, x2, y2

}; // end class
#endif // GAME_H

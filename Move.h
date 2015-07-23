#ifndef MOVE_H
#define MOVE_H
/* 
 * Move.h
 * @author Devin Rusnak 2015
 */
#include <stdlib.h>
#include <iostream>
using namespace std;

enum Piece_enum {King,Queen,Bishop,Rook,Knight,Pawn};

class Move
{
public:
   // Constructors
   Move();
   Move(Piece_enum,char,int,int); // Piece, rank, file, player
   ~Move();

   // Setters
   void setPiece(Piece_enum);
   void setRank(int);
   void setFile(char);
   void setPlayer(int);
   void setCapture(int);
   void setCastle(int);
   void setPromotion(int);
   void setEnPass(int);
   void setCheck(int);
   void setSpecify(int);

   // Getters
   Piece_enum getPiece();
   int getRank();
   int getFile();
   int getPlayer();
   int getCapture();
   int getValid();
   int getCastle();
   int getPromotion();
   int getEnPass();
   int getCheck();
   int getSpecify();

private:
   // Class variables
   enum Piece_enum piece;	// Piece being moved
   int dest[2];			// Piece destination coords
   int player;			// Player making move
   int capture;			// Flag for capture move
   int valid;			// Flag for out-of-bounds
   int castle;			// Flag for castle move
   int promotion;		// Flag for pawn promotion
   int enpass;			// Flag for en passant  
   int check;			// Flag for check
   int specify;			// Flag for ambiguity resolution

   // Internal methods
   void validityCheck();		
};
#endif // MOVE_H

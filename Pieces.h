#ifndef PIECES_H
#define PIECES_H
/*
 * Pieces.h
 * Devin Rusnak
 *
 * Note:
 *    0-7    -  Black's Pawns (left to right)
 *      8    -	Black's Left Rook
 * 	9    -  Black's Right Rook
 *      10   -  Black's Left Knight
 *	11   -  Black's Right Knight
 *	12   -  Black's Left Bishop
 *      13   -  Black's Right Bishop
 *	14   -  Black's Queen
 *	15   -  Black's King
 *
 *   16-23   -  White's Pawns (left to right)
 *      24   -  White's Left Rook
 *      25   -  White's Right Rook
 *      26   -  White's Left Knight
 *      27   -  White's Right Knight
 * 	28   -  White's Left Bishop
 *      29   -  White's Right Bishop
 *      30   -  White's Queen
 *      31   -  White's King
 */
#include <stdio.h>
#include <iostream>
#include "Piece.h"
using namespace std;

class Pieces 
{

public:
// constructors
   Pieces();
   ~Pieces();

// accessors
   Piece* getPiece(int);
   void setPiece(int, Piece*);

private:
// class data members
   Piece *piece_list[32];

// interal functions
   void initializePieces();

};
#endif // PIECES_H

/**
 *
 */
#include "Pieces.h"
using namespace std;

Pieces::Pieces()
{
  initializePieces();
}

Pieces::~Pieces()
{
  for(int i = 0; i < 32; i++)
     delete piece_list[i];
}

Piece* Pieces::getPiece(int i)
{
  return piece_list[i];
}

void Pieces::setPiece(int i, Piece *p)
{
  piece_list[i] = p;
}

void Pieces::initializePieces()
{
  for(int i = 0; i < 32; i++)
    piece_list[i] = new Piece(i,0,0);	/* initialize simple pieces */

  /* Add specific positions */
  piece_list[0]->setPosInt(0,1);	// Pawns
  piece_list[1]->setPosInt(1,1);
  piece_list[2]->setPosInt(2,1);
  piece_list[3]->setPosInt(3,1);
  piece_list[4]->setPosInt(4,1);
  piece_list[5]->setPosInt(5,1);
  piece_list[6]->setPosInt(6,1);
  piece_list[7]->setPosInt(7,1);
  
  piece_list[16]->setPosInt(0,6);
  piece_list[17]->setPosInt(1,6);
  piece_list[18]->setPosInt(2,6);
  piece_list[19]->setPosInt(3,6);
  piece_list[20]->setPosInt(4,6);
  piece_list[21]->setPosInt(5,6);
  piece_list[22]->setPosInt(6,6);
  piece_list[23]->setPosInt(7,6);

  piece_list[8]->setPosInt(0,0);	// Rooks
  piece_list[9]->setPosInt(7,0);
  piece_list[24]->setPosInt(0,7);
  piece_list[25]->setPosInt(7,7);

  piece_list[10]->setPosInt(1,0);	// Knights
  piece_list[11]->setPosInt(6,0);
  piece_list[26]->setPosInt(1,7);
  piece_list[27]->setPosInt(6,7);

  piece_list[12]->setPosInt(2,0);	// Bishops
  piece_list[13]->setPosInt(5,0);
  piece_list[28]->setPosInt(2,7);
  piece_list[29]->setPosInt(5,7);

  piece_list[14]->setPosInt(3,0);	// Queens
  piece_list[30]->setPosInt(3,7);

  piece_list[15]->setPosInt(4,0);	// Kings
  piece_list[31]->setPosInt(4,7);
} 

/*
 *
 *
 */

#include "Piece.h"
using namespace std;

// constructors
Piece::Piece() 
{
   piece_id = 0;
   position[0] = 0;
   position[1] = 0;
   pawn_flag = 0;
   is_alive = true;
   has_moved = false;
}

Piece::Piece(int id, int x, int y)
{
   piece_id = id;
   position[0] = x;
   position[1] = y;
   pawn_flag = 0;
   is_alive = true;
   has_moved = false;
}

Piece::Piece(Piece &p)
{
   piece_id = p.getID();
   position[0] = p.getPos()[0];
   position[1] = p.getPos()[1];
   pawn_flag = p.getPawnFlag();
   is_alive = p.isAlive();
   has_moved = p.isMoved();
}

Piece::~Piece()
{

}

void Piece::setID(int id)
{
   this->piece_id = id;
}

void Piece::setPos(int x, int y)
{
   if( this->isMoved() == false && 
     ( piece_id < 8 || (piece_id > 15 && piece_id < 24)) )
   	this->pawn_flag = 1;	
   this->has_moved = true;
   this->position[0] = x;
   this->position[1] = y;
}

void Piece::setPosInt(int x, int y)
{
   this->position[0] = x;
   this->position[1] = y;
}

void Piece::setAlive(bool a)
{
   this->is_alive = a;
}

void Piece::setMoved(bool m)
{
   this->has_moved = m;
}

int Piece::getID()
{
   return this->piece_id;
}

int* Piece::getPos()
{
   return this->position;
}

int Piece::getPawnFlag()
{
   return pawn_flag;
}

bool Piece::isAlive()
{
   return this->is_alive;
}

bool Piece::isMoved()
{
   return this->has_moved;
}

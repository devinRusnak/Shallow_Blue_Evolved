#ifndef PIECE_H
#define PIECE_H
/*
 * Piece.h
 * Devin Rusnak
 */

using namespace std;

class Piece 
{

public:
// constructors
   Piece();
   Piece(int,int,int);
   Piece(Piece&);
   ~Piece();   

// accessors
   void setID(int);
   void setPos(int,int);
   void setPosInt(int,int);
   void setAlive(bool);
   void setMoved(bool);
   int getID();
   int* getPos();
   int getPawnFlag();
   bool isAlive();
   bool isMoved();

private:
   int piece_id;
   int position[2];
   int pawn_flag;
   bool has_moved;
   bool is_alive;

};
#endif // PIECE_H

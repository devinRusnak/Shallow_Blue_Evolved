/*
 * Move.cpp
 * copyright Devin Rusnak 2015
 */
#include "Move.h"

/**
 *
 */
Move::Move()
{
  piece = Pawn;
  dest[0] = -1;
  dest[1] = -1;
  player = -1;
  file_ambig = -1;
  promotion = -1;
}

/**
 *
 */
Move::Move(Piece_enum p, char rank, int file, int playr)
{
  piece = p;
  dest[1] = file;
  player = playr;
  file_ambig = -1;
  promotion = -1;


  switch(rank)
  {
     case 'a': dest[0] = 0; break;
     case 'b': dest[0] = 1; break;
     case 'c': dest[0] = 2; break;
     case 'd': dest[0] = 3; break;
     case 'e': dest[0] = 4; break;
     case 'f': dest[0] = 5; break;
     case 'g': dest[0] = 6; break;
     case 'h': dest[0] = 7; break;
     default: cout << "error: rank out of bounds" << endl; dest[0] = -1; break;
  }
  validityCheck();
} 

/**
 *
 */
Move::~Move()
{

}

/**
 *
 */
void Move::setPiece(Piece_enum p)
{
  piece = p;
}

/**
 *
 */
void Move::setRank(int r)
{
  if(r < 0 || r > 8)
  {
     cout << "error: rank out of bounds" << endl;
     dest[1] = -1;
  }
  else
     dest[1] = r;
  
  validityCheck();
}

/**
 *
 */
void Move::setFile(char c)
{
  switch(c)
  {
    case 'a': dest[0] = 0; break;
    case 'b': dest[0] = 1; break;
    case 'c': dest[0] = 2; break;
    case 'd': dest[0] = 3; break;
    case 'e': dest[0] = 4; break;
    case 'f': dest[0] = 5; break;
    case 'g': dest[0] = 6; break;
    case 'h': dest[0] = 7; break;
    default: 
		cout << "error: file set" << endl; 
		dest[0] = -1;
  }
  validityCheck();
}

/**
 *
 */
void Move::setFileAmbig(char c)
{
 switch(c)
 {
    case 'a': file_ambig = 0; break;
    case 'b': file_ambig = 1; break;
    case 'c': file_ambig = 2; break;
    case 'd': file_ambig = 3; break;
    case 'e': file_ambig = 4; break;
    case 'f': file_ambig = 5; break;
    case 'g': file_ambig = 6; break;
    case 'h': file_ambig = 7; break;
    case '#': file_ambig = -1; break;
    default:
		cout << "error: file_ambig set" << endl;
                file_ambig = -1;
 }
}

/**
 *
 */
void Move::setPlayer(int p)
{
  player = p;
}

/**
 *
 */
void Move::setCapture(int c)
{
  capture = c;
}

/*
 *
 */
void Move::setCastle(int c)
{
  castle = c;
}

/**
 *
 */
void Move::setPromotion(int p)
{
  promotion = p;
}

/**
 *
 */
void Move::setEnPass(int e)
{
  enpass = e;
}

/**
 *
*/
void Move::setCheck(int c)
{
  check = c;
}

/**
 *
 */
void Move::setSpecify(int s)
{
	cout << s << endl;
  switch(s)
  {
     case 81: specify = 4; break;
     case 66: specify = 3; break;
     case 82: specify = 2; break;
     case 78: specify = 1; break;
     case  0: specify = 0; break;
     default:
		cout << "error: invalid promotion specify" << endl;
		specify = -1;
  }
}

/**
 *
 */
Piece_enum Move::getPiece()
{
  return piece;
}

/**
 *
 */
int Move::getRank()
{
  return dest[1];
}

/**
 *
 */
int Move::getFile()
{
  return dest[0];
}

/**
 *
 */
int Move::getFileAmbig()
{
  return file_ambig;
}

/**
 *
 */
int Move::getPlayer()
{
  return player;
}

/**
 *
 */
int Move::getCapture()
{
  return capture;
}

/**
 *
 */
int Move::getValid()
{
  return valid;
}

/**
 *
 */
int Move::getCastle()
{
  return castle;
}

/**
 *
 */
int Move::getPromotion()
{
  return promotion;
}

/**
 *
 */
int Move::getEnPass()
{
  return enpass;
}

/**
 *
 */
int Move::getCheck()
{
  return check;
}

/**
 *
 */
int Move::getSpecify()
{
  return specify;
} 

/**
 *
 */
void Move::validityCheck()
{
  if( dest[0] == -1 || dest[1] == -1 )
    valid = 0;
  else
    valid = 1;
}

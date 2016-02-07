/*!
 * \class Move Move.h
 * \brief An efficent representation of a desired move to be made.
 * \author Devin Rusnak
 * \date 2014-2016
 * \copyright GNU Public License.
 * 
 *   The general information stored in this class are: the piece
 * being moved, its intended destination, whose turn it is, and
 * what kind of move is intended to be made.
 */
#include "Move.h"

/*!
 * \memberof Move
 * \brief Default Constructor
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

/*!
 * \memberof Move
 * \brief Full-ish Constructor
 * 
 *   For the rank of the Piece in question the switch changes 
 * the vaule from a char that is used in the Algebraic Notation
 * to an integer that is much more easily used throughout the 
 * program.
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

/*!
 * \memberof Move
 * \brief Destructor
 */
Move::~Move()
{

}

/*!
 * setPiece(Piece_enum)
 * \memberof Move
 * \brief Sets the Piece being moved.
 * \param p - The enumerated value of the Piece being moved.
 */
void Move::setPiece(Piece_enum p)
{
  piece = p;
}

/*!
 * setRank(int)
 * \memberof Move
 * \brief Sets the rank of the intended destination.
 * \param r - The rank of the destination.
 * 
 *   Sets the rank of the intended destination and
 * does error checking that that the destination is not off 
 * the board. ValidityCheck() is called to set the valid flag.
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

/*!
 * setFile(char)
 * \memberof Move
 * \brief Sets the file of the intended destination.
 * \param c - The file of the destination.
 *
 *   The switch changes the file from a char to an int to allow
 * easier comparisons in other functions and allows for easy error
 * checking. At the end validityCheck() is called to set the valid 
 * flag accordingly.
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

/*!
 * setFileAmbig(char)
 * \memberof Move
 * \brief Sets the file of the piece that is intended to be moved.
 * \param c - The file of the piece being moved.
 *
 *   The switch changes the char file into an int to allow for easier
 * comparison in other functions and does error checking. The 
 * file_ambig is used to denote the intended piece to be moved in the 
 * case of amiguity.
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

/*!
 * setPlayer(int)
 * \memberof Move
 * \brief Sets player, which determins whose turn it is.
 * \param p - The value player is set to.
 */
void Move::setPlayer(int p)
{
  player = p;
}

/*!
 * setCapture(int)
 * \memberof Move
 * \brief Sets the capture flag to show the move type.
 * \param c - The value the capture flag is set to.
 */
void Move::setCapture(int c)
{
  capture = c;
}

/*!
 * setCastle(int)
 * \memberof Move
 * \brief Sets the castle flag to show the move type.
 * \param c - The value the castle flag is set to.
 */
void Move::setCastle(int c)
{
  castle = c;
}

/*!
 * setPromotion(int)
 * \memberof Move
 * \brief Sets the promotion flag to show the move type.
 * \param p - The value the promotion flag is set to.
 */
void Move::setPromotion(int p)
{
  promotion = p;
}

/*!
 * setEnPass(int)
 * \memberof Move
 * \brief Sets the en passant flag to show the move type.
 * \param e - The value the enpass flag is set to.
 */
void Move::setEnPass(int e)
{
  enpass = e;
}

/*!
 * setCheck(int)
 * \memberof Move
 * \brief Sets the check flag to show the opp. is in check.
 * \param c - The value the check flag is set to.
 */
void Move::setCheck(int c)
{
  check = c;
}

/*!
 * setSpecify(int)
 * \memberof Move
 * \brief Sets the specify flag to show which piece is being brought back.
 * \param s - The value the specify flag is set to.
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

/*!
 * getPiece()
 * \memberof Move
 * \brief Returns the enumerated value of the piece being moved.
 * \return The enumerated value of the piece being moved.
 */
Piece_enum Move::getPiece()
{
  return piece;
}

/*!
 * getRank()
 * \memberof Move
 * \brief Returns the rank of the destination.
 * \return The rank of the destination.
 */
int Move::getRank()
{
  return dest[1];
}

/*!
 * getFile()
 * \memberof Move
 * \brief Returns the file of the destination.
 * \return The file of the destination.
 */
int Move::getFile()
{
  return dest[0];
}

/*!
 * getFileAmbig()
 * \memberof Move
 * \brief Returns the file of the piece being moved.
 * \return The file of the piece being moved.
 */
int Move::getFileAmbig()
{
  return file_ambig;
}

/*!
 * getPlayer()
 * \memberof Move
 * \brief Returns the player whose turn it is.
 * \return The player whose turn it is.
 */
int Move::getPlayer()
{
  return player;
}

/*!
 * getCapture()
 * \memberof Move
 * \brief Returns the value of the capture flag.
 * \return The value of the capture flag.
 */
int Move::getCapture()
{
  return capture;
}

/*!
 * getValid()
 * \memberof Move
 * \brief Returns the value of the valid flag.
 * \return The value of the valid flag.
 */
int Move::getValid()
{
  return valid;
}

/*!
 * getCastle()
 * \memberof Move
 * \brief Returns the value of the castle flag.
 * \return The value of the castle flag.
 */
int Move::getCastle()
{
  return castle;
}

/*!
 * getPromotion()
 * \memberof Move
 * \brief Returns the value of the promotion flag.
 * \return The value of the promotion flag.
 */
int Move::getPromotion()
{
  return promotion;
}

/*!
 * getEnPass()
 * \memberof Move
 * \brief Returns the value of the en passant flag.
 * \return the value of the en passant flag.
 */
int Move::getEnPass()
{
  return enpass;
}

/*!
 * getCheck()
 * \memberof Move
 * \brief Returns the value of the check flag.
 * \return The value of the check flag.
 */
int Move::getCheck()
{
  return check;
}

/*!
 * getSpecify()
 * \memberof Move
 * \brief Returns the value of specify.
 * \return The value of specify.
 */
int Move::getSpecify()
{
  return specify;
} 

/*!
 * valudutyCheck()
 * \memberof Move
 * \brief Sets the valid flag.
 * 
 *   Checks to make sure the destination is on the board
 * and sets the valid flag accordingly.
 */
void Move::validityCheck()
{
  if( dest[0] == -1 || dest[1] == -1 )
    valid = 0;
  else
    valid = 1;
}

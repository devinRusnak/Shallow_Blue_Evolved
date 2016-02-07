/*!
 * \class Piece Piece.h
 * \brief Stores information about a piece.
 * \author Devin Rusank
 * \date 2014-2106
 * \copyright GNU Public License.
 *
 *   This class is used to hold all relevent information about an 
 * indvidual piece. Namely its position and flags needed for special
 * circumstances like castling and pawn promotion.
 */

#include "Piece.h"
using namespace std;

/*!
 * \brief Default Constuctor
 * \memberof Piece
 */
Piece::Piece() 
{
   piece_id = 0;
   position[0] = 0;
   position[1] = 0;
   pawn_flag = 0;
   is_alive = true;
   has_moved = false;
}

/*!
 * \brief Full-ish Constructor
 * \memberof Piece
 */
Piece::Piece(int id, int x, int y)
{
   piece_id = id;
   position[0] = x;
   position[1] = y;
   pawn_flag = 0;
   is_alive = true;
   has_moved = false;
}

/*!
 * \brief Copy Constructor
 * \memberof Piece
 */
Piece::Piece(Piece &p)
{
   piece_id = p.getID();
   position[0] = p.getPos()[0];
   position[1] = p.getPos()[1];
   pawn_flag = p.getPawnFlag();
   is_alive = p.isAlive();
   has_moved = p.isMoved();
}

/*!
 * \brief Destructor
 * \memberof Piece
 */
Piece::~Piece()
{

}

/*!
 * setID(int)
 * \memberof Piece
 * \brief Sets the calling piece's ID.
 * \param id - The ID given to set the piece's to.
 */
void Piece::setID(int id)
{
   this->piece_id = id;
}

/*!
 * setPos(int, int)
 * \memberof Piece
 * \brief Sets the X,Y cordinates of the calling piece.
 * \param x - Position on the x-axis.
 * \param y - Position on the y-axis.
 * 
 *   This is the setter for the cordinates that is used after
 * the game has started. Thus when a piece is moved its 
 * has_moved flag is changed and the pawn flag is set if the 
 * the piece is a pawn. These are used for pawn promotion,
 * castling, and pawn en-passant captures.
 */
void Piece::setPos(int x, int y)
{
   if( this->isMoved() == false && 
     ( piece_id < 8 || (piece_id > 15 && piece_id < 24)) )
   	this->pawn_flag = 1;	
   this->has_moved = true;
   this->position[0] = x;
   this->position[1] = y;
}

/*!
 * setPosInt(int, int)
 * \memberof Piece
 * \brief Sets the X,Y cordinates of the calling piece.
 * \param x - Position on the x-axis.
 * \param y - Position on the y-axis.
 *
 *   This is the setter for the cordinates that is used in 
 * the Pieces class when the Pieces are initially created.
 */
void Piece::setPosInt(int x, int y)
{
   this->position[0] = x;
   this->position[1] = y;
}

/*!
 * setAlive(bool)
 * \memberof Piece
 * \brief Sets the flag that represents if a piece is on the board.
 * \param a - The boolean value to set the calling piece's flag to.
 */
void Piece::setAlive(bool a)
{
   this->is_alive = a;
}

/*!
 * setMoved(bool)
 * \memberof Piece
 * \brief Sets the flag the shows if a piece has moved from its starting location.
 * \param m - The boolean value to set the calling piece's flag to.
 */
void Piece::setMoved(bool m)
{
   this->has_moved = m;
}

/*!
 * getID()
 * \memberof Piece
 * \brief Returns the ID of the calling Piece.
 * \return The ID of the calling Piece.
 */
int Piece::getID()
{
   return this->piece_id;
}

/*!
 * getPos()
 * \memberof Piece
 * \brief Returns the Position of the calling Piece in an int array.
 * \return A pointer to the int array that holds the calling Piece's position.
 */
int* Piece::getPos()
{
   return this->position;
}

/*!
 * getPawnFlag()
 * \memberof Piece
 * \brief Returns the pawn_flag of the calling Piece.
 * \return The calling Piece's pawn_flag.
 */
int Piece::getPawnFlag()
{
   return this->pawn_flag;
}

/*!
 * isAlive()
 * \memberof Piece
 * \brief Returns the is_alive flag of the calling Piece.
 * \return The boolean value of the calling Piece's is_alive flag.
 */
bool Piece::isAlive()
{
   return this->is_alive;
}

/*!
 * isMoved()
 * \memberof Piece
 * \brief Returns the has_moved flag of the calling Piece.
 * \return The boolean value of the calling Piece's has_moved flag.
 */
bool Piece::isMoved()
{
   return this->has_moved;
}

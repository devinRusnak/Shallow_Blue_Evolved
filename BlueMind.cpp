/*!
 * \class BlueMind  BlueMind.h Blue.h
 * \brief AI Controller
 * \author Devin Rusnak
 * \date 2014-2016
 * \copyright GNU Public License
 *
 * ...
 *
 */
#include "BlueMind.h"

/*!
 *
 */
BlueMind::BlueMind()
{
  
  // memset(&brain, 0, sizeof(BlueKnowledge));
}

/*!
 *
 */
BlueMind::~BlueMind()
{

}

/*!
 *
 */
Move* BlueMind::thinkItOver(int board[8][8])
{
   cout << "Blue is thinking..." << endl;
   /* update Blue's board */
   for(int y=0; y<8; y++)
   {
      for(int x=0; x<8; x++)
         brain.setBoard(board[x][y],x,y);
   }
   brain.printBoard();
   /* update vision based on new board */
   //vision.lookAroundYou();
	return 0;
}

/*!
 *
 */
void BlueMind::setBrain(BlueKnowledge b)
{
   brain = b;
}

/*!
 *
 */
BlueKnowledge BlueMind::getBrain()
{
   return brain;
}

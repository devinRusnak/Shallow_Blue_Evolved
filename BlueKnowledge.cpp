/*!
 * \class BlueKnowledge BlueKnowledge.h
 * \brief Stores information pertenent to BlueMind.
 * \author Devin Rusnak
 * \date 2014-2016
 * \copyright GNU Public License.
 *
 * ....
 */
#include "BlueKnowledge.h"


BlueKnowledge::BlueKnowledge() 
{
  for(int y=0; y<8; y++)
  {
     for(int x=0; x<8; x++)
        board[x][y] = -2;
  }
}

BlueKnowledge::~BlueKnowledge()
{ 

}

/*!
 *
 */
int BlueKnowledge::getCheck()
{
   return in_check;
}

/*!
 *
 */
int BlueKnowledge::getDebug()
{
   return debug;
}

/*!
 *
 */
int BlueKnowledge::getBoard(int x, int y)
{
   return board[x][y];
}

/*!
 *
 */
void BlueKnowledge::setCheck(int c)
{
   in_check = c;
}

/*!
 *
 */
void BlueKnowledge::setDebug(int d)
{
   debug = d;
}

/*!
 *
 */
void BlueKnowledge::setBoard(int val, int x, int y)
{
   for(int y=0; y<8; y++)
   {
     for(int x=0; x<8; x++)
       	board[x][y] = val;
   }
}

/*!
 *
 */
void BlueKnowledge::printBoard()
{
  for(int y=0; y<8; y++)
  {
     for(int x=0; x<8; x++)
     {
        cout << board[x][y] << " ";
        if(x==7)
          cout << endl;
     }
     cout << endl;
  }
}

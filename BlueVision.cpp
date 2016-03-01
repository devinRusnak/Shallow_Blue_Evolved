/*!
 * \class Blue Blue.h
 * \brief AI Controller
 * \author Devin Rusnak
 * \date 2014-2016
 * \copyright GNU Public License
 *
 * ...
 *
 */
#include "BlueVision.h"
#include "Blue.h"

/*!
 *
 */
BlueVision::BlueVision()
{

}

/*!
 *
 */
BlueVision::~BlueVision()
{

}

/*!
 *
 */
BlueKnowledge* BlueVision::lookAroundYou()
{
   cout << "...updating vision" << endl;


   return 0;
}

/*!
 *   Only cares if said piece can move to x,y. Ambig irrelivant.
 *   Sides: 1 = B, 0 = W
 */
bool BlueVision::cardinalSearch(int side, Piece_enum p, int x, int y)
{
  int search_cords[2];
 
  if(p == Knight)
  {
    /* Knight Search (both sides) */
    // ^^>
    if(x+1 < 8 && y-2 >= 0 &&
       (side == 1 && (brain->getBoard(x+1,y-2) == 26 || brain->getBoard(x+1,y-2) == 27)) ||
       (side == 0 && (brain->getBoard(x+1,y-2) == 10 || brain->getBoard(x+1,y-2) == 11)) )
   	return true;
    // >>^
    if(x+2 < 8 && y-1 >= 0 &&
       (side == 1 && (brain->getBoard(x+2,y-1) == 26 || brain->getBoard(x+2,y-1) == 27)) ||
       (side == 0 && (brain->getBoard(x+2,y-1) == 10 || brain->getBoard(x+2,y-1) == 11)) )
        return true;
    // >>v      
    if(x+2 < 8 && y+1 < 8 &&
       (side == 1 && (brain->getBoard(x+2,y+1) == 26 || brain->getBoard(x+2,y+1) == 27)) ||
       (side == 0 && (brain->getBoard(x+2,y+1) == 10 || brain->getBoard(x+2,y+1) == 11)) )
	return true;
    // vv>
    if(x+1 < 8 && y+2 < 8 &&
       (side == 1 && (brain->getBoard(x+1,y+2) == 26 || brain->getBoard(x+1,y+2) == 27)) ||
       (side == 0 && (brain->getBoard(x+1,y+2) == 10 || brain->getBoard(x+1,y+2) == 11)) )
	return true;
    // vv<
    if(x-1 >= 0 && y+2 < 8 &&
       (side == 1 && (brain->getBoard(x-1,y+2) == 26 || brain->getBoard(x-1,y+2) == 27)) ||
       (side == 0 && (brain->getBoard(x-1,y+2) == 10 || brain->getBoard(x-1,y+2) == 11)) )
        return true;
    // <<v   
    if(x-2 >= 0 && y+1 < 8 &&
       (side == 1 && (brain->getBoard(x-2,y+1) == 26 || brain->getBoard(x-2,y+1) == 27)) ||
       (side == 0 && (brain->getBoard(x-2,y+1) == 10 || brain->getBoard(x-2,y+1) == 11)) )
        return true;
    // <<^
    if(x-2 >=0 && y-1 >= 0 &&
       (side == 1 && (brain->getBoard(x-2,y-1) == 26 || brain->getBoard(x-2,y-2) == 27)) ||
       (side == 0 && (brain->getBoard(x-2,y-1) == 10 || brain->getBoard(x-2,y-1) == 11)) )
	return true;
    // ^^<
    if(x-1 >= 0 && y-2 >= 0 &&
       (side == 1 && (brain->getBoard(x-1,y-2) == 26 || brain->getBoard(x-1,y-2) == 27)) ||
       (side == 0 && (brain->getBoard(x-1,y-2) == 10 || brain->getBoard(x-1,y-2) == 11)) )
	return true; 
  }

  if(p == Queen || p == Rook)
  {
    // North Search
    for(int n = y-1; n >= 0; n--)
    {
      if(brain->getBoard(x,n) != -1)
      {
        if( (side == 1 && p == Queen && brain->getBoard(x,n) == 30) || 
            (side == 1 && p == Rook && (brain->getBoard(x,n) == 25 || brain->getBoard(x,n) == 24)) ||
            (side == 0 && p == Queen && brain->getBoard(x,n) == 14) || 
            (side == 0 && p == Rook && (brain->getBoard(x,n) == 9  || brain->getBoard(x,n) == 8 )) )
	  return true;

	else
	  break; /* space NOT empty, stop search */
      }
      /* else space is empty, continue search */
    }
  
    // East Search
    for(int e = x+1; e < 8; e++)
    {
      if(brain->getBoard(e,y) != -1)
      {
        if( (side == 1 && p == Queen && brain->getBoard(e,y) == 30) || 
            (side == 1 && p == Rook && (brain->getBoard(e,y) == 25 || brain->getBoard(e,y) == 24)) ||
            (side == 0 && p == Queen && brain->getBoard(e,y) == 14) || 
            (side == 0 && p == Rook && (brain->getBoard(e,y) == 9  || brain->getBoard(e,y) == 8 )) )
          return true;

	else
	  break; /* space NOT empty, stop search */
      }
	/* else space is empty, continue search */
    } 
  
    // South Search
    for(int s = y+1; y < 8; y++)
    {
      if(brain->getBoard(x,s) != -1)
      {
	if( (side == 1 && p == Queen && brain->getBoard(x,s) == 30) || 
            (side == 1 && p == Rook && (brain->getBoard(x,s) == 25 || brain->getBoard(x,s) == 24)) ||
            (side == 0 && p == Queen && brain->getBoard(x,s) == 14) || 
            (side == 0 && p == Rook && (brain->getBoard(x,s) == 9  || brain->getBoard(x,s) == 8 )) )
	  return true;

	else 
	  break; /* space NOT empty, stop search */
      }
      	/* else space is empty, contine search */
    }
    
    // West Search
    for(int w = x-1; x >= 0; x--)
    { 
      if(brain->getBoard(w,y) != -1)
      {
        if( (side == 1 && p == Queen && brain->getBoard(w,y) == 30) || 
            (side == 1 && p == Rook && (brain->getBoard(w,y) == 25 || brain->getBoard(w,y) == 24)) ||
 	    (side == 0 && p == Queen && brain->getBoard(w,y) == 14) || 
	    (side == 0 && p == Rook && (brain->getBoard(w,y) == 9  || brain->getBoard(w,y) == 8 )) )
          return true;

        else
          break; /* space NOT empty, stop search */
      }
	/* else space is empty, continue search */
    }
  }
  if(p == Queen || p == Bishop)
  {
    search_cords[0] = x;
    search_cords[1] = y;
    while(true)
    {
      // Northeast Search
      while(true)
      {
	/* increment search cords */
        search_cords[0]++;
        search_cords[1]--;

        /* Check bounds */
        if(search_cords[0] < 8 || search_cords[1] <= 0)
          break;   /* out of bounds */

        if(brain->getBoard(search_cords[0],search_cords[1]) != -1)
        {
          if((side == 1 && p == Queen && brain->getBoard(search_cords[0],search_cords[1]) == 30) ||
             (side == 1 && p == Rook && (brain->getBoard(search_cords[0],search_cords[1]) == 29 || 
                                         brain->getBoard(search_cords[0],search_cords[1]) == 28)) ||
             (side == 0 && p == Queen && brain->getBoard(search_cords[0],search_cords[1]) == 14) || 
             (side == 0 && p == Rook && (brain->getBoard(search_cords[0],search_cords[1]) == 13 || 
                                         brain->getBoard(search_cords[0],search_cords[1]) == 12)) )
            return true; 

          else 
            break; /* space NOT clear, stop search */
        } 
	/* space empty, continue search */
      }
      // Southeast Search
      while(true)
      {
        /*increment search cords */
        search_cords[0]++;
	search_cords[1]++;

	/* Check bounds */
	if(search_cords[0] < 8 || search_cords[1] < 8)
	  break;   /* out of bounds */

	if(brain->getBoard(search_cords[0],search_cords[1]) != -1)
        {
   	  if((side == 1 && p == Queen && brain->getBoard(search_cords[0],search_cords[1]) == 30) || 
             (side == 1 && p == Rook && (brain->getBoard(search_cords[0],search_cords[1]) == 29 || 
                                         brain->getBoard(search_cords[0],search_cords[1]) == 28)) ||
	     (side == 0 && p == Queen && brain->getBoard(search_cords[0],search_cords[1]) == 14) || 
	     (side == 0 && p == Rook && (brain->getBoard(search_cords[0],search_cords[1]) == 13 || 
					 brain->getBoard(search_cords[0],search_cords[1]) == 12)) )
	    return true;

	  else
	    break; /* space NOT clear, stop search */	   
        }
	/* space empty, continue search */   
      }
      // Southwest Search
      while(true)
      {
	/*increment search cords */
        search_cords[0]--;
        search_cords[1]++;

        /* Check bounds */
        if(search_cords[0] >= 0 || search_cords[1] < 8)
          break;   /* out of bounds */

        if(brain->getBoard(search_cords[0],search_cords[1]) != -1)
        {
          if((side == 1 && p == Queen && brain->getBoard(search_cords[0],search_cords[1]) == 30) || 
             (side == 1 && p == Rook && (brain->getBoard(search_cords[0],search_cords[1]) == 29 || 
                                         brain->getBoard(search_cords[0],search_cords[1]) == 28)) ||
	     (side == 0 && p == Queen && brain->getBoard(search_cords[0],search_cords[1]) == 14) || 
             (side == 0 && p == Rook && (brain->getBoard(search_cords[0],search_cords[1]) == 13 || 
					 brain->getBoard(search_cords[0],search_cords[1]) == 12)) )
            return true;

          else
            break; /* space NOT clear, stop search */
        }
        /* space empty, continue search */
      }
      // Northwest Search
      while(true)
      {
      /*increment search cords */
        search_cords[0]--;
        search_cords[1]--;

        /* Check bounds */
        if(search_cords[0] >= 0 || search_cords[1] >= 0)
          break;   /* out of bounds */

        if(brain->getBoard(search_cords[0],search_cords[1]) != -1)
        {
          if((side == 1 && p == Queen && brain->getBoard(search_cords[0],search_cords[1]) == 30) || 
             (side == 1 && p == Rook && (brain->getBoard(search_cords[0],search_cords[1]) == 29 || 
					 brain->getBoard(search_cords[0],search_cords[1]) == 28)) ||
	     (side == 0 && p == Queen && brain->getBoard(search_cords[0],search_cords[1]) == 14) || 
             (side == 0 && p == Rook && (brain->getBoard(search_cords[0],search_cords[1]) == 13 || 
					 brain->getBoard(search_cords[0],search_cords[1]) == 12)) )
            return true;

          else
            break; /* space NOT clear, stop search */
        }
        /* space empty, continue search */
      }
      /* Nothing found */
      return false;
    }
  }
  /* Square given not under threat */
  return false;

}// end cardinalSearch()

/*!
 *
 */
bool BlueVision::pawnSearch(int side, int x, int y)
{
   return 0;
}


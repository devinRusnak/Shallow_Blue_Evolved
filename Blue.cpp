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
#include "Blue.h"

/*!
 *
 */
Blue::Blue()
{
  in_check = 0;
  debug = 0;
}

/*!
 */
Blue::~Blue()
{

}

/*!
 *
 */
Move* Blue::decide(int board[8][8], Pieces *the_pieces)
{
  /* Being in check is an automatic desion to move out of */
  if(in_check)
  {
    if(debug)
      cout << "...escaping check" << endl;
  }

  /* Not in check, determine optimal move */
  else
  {
    if(debug)
      cout << "...updating vision" << endl;
    /* Run all the Vision Fucntions */
    determineCapturable(board, the_pieces);
    determineKingVulrn(board);

    the_move.setPlayer(1);
  

  }

  /* return filled out Move object */
  return &the_move;
}// end decide()

/*!
 *
 */
void Blue::determineCapturable(int board[8][8], Pieces *the_pieces)
{
  /* Cycle through all owned pieces */
  for(int p = 0; p < 15; p++)
  {
     if(cardinalSearch(board, Queen, the_pieces->getPiece(p)->getPos()[0], the_pieces->getPiece(p)->getPos()[1]) == 1)
    // TODO TODO TODO;
  }// end main loop
}

/*!
 *
 */
void Blue::determineKingVulrn(int board[8][8])
{

}

/*!
 *   Only cares if said piece can move to x,y. Ambig irrelivant.
 */
int Blue::cardinalSearch(int board[8][8], Piece_enum p, int x, int y)
{
  int search_cords[2];
  int rook_ambig = 0;
  int bishop_ambig = 0;
 
  if(p == Queen || p == Rook)
  {
    // North Search
    for(int n = y-1; n >= 0; n--)
    {
      if(board[x][n] != -1)
      {
        if(board[x][n] == 30 || board[x][n] == 25 || board[x][n] == 24)
	  return 1;
	else
	  break; /* space NOT empty, stop search */
      }
      /* else space is empty, continue search */
    }
  
    // East Search
    for(int e = x+1; e < 8; e++)
    {
      if(board[e][y] != -1)
      {
        if(board[e][y] == 30 || board[e][y] == 25 || board[e][y] == 24)
          return 1;
	else
	  break; /* space NOT empty, stop search */
      }
	/* else space is empty, continue search */
    } 
  
    // South Search
    for(int s = y+1; y < 8; y++)
    {
      if(board[x][s] != -1)
      {
	if(board[x][s] == 30 || board[x][s] == 25 || board[x][s] == 24)
	  return 1;
	else 
	  break; /* space NOT empty, stop search */
      }
      	/* else space is empty, contine search */
    }
    
    // West Search
    for(int w = x-1; x >= 0; x--)
    { 
      if(board[w][y] != -1)
      {
        if(board[w][y] == 30 || board[w][y] == 25 || board[w][y] == 24)
          return 1;
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
        if(board[search_cords[0]][search_cords[1]] != -1)
        {
          if(board[search_cords[0]][search_cords[1]] == 30 || board[search_cords[0]][search_cords[1]] == 29 || board[search_cords[0]][search_cords[1]] == 28)
            return 1; 
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
	if(board[search_cords[0]][search_cords[1]] != -1)
        {
   	  if(board[search_cords[0]][search_cords[1]] == 30 || board[search_cords[0]][search_cords[1]] == 29 || board[search_cords[0]][search_cords[1]] == 28)
	    return 1;
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
        if(board[search_cords[0]][search_cords[1]] != -1)
        {
          if(board[search_cords[0]][search_cords[1]] == 30 || board[search_cords[0]][search_cords[1]] == 29 || board[search_cords[0]][search_cords[1]] == 28)
            return 1;
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
        if(board[search_cords[0]][search_cords[1]] != -1)
        {
          if(board[search_cords[0]][search_cords[1]] == 30 || board[search_cords[0]][search_cords[1]] == 29 || board[search_cords[0]][search_cords[1]] == 28)
            return 1;
          else
            break; /* space NOT clear, stop search */
        }
        /* space empty, continue search */
      }
      /* Nothing found */
      return 0;
    }
  }
  /* Square given not under threat */
  cout << " UR yes? " << endl;
  return 0;
}

/*!
 *  only cares about capturablity, if one piece can do it, return 1
 */
int Blue::defenseKnightPawnSearch(int board[8][8], int x, int y)
{
  /* Knight Search */
  // ^^>
  if(x+1 < 8 && y-2 >= 0 &&
     board[x+1][y-2] == 26 ||
     board[x+1][y-2] == 27)
 	return 1;
  // >>^
  if(x+2 < 8 && y-1 >= 0 &&
     board[x+2][y-1] == 26 ||
     board[x+2][y-1] == 27)
        return 1;
  // >>v      
  if(x+2 < 8 && y+1 < 8 &&
     board[x+2][y+1] == 26 ||
     board[x+2][y+1] == 27)
	return 1;
  // vv>
  if(x+1 < 8 && y+2 < 8 &&
     board[x+1][y+2] == 26 ||
     board[x+1][y+2] == 27)
	return 1;
  // vv<
  if(x-1 >= 0 && y+2 < 8 &&
     board[x-1][y+2] == 26 ||
     board[x-1][y+2] == 27)
        return 1;
  // <<v   
  if(x-2 >= 0 && y+1 < 8 &&
     board[x-2][y+1] == 26 ||
     board[x-2][y+1] == 27)
        return 1;
  // <<^
  if(x-2 >=0 && y-1 >= 0 &&
     board[x-2][y-1] == 26 ||
     board[x-2][y-1] == 27)
	return 1;
  // ^^<
  if(x-1 >= 0 && y-2 >= 0 &&
     board[x-1][y-2] == 26 ||
     board[x-1][y-2] == 27)
	return 1;

  /* Pawn Check */
  if(x-1 >= 0 && y+1 < 8 &&
     board[x-1][y+1] > 15 &&
     board[x-1][y+1] < 24)
   	return 1;
  if(x+1 < 8 && y+1 < 8 &&
     board[x+1][y+1] > 15 &&
     board[x+1][y+1] < 24)
	return 1;

  return 0;
}

/*!
 *
 */
void Blue::setCheck(int c)
{
   in_check = c;
}

/*!
 *
 */
void Blue::setDebug(int d)
{
   debug = d;
}

/*  
 *  Game.cpp
 *  @author Devin Rusnak 2015
 */
#include "Game.h"
using namespace std;

/**
 *
 */
Game::Game()
{
   turn = 0;
   debug = 0;
   mode = 1;
   game_id = 1;
   active = 1;
   in_check = 0;
   memset(&translated_move, 0, sizeof(Move));
   initBoard();
}

/**
 *
 */
Game::Game(int m, int id, const char *file)
{
   turn = 0;
   debug = 0;
   mode = m;
   game_id = id;
   active = 1;
   in_check = 0;
   memset(&translated_move, 0, sizeof(Move));
   initBoard();
}

/**
 *
 */
Game::~Game()
{
   delete game_instance;
}

/**
 *
 */
void Game::start()
{
   active = 1;
}

/**
 *
 */
void Game::stop()
{
   active = 0;
   //TODO stop nicely
}

/**
 *
 */
void Game::debugToggle()
{
   if(debug)
   {
      debug = 0;
      update.setDebug(0);
   }
   else
   {
      debug = 1;
      update.setDebug(1);
   }
}

/**
 *
 */
bool Game::isActive() 
{
   return active;
}

/** TODO
 *
 */
int Game::validate(char *c)
{
   int pos[2];		// destination 
   int x=0, y=0;	// coordinates holders
   int ambiguous = 0;	// ambiguous counter

   /* Translate the c_string to a Move class object */
   translated_move = update.translateMove(turn,c);

   /* Check for input error */
   if(translated_move == 0)
   {
 	cout << "error: translation" << endl;
	return 0;
   }

   pos[0] = translated_move->getFile();			// destination X (file)
   pos[1] = itoiLogical(translated_move->getRank());	// destination Y (rank)

   if(debug)
      cout << "\n" << translated_move->getPiece() << " " << translated_move->getRank()
           << " " << translated_move->getFile() << " " << translated_move->getPlayer() 
	   << " " << pos[0] << " " << pos[1] << endl; 

   /* Check for quit flag */
   if(translated_move != NULL && 
      translated_move->getPlayer() == -2)
   {
      active = false;	// flag stop
      return 1;		// signal a valid move
   }
   
   /**********
    *  Pawn  *
    **********/
   if(translated_move->getPiece() == Pawn)
   {  
     /**
      *  Double First Move
      */
      if(translated_move->getPlayer() == 0)	/* White's turn */
      {
         if(pos[1]+2 == 6 &&
            game_board[pos[0]][pos[1]] == -1 &&		// TODO Can they hop first move??? TODO
            game_board[pos[0]][pos[1]+1] == -1 &&
            game_board[pos[0]][pos[1]+2] < 24 &&
            game_board[pos[0]][pos[1]+2] > 15)
         {
            /* valid pawn move */
	    boardSwap(0,pos[0],pos[1]+2,pos[0],pos[1]);	// update board
            turn++;					// increment turn
            return 1;					// return valid
         }
      }
      else					/* Black's turn */
      {
         if(pos[1]-2 == 1 &&
            game_board[pos[0]][pos[1]] == -1 &&
            game_board[pos[0]][pos[1]-1] == -1 &&
            game_board[pos[0]][pos[1]-2] < 8 &&
            game_board[pos[0]][pos[1]-2] >= 0)
         {
	    /* valid pawn move */
	    boardSwap(0,pos[0],pos[1]-2,pos[0],pos[1]);	// update board
            turn++;					// increment turn
            return 1;					// return valid
         }
      } /* end double move */

     /**
      *  Simple Move
      */
      if(translated_move->getPlayer() == 0)	/* White's turn */
      {
         if(game_board[pos[0]][pos[1]] == -1 &&
            game_board[pos[0]][pos[1]+1] < 24 && 
            game_board[pos[0]][pos[1]+1] > 15)
         {
            /* valid pawn move */
	    boardSwap(0,pos[0],pos[1]+1,pos[0],pos[1]);	// update board
            turn++;					// increment turn
            return 1;					// return valid
         }  
      }
      else					/* Black's Turn */
      {
         if(game_board[pos[0]][pos[1]] == -1 &&
            game_board[pos[0]][pos[1]-1] < 8 &&
            game_board[pos[0]][pos[1]-1] >= 0)
         {
            /* valid pawn move */
            boardSwap(0,pos[0],pos[1]-1,pos[0],pos[1]);	// update board
            turn++;					// increment turn
            return 1;					// return valid
         }
      } /* end simple move */

     /**
      *  Attack Move
      */
      if(translated_move->getCapture() == 1 && game_board[pos[0]][pos[1]] != -1)
      {
	 if(translated_move->getPlayer() == 0)	/* White's Turn */
	 {
	    /* check for presence of opp. piece */
	    if(game_board[pos[0]][pos[1]] < 16)	
	    {
		// Ambiguity check
		if( (game_board[pos[0]-1][pos[1]+1] > 15 && game_board[pos[0]-1][pos[1]+1] < 24) && 
		    (game_board[pos[0]+1][pos[1]+1] > 15 && game_board[pos[0]+1][pos[1]+1] < 24) )
		{
		    cout << "error: ambiguous move, specify file and/or rank.\n" << endl;
		    return 0;
		}
		else if( game_board[pos[0]-1][pos[1]+1] > 15 && game_board[pos[0]-1][pos[1]+1] < 24 )
		{
                   /* valid pawn capture */
		   boardSwap(1,pos[0]-1,pos[1]+1,pos[0],pos[1]);	// update board
		   turn++;						// increment turn
		   return 1;						// return valid
		}
		else if( game_board[pos[0]+1][pos[1]+1] > 15 && game_board[pos[0]+1][pos[1]+1] < 24 )
 		{
                   /* valid pawn capture */
		   boardSwap(1,pos[0]+1,pos[1]+1,pos[0],pos[1]);	// update board
                   turn++;						// increment turn
                   return 1;						// return valid
		}
	    }
	 }
	 else					/* Black's Turn */
	 {
            /* check for presence of opp. piece */
  	    if(game_board[pos[0]][pos[1]] > 15)
	    {
	    	// Ambiguity check
                if( (game_board[pos[0]-1][pos[1]-1] >= 0 && game_board[pos[0]-1][pos[1]-1] < 8) &&
                    (game_board[pos[0]+1][pos[1]-1] >= 0 && game_board[pos[0]+1][pos[1]-1] < 8) )
                {
                    cout << "error: ambiguous move, specify file and/or rank.\n" << endl;
                    return 0;
                }
                else if( game_board[pos[0]-1][pos[1]-1] >= 0 && game_board[pos[0]-1][pos[1]-1] < 8 )
                {
                   /* valid pawn capture */
                   boardSwap(1,pos[0]-1,pos[1]-1,pos[0],pos[1]);	// update board
                   turn++;						// increment turn
                   return 1;						// return valid
                }
                else if( game_board[pos[0]+1][pos[1]-1] >= 0 && game_board[pos[0]+1][pos[1]-1] < 8 )
                {
                   /* valid pawn capture */
                   boardSwap(1,pos[0]+1,pos[1]-1,pos[0],pos[1]);	// update board
                   turn++;						// increment turn
                   return 1;						// return valid
                }
    	    }
	 }
      } /* end attack */

     /**
      *  Pawn En Passant
      */
      if(translated_move->getCapture() == 1 )
      {
     		// TODO TODO TODO
      } /* end en passant */ 

   } /* end pawn */
   
  /************
   *  Knight  *
   ************/
   else if(translated_move->getPiece() == Knight) 
   { 
       if(translated_move->getPlayer() == 0)		/* White's Turn */
       {
	   if( pos[0]+1 < 8 && pos[1]-2 >= 0 && 
               game_board[pos[0]+1][pos[1]-2] == 26 || 
	       game_board[pos[0]+1][pos[1]-2] == 27 )
           {
                /* found a knight */
		x = pos[0]+1; 
		y = pos[1]-2;
                ambiguous++;
           }
 	   if( pos[0]+2 < 8 && pos[1]-1 >= 0 &&
               game_board[pos[0]+2][pos[1]-1] == 26 ||
               game_board[pos[0]+2][pos[1]-1] == 27 )
           {
                /* found a knight */
		x = pos[0]+2;
            	y = pos[1]-1;
		ambiguous++;
	   }
	   if( pos[0]+2 < 8 && pos[1]+1 < 8 &&
               game_board[pos[0]+2][pos[1]+1] == 26 ||
	       game_board[pos[0]+2][pos[1]+1] == 27 )
           {
                /* found a knight */
		x = pos[0]+2;
		y = pos[1]+1;
 		ambiguous++;
	   }
	   if( pos[0]+1 < 8 && pos[1]+2 < 8 &&
 	       game_board[pos[0]+1][pos[1]+2] == 26 ||
	       game_board[pos[0]+1][pos[1]+2] == 27 )
	   {
                /* found a knight */
		x = pos[0]+1;
		y = pos[1]+2;
		ambiguous++;
	   }
	   if( pos[0]-1 >= 0 && pos[1]+2 < 8 &&
	       game_board[pos[0]-1][pos[1]+2] == 26 ||
	       game_board[pos[0]-1][pos[1]+2] == 27 )
	   {
                /* found a knight */
		x = pos[0]-1;
		y = pos[1]+2;
		ambiguous++;
	   }
	   if( pos[0]-2 >= 0 && pos[1]+1 < 8 &&
	       game_board[pos[0]-2][pos[1]+1] == 26 ||
	       game_board[pos[0]-2][pos[1]+1] == 27 )
	   {
		/* found a knight */
		x = pos[0]-2;
		y = pos[1]+1;
		ambiguous++;
	   }
	   if( pos[0]-2 >= 0 && pos[1]-1 >= 0 &&
	       game_board[pos[0]-2][pos[1]-1] == 26 ||	
	       game_board[pos[0]-2][pos[1]-1] == 27 )
	   {
		/* found a knight */
		x = pos[0]-2;
		y = pos[1]-1;
		ambiguous++;
	   }
	   if( pos[0]-1 >= 0 && pos[1]-2 >= 0 &&
	       game_board[pos[0]-1][pos[1]-2] == 26 ||
	       game_board[pos[0]-1][pos[1]-2] == 27 )
	   {
                /* found a knight */
		x = pos[0]-1;
		y = pos[1]-2;
		ambiguous++;
	   }
 	   if(debug)
              cout << "ambig: " << ambiguous << endl;

	   if( ambiguous == 1)
	   {
              /* found a single knight that can move to destination */
	      if(game_board[pos[0]][pos[1]] == -1)
                 boardSwap(0,x,y,pos[0],pos[1]);	// a move 
	      else if(translated_move->getCapture() == 1 && game_board[pos[0]][pos[1]] < 16)
                 boardSwap(1,x,y,pos[0],pos[1]);	// or a capture 
              turn++;					// increment turn
              return 1;					// return valid
	   }
	   else if( ambiguous > 1)	
           {
		/* ambiguous move given */
		cout << "error: ambiguous move, specify file/rank." << endl;
		return 0;	
	   }
           return 0;	/* invalid move catch */
       }
       else					/* Black's Turn */
       {
           if( pos[0]+1 < 8 && pos[1]-2 >= 0 &&
               game_board[pos[0]+1][pos[1]-2] == 10 ||
               game_board[pos[0]+1][pos[1]-2] == 11 )
           {
		/* found a knight */
                x = pos[0]+1;
                y = pos[1]-2;
                ambiguous++;
           }
           if( pos[0]+2 < 8 && pos[1]-1 >= 0 &&
               game_board[pos[0]+2][pos[1]-1] == 10 ||
               game_board[pos[0]+2][pos[1]-1] == 11 )
           {
		/* found a knight */
                x = pos[0]+2;
                y = pos[1]-1;
                ambiguous++;
           }
           if( pos[0]+2 < 8 && pos[1]+1 < 8 &&
               game_board[pos[0]+2][pos[1]+1] == 10 ||
               game_board[pos[0]+2][pos[1]+1] == 11 )
           {
		/* found a knight */
                x = pos[0]+2;
                y = pos[1]+1;
                ambiguous++;
           }
           if( pos[0]+1 < 8 && pos[1]+2 < 8 &&
               game_board[pos[0]+1][pos[1]+2] == 10 ||
               game_board[pos[0]+1][pos[1]+2] == 11 )
           {
		/* found a knight */
                x = pos[0]+1;
                y = pos[1]+2;
                ambiguous++;
           }
           if( pos[0]-1 >= 0 && pos[1]+2 < 8 &&
               game_board[pos[0]-1][pos[1]+2] == 10 ||
               game_board[pos[0]-1][pos[1]+2] == 11 )
           {
                /* found a knight */
                x = pos[0]-1;
                y = pos[1]+2;
                ambiguous++;
           }
           if( pos[0]-2 >= 0 && pos[1]+1 < 8 &&
               game_board[pos[0]-2][pos[1]+1] == 10 ||
               game_board[pos[0]-2][pos[1]+1] == 11 )
           {
		/* found a knight */
                x = pos[0]-2;
                y = pos[1]+1;
                ambiguous++;
           }
           if( pos[0]-2 >= 0 && pos[1]-1 >= 0 &&
               game_board[pos[0]-2][pos[1]-1] == 10 ||
               game_board[pos[0]-2][pos[1]-1] == 11 )
           {
		/* found a knight */
                x = pos[0]-2;
                y = pos[1]-1;
                ambiguous++;
           }
           if( pos[0]-1 >= 0 && pos[1]-2 >= 0 &&
               game_board[pos[0]-1][pos[1]-2] == 10 ||
               game_board[pos[0]-1][pos[1]-2] == 11 )
           {
		/* found a knight */
                x = pos[0]-1;
                y = pos[1]-2;
                ambiguous++;
           }
           if(debug)
              cout << "ambig: " << ambiguous << endl;

           if( ambiguous == 1)
           {
	      /* found a single knight that can move to destination */
              if(game_board[pos[0]][pos[1]] == -1)
                 boardSwap(0,x,y,pos[0],pos[1]);        // a move
              else if(translated_move->getCapture() == 1 && game_board[pos[0]][pos[1]] > 15)         
                 boardSwap(1,x,y,pos[0],pos[1]);        // a capture  
              turn++;					// increment turn
              return 1;					// return valid
           }
           else if( ambiguous > 1 ) 
	   {
		/* ambiguous move given */
                cout << "error: ambiguous move, specify file/rank." << endl;
                return 0;
           }
           return 0;	/* invalid move catch */
       }
   } /* end knight */

   /**
    *  Rook
    */
   else if(translated_move->getPiece() == Rook) 
   {
     if(translated_move->getPlayer() == 0) 		/* White's turn */
     {
       /* check for ambiguous move */
       if(cardinalSearch(Rook, 0, pos[0], pos[1]) == 1)
       {
         if(game_board[pos[0]][pos[1]] == -1)
         {
		/* valid rook move */
         	boardSwap(0, cords[0], cords[1], pos[0], pos[1]);	// update board
                turn++;							// increment turn
                return 1;						// return valid
         }
         else if(translated_move->getCapture() == 1 && game_board[pos[0]][pos[1]] < 16)
         {
		/* valid rook capture */
         	boardSwap(1, cords[0], cords[1], pos[0], pos[1]);	// update board
                turn++;							// increment turn
 		return 1;						// return valid
         }
       }
       return 0; /* invalid move catch */
     }
     else						/* Black's turn */
     {
       /* check for ambiguous move */
       if(cardinalSearch(Rook, 1, pos[0], pos[1]) == 1)
       {
         if(game_board[pos[0]][pos[1]] == -1)
         {
		/* valid rook move */
                boardSwap(0, cords[0], cords[1], pos[0], pos[1]);	// update board
   		turn++;							// increment turn
		return 1;						// return valid
         }
         else if(translated_move->getCapture() == 1 && game_board[pos[0]][pos[1]] > 15)
         {
		/* valid rook capture */
                boardSwap(1, cords[0], cords[1], pos[0], pos[1]);	// update board
		turn++;							// increment turn
 		return 1;						// return valid
 	 }
	 return 0; /* invalid move catch */
       }
     }
   } /* end rook */

   /**
    *  Bishop
    */
   else if(translated_move->getPiece() == Bishop) 
   {
     if(translated_move->getPlayer() == 0) 		/* White's turn */
     {
       /* check for ambiguous move */
       if(cardinalSearch(Bishop, 0, pos[0], pos[1]) == 1)
       {
         if(game_board[pos[0]][pos[1]] == -1)
         {
		/* valid bishop move */
                boardSwap(0, cords[0], cords[1], pos[0], pos[1]);	// update board
                turn++;							// increment turn
                return 1;						// return valid 
         }
         else if(translated_move->getCapture() == 1 && game_board[pos[0]][pos[1]] < 16)
         {
		/* valid bishop capture */
                boardSwap(1, cords[0], cords[1], pos[0], pos[1]);	// update board
                turn++;							// increment turn
                return 1;						// return valid
         }
         return 0; /* invalid move catch */
       }
     }
     else       					/* Black's turn */
     {
       /* check for ambiguous move */
       if(cardinalSearch(Bishop, 1, pos[0], pos[1]) == 1)
       {
         if(game_board[pos[0]][pos[1]] == -1)
         {
         	/* valid bishop move */
                boardSwap(0, cords[0], cords[1], pos[0], pos[1]);	// update board
                turn++;							// increment turn
                return 1;						// return valid
         }
         else if(translated_move->getCapture() == 1 && game_board[pos[0]][pos[1]] > 15)
         {
		/* valid bishop capture */
                boardSwap(1, cords[0], cords[1], pos[0], pos[1]);	// update board
                turn++;							// increment turn
                return 1;						// return valid
         }
         return 0; /* invalid move catch */
       }
     }
   } /* end bishop */

   /**
    *  Queen
    */
   else if(translated_move->getPiece() == Queen) 
   {
     if(translated_move->getPlayer() == 0) 		/* White's turn */
     {
       /* check for queen's existence */
       if(cardinalSearch(Queen, 0, pos[0], pos[1]) == 1)
       {
         if(game_board[pos[0]][pos[1]] == -1)
         {
		/* valid queen move */
                boardSwap(0, cords[0], cords[1], pos[0], pos[1]);	// update board
                turn++;							// increment turn
                return 1;						// return valid
         }
         else if(translated_move->getCapture() == 1 && game_board[pos[0]][pos[1]] < 16)
         {
		/* valid queen capture */
                boardSwap(1, cords[0], cords[1], pos[0], pos[1]);	// update board
                turn++;							// increment turn
                return 1;						// return valid 
         }
         return 0; /* invalid move catch */
       }
     }
     else       /* Black's turn */
     {
       if(cardinalSearch(Queen, 1, pos[0], pos[1]) == 1)
       {
         if(game_board[pos[0]][pos[1]] == -1)
         {
                boardSwap(0, cords[0], cords[1], pos[0], pos[1]);
                turn++;
                return 1;
         }
         else if(translated_move->getCapture() == 1 && game_board[pos[0]][pos[1]] > 15)
         {
                boardSwap(1, cords[0], cords[1], pos[0], pos[1]);
                turn++;
                return 1;
         }
         /* else invalid move */
       }
     } 
   } /* end queen */

   /**
    *  King
    */
   else if(translated_move->getPiece() == King) 
   {

   } /* end king */
   else
   {
     /* else invalid move given */
     cout << "5error: invalid move" << endl;
     return 0;
   }
} /* end validate */

/**
 *
 */
int Game::kingSafe(int mode, int turn, int x_offset, int y_offset)
{
   int x,y;	  // king's x,y
   int in_check;  // check flag

   // Checkmate Check
   if(mode == 0)
   {
     if(turn == 0)			/* White's turn */
     {
       if( kingSafe(1,0,0,0) == 1 ) 	/* King is currnetly in check */
       {
          if(debug)
             cout << "white king in check" << endl;
      
       	  /* search for a free space that would not put king into check */
          x = the_pieces.getPiece(31)->getPos()[0];
          y = the_pieces.getPiece(31)->getPos()[1];

	  // north
          if( y-1 >= 0 && kingSafe(1,0,0,-1) == 0  && kingAround(0, x, y-1) == 0 &&
              game_board[x][y-1] < 15 || game_board[x][y-1] == -1 )
          	return 0;
          // northeast
          else if( x+1 < 8 && y-1 >= 0 && kingSafe(1,0,1,-1) == 0 && kingAround(0, x+1, y-1) == 0 &&
                   game_board[x+1][y-1] < 15 || game_board[x+1][y-1] == -1 )
          	return 0;
          // east
          else if( x+1 < 8 && kingSafe(1,0,1,0) == 0 && kingAround(0, x+1, y) == 0 &&
                   game_board[x+1][y] < 15 || game_board[x+1][y] == -1 )
          	return 0;
          // southeast
          else if( x-1 >= 0 && y+1 < 8 && kingSafe(1,0,-1,1) == 0 && kingAround(0, x-1, y+1) == 0 &&
                   game_board[x-1][y+1] < 15 || game_board[x-1][y+1] == -1 )
       		return 0;
          // south
          else if( y+1 < 8 && kingSafe(1,0,0,1) == 0 && kingAround(0, x, y+1) == 0 &&
                   game_board[x][y+1] < 15 || game_board[x][y+1] == -1 )
          	return 0;
          // southwest
  	  else if( x-1 >= 0 && y+1 < 8 && kingSafe(1,0,-1,1) == 0 && kingAround(0, x-1, y+1) == 0 &&
                   game_board[x-1][y+1] < 15 || game_board[x-1][y+1] == -1 )
          	return 0;
          // west
          else if( x-1 >= 0 && kingSafe(1,0,-1,0) == 0 && kingAround(0, x-1, y) == 0 &&
                   game_board[x-1][y] < 15 || game_board[x-1][y] == -1 )
          	return 0;
          // northwest
          else if( x-1 >= 0 && y-1 >= 0 && kingSafe(1,0,-1,-1) == 0 && kingAround(0, x-1, y-1) == 0 &&
                   game_board[x-1][y-1] < 15 || game_board[x-1][y-1] == -1 )
          	return 0;
          /**
           *  Checkmate
           */
          else
          {
                cout << game_board[0][0] << endl;
		if(debug)
		   cout << "checkmate white" << endl;

                // TODO stuff
		return 1;
          }
       } 
     }

     else			/* Black's Turn */
     {
       if( kingSafe(1,1,0,0) == 1 )	/* king is in check */
       {
          if(debug)
             cout << "black king in check" << endl;

	  /* search for a free space that would not put king into check */
          x = the_pieces.getPiece(31)->getPos()[0];
          y = the_pieces.getPiece(31)->getPos()[1];

          // north
          if( y-1 >= 0 && kingSafe(1,0,0,-1) == 0  && kingAround(0, x, y-1) == 0 &&
              game_board[x][y-1] < 15 || game_board[x][y-1] == -1 )
                return 0;
          // northeast
          else if( x+1 < 8 && y-1 >= 0 && kingSafe(1,0,1,-1) == 0 && kingAround(0, x+1, y-1) == 0 &&
                   game_board[x+1][y-1] < 15 || game_board[x+1][y-1] == -1 )
                return 0;
          // east
          else if( x+1 < 8 && kingSafe(1,0,1,0) == 0 && kingAround(0, x+1, y) == 0 &&
                   game_board[x+1][y] < 15 || game_board[x+1][y] == -1 )
                return 0;
          // southeast
          else if( x-1 >= 0 && y+1 < 8 && kingSafe(1,0,-1,1) == 0 && kingAround(0, x-1, y+1) == 0 &&
                   game_board[x-1][y+1] < 15 || game_board[x-1][y+1] == -1 )
                return 0;
          // south
          else if( y+1 < 8 && kingSafe(1,0,0,1) == 0 && kingAround(0, x, y+1) == 0 &&
                   game_board[x][y+1] < 15 || game_board[x][y+1] == -1 )
                return 0;
          // southwest
          else if( x-1 >= 0 && y+1 < 8 && kingSafe(1,0,-1,1) == 0 && kingAround(0, x-1, y+1) == 0 &&
                   game_board[x-1][y+1] < 15 || game_board[x-1][y+1] == -1 )
                return 0;
          // west
          else if( x-1 >= 0 && kingSafe(1,0,-1,0) == 0 && kingAround(0, x-1, y) == 0 &&
                   game_board[x-1][y] < 15 || game_board[x-1][y] == -1 )
                return 0;
          // northwest
          else if( x-1 >= 0 && y-1 >= 0 && kingSafe(1,0,-1,-1) == 0 && kingAround(0, x-1, y-1) == 0 &&
                   game_board[x-1][y-1] < 15 || game_board[x-1][y-1] == -1 )
                return 0;
	  /**
           *  Checkmate
           */
          else
          {
                cout << game_board[0][0] << endl;
		if(debug)
		   cout << "checkmate black" << endl;
                
                // TODO stuff
		return 1;
          }
       }
     }

   } /* end mode 0 */

   // Check
   else if(mode == 1)
   {
     if(turn == 0 )		/* White's Turn */
     {
        x = the_pieces.getPiece(31)->getPos()[0] + x_offset;
        y = the_pieces.getPiece(31)->getPos()[1] + y_offset;
        if(debug)
	  cout << "white king @ " << x << ", " << y << endl;
  
        if( cardinalSearch(Queen, 1, x, y) != 0 || 
            cardinalSearch(Bishop, 1, x, y) != 0 ||
            cardinalSearch(Rook, 1, x, y) != 0 ||
            cardinalSearch(Knight, 1, x, y) != 0 )
        {
          if(debug)
             cout << "Q/B/R/K check" << endl;
	  in_check = 1;
          return 1;
        }
        // look for pawns
        else if( x-1 >= 0 && y-1 >= 0 && 
                 game_board[x-1][y-1] >= 0 && game_board[x-1][y-1] < 8 )
        {
          if(debug)
	    cout << "Pawn check" << endl;
          in_check = 1;
          return 1;
        }
        else if( x+1 < 8 && y-1 >= 0 &&
                 game_board[x+1][y-1] >= 0 && game_board[x+1][y-1] < 8 )
        {
          if(debug)
	    cout << "Pawn check" << endl;
          in_check = 1;
          return 1;
        }
        else if( kingAround(0, x, y) == 1 )
        {
          if(debug)
	    cout << "King check" << endl;
          in_check = 1;
          return 1;
        }
        // look for king
        
        /* else not in check */
        return 0;
     }
     else		/* Black's Turn */
     {
        x = the_pieces.getPiece(15)->getPos()[0] + x_offset;
        y = the_pieces.getPiece(15)->getPos()[1] + y_offset;

        if( cardinalSearch(Queen, 0, x, y) != 0 ||
            cardinalSearch(Bishop, 0, x, y) != 0 ||
            cardinalSearch(Rook, 0, x, y) != 0 ||
            cardinalSearch(Knight, 0, x, y) != 0 )
        {
          in_check = 1;
          return 1;
        }
        // Look for pawns
        else if( x-1 >= 0 && y-1 >= 0 &&
                 game_board[x-1][y-1] > 15 && game_board[x-1][y-1] < 24 )
        {
          in_check = 1;
          return 1;
        }
        else if( x+1 < 8 && y-1 >= 0 &&
                 game_board[x+1][y-1] > 15 && game_board[x+1][y-1] < 24 )
        {
          in_check = 1;
          return 1;
        }
        else if( kingAround(1, x, y) == 1 )
        {
          in_check = 1;
          return 1;
        }
        /* else not in check */
        return 0;
     }

   } /* end mode 1 */

} /* end king safe */

/**
 *
 */
int Game::moveSafe(int turn, int x1, int y1, int x2, int y2)
{
   int temp_board[8][8];		// saved copy of game_board whilst I mess with it
   int x, y;				// king's x,y
   int valid;				// int to be returned if valid move or not

   /* save game_board */
   for(int gY = 0; gY < 8; gY++) 
   {
     for(int gX = 0; gX < 8; gX++)
        temp_board[gX][gY] = game_board[gX][gY];
   }   


   /* assume the move from x1,y1 -> x2,y2 is made */
   game_board[x2][y2] = game_board[x1][y1];
   game_board[x1][y1] = -1;
    
   /* check to see if the king is now in check */
   if(turn == 0)			/* White's Turn */
   {
      /* get the king's cords */
      x = the_pieces.getPiece(31)->getPos()[0];
      y = the_pieces.getPiece(31)->getPos()[1];

      /* make sure not now in check */
      if( kingSafe(1,0,0,0) == 1 )
	 valid = 1;
      else
         valid = 0;
   }
   else					/* Black's Turn */
   {
      /* get the king's cords */
      x = the_pieces.getPiece(15)->getPos()[0];
      y = the_pieces.getPiece(15)->getPos()[1];

      /* make sure not now in check */
      if( kingSafe(1,1,0,0) == 1 )
         valid = 1;
      else
         valid = 0;
   }   

   /* reset game board */
   for(int tY = 0; tY < 8; tY++)
   {
     for(int tX = 0; tX < 8; tX++)
        game_board[tX][tY] = temp_board[tX][tY];
   }
   return valid;

} /* end move safe */

/**
 *
 */
Game* Game::getGame()
{
   if(game_instance == NULL)
      game_instance = new Game();

   return game_instance;  
}

/**
 * Assigned each space individually to avoid using
 * non-supported extended initialization lists. 
 */
void Game::initBoard()
{
  /* set empty spaces */
  for(int y=0; y<8; y++) {
     for(int x=0; x<8; x++) 
        game_board[x][y] = -1;
  }
  /* set black's pieces */
  game_board[0][0] = 8;
  game_board[1][0] = 10;
  game_board[2][0] = 12;
  game_board[3][0] = 14;
  game_board[4][0] = 15;
  game_board[5][0] = 13;
  game_board[6][0] = 11;
  game_board[7][0] = 9;
  game_board[0][1] = 0;
  game_board[1][1] = 1;
  game_board[2][1] = 2;
  game_board[3][1] = 3;
  game_board[4][1] = 4;
  game_board[5][1] = 5;
  game_board[6][1] = 6;
  game_board[7][1] = 7;

  /* set white's pieces */
  game_board[0][6] = 16;
  game_board[1][6] = 17;
  game_board[2][6] = 18;
  game_board[3][6] = 19;
  game_board[4][6] = 20;
  game_board[5][6] = 21;
  game_board[6][6] = 22;
  game_board[7][6] = 23;
  game_board[0][7] = 24;
  game_board[1][7] = 26;
  game_board[2][7] = 28;
  game_board[3][7] = 30;
  game_board[4][7] = 31;
  game_board[5][7] = 29;
  game_board[6][7] = 27;
  game_board[7][7] = 25;
} /* end initialize board */

/**
 *
 */
char Game::itoaBoard(int b)
{
   if(b == -1)  return '/';
   if(b < 8 && b >= 0)  return 'p';
   if(b > 15 && b < 24)  return 'P';
   if(b == 8 || b == 9)  return 'r';
   if(b == 10 || b == 11)  return 'n';
   if(b == 12 || b == 13)  return 'b';
   if(b == 14)  return 'q';
   if(b == 15)  return 'k';
   if(b == 24 || b == 25)  return 'R';
   if(b == 26 || b == 27)  return 'N';
   if(b == 28 || b == 29)  return 'B';
   if(b == 30)  return 'Q';
   if(b == 31)  return 'K';
   return '?';
}

/**
 *
 */
int Game::itoiLogical(int a)
{
  if(a == 1) return 7;
  if(a == 2) return 6;
  if(a == 3) return 5;
  if(a == 4) return 4;
  if(a == 5) return 3;
  if(a == 6) return 2;
  if(a == 7) return 1;
  if(a == 8) return 0;
  return -1;
}

/**
 *
 */
char Game::itoaFile(int r)
{
   if(r == 0)  return 'a';
   if(r == 1)  return 'b';
   if(r == 2)  return 'c';
   if(r == 3)  return 'd';
   if(r == 4)  return 'e';
   if(r == 5)  return 'f';
   if(r == 6)  return 'g';
   if(r == 7)  return 'h';
   return '?';
}

/**
 *
 */
void Game::printBoard()
{
  for(int y=-1; y<9; y++)
  {
     for(int x=-1; x<9; x++)
     { 
	if(x == -1 && (y == -1 || y == 8)) 		// corner
	    cout << "%  ";

        if((x == -1 || x == 8) && (y >= 0 && y < 8)) 	// ranks
	    cout << (8-y) << "  ";

        if(x >= 0 && x < 8 && (y == -1 || y == 8)) 	// file
	    cout << itoaFile(x) << " ";

	if((x >= 0 && x < 8) && (y >= 0 && y < 8)) 	// pieces
	    cout << itoaBoard(game_board[x][y]) << " ";

	if(x == 8 && (y == -1 || y == 8)) 		// corner
	    cout << "%  ";
	
	if(x == 7) cout << " ";				// new lines
	if(x == 8 && (y == -1 || y == 7)) cout << endl;
     }
     cout << endl;
   }	
   cout << endl;

} /* end print board */

/**
 *
 */
void Game::boardSwap(int cap, int x1, int y1, int x2, int y2)
{  
   the_pieces.getPiece(game_board[x1][y1])->setPos(x2,y2);
   if(cap)	/* capture move */
   {
      the_pieces.getPiece(game_board[x2][y2])->setPos(-1,-1);
      the_pieces.getPiece(game_board[x2][y2])->setAlive(false);
   }  
   game_board[x2][y2] = game_board[x1][y1];
   game_board[x1][y1] = -1;
}

/**
 *
 */
int Game::cardinalSearch(Piece_enum p, int t, int x, int y)
{
   int search_cords[2];
   int rook_ambig = 0;
   int bishop_ambig = 0;

   // North Search
   if(p == Queen || p == Rook)
   {
     for(int n = y; n >= 0; n--)
     {
        if(game_board[x][n] != -1)
        {
	  if(t == 0 && p == Queen && game_board[x][n] == 30)
          {
	     cords[0] = x;
             cords[1] = n;
             return 1;
          }
          if(t == 1 && p == Queen && game_board[x][n] == 14)
          {
             cords[0] = x;
             cords[1] = n;
             return 1;
          }
          if(t == 0 && p == Rook && game_board[x][n] == 24 || game_board[x][n] == 25)
	  {
             cords[0] = x;
             cords[1] = n;
             rook_ambig++;
          }
          if(t == 1 && p == Rook && game_board[x][n] == 8 || game_board[x][n] == 9)
          {
             cords[0] = x;
             cords[1] = n;
             rook_ambig++;
          }
	  /* else space is not empty and not target -> blocked, stop search */
          break;
        }
	/* else space is empty -> continue search */

     } /* end north */
     
     // East Search
     for(int e = x; e < 8; e++)
     {
	if(game_board[e][y] != -1)
        {
          if(t == 0 && p == Queen && game_board[e][y] == 30)
          {
             cords[0] = e;
             cords[1] = y;
             return 1;
          }
          if(t == 1 && p == Queen && game_board[e][y] == 14)
          {
             cords[0] = e;
             cords[1] = y;
             return 1;
          }
          if(t == 0 && p == Rook && game_board[e][y] == 24 || game_board[e][y] == 25)
          {
             cords[0] = e;
             cords[1] = y;
             rook_ambig++;
          }
          if(t == 1 && p == Rook && game_board[e][y] == 8 || game_board[e][y] == 9)
          {
             cords[0] = e;
             cords[1] = y;
             rook_ambig++;
          }
          /* else space is not empty and not target -> blocked, stop search */
          break;
        }
        /* else space is empty -> continue search */

     } /* end east */
     
     // South Search
     for(int s = y; s < 8; s++)
     {
	if(game_board[x][s] != -1)
        {
          if(t == 0 && p == Queen && game_board[x][s] == 30)
          {
             cords[0] = x;
             cords[1] = s;
             return 1;
          }
          if(t == 1 && p == Queen && game_board[x][s] == 14)
          {
             cords[0] = x;
             cords[1] = s;
             return 1;
          }
          if(t == 0 && p == Rook && game_board[x][s] == 24 || game_board[x][s] == 25)
          {
             cords[0] = x;
             cords[1] = s;
             rook_ambig++;
          }
          if(t == 1 && p == Rook && game_board[x][s] == 8 || game_board[x][s] == 9)
          {
             cords[0] = x;
             cords[1] = s;
             rook_ambig++;
          }
          /* else space is not empty and not target -> blocked, stop search */
          break;
        }
        /* else space is empty -> continue search */

     } /* end south */

     // West Search
     for(int e = 0; e >= 0; e--)
     {
	if(game_board[e][y] != -1)
        {
          if(t == 0 && p == Queen && game_board[e][y] == 30)
          {
             cords[0] = e;
             cords[1] = y;
             return 1;
          }
          if(t == 1 && p == Queen && game_board[e][y] == 14)
          {
             cords[0] = e;
             cords[1] = y;
             return 1;
          }
          if(t == 0 && p == Rook && game_board[e][y] == 24 || game_board[e][y] == 25)
          {
             cords[0] = e;
             cords[1] = y;
             rook_ambig++;
          }
          if(t == 1 && p == Rook && game_board[e][y] == 8 || game_board[e][y] == 9)
          {
             cords[0] = e;
             cords[1] = y;
             rook_ambig++;
          }
          /* else space is not empty and not target -> blocked, stop search */
          break;
        }
        /* else space is empty -> continue search */

     } /* end west */

     if(p == Rook && rook_ambig == 1)
     	return 1;
     else if(p == Rook && rook_ambig > 1)
     {
        cout << "error: ambiguous move, specify file/rank" << endl;
        return 2;
     }
   } /* end n,s,e,w */

   if(p == Queen || p == Bishop)
   {
     // Northeast Search
     search_cords[0] = x;
     search_cords[1] = y;
     while(true)
     {
       search_cords[0]++;
       search_cords[1]--;
       if(search_cords[0] < 8 && search_cords[1] >= 0)
       {
	 if(game_board[search_cords[0]][search_cords[1]] != -1)
         {
	    if(t == 0 && p == Queen && game_board[search_cords[0]][search_cords[1]] == 30)
            {
		cords[0] = search_cords[0];
                cords[1] = search_cords[1];
                return 1;
            }
            if(t == 1 && p == Queen && game_board[search_cords[0]][search_cords[1]] == 14)
            {
		cords[0] = search_cords[0];
                cords[1] = search_cords[1];
                return 1;
            }
            if(t == 0 && p == Bishop &&
               game_board[search_cords[0]][search_cords[1]] == 28 ||
               game_board[search_cords[0]][search_cords[1]] == 29 )
            {
            	cords[0] = search_cords[0];
 		cords[1] = search_cords[1];
                bishop_ambig++;
                break;
            } 
            if(t == 1 && p == Bishop &&
               game_board[search_cords[0]][search_cords[1]] == 12 ||
 	       game_board[search_cords[0]][search_cords[1]] == 13 )
            {
		cords[0] = search_cords[0];
 		cords[1] = search_cords[1];
                bishop_ambig++;
                break;
            }
	    /* else space is not empty and is not target -> blocked, stop searching */
            break;
         }
       }
       else	/* off board */
         break;

     } /* end northeast */
     
     // Southeast Search
     search_cords[0] = x;
     search_cords[1] = y;
     while(true)
     {
       search_cords[0]++;
       search_cords[1]++;
       if(search_cords[0] < 8 && search_cords[1] < 8)
       {
	 if(game_board[search_cords[0]][search_cords[1]] != -1)
         {
            if(t == 0 && p == Queen && game_board[search_cords[0]][search_cords[1]] == 30)
            {
                cords[0] = search_cords[0];
                cords[1] = search_cords[1];
                return 1;
            }
            if(t == 1 && p == Queen && game_board[search_cords[0]][search_cords[1]] == 14)
            {
                cords[0] = search_cords[0];
                cords[1] = search_cords[1];
                return 1;
            }
            if(t == 0 && p == Bishop &&
               game_board[search_cords[0]][search_cords[1]] == 28 ||
               game_board[search_cords[0]][search_cords[1]] == 29 )
            {
                cords[0] = search_cords[0];
                cords[1] = search_cords[1];
                bishop_ambig++;
                break;
            }
            if(t == 1 && p == Bishop &&
               game_board[search_cords[0]][search_cords[1]] == 12 ||
               game_board[search_cords[0]][search_cords[1]] == 13 )
            {
                cords[0] = search_cords[0];
                cords[1] = search_cords[1];
                bishop_ambig++;
                break;
            }
            /* else space is not empty and is not target -> blocked, stop searching */
            break;
         }
       }
       else	/* off board */
	 break;

     } /* end southeast */
   
     // Southwest Search
     search_cords[0] = x;
     search_cords[1] = y;
     while(true)
     {
       search_cords[0]--;
       search_cords[1]++;
       if(search_cords[0] >= 0 && search_cords[1] < 8)
       {
	  if(game_board[search_cords[0]][search_cords[1]] != -1)
         {
            if(t == 0 && p == Queen && game_board[search_cords[0]][search_cords[1]] == 30)
            {
                cords[0] = search_cords[0];
                cords[1] = search_cords[1];
                return 1;
            }
            if(t == 1 && p == Queen && game_board[search_cords[0]][search_cords[1]] == 14)
            {
                cords[0] = search_cords[0];
                cords[1] = search_cords[1];
                return 1;
            }
            if(t == 0 && p == Bishop &&
               game_board[search_cords[0]][search_cords[1]] == 28 ||
               game_board[search_cords[0]][search_cords[1]] == 29 )
            {
                cords[0] = search_cords[0];
                cords[1] = search_cords[1];
                bishop_ambig++;
                break;
            }
            if(t == 1 && p == Bishop &&
               game_board[search_cords[0]][search_cords[1]] == 12 ||
               game_board[search_cords[0]][search_cords[1]] == 13 )
            {
                cords[0] = search_cords[0];
                cords[1] = search_cords[1];
                bishop_ambig++;
                break;
            }
            /* else space is not empty and is not target -> blocked, stop searching */
            break;
         }
       }
       else	/* off board */
         break;

     } /* end southwest */

     // Northwest Search
     search_cords[0] = x;
     search_cords[1] = y;
     while(true)
     {
       search_cords[0]--;
       search_cords[1]--;
       if(search_cords[0] >= 0 && search_cords[1] >= 0)
       {
         if(game_board[search_cords[0]][search_cords[1]] != -1)
         {
            if(t == 0 && p == Queen && game_board[search_cords[0]][search_cords[1]] == 30)
            {
                cords[0] = search_cords[0];
                cords[1] = search_cords[1];
                return 1;
            }
            if(t == 1 && p == Queen && game_board[search_cords[0]][search_cords[1]] == 14)
            {
                cords[0] = search_cords[0];
                cords[1] = search_cords[1];
                return 1;
            }
            if(t == 0 && p == Bishop &&
               game_board[search_cords[0]][search_cords[1]] == 28 ||
               game_board[search_cords[0]][search_cords[1]] == 29 )
            {
                cords[0] = search_cords[0];
                cords[1] = search_cords[1];
                bishop_ambig++;
                break;
            }
            if(t == 1 && p == Bishop &&
               game_board[search_cords[0]][search_cords[1]] == 12 ||
               game_board[search_cords[0]][search_cords[1]] == 13 )
            {
                cords[0] = search_cords[0];
                cords[1] = search_cords[1];
                bishop_ambig++;
                break;
            }
            /* else space is not empty and is not target -> blocked, stop searching */
            break;
         }
       }
       else	/* off board */
         break;

     } /* end northwest */

     if(p == Bishop && bishop_ambig == 1)
        return 1;
     else if(p == Bishop && bishop_ambig > 1)
     {
        cout << "error: ambiguous move, specify file/rank" << endl;
        return 2;
     }
  } /* end ne,se,sw,nw */

} /* end cardinal */

/**
 *
 */
int Game::kingAround(int turn, int x, int y)
{
   if(turn == 0)
   {
      if( y-1 >= 0 && game_board[x][y-1] == 16 )		// north
        return 1;
      if( x+1 < 8 &&  y-1 >= 0 && game_board[x+1][y-1] == 16 )	// northeast
        return 1;
      if( x+1 < 8 && game_board[x+1][y] == 16 )			// east
        return 1;
      if( x+1 < 8 && y+1 < 8 && game_board[x+1][y+1] == 16 )	// southeast
        return 1;
      if( y+1 < 8 && game_board[x][y+1] == 16 )			// south
        return 1;
      if( x-1 >= 0 && y+1 < 8 && game_board[x-1][y+1] == 16 )	// southwest
        return 1;
      if( x-1 >= 0 && game_board[x-1][y] == 16 )		// west
        return 1;
      if( x-1 >= 0 && y-1 >= 0 && game_board[x-1][y-1] == 16 )	// northwest
        return 1;
   }
   else
   {
      if( y-1 >= 0 && game_board[x][y-1] == 31 )                // north
        return 1;
      if( x+1 < 8 &&  y-1 >= 0 && game_board[x+1][y-1] == 31 )  // northeast
        return 1;
      if( x+1 < 8 && game_board[x+1][y] == 31 )                 // east
        return 1;
      if( x+1 < 8 && y+1 < 8 && game_board[x+1][y+1] == 31 )    // southeast
        return 1;
      if( y+1 < 8 && game_board[x][y+1] == 31 )                 // south
        return 1;
      if( x-1 >= 0 && y+1 < 8 && game_board[x-1][y+1] == 31 )   // southwest
        return 1;
      if( x-1 >= 0 && game_board[x-1][y] == 31 )                // west
        return 1;
      if( x-1 >= 0 && y-1 >= 0 && game_board[x-1][y-1] == 31 )  // northwest
        return 1;
   }
   return 0;
}

/**
 *
 */
void Game::recordMove(int p, char *move)
{
  recorder.writeMove(p,move);
}

/**
 *
 */
void Game::kingCheck(int turn)
{
  if(turn == 0)
    kingSafe(0,0,0,0);
  else
    kingSafe(0,1,0,0);
}

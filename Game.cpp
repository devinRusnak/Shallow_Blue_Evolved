/*!  
 * \class Game Game.h
 * \brief Processes moves and maintains game logic/info.
 * \author Devin Rusnak
 * \date 2014-2016
 * \copyright GNU Public License.
 *
 *   Game maintains the information about the game board and creates
 * all other needed class instances. Its main function validate(char*),
 * recieves a pointer to a char array that is the user(s) input. It 
 * translates this into a Move class instance that it then uses to check
 * if the move is legal and updates the game board and all other items if
 * it is.
 */
#include "Game.h"
using namespace std;

/*!
 * \memberof Game
 * \brief Default Constructor
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

/*!
 * \memberof Game
 * \brief Detailed Constructor
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

/*!
 * \memberof Game
 * \brief Destructor
 *
 * Deletes the instance of itself.
 */
Game::~Game()
{
   delete game_instance;
}

/*!
 * start()
 * \memberof Game
 * \brief Changes the flag for an active game to true.
 */
void Game::start()
{
   active = 1;
}

/*!
 * stop()
 * \memberof Game
 * \brief Changes the flag for an active game to false.
 */
void Game::stop()
{
   active = 0;
   //TODO stop nicely
}

/*!
 * debugToggle()
 * \memberof Game
 * \brief Toggles the flag for debug mode.
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

/*!
 * isActive()
 * \memberof Game
 * \breif Returns a boolean coresponding to the state of the game.
 * \return A boolean that states if the game is active or not.
 */
bool Game::isActive() 
{
   return active;
}

/*!
 * validate(char*)
 * \memberof Game
 * \brief Takes the user(s)' input and checks if it is a valid move.
 * \param c - A pointer to a char array that contains the user(s) move input.
 *
 *   TODO   
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
   if( pos[0] < 0 || pos[0] > 7 || pos[1] < 0 || pos[1] > 7 )
   {
	cout << "error: out of bounds" << endl;
	return 0;
   }

   if(debug)
      cout << "\n" << translated_move->getPiece() << " " << translated_move->getRank()
           << " " << translated_move->getFile() << " " << translated_move->getPlayer() 
	   << " " << pos[0] << " " << pos[1] << " " << translated_move->getPromotion() 
	   << " " << translated_move->getSpecify() << " " << translated_move->getCapture() << endl; 

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
  
     /* move check */
     if(translated_move->getCapture() == 0 && translated_move->getPromotion() == 0 &&
        translated_move->getEnPass() == 0)
     {
       /**
        *  Double First Move
        */
        if(translated_move->getPlayer() == 0)		/* White's turn */
        {
           if(pos[1]+2 == 6 &&
              game_board[pos[0]][pos[1]] == -1 &&		// TODO Can they hop first move??? TODO
              game_board[pos[0]][pos[1]+1] == -1 &&
              game_board[pos[0]][pos[1]+2] < 24 &&
              game_board[pos[0]][pos[1]+2] > 15 &&
              moveSafe(0,pos[0],pos[1]+2,pos[0],pos[1]) == 0)
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
              game_board[pos[0]][pos[1]-2] >= 0 &&
       	      moveSafe(1,pos[0],pos[1]-2,pos[0],pos[1]) == 0)
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
              game_board[pos[0]][pos[1]+1] > 15 &&
              moveSafe(0,pos[0],pos[1]+1,pos[0],pos[1]) == 0)
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
              game_board[pos[0]][pos[1]-1] >= 0 &&
              moveSafe(1,pos[0],pos[1]-1,pos[0],pos[1]) == 0)
           {
              /* valid pawn move */
              boardSwap(0,pos[0],pos[1]-1,pos[0],pos[1]);	// update board
              turn++;						// increment turn
              return 1;						// return valid
           }
        } /* end simple move */
      }
     /*****************
      *  Attack Move  *
      *****************/
      if(translated_move->getCapture() == 1 && game_board[pos[0]][pos[1]] != -1)
      {
	 if(translated_move->getPlayer() == 0)	/* White's Turn */
	 {
	    /* check for presence of opp. piece */
	    if(game_board[pos[0]][pos[1]] < 16)	
	    {
		// Ambiguity check
		if( (pos[0]-1 >= 0 && pos[1]+1 < 8 && 
		     game_board[pos[0]-1][pos[1]+1] > 15 && game_board[pos[0]-1][pos[1]+1] < 24) && 
		    (pos[0]+1 < 8 && pos[1]+1 < 8 && 
		     game_board[pos[0]+1][pos[1]+1] > 15 && game_board[pos[0]+1][pos[1]+1] < 24) )
		{
                    if(translated_move->getFileAmbig() == -1)
		    {
		       cout << "error: ambiguous move, specify file and/or rank.\n" << endl;
		       return 0;
		    }
		    /* valid specification */
		    if(moveSafe(0,translated_move->getFileAmbig(),pos[1]+1,pos[0],pos[1]) == 0)
		    {
		    	boardSwap(1,translated_move->getFileAmbig(),pos[1]+1,pos[0],pos[1]);		// update board
		    	translated_move->setFileAmbig('#');						// reset file_ambig
		    	turn++;										// increment turn
		    	return 1;									// return valid
		    }
		    else
		    {
			cout << "error: move would place your king into check" << endl;
			return 0;
		    }
		}
		else if( pos[0]-1 >= 0 && pos[1]+1 < 8 && 
		         game_board[pos[0]-1][pos[1]+1] > 15 && game_board[pos[0]-1][pos[1]+1] < 24 &&
                         moveSafe(0,pos[0]-1,pos[1]+1,pos[0],pos[1]) == 0 )
		{
                   /* valid pawn capture */
		   boardSwap(1,pos[0]-1,pos[1]+1,pos[0],pos[1]);	// update board
		   turn++;						// increment turn
		   return 1;						// return valid
		}
		else if( pos[0]+1 < 8 && pos[1]+1 < 8 && 
			 game_board[pos[0]+1][pos[1]+1] > 15 && game_board[pos[0]+1][pos[1]+1] < 24 &&
                         moveSafe(0,pos[0]+1,pos[1]+1,pos[0],pos[1]) == 0 )
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
                if( (pos[0]-1 >= 0 && pos[1]-1 >= 0 &&
		     game_board[pos[0]-1][pos[1]-1] >= 0 && game_board[pos[0]-1][pos[1]-1] < 8) &&
                    (pos[0]+1 < 8 && pos[1]-1 >= 0 &&
		     game_board[pos[0]+1][pos[1]-1] >= 0 && game_board[pos[0]+1][pos[1]-1] < 8) )
                {
		    if(translated_move->getFileAmbig() == -1)
		    {
                       cout << "error: ambiguous move, specify file and/or rank.\n" << endl;
                       return 0;
		    }
		    /* valid specification */
		    if(moveSafe(1,translated_move->getFileAmbig(),pos[1]-1,pos[0],pos[1]) == 0)
		    {
		    	boardSwap(1,translated_move->getFileAmbig(),pos[1]-1,pos[0],pos[1]);		// update board
		    	translated_move->setFileAmbig('#');						// reset file_ambig
		    	turn++;										// increment turn
		    	return 1;									// return valid
		    }
		    else
		    {
			cout << "error: move would place your king into check" << endl;
			return 0;
		    }
                }
                else if( pos[0]-1 >= 0 && pos[1]-1 >= 0 &&
			 game_board[pos[0]-1][pos[1]-1] >= 0 && game_board[pos[0]-1][pos[1]-1] < 8 &&
                         moveSafe(1,pos[0]-1,pos[1]-1,pos[0],pos[1]) == 0 )
                {
                   /* valid pawn capture */
                   boardSwap(1,pos[0]-1,pos[1]-1,pos[0],pos[1]);	// update board
                   turn++;						// increment turn
                   return 1;						// return valid
                }
                else if( pos[0]+1 < 8 && pos[1]-1 >= 0 && 
			 game_board[pos[0]+1][pos[1]-1] >= 0 && game_board[pos[0]+1][pos[1]-1] < 8 &&
                         moveSafe(1,pos[0]+1,pos[1]-1,pos[0],pos[1]) == 0 )
                {
                   /* valid pawn capture */
                   boardSwap(1,pos[0]+1,pos[1]-1,pos[0],pos[1]);	// update board
                   turn++;						// increment turn
                   return 1;						// return valid
                }
    	    }
	 }
      } /* end attack */

     /*********************
      *  Pawn En Passant  *
      *********************/
      if(translated_move->getEnPass() == 1)
      {
        if(translated_move->getPlayer() == 0)	/* white's turn */
	{
	  /* valid en passant check */
	  if((translated_move->getFileAmbig() == pos[0]+1 ||
              translated_move->getFileAmbig() == pos[0]-1) &&
    	      game_board[pos[0]][pos[1]] == -1 &&
              the_pieces.getPiece(game_board[pos[0]][pos[1]+1])->getPawnFlag() == 1)
          {
          	the_pieces.getPiece(game_board[translated_move->getFileAmbig()][pos[1]+1])->setPos(pos[0],pos[1]);	// set piece's new pos
	        the_pieces.getPiece(game_board[pos[0]][pos[1]+1])->setPos(-1,-1);					// set captured piece pos
      		the_pieces.getPiece(game_board[pos[0]][pos[1]+1])->setAlive(false);					// set captured dead
		game_board[pos[0]][pos[1]] = game_board[translated_move->getFileAmbig()][pos[1]+1];			// update board 
		game_board[translated_move->getFileAmbig()][pos[1]+1] = -1;
		game_board[pos[0]][pos[1]+1] = -1;	
		return 1;
          }
	  else
	  {
		cout << "error: invalid pawn en passant capture" << endl;
		return 0;
	  }
  
	}
	else					/* black's turn */
	{
	  /* valid en assant check */
	  if((translated_move->getFileAmbig() == pos[0]+1 ||
              translated_move->getFileAmbig() == pos[0]-1) &&
	      game_board[pos[0]][pos[1]] == -1 &&
	      the_pieces.getPiece(game_board[pos[0]][pos[1]-1])->getPawnFlag() == 1)
          {
            	the_pieces.getPiece(game_board[translated_move->getFileAmbig()][pos[1]-1])->setPos(pos[0],pos[1]);      // set piece's new pos
                the_pieces.getPiece(game_board[pos[0]][pos[1]-1])->setPos(-1,-1);                                       // set captured piece pos
                the_pieces.getPiece(game_board[pos[0]][pos[1]-1])->setAlive(false);                                     // set captured dead
                game_board[pos[0]][pos[1]] = game_board[translated_move->getFileAmbig()][pos[1]-1];                     // update board 
                game_board[translated_move->getFileAmbig()][pos[1]-1] = -1;
                game_board[pos[0]][pos[1]-1] = -1;
                return 1; 
          }
	  else
	  {
		cout << "error: invalid pawn en passant capture" << endl;
		return 0;
	  } 
	}
      } /* end en passant */ 

      /***************
       *  Promotion  *
       ***************/
      else if(translated_move->getPromotion() == 1)
      {
	cout << "PROMOTE THAT SHIT" << endl;
	if(translated_move->getPlayer() == 0)		/* white's turn */
 	{
	   /* not in correct row or spot is not empty */
	   if(pos[1] != 0 || game_board[pos[0]][0] != -1)
	   {
		cout << "error: invalid promotion" << endl;
		return 0;
	   }
	   switch(translated_move->getSpecify())	// switch of promotion choices
	   {
		case 4:		/***********
    				 *  Queen  *
				 ***********/
				if(debug)
				   cout << "debug: queen promotion" << endl;

				/* queen is is alive */
				if(the_pieces.getPiece(30)->isAlive() == true)
				{
				   cout << "error: promotion piece alive" << endl;
				   return 0;
				}
				/* check pawn is in correct location */
				if(game_board[pos[0]][1] > 15 &&
				   game_board[pos[0]][1] < 24)
				{
				   the_pieces.getPiece(30)->setPos(pos[0],0);			// resurrect queen
				   the_pieces.getPiece(30)->setAlive(true);			
				   the_pieces.getPiece(game_board[pos[0]][1])->setPos(-1,-1);	// remove pawn
				   the_pieces.getPiece(game_board[pos[0]][1])->setAlive(false);	// set dead
				   game_board[pos[0]][1] = -1;					// update board
				   game_board[pos[0]][0] = 30;
				   turn++;							// increment turn
				   return 1;							// return valid
				}
				else
				   cout << "error: invalid promotion" << endl;
				break;

		case 3:		/************
 				 *  Bishop  *
				 ************/
				if(debug)
				   cout << "debug: bishop promotion" << endl;

				if(pos[0]%2 == 0)	/* white-square bishop */
				{
					/* bishop is still alive */
					if(the_pieces.getPiece(29)->isAlive() == true)
					{
					   cout << "error: promotion piece alive" << endl;
					   return 0;
					}
					/* check pawn is in correct location */
					if(game_board[pos[0]][1] > 15 &&
					   game_board[pos[0]][1] < 24) 
					{
					   the_pieces.getPiece(29)->setPos(pos[0],0);                   // resurrect bishop
                                  	   the_pieces.getPiece(29)->setAlive(true);
                                   	   the_pieces.getPiece(game_board[pos[0]][1])->setPos(-1,-1);   // remove pawn
                                   	   the_pieces.getPiece(game_board[pos[0]][1])->setAlive(false); // set dead
                                   	   game_board[pos[0]][1] = -1;                                  // update board
                                   	   game_board[pos[0]][0] = 29;
                                   	   turn++;                                                      // increment turn
                                   	   return 1;							// return valid
					}
					else
					   cout << "error: invalid promotion" << endl;
				}
				else			/* black-square bishop */
				{
					/* bishop is still alive */
                                        if(the_pieces.getPiece(28)->isAlive() == true)
                                        {
                                           cout << "error: promotion piece alive" << endl;
                                           return 0;
                                        }
                                        /* check pawn is in correct location */
                                        if(game_board[pos[0]][1] > 15 &&
                                           game_board[pos[0]][1] < 24)
                                        {
                                           the_pieces.getPiece(28)->setPos(pos[0],0);                   // resurrect bishop
                                           the_pieces.getPiece(28)->setAlive(true);
                                           the_pieces.getPiece(game_board[pos[0]][1])->setPos(-1,-1);   // remove pawn
                                           the_pieces.getPiece(game_board[pos[0]][1])->setAlive(false); // set dead
                                           game_board[pos[0]][1] = -1;                                  // update board
                                           game_board[pos[0]][0] = 28;
                                           turn++;                                                      // increment turn
                                           return 1;                                                    // return valid
                                        }
                                        else
                                           cout << "error: invalid promotion" << endl;
				}
				break;

		case 2:		/**********
				 *  Rook  *
				 **********/
				if(debug)
				   cout << "debug: rook promotion" << endl;

				/* rooks are still alive */
				if(the_pieces.getPiece(24)->isAlive() == true && 
				   the_pieces.getPiece(25)->isAlive() == true)
				{
				   cout << "error: promotion piece both alive" << endl;
				   return 0;
				}				
				/* check pawn is in correct location */
				if(game_board[pos[0]][1] > 15 &&
				   game_board[pos[0]][1] < 24)
				{
				   if(the_pieces.getPiece(24)->isAlive() == false)
				   {
				   	the_pieces.getPiece(24)->setPos(pos[0],0);                   // resurrect rook
                                        the_pieces.getPiece(24)->setAlive(true);
                                        the_pieces.getPiece(game_board[pos[0]][1])->setPos(-1,-1);   // remove pawn
                                        the_pieces.getPiece(game_board[pos[0]][1])->setAlive(false); // set dead
                                        game_board[pos[0]][1] = -1;                                  // update board
                                        game_board[pos[0]][0] = 24;
                                        turn++;                                                      // increment turn
                                        return 1;                                                    // return valid
				   }
				   else
				   {
					the_pieces.getPiece(25)->setPos(pos[0],0);                   // resurrect rook
                                        the_pieces.getPiece(25)->setAlive(true);
                                        the_pieces.getPiece(game_board[pos[0]][1])->setPos(-1,-1);   // remove pawn
                                        the_pieces.getPiece(game_board[pos[0]][1])->setAlive(false); // set dead
                                        game_board[pos[0]][1] = -1;                                  // update board
                                        game_board[pos[0]][0] = 25;
                                        turn++;                                                      // increment turn
                                        return 1;                                                    // return valid
				   }
				}
				else
				   cout << "error: invalid promotion" << endl;
				break;

		case 1:		/************
				 *  Knight  *
				 ************/  
				if(debug)
				   cout << "debug: knight promotion" << endl;

				/* both knights are still alive */
				if(the_pieces.getPiece(26)->isAlive() == true &&
				   the_pieces.getPiece(27)->isAlive() == true)
				{
				   cout << "error: promotion piece both alive" << endl;
				   return 0;
				}
			 	/* check pawn is in correct location */
				if(game_board[pos[0]][1] > 15 &&
				   game_board[pos[0]][1] < 24)
				{
				   if(the_pieces.getPiece(26)->isAlive() == false)
                                   {
                                        the_pieces.getPiece(26)->setPos(pos[0],0);                   // resurrect knight
                                        the_pieces.getPiece(26)->setAlive(true);
                                        the_pieces.getPiece(game_board[pos[0]][1])->setPos(-1,-1);   // remove pawn
                                        the_pieces.getPiece(game_board[pos[0]][1])->setAlive(false); // set dead
                                        game_board[pos[0]][1] = -1;                                  // update board
                                        game_board[pos[0]][0] = 26;
                                        turn++;                                                      // increment turn
                                        return 1;                                                    // return valid
                                   }
                                   else
                                   {
                                        the_pieces.getPiece(27)->setPos(pos[0],0);                   // resurrect knight
                                        the_pieces.getPiece(27)->setAlive(true);
                                        the_pieces.getPiece(game_board[pos[0]][1])->setPos(-1,-1);   // remove pawn
                                        the_pieces.getPiece(game_board[pos[0]][1])->setAlive(false); // set dead
                                        game_board[pos[0]][1] = -1;                                  // update board
                                        game_board[pos[0]][0] = 27;
                                        turn++;                                                      // increment turn
                                        return 1;                                                    // return valid
                                   }
				}
				else
				   cout << "error: invalid promotion" << endl;
				break;

		default: cout << "error: bad spec" << endl;
           } // end switch
	}
	else						/* black's turn */
	{
           /* not in correct row or spot is not empty */
           if(pos[1] != 7 || game_board[pos[0]][7] != -1)
           {
                cout << "error: invalid promotion" << endl;
                return 0;
           }
           switch(translated_move->getSpecify())        // switch of promotion choices
           {
	   	case 4:         /***********
                                 *  Queen  *
                                 ***********/
                                if(debug)
                                   cout << "debug: queen promotion" << endl;

                                /* queen is is alive */
                                if(the_pieces.getPiece(14)->isAlive() == true)
                                {
                                   cout << "error: promotion piece alive" << endl;
                                   return 0;
                                }
                                /* check pawn is in correct location */
                                if(game_board[pos[0]][7] >= 0 &&
                                   game_board[pos[0]][7] < 8)
                                {
                                   the_pieces.getPiece(14)->setPos(pos[0],7);                   // resurrect queen
                                   the_pieces.getPiece(14)->setAlive(true);
                                   the_pieces.getPiece(game_board[pos[0]][6])->setPos(-1,-1);   // remove pawn
                                   the_pieces.getPiece(game_board[pos[0]][6])->setAlive(false); // set dead
                                   game_board[pos[0]][6] = -1;                                  // update board
                                   game_board[pos[0]][7] = 14;
                                   turn++;                                                      // increment turn
                                   return 1;                                                    // return valid
                                }
                                else
                                   cout << "error: invalid promotion" << endl;
                                break;

		case 3:         /************
                                 *  Bishop  *
                                 ************/
                                if(debug)
                                   cout << "debug: bishop promotion" << endl;

                                if(pos[0]%2 == 0)       /* white-square bishop */
                                {
                                        /* bishop is still alive */
                                        if(the_pieces.getPiece(12)->isAlive() == true)
                                        {
                                           cout << "error: promotion piece alive" << endl;
                                           return 0;
                                        }
                                        /* check pawn is in correct location */
                                        if(game_board[pos[0]][7] >= 0 &&
                                           game_board[pos[0]][7] < 8)
                                        {
                                           the_pieces.getPiece(12)->setPos(pos[0],7);                   // resurrect bishop
                                           the_pieces.getPiece(12)->setAlive(true);
                                           the_pieces.getPiece(game_board[pos[0]][6])->setPos(-1,-1);   // remove pawn
                                           the_pieces.getPiece(game_board[pos[0]][6])->setAlive(false); // set dead
                                           game_board[pos[0]][6] = -1;                                  // update board
                                           game_board[pos[0]][7] = 12;
                                           turn++;                                                      // increment turn
                                           return 1;                                                    // return valid
                                        }
                                        else
                                           cout << "error: invalid promotion" << endl;
                                }
                                else                    /* black-square bishop */
                                {
                                        /* bishop is still alive */
                                        if(the_pieces.getPiece(13)->isAlive() == true)
                                        {
                                           cout << "error: promotion piece alive" << endl;
                                           return 0;
                                        }
                                        /* check pawn is in correct location */
                                        if(game_board[pos[0]][7] >= 0 &&
                                           game_board[pos[0]][7] < 8)
                                        {
                                           the_pieces.getPiece(13)->setPos(pos[0],7);                   // resurrect bishop
                                           the_pieces.getPiece(13)->setAlive(true);
                                           the_pieces.getPiece(game_board[pos[0]][6])->setPos(-1,-1);   // remove pawn
                                           the_pieces.getPiece(game_board[pos[0]][6])->setAlive(false); // set dead
                                           game_board[pos[0]][6] = -1;                                  // update board
                                           game_board[pos[0]][7] = 13;
                                           turn++;                                                      // increment turn
                                           return 1;                                                    // return valid
                                        }
                                        else
                                           cout << "error: invalid promotion" << endl;
                                }
                                break;

		case 2:         /**********
                                 *  Rook  *
                                 **********/
                                if(debug)
                                   cout << "debug: rook promotion" << endl;

                                /* rooks are still alive */
                                if(the_pieces.getPiece(8)->isAlive() == true &&
                                   the_pieces.getPiece(9)->isAlive() == true)
                                {
                                   cout << "error: promotion piece both alive" << endl;
                                   return 0;
                                }
                                /* check pawn is in correct location */
                                if(game_board[pos[0]][6] >= 0 &&
                                   game_board[pos[0]][6] < 8)
                                {
                                   if(the_pieces.getPiece(8)->isAlive() == false)
                                   {
                                        the_pieces.getPiece(8)->setPos(pos[0],7);                   // resurrect rook
                                        the_pieces.getPiece(8)->setAlive(true);
                                        the_pieces.getPiece(game_board[pos[0]][6])->setPos(-1,-1);   // remove pawn
                                        the_pieces.getPiece(game_board[pos[0]][6])->setAlive(false); // set dead
                                        game_board[pos[0]][6] = -1;                                  // update board
                                        game_board[pos[0]][7] = 8;
                                        turn++;                                                      // increment turn
                                        return 1;                                                    // return valid
                                   }
                                   else
                                   {
                                        the_pieces.getPiece(9)->setPos(pos[0],7);                   // resurrect rook
                                        the_pieces.getPiece(9)->setAlive(true);
                                        the_pieces.getPiece(game_board[pos[0]][6])->setPos(-1,-1);   // remove pawn
                                        the_pieces.getPiece(game_board[pos[0]][6])->setAlive(false); // set dead
                                        game_board[pos[0]][6] = -1;                                  // update board
                                        game_board[pos[0]][7] = 9;
                                        turn++;                                                      // increment turn
                                        return 1;                                                    // return valid
                                   }
                                }
                                else
                                   cout << "error: invalid promotion" << endl;
                                break;

		case 1:         /************
                                 *  Knight  *
                                 ************/
                                if(debug)
                                   cout << "debug: knight promotion" << endl;

                                /* both knights are still alive */
                                if(the_pieces.getPiece(10)->isAlive() == true &&
                                   the_pieces.getPiece(11)->isAlive() == true)
                                {
                                   cout << "error: promotion piece both alive" << endl;
                                   return 0;
                                }
                                /* check pawn is in correct location */
                                if(game_board[pos[0]][6] >= 0 &&
                                   game_board[pos[0]][6] < 8)
                                {
                                   if(the_pieces.getPiece(10)->isAlive() == false)
                                   {
                                        the_pieces.getPiece(10)->setPos(pos[0],7);                   // resurrect knight
                                        the_pieces.getPiece(10)->setAlive(true);
                                        the_pieces.getPiece(game_board[pos[0]][6])->setPos(-1,-1);   // remove pawn
                                        the_pieces.getPiece(game_board[pos[0]][6])->setAlive(false); // set dead
                                        game_board[pos[0]][6] = -1;                                  // update board
                                        game_board[pos[0]][7] = 10;
                                        turn++;                                                      // increment turn
                                        return 1;                                                    // return valid
                                   }
                                   else
                                   {
                                        the_pieces.getPiece(11)->setPos(pos[0],7);                   // resurrect knight
                                        the_pieces.getPiece(11)->setAlive(true);
                                        the_pieces.getPiece(game_board[pos[0]][6])->setPos(-1,-1);   // remove pawn
                                        the_pieces.getPiece(game_board[pos[0]][6])->setAlive(false); // set dead
                                        game_board[pos[0]][6] = -1;                                  // update board
                                        game_board[pos[0]][7] = 11;
                                        turn++;                                                      // increment turn
                                        return 1;                                                    // return valid
                                   }
                                }
                                else
                                   cout << "error: invalid promotion" << endl;
                                break;

                default: cout << "error: bad spec" << endl;
	   } // end switch
	}
      } // end promotion 
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
	      if(translated_move->getCapture() == 0 && game_board[pos[0]][pos[1]] == -1 && 
		 moveSafe(0,x,y,pos[0],pos[1]) == 0) 
                 boardSwap(0,x,y,pos[0],pos[1]);	// a move 

	      else if(translated_move->getCapture() == 1 && game_board[pos[0]][pos[1]] < 16 &&
                      moveSafe(0,x,y,pos[0],pos[1]) == 0)
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
              if(translated_move->getCapture() == 0 && game_board[pos[0]][pos[1]] == -1 && 
		 moveSafe(1,x,y,pos[0],pos[1]) == 0)
                 boardSwap(0,x,y,pos[0],pos[1]);        // a move

              else if(translated_move->getCapture() == 1 && game_board[pos[0]][pos[1]] > 15 &&
                      moveSafe(1,x,y,pos[0],pos[1]) == 0)         
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
         if(translated_move->getCapture() == 0 && game_board[pos[0]][pos[1]] == -1 && 
     	    moveSafe(0,cords[0],cords[1],pos[0],pos[1]) == 0)
         {
		/* valid rook move */
         	boardSwap(0, cords[0], cords[1], pos[0], pos[1]);	// update board
                turn++;							// increment turn
                return 1;						// return valid
         }
         else if(translated_move->getCapture() == 1 && game_board[pos[0]][pos[1]] < 16 &&
                 moveSafe(0,cords[0],cords[1],pos[0],pos[1]) == 0)
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
         if(translated_move->getCapture() == 0 && game_board[pos[0]][pos[1]] == -1 && 
	    moveSafe(1,cords[0],cords[1],pos[0],pos[1]) == 0)
         {
		/* valid rook move */
                boardSwap(0, cords[0], cords[1], pos[0], pos[1]);	// update board
   		turn++;							// increment turn
		return 1;						// return valid
         }
         else if(translated_move->getCapture() == 1 && game_board[pos[0]][pos[1]] > 15 &&
                 moveSafe(1,cords[0],cords[1],pos[0],pos[1]) == 0)
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
         if(translated_move->getCapture() == 0 && game_board[pos[0]][pos[1]] == -1 && 
	    moveSafe(0,cords[0],cords[1],pos[0],pos[1]) == 0)
         {
		/* valid bishop move */
                boardSwap(0, cords[0], cords[1], pos[0], pos[1]);	// update board
                turn++;							// increment turn
                return 1;						// return valid 
         }
         else if(translated_move->getCapture() == 1 && game_board[pos[0]][pos[1]] < 16 &&
                 moveSafe(0,cords[0],cords[1],pos[0],pos[1]) == 0)
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
         if(translated_move->getCapture() == 0 && game_board[pos[0]][pos[1]] == -1 && 
	    moveSafe(1,cords[0],cords[1],pos[0],pos[1]) == 0)
         {
         	/* valid bishop move */
                boardSwap(0, cords[0], cords[1], pos[0], pos[1]);	// update board
                turn++;							// increment turn
                return 1;						// return valid
         }
         else if(translated_move->getCapture() == 1 && game_board[pos[0]][pos[1]] > 15 &&
                 moveSafe(1,cords[0],cords[1],pos[0],pos[1]) == 0)
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

   /***********
    *  Queen  *
    ***********/
   else if(translated_move->getPiece() == Queen) 
   {
     if(translated_move->getPlayer() == 0) 		/* White's turn */
     {
       /* Queenside Castle */
       if(translated_move->getCastle() == 1)
       {
         if(debug)
	    cout << "queenside castle" << endl;

	 if( game_board[0][7] == 24 && the_pieces.getPiece(24)->isMoved() == false &&
             game_board[4][7] == 31 && the_pieces.getPiece(31)->isMoved() == false &&
 	     game_board[1][7] == -1 && game_board[2][7] == -1 && game_board[3][7] == -1 )
         {
             boardSwap(0,4,7,2,7);	// move king
	     boardSwap(0,0,7,3,7);	// move rook
	  
	     if( kingSafe(1,0,0,0) == 1 || kingAround(0,0,0) == 1 )	// check if castle would end in check
             {
		cout << "error: castle would put king in check!" << endl;
		boardSwap(0,2,7,4,7);	// move king back
		boardSwap(0,3,7,0,7);	// move rook back
	     }
	     else
	     {
	     	turn++;			// increment turn
	     	return 1;		// return valid
	     }
         }
       }// end castle
       else
       {
         /* check for queen's existence */
         if(cardinalSearch(Queen, 0, pos[0], pos[1]) == 1)
         {
		if(debug)
		   cout << "Queen Found" << endl;
           if(translated_move->getCapture() == 0 && game_board[pos[0]][pos[1]] == -1 && 
	      moveSafe(0,cords[0],cords[1],pos[0],pos[1]) == 0)
           {
		/* valid queen move */
                boardSwap(0, cords[0], cords[1], pos[0], pos[1]);	// update board
                turn++;							// increment turn
                return 1;						// return valid
           }
           else if(translated_move->getCapture() == 1 && game_board[pos[0]][pos[1]] < 16 && 
                 moveSafe(0,cords[0],cords[1],pos[0],pos[1]) == 0)
           {
		/* valid queen capture */
                boardSwap(1, cords[0], cords[1], pos[0], pos[1]);	// update board
                turn++;							// increment turn
                return 1;						// return valid 
           }
           return 0; /* invalid move catch */
         }
       }
     }
     else       /* Black's turn */
     {
       /* Queenside Castle */
       if(translated_move->getCastle() == 1)
       {
         if(debug)
            cout << "queenside castle" << endl;

         if( game_board[0][0] == 8 && the_pieces.getPiece(8)->isMoved() == false &&
             game_board[4][0] == 15 && the_pieces.getPiece(15)->isMoved() == false &&
             game_board[1][0] == -1 && game_board[2][0] == -1 && game_board[3][0] == -1 )
         {
             boardSwap(0,4,0,2,0);	// move king
	     boardSwap(0,0,0,3,0);	// move rook

	     if( kingSafe(1,1,0,0) == 1 || kingAround(1,0,0) == 1 )	// check if castle would end in check
	     {
		cout << "error: castle would put king in check!" << endl;
		boardSwap(0,2,0,4,0);	// move king back
		boardSwap(0,3,0,0,0);	// move rook back
	     }
             else
             {
	       turn++;			// increment turn
	       return 1;		// return valid
	     }
         }
       }// end castle
       else
       {
         /* check for queen's existence */
         if(cardinalSearch(Queen, 1, pos[0], pos[1]) == 1)
         {
cout << 1 << endl;
           if(translated_move->getCapture() == 0 && game_board[pos[0]][pos[1]] == -1 && 
	      moveSafe(1,cords[0],cords[1],pos[0],pos[1]) == 0)
           {  
		/* valid queen move */
                boardSwap(0, cords[0], cords[1], pos[0], pos[1]);	// update board
                turn++;							// increment turn
                return 1;						// return valid
           }
           else if(translated_move->getCapture() == 1 && game_board[pos[0]][pos[1]] > 15 &&
                 moveSafe(1,cords[0],cords[1],pos[0],pos[1]) == 0)
           {
		/* valid queen capture */
                boardSwap(1, cords[0], cords[1], pos[0], pos[1]);	// update board
                turn++;							// increment turn
                return 1;						// return valid 
           }
cout << 2 << endl;
           return 0; /* invalid move catch */
         }
cout << 3 << endl;
       }
     }  
   } /* end queen */

   /**********
    *  King  *
    **********/
   else if(translated_move->getPiece() == King) 
   {
     if(translated_move->getPlayer() == 0)		/* white's turn */
     {
        /* kingside castle */
        if(translated_move->getCastle() == 1)
        { 
          if(debug)
	     cout << "kingside castle" << endl;
	  
	  if( game_board[7][7] == 25 && the_pieces.getPiece(25)->isMoved() == false &&
              game_board[4][7] == 31 && the_pieces.getPiece(31)->isMoved() == false &&
	      game_board[5][7] == -1 && game_board[6][7] == -1 )
          {
		boardSwap(0,4,7,6,7);			// move king
		boardSwap(0,7,7,5,7);			// move rook
               
		if( kingSafe(1,0,0,0) == 1 || kingAround(0,0,0) == 1 )	// check if castle would end in check
		{
		   cout << "error: castle would put king in check!" << endl;
		   boardSwap(0,6,7,4,7);		// move knig back
		   boardSwap(0,5,7,7,7);		// move rook back
                }
                else
	        {
		  turn++;				// increment turn 
		  return 1;				// return valid
	        }
          }

        }// end castle
	
	x = the_pieces.getPiece(31)->getPos()[0];	// get king's x,y
	y = the_pieces.getPiece(31)->getPos()[1];

	/* check for invalid move */
	if( abs(x-pos[0]) == 1 || abs(y-pos[1]) == 1 )
	{
	   // check for valid capture
	   if(translated_move->getCapture() == 1 &&
	      game_board[pos[0]][pos[1]] >= 0 &&
	      game_board[pos[0]][pos[1]] < 15 &&
	      moveSafe(0,x,y,pos[0],pos[1]) == 0 && 
	      kingAround(0,pos[0],pos[1]) == 0)
	   {
	   	the_pieces.getPiece(31)->setPos(pos[0],pos[1]);      				// set piece's new pos
                the_pieces.getPiece(game_board[pos[0]][pos[1]])->setPos(-1,-1);                 // set captured piece's pos
                the_pieces.getPiece(game_board[pos[0]][pos[1]])->setAlive(false);		// set captured dead
                game_board[pos[0]][pos[1]] = 31;     						// update board 
                game_board[x][y] = -1;
                return 1;
	   }
	   
	   // check for valid move
	   if(game_board[pos[0]][pos[1]] == -1 &&
	      moveSafe(0,x,y,pos[0],pos[1]) == 0 &&
	      kingAround(0,pos[0],pos[1]) == 0)
	   {
	   	the_pieces.getPiece(31)->setPos(pos[0],pos[1]);		// set king's new pos
		game_board[pos[0]][pos[1]] = 31;			// update board
		game_board[x][y] = -1;
		return 1;
	   }     
	}	
     }// end white's turn

     else						/* black's turn */
     {
       /* kingside castle */
        if(translated_move->getCastle() == 1)
        {
          if(debug)
             cout << "kingside castle" << endl;

          if( game_board[7][0] == 9 && the_pieces.getPiece(9)->isMoved() == false &&
              game_board[4][0] == 15 && the_pieces.getPiece(15)->isMoved() == false &&
              game_board[5][0] == -1 && game_board[6][0] == -1 )
          {
           	boardSwap(0,4,0,6,0);			// move king
		boardSwap(0,0,0,5,0);			// move rook

                if( kingSafe(1,1,0,0) == 1 || kingAround(1,0,0) == 1 )	// check if castle would end in check
		{
		   cout << "error: castle would put king in check!" << endl;
		   boardSwap(0,6,0,4,0);		// move king back
		   boardSwap(0,5,0,0,0);		// move rook back
 		}
		else
		{
		  turn++;				// increment turn
		  return 1;				// return valid
	        }
          }

        }// end castle
	
	x = the_pieces.getPiece(15)->getPos()[0];	// get king's x,y
	y = the_pieces.getPiece(15)->getPos()[1];
	
	/* check for invalid move */
	if( abs(x-pos[0]) == 1 || abs(y-pos[1]) == 1 )
	{
	   // check for valid capture
           if(translated_move->getCapture() == 1 &&
              game_board[pos[0]][pos[1]] >= 16 &&
              game_board[pos[0]][pos[1]] < 32 &&
              moveSafe(1,x,y,pos[0],pos[1]) == 0 &&
              kingAround(1,pos[0],pos[1]) == 0)
           {
                the_pieces.getPiece(15)->setPos(pos[0],pos[1]);                                 // set piece's new pos
                the_pieces.getPiece(game_board[pos[0]][pos[1]])->setPos(-1,-1);                 // set captured piece's pos
                the_pieces.getPiece(game_board[pos[0]][pos[1]])->setAlive(false);		// set captured dead
                game_board[pos[0]][pos[1]] = 15;                                                // update board 
                game_board[x][y] = -1;
                return 1;
           }

	   // check for valid move
	   if(game_board[pos[0]][pos[1]] == -1 &&
	      moveSafe(1,x,y,pos[0],pos[1]) == 0 &&
	      kingAround(1,pos[0],pos[1]) == 0)
	   {
	   	the_pieces.getPiece(15)->setPos(pos[0],pos[1]);		// update king's pos
		game_board[pos[0]][pos[1]] = 15;			// update board
		game_board[x][y] = -1;	
		return 1;
	   }

	}
     }// end black's turn

   } /* end king */
   else
   {
     /* else invalid move given */
     cout << "error: invalid move" << endl;
     return 0;
   }
} /* end validate */

/**
 *
 */
int Game::kingSafe(int mode, int turn, int x_offset, int y_offset)
{
   int x,y;	  	// king's x,y
   int in_check = 0;	// check flag

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
          in_check = 0;
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
        in_check = 0;
        x = the_pieces.getPiece(31)->getPos()[0] + x_offset;
        y = the_pieces.getPiece(31)->getPos()[1] + y_offset;
  
        if( cardinalSearch(Queen, 1, x, y) != 0 || 
            cardinalSearch(Bishop, 1, x, y) != 0 ||
            cardinalSearch(Rook, 1, x, y) != 0 ||
            cardinalSearch(Knight, 1, x, y) != 0 )
        {
          if(debug)
             cout << "Q/B/R/K check" << endl;
	  in_check = 1;
        }
        // look for pawns
        else if( x-1 >= 0 && y-1 >= 0 && 
                 game_board[x-1][y-1] >= 0 && game_board[x-1][y-1] < 8 )
        {
          if(debug)
	    cout << "pawn check" << endl;
          in_check = 1;
        }
        else if( x+1 < 8 && y-1 >= 0 &&
                 game_board[x+1][y-1] >= 0 && game_board[x+1][y-1] < 8 )
        {
          if(debug)
	    cout << "pawn check" << endl;
          in_check = 1;
        }
        else if( kingAround(0, x, y) == 1 )
        {
          if(debug)
	    cout << "king check" << endl;
          in_check = 1;
        }
        if(in_check)
	   cout << "White king is in check!" << endl;

        return in_check;
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
          if(debug)
            cout << "Q/B/R/K check" << endl;           
          in_check = 1;
        }
        // Look for pawns
        else if( x-1 >= 0 && y-1 >= 0 &&
                 game_board[x-1][y-1] > 15 && game_board[x-1][y-1] < 24 )
        {
          if(debug)
            cout << "pawn check" << endl;
          in_check = 1;
        }
        else if( x+1 < 8 && y-1 >= 0 &&
                 game_board[x+1][y-1] > 15 && game_board[x+1][y-1] < 24 )
        {
          if(debug)
            cout << "pawn check" << endl;
          in_check = 1;
        }
        else if( kingAround(1, x, y) == 1 )
        {
          if(debug)
            cout << "king check" << endl;
          in_check = 1;
        }
        if(in_check)
	  cout << "Black king is in check!" << endl;

        return in_check;
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

   if(debug)
     cout << "! " << x1 << " " << y1 << " " << x2 << " " << y2 << endl;
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
      /* make sure not now in check */
      if( kingSafe(1,0,0,0) == 1 )
	 valid = 1;
      else
         valid = 0;
   }
   else					/* Black's Turn */
   {
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
   if(valid)
     cout << "error: move would place your king into check" << endl;
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

}// end print board

void Game::printLogicalBoard()
{
  for(int y=0; y<8; y++) 
  {
    for(int x=0; x<8; x++)
    {
      cout << game_board[x][y] << "\t";
    }
    cout << endl;
  }
  cout << endl;
  
}// end logical board

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
     for(int n = y-1; n >= 0; n--)
     {
        if(game_board[x][n] != -1)
        {

	  if(t == 0 && p == Queen && game_board[x][n] == 30)
          {
	     cords[0] = x;
             cords[1] = n;
             return 1;
          }
          else if(t == 1 && p == Queen && game_board[x][n] == 14)
          {
             cords[0] = x;
             cords[1] = n;
             return 1;
          }
          else if(t == 0 && p == Rook && (game_board[x][n] == 24 || game_board[x][n] == 25) )
	  {
             cords[0] = x;
             cords[1] = n;
             rook_ambig++;
          }
          else if(t == 1 && p == Rook && (game_board[x][n] == 8 || game_board[x][n] == 9) )
          {
             cords[0] = x;
             cords[1] = n;
             rook_ambig++;
          }
	  else
            break;	/* else space is not empty and not target -> blocked, stop search */
        }
	/* else space is empty -> continue search */

     } /* end north */
     
     // East Search
     for(int e = x+1; e < 8; e++)
     {
	if(game_board[e][y] != -1)
        {
          if(t == 0 && p == Queen && game_board[e][y] == 30)
          {
             cords[0] = e;
             cords[1] = y;
             return 1;
          }
          else if(t == 1 && p == Queen && game_board[e][y] == 14)
          {
             cords[0] = e;
             cords[1] = y;
             return 1;
          }
          else if(t == 0 && p == Rook && (game_board[e][y] == 24 || game_board[e][y] == 25) )
          {
             cords[0] = e;
             cords[1] = y;
             rook_ambig++;
          }
          else if(t == 1 && p == Rook && (game_board[e][y] == 8 || game_board[e][y] == 9) )
          {
             cords[0] = e;
             cords[1] = y;
             rook_ambig++;
          }
          else 
	    break;	/* else space is not empty and not target -> blocked, stop search */
        }
        /* else space is empty -> continue search */

     } /* end east */
     
     // South Search
     for(int s = y+1; s < 8; s++)
     {
	if(game_board[x][s] != -1)
        {
          if(t == 0 && p == Queen && game_board[x][s] == 30)
          { 
             cords[0] = x;
             cords[1] = s;
             return 1;
          }
          else if(t == 1 && p == Queen && game_board[x][s] == 14)
          {
             cords[0] = x;
             cords[1] = s;
             return 1;
          }
          else if(t == 0 && p == Rook && (game_board[x][s] == 24 || game_board[x][s] == 25) )
          {
             cords[0] = x;
             cords[1] = s;
             rook_ambig++;
          }
          else if(t == 1 && p == Rook && (game_board[x][s] == 8 || game_board[x][s] == 9) )
          {
             cords[0] = x;
             cords[1] = s;
             rook_ambig++;
          }
          else
	    break;	/* else space is not empty and not target -> blocked, stop search */
          
        }
        /* else space is empty -> continue search */

     } /* end south */

     // West Search
     for(int w = x-1; w >= 0; w--)
     {
	if(game_board[w][y] != -1)
        {
          if(t == 0 && p == Queen && game_board[w][y] == 30)
          {
             cords[0] = w;
             cords[1] = y;
             return 1;
          }
          else if(t == 1 && p == Queen && game_board[w][y] == 14)
          {
             cords[0] = w;
             cords[1] = y;
             return 1;
          }
          else if(t == 0 && p == Rook && (game_board[w][y] == 24 || game_board[w][y] == 25) )
          {
             cords[0] = w;
             cords[1] = y;
             rook_ambig++;
          }
          else if(t == 1 && p == Rook && (game_board[w][y] == 8 || game_board[w][y] == 9) )
          {
             cords[0] = w;
             cords[1] = y;
             rook_ambig++;
          }
          else 
	    break;/* else space is not empty and not target -> blocked, stop search */
          
        }
        /* else space is empty -> continue search */

     } /* end west */

     if(p == Rook && rook_ambig == 1)
     	return 1;
     else if(p == Rook && rook_ambig > 1)
     {
	if(debug)
	   cout << "rook ambig: " << rook_ambig << endl;
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
            else if(t == 1 && p == Queen && game_board[search_cords[0]][search_cords[1]] == 14)
            {
		cords[0] = search_cords[0];
                cords[1] = search_cords[1];
                return 1;
            }
            else if(t == 0 && p == Bishop &&
               (game_board[search_cords[0]][search_cords[1]] == 28 ||
                game_board[search_cords[0]][search_cords[1]] == 29 ))
            {
            	cords[0] = search_cords[0];
 		cords[1] = search_cords[1];
                bishop_ambig++;
                break;
            } 
            else if(t == 1 && p == Bishop &&
               (game_board[search_cords[0]][search_cords[1]] == 12 ||
 	        game_board[search_cords[0]][search_cords[1]] == 13 ))
            {
		cords[0] = search_cords[0];
 		cords[1] = search_cords[1];
                bishop_ambig++;
                break;
            }
	    else
		break;	/* else space is not empty and is not target -> blocked, stop searching */
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
            else if(t == 1 && p == Queen && game_board[search_cords[0]][search_cords[1]] == 14)
            {
                cords[0] = search_cords[0];
                cords[1] = search_cords[1];
                return 1;
            }
            else if(t == 0 && p == Bishop &&
               (game_board[search_cords[0]][search_cords[1]] == 28 ||
                game_board[search_cords[0]][search_cords[1]] == 29 ))
            {
                cords[0] = search_cords[0];
                cords[1] = search_cords[1];
                bishop_ambig++;
                break;
            }
            else if(t == 1 && p == Bishop &&
               (game_board[search_cords[0]][search_cords[1]] == 12 ||
                game_board[search_cords[0]][search_cords[1]] == 13 ))
            {
                cords[0] = search_cords[0];
                cords[1] = search_cords[1];
                bishop_ambig++;
                break;
            }
            else 
		break;	/* else space is not empty and is not target -> blocked, stop searching */
            
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
            else if(t == 1 && p == Queen && game_board[search_cords[0]][search_cords[1]] == 14)
            {
                cords[0] = search_cords[0];
                cords[1] = search_cords[1];
                return 1;
            }
            else if(t == 0 && p == Bishop &&
               (game_board[search_cords[0]][search_cords[1]] == 28 ||
                game_board[search_cords[0]][search_cords[1]] == 29 ))
            {
                cords[0] = search_cords[0];
                cords[1] = search_cords[1];
                bishop_ambig++;
                break;
            }
            else if(t == 1 && p == Bishop &&
               (game_board[search_cords[0]][search_cords[1]] == 12 ||
                game_board[search_cords[0]][search_cords[1]] == 13 ))
            {
                cords[0] = search_cords[0];
                cords[1] = search_cords[1];
                bishop_ambig++;
                break;
            }
            else 
		break;	/* else space is not empty and is not target -> blocked, stop searching */
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
            else if(t == 1 && p == Queen && game_board[search_cords[0]][search_cords[1]] == 14)
            {
                cords[0] = search_cords[0];
                cords[1] = search_cords[1];
                return 1;
            }
            else if(t == 0 && p == Bishop &&
               (game_board[search_cords[0]][search_cords[1]] == 28 ||
                game_board[search_cords[0]][search_cords[1]] == 29 ))
            {
                cords[0] = search_cords[0];
                cords[1] = search_cords[1];
                bishop_ambig++;
                break;
            }
            else if(t == 1 && p == Bishop &&
               (game_board[search_cords[0]][search_cords[1]] == 12 ||
                game_board[search_cords[0]][search_cords[1]] == 13 ))
            {
                cords[0] = search_cords[0];
                cords[1] = search_cords[1];
                bishop_ambig++;
                break;
            }
	    else
		break;	/* else space is not empty and is not target -> blocked, stop searching */
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

  return 0;
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
      
      return 0;	
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

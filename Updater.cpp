#include "Updater.h"
using namespace std;

Updater::Updater()
{
  memset(&input, 0, sizeof(char)*18); 
}

Updater::~Updater()
{
   // delete input?
}

Move* Updater::translateMove(int turn, char *move_given)
{
   move.setPlayer(turn%2);	// 0-white 1-black
   input = move_given;

   // check for input error
   if((strlen(input) > 5 || strlen(input) < 2) && input[0] != 'q')	
   {
      cout << "error: invalid move" << endl;
      if(debug)
         cout << input << endl;
      return 0;		
   }

   switch( input[0] )
   {
      case 'K':
    		// King Move
        	move.setPiece(King);
		if(strlen(input) == 4 && input[1] == 'x')
		{	// capture
		   move.setCapture(1);
                   move.setFile(input[2]);
                   move.setRank(atoi(&input[3]));
                }
	        else
                {	// move
	 	   move.setFile(input[1]); 
        	   move.setRank(atoi(&input[2]));
                }
	        break;

      case 'Q':
 		// Queen Move
		move.setPiece(Queen);
		if(strlen(input) == 4 && input[1] == 'x')
                {       // capture
                   move.setCapture(1);
                   move.setFile(input[2]);
                   move.setRank(atoi(&input[3]));
                }
                else
                {       // move
                   move.setFile(input[1]);   
                   move.setRank(atoi(&input[2]));
                }
		break;

      case 'R':
		// Rook Move
		move.setPiece(Rook);
		if(strlen(input) == 4 && input[1] == 'x')
                {       // capture
                   move.setCapture(1);
                   move.setFile(input[2]);
                   move.setRank(atoi(&input[3]));
                }
                else
                {       // move
                   move.setFile(input[1]);   
                   move.setRank(atoi(&input[2]));
                }
      		break;

      case 'B':
		// Bishop Move
		move.setPiece(Bishop);
		if(strlen(input) == 4 && input[1] == 'x')
                {       // capture
                   move.setCapture(1);
                   move.setFile(input[2]);
                   move.setRank(atoi(&input[3]));
                }
                else
                {       // move
                   move.setFile(input[1]);   
                   move.setRank(atoi(&input[2]));
                }
 		break;

      case 'N':
		// Knight Move
		move.setPiece(Knight);
		if(strlen(input) == 4 && input[1] == 'x')
                {       // capture
                   move.setCapture(1);
                   move.setFile(input[2]);
                   move.setRank(atoi(&input[3]));
                }
                else
                {       // move
                   move.setFile(input[1]);   
                   move.setRank(atoi(&input[2]));
                }
      		break;

      case '0':
      		// Castle or Win delcaration
      		if(input[1] != '-')
	  	cout << "error: invalid move" << input << endl;
	
      		break;

      case '1':
		// end of game
      		
		break;

      case 'O':
		// Castle

      		break;

      case 'x':	
		// Pawn Capture Move
                move.setPiece(Pawn);
		move.setCapture(1);
                move.setFile(input[1]);
		move.setRank(atoi(&input[2]));
		break;

      case 'a':
		// Pawn Move
		move.setPiece(Pawn);
                if(strlen(input) == 4 && input[1] == 'x')
  		{
		   
		}
		move.setFile('a');
        	move.setRank(atoi(&input[1]));
      		break;

      case 'b':
		// Pawn Move
 		move.setPiece(Pawn);
		move.setFile('b');
        	move.setRank(atoi(&input[1]));
     		break;

      case 'c':
		// Pawn Move
		move.setPiece(Pawn);
		move.setFile('c');
        	move.setRank(atoi(&input[1]));
      		break;

      case 'd':
		// Pawn Move
       		move.setPiece(Pawn);
		move.setFile('d');
        	move.setRank(atoi(&input[1]));
      		break;

      case 'e':
		// Pawn Move
		move.setPiece(Pawn);
		move.setFile('e');
        	move.setRank(atoi(&input[1]));
		break;

      case 'f':
		// Pawn Move
		move.setPiece(Pawn);
		move.setFile('f');
        	move.setRank(atoi(&input[1]));
      		break;

      case 'g':
		// Pawn Move
		move.setPiece(Pawn);
		move.setFile('g');
        	move.setRank(atoi(&input[1]));
      		break;

      case 'h':
		// Pawn Move
		move.setPiece(Pawn);
		move.setFile('h');
        	move.setRank(atoi(&input[1]));
      		break;

      case 'q':
                if(strlen(input) != 1)
		{
		   cout << "error: bad input" << endl;
 		   return 0;
		}
        	do 
        	{
       	   	   cout << "\nare you sure you want to quit...? [y/n]\n>:";
           	   cin >> input;
           	   if(input[0] == 'y' || input[0] == 'Y')
	   	   {
 	      		cout << "...white surrenders" << endl;
	      		move.setPlayer(-2);
	      		return &move;
           	   }
	   	   else if(input[0] == 'n' || input[0] == 'N')
	      		break;
	   	   else
	      		cout << "error: can you read...? [y/n]!" << endl;
        	}while(true);
        	break;
      default:
        	cout << "error: invalid input: " << input << endl;
                return 0;
        	break;
   }// end switch

   if( move.getValid() )
      return &move;
   else
      return 0;

}// end translate move

int Updater::getInputFile(char c)
{
   return 0;
}

void Updater::setDebug(int d)
{
   debug = d;
}

/*!
 * \class Driver
 * \brief The class containing main and handles the command line arguments.
 * \author Devin Rusnak
 * \date 2014-2016
 * \copyright GNU Public License.
 * 
 *   Driver is the class that contains the main function. It handles the command line 
 * arguments and sets up the other classes that are needed to play a game. It is 
 * also responsible for maintaining the main control loop and promts the user(s)
 * for input that it passes along to the Updater class. At the end of use it also
 * makes sure the program terminates cleanly.
 */
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <getopt.h>
#include <iostream>
#include "Game.h"
using namespace std;

int main(int argc, char *argv[]) 
{	
   /* Variables */
   int cmd_line = 0;	// optarg return
   int mode = 0;	// HvC or CvC
   int debug = 0;	// debug flag
   int games = 1;	// # of games to be played
   int valid = 0;	// validate move return value
   char input[18];	// user keyboard input   
   
   cout << "****************************" << endl
	<< "Shallow Blue Evolved v0.1" << endl
        << "Copyright Devin Rusnak 2015" <<  endl
	<< "****************************\n" << endl;

   /* Handle command line arguments */
   while((cmd_line = getopt(argc, argv, "hdm:")) != -1)
   {	
      switch(cmd_line)
      {
	 case 'h':
	    		cout << "*** Help Menu ***" << endl
		 	     << "-h\t Prints this help info." << endl
           		     << "-d\t Sets debug mode." << endl
		 	     << "-m:\t 0/Default-Hum v Comp, 1-Hum v Hum, 2-Comp v Comp" << endl
	 		     << endl;
	    		exit(0);
            		break;

	 case 'd':
			debug = 1;
	    		break;

	 case 'm':
	    		mode = atoi(optarg);
            		break;

	 case ':':
			cout << "missing argument" << endl;
			break;
		 
	 case '?':	
			cout << "unknown command line argument: " << cmd_line
			     << endl << endl;
			break;

         default:
	    		cout << "unknown command line argument: " << cmd_line 
		 	     << "\n" << endl;
           		break;
      }/* end switch */
   }/* end getopt */

   /* initialize Game object */
   Game game_ref;

   if(debug)
   {
	cout << "Debug Mode..." << endl;
        game_ref.debugToggle();
   }
   if(mode == 0)
        cout << "Human vs Computer...\n" << endl;
   else if(mode == 1)
   {
        cout << "Human vs Human...\n" << endl;
        game_ref.setMode(1);
   }
   else
        cout << "Computer vs Computer...\n" << endl; 


  /**
   *  HvC(0) or HvH(1)
   */ 
   if(mode != 2)
   {
      /* Main command loop, continue whilst game is active */ 
      do 
      {  
        /* White - Human's Turn */
        do	
        {
	   if(debug)
	       game_ref.printLogicalBoard();
           game_ref.printBoard();	
	   cout << "\nWhite's Turn" << endl;
           game_ref.kingCheck(0);	// in check?
	   cout << "Move: ";
	   cin >> input;		// Get move input

	}while(game_ref.validate(input) != 1);
	//game_ref.recordMove(0,input);	// Record white's move 	 

        /* Black - Blue's Turn */
        if(mode == 0)
	{
	  if(debug)
	     game_ref.printLogicalBoard();
	  game_ref.printBoard();
          cout << "Blue is thinking..." << endl; 
          game_ref.kingCheck(1);	// in check?
        } 	  

        /* Black - Human's Turn */
        if(mode == 1)
        {
      	  do	
          {
             if(debug)
	       game_ref.printLogicalBoard();
	     game_ref.printBoard();
 	     cout << "\nBlack's Turn" << endl;
             game_ref.kingCheck(1);	// in check? 
             cout << "Move: ";
             cin >> input;
          }while(game_ref.validate(input) != 1);
        }

      }while(game_ref.isActive()); 
    
	  /* TODO GAME OVER TODO */
   } 
  /**
   *  CvC
   */ 
   if(mode == 2)
   {
  
   } 

   cout << "...exiting" << endl;
   /* exit nicely */

   exit(0);
} // end main

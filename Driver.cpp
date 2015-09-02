/**
 * @author Devin Rusnak
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
        cout << "Human vs Human...\n" << endl;
   else
        cout << "Computer vs Computer...\n" << endl; 


  /**
   *  HvC(0) or HvH(1)
   */ 
   if(mode != 2)
   {
      
      do 
      {  
         
         do	
         {
	    if(debug)
	       game_ref.printLogicalBoard();
            game_ref.printBoard();	
	    cout << "\nWhite's Turn" << endl;
            game_ref.kingCheck(0);
	    cout << "Move: ";
	    cin >> input;		// Get move input

	 }while(game_ref.validate(input) != 1);
	 //game_ref.recordMove(0,input);	// Record white's move 	 

        
         if(mode == 0)	
	   ; 

         // Black's Turn 
         else if(game_ref.isActive())
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

      // Game Over TODO
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

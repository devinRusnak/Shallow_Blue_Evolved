/*!
 *  Updater.h 
 *  \author Devin Rusnak
 */

#ifndef UPDATER_H
#define UPDATER_H

#include <string>
#include <cstring>
#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include "Stats.h"
#include "Move.h"
using namespace std;

class Updater
{

public:
   // Constructor
   Updater();
   // Destructor
   ~Updater();

   // Methods
   void setDebug(int);
   Move* translateMove(int, char*);

private:
   // variables:
   char *input;
   int debug;
   Move move;

   int getInputFile(char);

}; // end class
#endif // UPDATER_H

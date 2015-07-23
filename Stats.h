#ifndef STATS_H
#define STATS_H

#include <stdio.h>
#include <string.h>
#include <iostream>
#include <fstream>

using namespace std;

class Stats
{
   
public:
   
   // Constructors
   Stats();

   // Destructor
   ~Stats();

   // Methods
   void setID(int);
   void writeMove(int, const char*);
   int getID();

private:

   // variables
   int game_id;
   char *filename;

}; //end class

#endif // STATS_H

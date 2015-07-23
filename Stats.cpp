#include "Stats.h"
using namespace std;

Stats::Stats()
{
   game_id = 1;
   memset(&filename, 0, sizeof(char)*42);
}

Stats::~Stats()
{
  
}

void Stats::writeMove(int p, const char *move)
{
   ofstream file;
   file.open("sbe_1.txt");

   if(p == 0)
   {
      file << move;
      file << ", ";
   }
   else
   {
      file << move;
      file << endl;
   }
   file.close();
}

int Stats::getID()
{
   return game_id;
}

void Stats::setID(int id)
{
   game_id = id;
}

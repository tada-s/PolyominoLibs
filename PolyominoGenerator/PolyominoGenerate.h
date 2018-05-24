#ifndef POLYOMINOGENERATE_H_INCLUDED
#define POLYOMINOGENERATE_H_INCLUDED

#include <vector>
#include <set>
#include <utility>
#include <map>
#include <stack>

#include "PolyominoTag.h"

#define PROGPARAM_SPACESIZE 512

set< string > spawnBlock(int spawnLife);
set< string > spawnTileBlock(int spawnLife, vector<Tile> tiles);

string spawnRandomBlock(int spawnLife);
string spawnRandomTileBlock(int spawnLife, vector<Tile> tiles);

#endif // POLYOMINOGENERATE_H_INCLUDED

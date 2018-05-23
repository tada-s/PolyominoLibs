#ifndef POLYOMINOGENERATE_H_INCLUDED
#define POLYOMINOGENERATE_H_INCLUDED

#include <vector>
#include <set>
#include <utility>
#include <map>

#include "PolyominoTag.h"

#define PROGPARAM_SPACESIZE 512

set< string > spawnBlock(int spawnLife);

string spawnRandomBlock(int spawnLife);

#endif // POLYOMINOGENERATE_H_INCLUDED

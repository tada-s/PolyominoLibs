#ifndef POLYOMINOTAG_H_INCLUDED
#define POLYOMINOTAG_H_INCLUDED

#include <algorithm>
#include <vector>
#include <utility>
#include <string>
#include <strings.h>
#include <math.h>

#define INF (1<<29)

using namespace std;

typedef vector< pair<int, int> > Tile;

string tagTile(Tile tile);
Tile decodeTag(string tag);

#endif // POLYOMINOTAG_H_INCLUDED

#ifndef POLYOMINOTILING_H_INCLUDED
#define POLYOMINOTILING_H_INCLUDED

#include <vector>
#include <vector>
#include <utility>
#include <stdio.h>

using namespace std;

#define PRINTOPTION_NOPRINT 0
#define PRINTOPTION_NUMBERS 1
#define PRINTOPTION_LINES 2

#define INF (1<<29)

typedef vector< pair<int, int> > Tile;
typedef vector< vector<int> > Matrix;

int allTiling(vector<Tile> &sigma, Matrix &region, int printOption);
bool existTiling(vector<Tile> &sigma, Matrix &region, int printOption);


#endif // POLYOMINOTILING_H_INCLUDED

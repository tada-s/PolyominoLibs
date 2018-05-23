#include "PolyominoTiling.h"

int calcFreeSpaces(Matrix &region);

int recursiveTiling(vector<Tile> &sigma, Matrix &region, int lastI, int lastJ);
bool checkBacktrack(vector<Tile> &sigma, Matrix &region, int lastI, int lastJ);
bool printSolution(vector<Tile> &sigma, Matrix &region);

bool isValidIndex(Matrix &region, int i, int j);
bool isFree(Tile &t, Matrix &region, int i, int j);

void putTile(Tile &t, Matrix &region, int i, int j);
void removeTile(Tile &t, Matrix &region, int i, int j);

bool isTilable(Tile &t, Matrix &region, int i, int j);

int freeSpace;
int tileNumber = 2;
int printTilings = PRINTOPTION_NOPRINT;
bool onlyOneTiling = false;

int allTiling(vector<Tile> &sigma, Matrix &region, int printOption){
    freeSpace = calcFreeSpaces(region);
    tileNumber = 2;
    printTilings = printOption;
    return recursiveTiling(sigma, region, 0, 0);
}

bool existTiling(vector<Tile> &sigma, Matrix &region, int printOption){
    freeSpace = calcFreeSpaces(region);
    tileNumber = 2;
    printTilings = printOption;
    onlyOneTiling = true;
    return recursiveTiling(sigma, region, 0, 0) > 0;
}

int calcFreeSpaces(Matrix &region){
    int c = 0;
    for(int i = 0; i < region.size(); i++){
        for(int j = 0; j < region[i].size(); j++){
            if(region[i][j] == 0){
                c++;
            }
        }
    }
    return c;
}

int recursiveTiling(vector<Tile> &sigma, Matrix &region, int lastI, int lastJ){

    if(freeSpace == 0){
        /* Found a tiling! */
        printSolution(sigma, region);
        return 1;

    }else if(checkBacktrack(sigma, region, lastI, lastJ)){
        /* Backtracking condition */
        return 0;
    }else{

        /* Brute force recursive call */

        int tN = 0;
        for(int i = lastI; i < region.size(); i++){
            for(int j = (i == lastI ? lastJ : 0); j < region[i].size(); j++){
                for(int k = 0; k < sigma.size(); k++){
                    Tile t = sigma[k];
                    if(isFree(t, region, i, j)){
                        putTile(t, region, i, j);
                        tN += recursiveTiling(sigma, region, i, j);
                        removeTile(t, region, i, j);
                    }
                    // break if a tiling found (if the 'onlyOneTiling' flag is activated)
                    if(tN != 0 && onlyOneTiling){
                        return tN;
                    }
                }
            }
        }
        return tN;
    }
}

bool printSolution(vector<Tile> &sigma, Matrix &region){
    if(printTilings == PRINTOPTION_NUMBERS){
        for(int i = 0; i < region.size(); i++){
            for(int j = 0; j < region[i].size(); j++){
                printf("%2d ", region[i][j]);
            }
            printf("\n");
        }
        printf("------------\n");
    }else if(printTilings == PRINTOPTION_LINES){
        printf("+");
        for(int j = 0; j < region[0].size(); j++){
            printf("--+");
        }
        printf("\n");
        for(int i = 0; i < region.size(); i++){
            printf("|");
            for(int j = 0; j < region[i].size() - 1; j++){
                if(region[i][j] == region[i][j + 1]){
                    printf("   ");
                }else{
                    printf("  |");
                }
            }
            printf("  |");
            printf("\n");
            if(i != region.size() - 1){
                printf("+");
                for(int j = 0; j < region[i].size(); j++){
                    if(region[i][j] == region[i + 1][j]){
                        printf("  +");
                    }else{
                        printf("--+");
                    }
                }
                printf("\n");
            }
        }
        printf("+");
        for(int j = 0; j < region[0].size(); j++){
            printf("--+");
        }
        printf("\n");
        printf("\n");
    }
}

bool checkBacktrack(vector<Tile> &sigma, Matrix &region, int lastI, int lastJ){
    if(!(lastI == 0 && lastJ == 0)){
        /* Checks if there is a empty cell before the current position */
        for(int i = 0; i <= lastI; i++){
            for(int j = 0; j <= ((i == lastI) ? lastJ : region[i].size() - 1); j++){
                if(region[i][j] == 0){
                    return true;
                }
            }
        }
        /* Checks if there exist some cell that is impossible to tile */
        for(int i = lastI; i < region.size(); i++){
            for(int j = ((i == lastI) ? lastJ + 1 : 0); j < region[i].size(); j++){
                if(region[i][j] == 0){
                    bool tilable = false;
                    for(int k = 0; k < sigma.size(); k++){
                        tilable |= isTilable(sigma[k], region, i, j);
                    }
                    if(!tilable){
                        return true;
                    }
                }
            }
        }
    }
    return false;
}

bool isValidIndex(Matrix &region, int i, int j){
    /* Check if is a valid array index */
    if(i < region.size()){
        if(j < region[i].size()){
            return true;
        }
    }
    return false;
}

bool isFree(Tile &t, Matrix &region, int i, int j){
    /* Checks if a tile t fits on region at position (i, j) */
    for(int k = 0; k < t.size(); k++){
        int x = i + t[k].first;
        int y = j + t[k].second;
        if(isValidIndex(region, x, y)){
            if(region[x][y] != 0){
                return false;
            }
        }else{
            return false;
        }
    }
    return true;
}

Tile normalizedTile(Tile &t){
    Tile newt;
    int minX = -INF;
    int minY = -INF;
    for(int k = 0; k < t.size(); k++){
        minX = min(minX, t[k].first);
        minY = min(minY, t[k].second);
    }
    for(int k = 0; k < t.size(); k++){
        newt.push_back(make_pair(t[k].first - minX, t[k].second - minY));
    }
    return newt;
}

void putTile(Tile &t, Matrix &region, int i, int j){
    /* Puts a tile t at position (i, j) */
    for(int k = 0; k < t.size(); k++){
        int x = i + t[k].first;
        int y = j + t[k].second;
        region[x][y] = tileNumber;
        freeSpace--;
    }
    tileNumber++;
}

void removeTile(Tile &t, Matrix &region, int i, int j){
    /* Removes a tile t at position (i, j) */
    for(int k = 0; k < t.size(); k++){
        int x = i + t[k].first;
        int y = j + t[k].second;
        region[x][y] = 0;
        freeSpace++;
    }
    tileNumber--;
}

bool isTilable(Tile &t, Matrix &region, int i, int j){
    /* Checks if there is a way to occupy a cell at position (i, j) by tile t */
    bool tilable = false;
    for(int k = 0; k < t.size(); k++){
        int x = i - t[k].first;
        int y = j - t[k].second;
        if(isFree(t, region, x, y)){
            return true;
        }
    }
    return false;
}

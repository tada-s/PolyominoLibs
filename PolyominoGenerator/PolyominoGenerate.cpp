#include "PolyominoGenerate.h"

#define INF (1<<29)

using namespace std;

typedef vector< pair<int, int> > Tile;

bool isBoundary(int i, int j);
void recursiveSpawnBlock(int spawnLife);
void recursiveSpawnTileBlock(int spawnLife);

int space[PROGPARAM_SPACESIZE][PROGPARAM_SPACESIZE];
set< string > generatedTileSet;
set< string > outputTileSet;
vector< pair<int, int> > boundary;
Tile currentTile;

vector<Tile> inputTileSet;
vector< vector< pair<int, int> > > tilesBoundary;
set< pair<int, int> > boundarySet;

int nx[] = {0, 1, 0, -1};
int ny[] = {1, 0, -1, 0};

set< string > spawnTileBlock(int spawnLife, vector<Tile> tiles){
    // Reset global variables
    outputTileSet.clear();
    generatedTileSet.clear();
    boundarySet.clear();
    currentTile.clear();
    for(int i = 0; i < PROGPARAM_SPACESIZE; i++){
        for(int j = 0; j < PROGPARAM_SPACESIZE; j++){
            space[i][j] = 0;
        }
    }
    tilesBoundary.clear();

    inputTileSet = tiles;

    // Calculate boundaries for inputTileSet
    for(int t = 0; t < inputTileSet.size(); t++){
        vector< pair<int, int> > b;
        for(int p = 0; p < inputTileSet[t].size(); p++){
            int i = inputTileSet[t][p].first;
            int j = inputTileSet[t][p].second;
            for(int k = 0; k < 4; k++){
                int bi = i + nx[k];
                int bj = j + ny[k];
                bool boundaryAlredyExist = false;
                for(int k2 = 0; k2 < b.size(); k2++){
                    if(b[k2].first == bi && b[k2].second == bj){
                        boundaryAlredyExist = true;
                        break;
                    }
                }
                if(!boundaryAlredyExist){
                    b.push_back(make_pair(bi, bj));
                }
            }
        }
        tilesBoundary.push_back(b);
    }

    // Initialize a boundary setting the position in the middle of space matrix
    boundarySet.insert(make_pair(PROGPARAM_SPACESIZE / 2, PROGPARAM_SPACESIZE / 2));

    // Start recursive algorithm
    recursiveSpawnTileBlock(spawnLife);

    return outputTileSet;
}

void recursiveSpawnTileBlock(int spawnLife){
    if(spawnLife == 0){
        // If a tile is found then save the tile (in unique string representation format as "tag").
        string tag = tagTile(currentTile);
        outputTileSet.insert(tag);
    }else{
        // For each boundary cell
        set< pair<int, int> >::iterator iter;
        for(iter = boundarySet.begin(); iter != boundarySet.end(); iter++){
            // Select as a new cell for the generating tile
            int i = iter->first;
            int j = iter->second;

            for(int t = 0; t < inputTileSet.size(); t++){
                for(int p = 0; p < inputTileSet[t].size(); p++){
                    int tileI = inputTileSet[t][p].first;
                    int tileJ = inputTileSet[t][p].second;


                    // Check if the cell does not overlap
                    bool overlap = false;
                    for(int p2 = 0; p2 < inputTileSet[t].size(); p2++){
                        int tileI2 = inputTileSet[t][p2].first - tileI + i;
                        int tileJ2 = inputTileSet[t][p2].second - tileJ + j;
                        if(space[tileI2][tileJ2] == 1){
                            overlap = true;
                        }
                    }
                    if(!overlap){
                        // Remove cells from boundary
                        stack< pair<int, int> > removedBoundaryCells;
                        for(int p2 = 0; p2 < inputTileSet[t].size(); p2++){
                            int tileI2 = inputTileSet[t][p2].first - tileI + i;
                            int tileJ2 = inputTileSet[t][p2].second - tileJ + j;
                            set< pair<int, int> >::iterator iter = boundarySet.find(make_pair(tileI2, tileJ2));
                            if(iter != boundarySet.end()){
                                boundarySet.erase(iter);
                                removedBoundaryCells.push(make_pair(tileI2, tileJ2));
                            }
                        }

                        // Add the selected cell to the current Tile and update the boundary
                        stack< pair<int, int> > insertedBoundaryCells;
                        for(int p2 = 0; p2 < tilesBoundary[t].size(); p2++){
                            int bI = tilesBoundary[t][p2].first - tileI + i;
                            int bJ = tilesBoundary[t][p2].second - tileJ + j;
                            set< pair<int, int> >::iterator iter = boundarySet.find(make_pair(bI, bJ));
                            if(iter == boundarySet.end()){
                                boundarySet.insert(make_pair(bI, bJ));
                                insertedBoundaryCells.push(make_pair(bI, bJ));
                            }
                        }
                        for(int p2 = 0; p2 < inputTileSet[t].size(); p2++){
                            int tileI2 = inputTileSet[t][p2].first - tileI + i;
                            int tileJ2 = inputTileSet[t][p2].second - tileJ + j;
                            space[tileI2][tileJ2] = 1;
                            currentTile.push_back(make_pair(tileI2, tileJ2));
                        }

                        // Recursion call to add next tile
                        string tag = tagTile(currentTile);
                        if(generatedTileSet.find(tag) == generatedTileSet.end()){
                            generatedTileSet.insert(tag);
                            recursiveSpawnTileBlock(spawnLife - 1);
                        }

                        // Revert changes done in this loop to prepare for the next iteration
                        for(int p2 = 0; p2 < inputTileSet[t].size(); p2++){
                            int tileI2 = inputTileSet[t][p2].first - tileI + i;
                            int tileJ2 = inputTileSet[t][p2].second - tileJ + j;
                            space[tileI2][tileJ2] = 0;
                            currentTile.pop_back();
                        }
                        while(!insertedBoundaryCells.empty()){
                            boundarySet.erase(insertedBoundaryCells.top());
                            insertedBoundaryCells.pop();
                        }
                        while(!removedBoundaryCells.empty()){
                            boundarySet.insert(removedBoundaryCells.top());
                            removedBoundaryCells.pop();
                        }
                    }
                }
            }

            iter = boundarySet.find(make_pair(i, j));
        }
    }
}

string spawnRandomTileBlock(int spawnLife, vector<Tile> tiles){
    return "";
}


set< string > spawnBlock(int spawnLife){
    // Reset global variables
    outputTileSet.clear();
    generatedTileSet.clear();
    boundary.clear();
    currentTile.clear();
    for(int i = 0; i < PROGPARAM_SPACESIZE; i++){
        for(int j = 0; j < PROGPARAM_SPACESIZE; j++){
            space[i][j] = 0;
        }
    }

    // Initialize a boundary setting the position in the middle of space matrix
    boundary.push_back(make_pair(PROGPARAM_SPACESIZE / 2, PROGPARAM_SPACESIZE / 2));

    // Start recursive algorithm
    recursiveSpawnBlock(spawnLife);

    return outputTileSet;
}

void recursiveSpawnBlock(int spawnLife){
    if(spawnLife == 0){
        // If a tile is found then save the tile (in unique string representation format as "tag").
        string tag = tagTile(currentTile);
        outputTileSet.insert(tag);
    }else{
        // For each boundary cell
        for(int k = 0; k < boundary.size(); k++){
            // Select as a new cell for the generating tile
            int i = boundary[k].first;
            int j = boundary[k].second;

            // Remove the selected cell from boundary
            swap(boundary[k], boundary[boundary.size() - 1]);
            boundary.pop_back();

            // Add the selected cell to the current Tile and update the boundary
            int addedBoundary = 0;
            for(int k2 = 0; k2 < 4; k2++){
                int i2 = i + nx[k2];
                int j2 = j + ny[k2];
                if(!isBoundary(i2, j2) && space[i2][j2] == 0){
                    boundary.push_back(make_pair(i2, j2));
                    addedBoundary++;
                }
            }
            space[i][j] = 1;
            currentTile.push_back(make_pair(i, j));

            // Check if the current Tile was already generated
            string tile = tagTile(currentTile);
            if(generatedTileSet.find(tile) == generatedTileSet.end()){
                // If not, add the next cell recursively
                generatedTileSet.insert(tile);
                recursiveSpawnBlock(spawnLife - 1);
            }

            // Revert changes done in this loop to prepare for the next iteration
            currentTile.pop_back();
            space[i][j] = 0;
            while(addedBoundary--){
                boundary.pop_back();
            }

            boundary.push_back(make_pair(i, j));
            swap(boundary[k], boundary[boundary.size() - 1]);
        }
    }
}

bool isBoundary(int i, int j){
    for(int k = 0; k < 4; k++){
        if(space[i + nx[k]][j + ny[k]] == 1){
            return true;
        }
    }
    return false;
}

// (It doesn't have an uniform distribution)
string spawnRandomBlock(int spawnLife){
    // Reset global variables
    boundary.clear();
    currentTile.clear();
    for(int i = 0; i < PROGPARAM_SPACESIZE; i++){
        for(int j = 0; j < PROGPARAM_SPACESIZE; j++){
            space[i][j] = 0;
        }
    }

    // Initialize a boundary setting the position in the middle of space matrix
    boundary.push_back(make_pair(PROGPARAM_SPACESIZE / 2, PROGPARAM_SPACESIZE / 2));

    // Start an iterative version
    while(spawnLife--){
        // Select a random cell from boundary as a new cell for the generating tile
        int k = rand() % boundary.size();
        int i = boundary[k].first;
        int j = boundary[k].second;

        // Remove the selected cell from boundary
        swap(boundary[k], boundary[boundary.size() - 1]);
        boundary.pop_back();

        // Add the selected cell to the current Tile and update the boundary
        int addedBoundary = 0;
        for(int k2 = 0; k2 < 4; k2++){
            int i2 = i + nx[k2];
            int j2 = j + ny[k2];
            if(!isBoundary(i2, j2) && space[i2][j2] == 0){
                boundary.push_back(make_pair(i2, j2));
                addedBoundary++;
            }
        }
        space[i][j] = 1;
        currentTile.push_back(make_pair(i, j));
    }

    string returnStr = tagTile(currentTile);
    boundary.clear();
    currentTile.clear();
    tagTile(currentTile);
    return returnStr;

}

#include "PolyominoGenerate.h"

#define INF (1<<29)

using namespace std;

typedef vector< pair<int, int> > Tile;

bool isBoundary(int i, int j);
void recursiveSpawnBlock(int spawnLife);

int space[PROGPARAM_SPACESIZE][PROGPARAM_SPACESIZE];
set< string > generatedTileSet;
set< string > outputTileSet;
vector< pair<int, int> > boundary;
Tile currentTile;


int nx[] = {0, 1, 0, -1};
int ny[] = {1, 0, -1, 0};

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

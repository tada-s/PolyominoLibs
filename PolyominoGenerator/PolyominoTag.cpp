#include "PolyominoTag.h"

string tagTile(Tile tile){
    if(tile.size() == 0){
        return "";
    }

    // Find the smallest surrounding box coordinates
    int minX = INF;
    int minY = INF;
    int maxX = -INF;
    int maxY = -INF;
    for(int i = 0; i < tile.size(); i++){
        minX = min(minX, tile[i].first);
        minY = min(minY, tile[i].second);
        maxX = max(maxX, tile[i].first);
        maxY = max(maxY, tile[i].second);
    }

    // Reshape the rectangular box to square box
    int boxSize = max(maxX - minX, maxY - minY) + 1;

    bool firstString = true;
    char smallestStr[boxSize * boxSize + 2];
    char str[boxSize * boxSize + 2];

    // Linear transformation variables to rotate 90 degrees
    int lTransfXa[] = {1    , -1   , 1    , -1   };
    int lTransfXb[] = {-minX , maxX, -minX , maxX};
    int lTransfYa[] = {1    , 1    , -1   , -1   };
    int lTransfYb[] = {-minY , -minY , maxY, maxY};

    for(int sw = 0; sw < 2; sw++){
        // For each two combination: reflected and not reflected
        for(int k = 0; k < 4; k++){
            // For each 4 rotation

            // Initialize a new box with corresponding string with zeros
            for(int i = 0; i < boxSize * boxSize + 2; i++){
                str[i] = '0';
            }
            str[0] = '1';
            str[boxSize * boxSize + 1] = '\0';

            // Set the bits of the string according to the position in the box
            for(int i = 0; i < tile.size(); i++){
                // Linear transformation to rotate
                int x = tile[i].first * lTransfXa[k] + lTransfXb[k];
                int y = tile[i].second * lTransfYa[k] + lTransfYb[k];
                // Swap variables to reflect
                int p = sw ? (x * boxSize + y + 1) : (y * boxSize + x + 1);
                // Set the bit
                str[p] = '1';
            }

            // Choose lexicographically the smallest string
            if(firstString){
                strcpy(smallestStr, str);
                firstString = false;
            }else{
                if(strcmp(smallestStr, str) > 0){
                    strcpy(smallestStr, str);
                }
            }
        }
    }

    return smallestStr;
}

Tile decodeTag(string tag){
    Tile tile;

    int boxSize = (int)sqrt(tag.length());
    for(int i = 0; i < boxSize; i++){
        for(int j = 0; j < boxSize; j++){
            if(tag[i * boxSize + j + 1] == '1'){
                tile.push_back(make_pair(i, j));
            }
        }
    }

    return tile;
}

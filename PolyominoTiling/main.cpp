#include <stdio.h>
#include <vector>
#include <utility>
#include <strings.h>
#include "PolyominoTiling.h"

using namespace std;

typedef vector< pair<int, int> > Tile;
typedef vector< vector<int> > Matrix;

int main(int argc, char *argv[]){
    int printOption = PRINTOPTION_NOPRINT;
    bool onlyOneTiling = false;

    if(argc < 2){
        printf("Syntax: PolyominoTiling.exe <InputFile> [optional:-p | -P] [optional:-s]\n", argv[1]);
        printf("   -p : output each completed tiling enumerating the tiles.\n");
        printf("   -P : output each completed tiling with lines.\n");
        printf("   -s : stop when found a perfect tiling.\n");
        return -1;
    }
    FILE *fpIn;
    fpIn = fopen(argv[1], "r");
    if(fpIn == NULL){
        printf("Error starting to read the file %s.\n", argv[1]);
        return -2;
    }
    for(int i = 2; i < argc; i++){
        if(strcmp(argv[i], "-p") == 0){
            printOption = PRINTOPTION_NUMBERS;
        }else if(strcmp(argv[i], "-P") == 0){
            printOption = PRINTOPTION_LINES;
        }else if(strcmp(argv[i], "-s") == 0){
            onlyOneTiling = true;
        }else{
            printf("Unknown optional parameter %s\n.", argv[i]);
        }
    }

    /* Load Tiles from file */

    vector<Tile> sigma;
    int a;
    fscanf(fpIn, "%d", &a);
    for(int i = 0; i < a; i++){
        Tile t;
        int b;
        fscanf(fpIn, "%d", &b);
        for(int j = 0; j < b; j++){
            int x, y;
            fscanf(fpIn, "%d %d", &x, &y);
            t.push_back(make_pair(x, y));
        }
        sigma.push_back(t);
    }

    /* Load Region from file */

    Matrix region;
    int m, n;
    fscanf(fpIn, "%d%d", &m, &n);
    region.resize(m);
    for(int i = 0; i < m; i++){
        for(int j = 0; j < n; j++){
            int x;
            fscanf(fpIn, "%d", &x);
            region[i].push_back(x);
        }
    }

    fclose(fpIn);

    /* Start calcuation */

    if(onlyOneTiling){
        printf("Calculating the tilability for %s\n", argv[1]);
        bool exist = existTiling(sigma, region, printOption);
        printf("%s\n", exist ? "There exist a tiling" : "Is not tileable");
    }else{
        printf("Counting tilings for %s\n", argv[1]);
        int tN = allTiling(sigma, region, printOption);
        printf("Number of tiling is: %d\n", tN);
    }

    return 0;
}


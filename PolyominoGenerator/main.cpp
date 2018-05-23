#include <bits/stdc++.h>
#include "PolyominoGenerate.h"
#include "PolyominoTag.h"

using namespace std;

#define INF (1<<29)
typedef vector< pair<int, int> > Tile;

void writeTileToFile(string str, FILE* fp);
void printTile(string str);

#define OPTIONVALUE_PRINTCONSOLE_FALSE 0
#define OPTIONVALUE_PRINTCONSOLE_TRUE 1
#define OPTIONVALUE_GENERATE_ALL 1
#define OPTIONVALUE_GENERATE_RANDOM 2

int OPTION_PRINTCONSOLE = OPTIONVALUE_PRINTCONSOLE_FALSE;
int OPTION_GENERATE = OPTIONVALUE_GENERATE_RANDOM;


unsigned long mix(unsigned long a, unsigned long b, unsigned long c){
    a=a-b;  a=a-c;  a=a^(c >> 13);
    b=b-c;  b=b-a;  b=b^(a << 8);
    c=c-a;  c=c-b;  c=c^(b >> 13);
    a=a-b;  a=a-c;  a=a^(c >> 12);
    b=b-c;  b=b-a;  b=b^(a << 16);
    c=c-a;  c=c-b;  c=c^(b >> 5);
    a=a-b;  a=a-c;  a=a^(c >> 3);
    b=b-c;  b=b-a;  b=b^(a << 10);
    c=c-a;  c=c-b;  c=c^(b >> 15);
    return c;
}

int main(int argc, char* argv[]){
    // Just to randomize the seed for C random number generator.
    srand(mix(clock(), time(NULL), getpid()));

    if(argc < 3){
        printf("Syntax: PolyominoGenerator.exe <OutputFile> <blockNumber> [optional: -p] [optional: -r | -a]\n", argv[1]);
        printf("   -p : print generated polyomino.\n");
        printf("   -r : (default) generate a single random polyomino of size <blockNumber>. \n");
        printf("   -a : generate all polyomino of size <blockNumber>.\n");
        return -1;
    }
    FILE *fpOut;
    int blockNumber = 0;

    fpOut = fopen(argv[1], "w");
    blockNumber = atoi(argv[2]);
    if(fpOut == NULL){
        printf("Error writing the file %s.\n", argv[1]);
        return -2;
    }
    for(int i = 3; i < argc; i++){
        if(strcmp(argv[i], "-p") == 0){
            OPTION_PRINTCONSOLE = OPTIONVALUE_PRINTCONSOLE_TRUE;
        }else if(strcmp(argv[i], "-r") == 0){
            OPTION_GENERATE = OPTIONVALUE_GENERATE_RANDOM;
        }else if(strcmp(argv[i], "-a") == 0){
            OPTION_GENERATE = OPTIONVALUE_GENERATE_ALL;
        }else{
            printf("Unknown optional parameter %s\n.", argv[i]);
        }
    }

    set<string> tileSet;
    switch(OPTION_GENERATE){
        case OPTIONVALUE_GENERATE_ALL:
            tileSet = spawnBlock(blockNumber);
            break;
        case OPTIONVALUE_GENERATE_RANDOM:
            tileSet.insert(spawnRandomBlock(blockNumber));
            break;
    }

    for(set<string>::iterator iter = tileSet.begin(); iter != tileSet.end(); iter++){
        writeTileToFile(*iter, fpOut);
        printTile(*iter);
    }

    printf("Total number of printed tiles: %d\n", tileSet.size());

    fclose(fpOut);

    return 0;
}

void writeTileToFile(string str, FILE* fp){
    int boxSize = (int)sqrt(str.length());

    fprintf(fp, "%d %d\n", boxSize, boxSize);
    for(int i = 0; i < boxSize; i++){
        for(int j = 0; j < boxSize; j++){
            fprintf(fp, "%d ", str[i * boxSize + j + 1] - '0');
        }
        fprintf(fp, "\n");
    }
    fprintf(fp, "\n");
}

void printTile(string str){
    int boxSize = (int)sqrt(str.length());

    if(OPTION_PRINTCONSOLE == OPTIONVALUE_PRINTCONSOLE_TRUE){
        printf("%s\n", str.c_str());
        int region[boxSize + 2][boxSize + 2];
        for(int i = 0; i < boxSize + 2; i++){
            for(int j = 0; j < boxSize + 2; j++){
                region[i][j] = 0;
            }
        }
        for(int i = 0; i < boxSize; i++){
            for(int j = 0; j < boxSize; j++){
                region[i + 1][j + 1] = str[i * boxSize + j + 1] - '0';
            }
        }

        boxSize = boxSize + 2;

        printf("+");
        for(int j = 0; j < boxSize; j++){
            printf("--+");
        }
        printf("\n");
        for(int i = 0; i < boxSize; i++){
            printf("|");
            for(int j = 0; j < boxSize - 1; j++){
                if(region[i][j] == region[i][j + 1]){
                    printf("   ");
                }else{
                    printf("  |");
                }
            }
            printf("  |");
            printf("\n");
            if(i != boxSize - 1){
                printf("+");
                for(int j = 0; j < boxSize; j++){
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
        for(int j = 0; j < boxSize; j++){
            printf("--+");
        }
        printf("\n");
        printf("\n");
    }
}


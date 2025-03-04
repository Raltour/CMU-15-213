#include "cachelab.h"
#include <stdlib.h>
#include <unistd.h>
#include <bits/getopt_core.h>
#include <stdbool.h>


typedef struct Line {
    bool vilid_bit;
    bool dirty_bit;
    bool* tag;
    char* block;
}Line;

Line* newline(int tag_size, int block_size) {
    bool* tag = (bool*)malloc(sizeof(bool) * tag_size);
    char* block = (char*)malloc(sizeof(char) * block_size);

    Line line = {
        .vilid_bit = false,
        .dirty_bit = false,
        .tag = tag,
        .block = block
    };

    return &line;
}

void deleteline(Line* line) {
    free(line->block);
    free(line->block);
}

typedef struct Cache {

}Cache;


int main(int argc, int* argv)
{
    int opt, s, E, b;

    while ((opt = getopt(argc, argv, "sEb:")) != -1) {
        switch(opt) {
            case 's' :
                s = atoi(optarg);
                break;
            case 'E' :
                E = atoi(optarg);
                break;
            case 'b' :
                b = atoi(optarg);
                break;
        }
    }

    printSummary(0, 0, 0);
    return 0;
}

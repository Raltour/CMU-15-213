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
    int max_lines;
    int sets_num;
    Line*** sets;
}Cache;

Cache* newCache(int S, int E, int tag_size, int block_size) {
    
    Line*** sets = (Line***)malloc(sizeof(Line**) * S);
    for (int i = 0; i < S; i++) {
        sets[i] = (Line**)malloc(sizeof(Line*) * E);
        for (int j = 0; j < E; j++) {
            sets[i][j] = newline(tag_size, block_size);
        }
    }

    Cache cache = {
        .max_lines = E,
        .sets_num = S,
        .sets = sets
    };

    return &cache;
}

void deletecache(Cache* cache) {
    for (int i = 0; i < cache->sets_num; i++) {
        for (int j = 0; j < cache->max_lines; j ++) {
            deleteline(cache->sets[i][j]);
        }
        free(cache->sets[i]);
    }
    free(cache->sets);
}


int main(int argc, int* argv)
{
    int opt, s, E, b;

    while ((opt = getopt(argc, argv, "s:E:b:")) != -1) {
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

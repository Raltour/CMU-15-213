#include "cachelab.h"
#include <stdlib.h>
// #include <unistd.h>
// #include <bits/getopt_core.h>
#include <stdbool.h>


typedef struct Line {
    bool vilid_bit;
    bool dirty_bit;
    unsigned tag;
    struct Line* prev;
    struct Line* next;
}Line;

Line* newline(Line* prev, Line* next) {
    Line line = {
        .vilid_bit = false,
        .dirty_bit = false,
        .tag = 0,
        .prev = prev,
        .next = next
    };

    return &line;
}

typedef struct Set {
    Line* first;
    Line* last;
    int lines_num;
}Set;

Set* newSet(int lines_num) {
    Set set;
    set.last = newline(NULL, NULL);
    set.first = set.last;
    for (int i = 1; i < lines_num; i++) {
        set.first = newline(NULL, set.first);
        set.first->next->prev = set.first;
    }
    set.lines_num = lines_num;

    return &set;
}

typedef struct Cache {
    int lines_num;
    int sets_num;
    Set* sets;
}Cache;

Cache* newCache(int S, int E) {
    
    Set** sets = (Set**)malloc(sizeof(Set*) * S);
    for (int i = 0; i < S; i++) {
        sets[i] = newSet(E);
    }

    Cache cache = {
        .lines_num = E,
        .sets_num = S,
        .sets = sets
    };

    return &cache;
}

void deletecache(Cache* cache) {
    free(cache->sets);
}

//查找set中是否有某一个tag,若有则返回这个line
Line* find_line(Set* set, int lines_num, unsigned int tag) {
    Line* p = set->first;
    while (p != set->last) {
        if (p->vilid_bit && p->tag == tag) {
            return p;
        }
        p = p->next;
    }
    if (p->vilid_bit && p->tag == tag) {
        return p;
    }
    return NULL;
}

//将链表中的一项移动到末端
void move_to_last(Set* set, Line* line) {
    if (line == set->last) {
        return ;
    } else if (line == set->first) {
        set->last->next = set->first;
        set->last = set->first;
        set->first = set->first->next;
        set->last->next = NULL;
    } else {
        set->last->next = line;
        set->last = line;
        line->prev->next = line->next;
        line->next = NULL;
    }
}

//确定某一块不在内存当中时，调用此函数，将这一块加载进来，覆盖掉链表的头部，即使用时间最久远的一项
void cache_load(Cache* cache, int tag, int set_index, int offset) {
    set_index %= cache->sets_num;
    Set* aimSet = &cache->sets[set_index];

    aimSet->first->tag = tag;
    aimSet->first->vilid_bit = true;
    move_to_last(aimSet, aimSet->first);
}


int main(int argc, int* argv)
{
    int opt, s, E, b;

    // while ((opt = getopt(argc, argv, "s:E:b:")) != -1) {
    //     switch(opt) {
    //         case 's' :
    //             s = atoi(optarg);
    //             break;
    //         case 'E' :
    //             E = atoi(optarg);
    //             break;
    //         case 'b' :
    //             b = atoi(optarg);
    //             break;
    //     }
    // }

    int S = 1 << s;
    int B = 1 << b;
    int tag_size = 4;

    Cache* myCache = newCache(S, E);

    printSummary(0, 0, 0);

    deletecache(myCache);
    return 0;
}

/*
 * mm-naive.c - The fastest, least memory-efficient malloc package.
 * 
 * In this naive approach, a block is allocated by simply incrementing
 * the brk pointer.  A block is pure payload. There are no headers or
 * footers.  Blocks are never coalesced or reused. Realloc is
 * implemented directly using mm_malloc and mm_free.
 *
 * Use a Segregated Free Lists, the Segregated Fits.
 * With Header, Footer, succ to manipulate the neighbor blocks
 * in order to implements coalesce.
 * Handle request for memmory between 16 to 8184 bytes.
 */
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <unistd.h>
#include <string.h>

#include "mm.h"
#include "memlib.h"

/*********************************************************
 * NOTE TO STUDENTS: Before you do anything else, please
 * provide your team information in the following struct.
 ********************************************************/
team_t team = {
    /* Team name */
    "COOL!",
    /* First member's full name */
    "HAHAHA",
    /* First member's email address */
    "WishToBecomeAStudentInCMU@cs.cmu.edu",
    /* Second member's full name (leave blank if none) */
    "",
    /* Second member's email address (leave blank if none) */
    ""
};



/*word size and double word size*/
#define WSIZE 4
#define DSIZE 8

/*get the size_t number in the address ptr*/
#define DEREF(ptr) (*(unsigned int *)(ptr))

/*put the number val to the address ptr*/
#define PUT(ptr, val) ((DEREF(ptr)) = (val))

/*return a pointer to the next block in the free list*/
#define NEXT_NODE(ptr) (*(size_t *)(ptr))

/*get the block size by the ptr to header or footer*/
#define GET_BLOCK_SIZE(ptr) (DEREF((size_t *)(ptr)) & ~0x7)

/*check if alloced by ptr to header or footer*/
#define IS_ALLOCED(ptr) (DEREF((size_t *)(ptr)) & 0x1)

/*compute the address of the header and footer of the block*/
#define HEADER(bp) ((size_t *)(bp) - 1)
#define FOOTER(bp) ((char *)(bp) + GET_BLOCK_SIZE((char *)(bp) - WSIZE) - DSIZE)

/*compute the address of the neighbor block*/
#define PREV_BLKP(bp) ((char *)(bp) - GET_BLOCK_SIZE((char *)(bp) - DSIZE))
#define NEXT_BLKP(bp) ((char *)(bp) + GET_BLOCK_SIZE((char *)(bp) - WSIZE))



static int ln2(int x);

static int pow2(int x);

static size_t* getListPtr(int k);

static void arrayPtrToNext(int k);

static int adjustSize(int size);

static size_t* splitBlock(size_t* ptr, int k);

static size_t *extendHeap(int k);

static void* place(size_t * ptr, int k, int state);

static void* coalesce(void* ptr);

static void inserFreeBlock(void* ptr);



/* 
 * mm_init - initialize the malloc package.
 */
int mm_init(void) {
    void *p = mem_sbrk(10 * sizeof(size_t));
    if (p == (void *) - 1) {
        return 0;
    } else {
        size_t *array = (size_t*)p;
        for (int i = 0; i < 10; i++) {
            *array = 0;
            array++;
        }
    }

    mem_sbrk(4);
    // void* next = mem_heap_hi() + 1;
    // int incr = ((int)(next + 7) & ~0x7) - (int)next;
    // mem_sbrk(incr);
    return 0;
}

/* 
 * mm_malloc - Allocate a block by incrementing the brk pointer.
 * Always allocate a block whose size is a multiple of the alignment.
 * this function also checks the size, return NULL if k is illegal
 */
void *mm_malloc(size_t size) {
    if (size <= 0 || size > 8192)
        return NULL;

    int k = adjustSize(size);
    int temp_k = k;

    size_t* block = NULL;
    while (k < 14) {
        block = getListPtr(k);
        if (*block == 0) {
            k++;
        } else {
            block = (size_t*)(*block);
            arrayPtrToNext(k);
            block = splitBlock(block, temp_k);
            break;
        }
    }
    if (k == 14) {
        block = extendHeap(temp_k);
        block++;
        place(block, temp_k, 1);
    }

    return (void *)block;
}

/*
 * mm_free - Freeing a block does nothing.
 */
void mm_free(void *ptr) {
    void* block_ptr = coalesce(ptr);
    inserFreeBlock(block_ptr);
}

/*
 * mm_realloc - Implemented simply in terms of mm_malloc and mm_free
 */
void *mm_realloc(void *ptr, size_t size) {
    return NULL;
}



/**
 * return log(2)(x)
 */
// static int ln2(int x) {
//     int k = 0;
//     while (x > 1) {
//         x /= 2;
//         k++;
//     }

//     return k;
// }
static int ln2(int x) {
    int k = 0;
    int size = 1;
    while (size < x) {
        size <<= 1;
        k++;
    }
    return k;
}

/**
 * return 2 ^ x
 */
static int pow2(int x) {
    int num = 1;
    for (int i = 0; i < x; i++) {
        num *= 2;
    }
    return num;
}

/**
 * return a pointer to the right position in the list array.
 */
static size_t* getListPtr(int k) {
    size_t *p = (size_t *)mem_heap_lo();
    p += (k - 4);
    return p;
}

/**
 * because the free list in the array is used
 * the pointer should point to the next block in the free list
 */
static void arrayPtrToNext(int k) {
    size_t *p = (size_t *)mem_heap_lo();
    p += (k - 4);
    PUT(p, NEXT_NODE(DEREF(p)));
}

/**
 * return the pow, satisfy that
 * 2 ^ k >= payload(size here) + 8(overhead)
 */
static int adjustSize(int size) {
    size += 8;
    int k = ln2(size);
    if (k < 4)
        k = 4;
    return k;
}

/**
 * get the ponter to the right position in the array and the size we need.
 * return the pointer to the block used to malloc.
 * (not to the header, to the start of the malloc memorry)
 * split the original block to meet the size.
 * the leaved block(if exist)
 */
static size_t* splitBlock(size_t* ptr, int k) {
    int block_size = GET_BLOCK_SIZE(*ptr - WSIZE);
    int block_k = ln2(block_size);
    if (block_k != k) {
        place(ptr + k, block_k - k, 0);
    }

    place((size_t*)(*ptr), k, 1);

    return (size_t*)(*ptr);
}

/**
 * get more heap memmory for malloc.
 * size = 2 ^ k
 */
static size_t *extendHeap(int k) {
    return (size_t*)mem_sbrk(pow2(k));
}

/**
 * init the header and the footer according to the k(size = 2 ^ k)
 * set the state to show weather the block is allocated or free
 * if the block is free, set the pointer to the next free block
 * return the block pointer
 */
static void* place(size_t * ptr, int k, int state) {
    if (state) {
        PUT((ptr - 1), pow2(k) | 0x1);
        PUT((ptr + k - 2), pow2(k) | 0x1);
    } else {
        PUT((ptr - 1), pow2(k));
        PUT((ptr + k - 2), pow2(k));
        PUT((ptr), DEREF(getListPtr(k)));
    }
    return (void*)ptr;
}

/**
 * merge the free blocks
 */
static void* coalesce(void* ptr) {
    int prev = IS_ALLOCED(ptr - DSIZE);
    int next = IS_ALLOCED(ptr + GET_BLOCK_SIZE(ptr - WSIZE) - WSIZE);
    if (ptr - DSIZE < mem_heap_lo() + 44) {
        prev = 1;
    }
    if (ptr + GET_BLOCK_SIZE(ptr - WSIZE) - WSIZE > mem_heap_hi()) {
        next = 1;//just ignore the area over the heap
    }
    if (prev && next) {
        return place((size_t*)(ptr), ln2(GET_BLOCK_SIZE(HEADER(ptr))), 0);
    } else if (!prev && next){
        int prev_size = GET_BLOCK_SIZE(ptr - DSIZE);
        return place((size_t*)(ptr - prev_size), ln2(GET_BLOCK_SIZE(HEADER(ptr)) + prev_size), 0);
    } else if (prev && !next) {
        int next_size = GET_BLOCK_SIZE(ptr +GET_BLOCK_SIZE(ptr - WSIZE) - WSIZE);
        return place((size_t*)(ptr), ln2(GET_BLOCK_SIZE(ptr) + next_size), 0);
    } else {
        int prev_size = GET_BLOCK_SIZE(ptr - DSIZE);
        int next_size = GET_BLOCK_SIZE(ptr +GET_BLOCK_SIZE(ptr - WSIZE) - WSIZE);
        return place((size_t*)(ptr - prev_size), ln2(GET_BLOCK_SIZE(HEADER(ptr)) + prev_size + next_size), 0);
    }
}

/**
 * insert the free block back to the array of free list.
 */
static void inserFreeBlock(void* ptr) {
    int size = GET_BLOCK_SIZE(ptr);
    int k = ln2(size);
    PUT(ptr, *getListPtr(k));
    PUT(getListPtr(k), (size_t*)ptr);
}



// /* single word (4) or double word (8) alignment */
// #define ALIGNMENT 8

// /* rounds up to the nearest multiple of ALIGNMENT */
// #define ALIGN(size) (((size) + (ALIGNMENT-1)) & ~0x7)

// #define SIZE_T_SIZE (ALIGN(sizeof(size_t)))

// /* 
//  * mm_init - initialize the malloc package.
//  */
// int mm_init(void)
// {
//     return 0;
// }

// /* 
//  * mm_malloc - Allocate a block by incrementing the brk pointer.
//  *     Always allocate a block whose size is a multiple of the alignment.
//  */
// void *mm_malloc(size_t size)
// {
//     int newsize = ALIGN(size + SIZE_T_SIZE);
//     void *p = mem_sbrk(newsize);
//     if (p == (void *)-1)
// 	return NULL;
//     else {
//         *(size_t *)p = size;
//         return (void *)((char *)p + SIZE_T_SIZE);
//     }
// }

// /*
//  * mm_free - Freeing a block does nothing.
//  */
// void mm_free(void *ptr)
// {
// }

// /*
//  * mm_realloc - Implemented simply in terms of mm_malloc and mm_free
//  */
// void *mm_realloc(void *ptr, size_t size)
// {
//     void *oldptr = ptr;
//     void *newptr;
//     size_t copySize;
    
//     newptr = mm_malloc(size);
//     if (newptr == NULL)
//       return NULL;
//     copySize = *(size_t *)((char *)oldptr - SIZE_T_SIZE);
//     if (size < copySize)
//       copySize = size;
//     memcpy(newptr, oldptr, copySize);
//     mm_free(oldptr);
//     return newptr;
// }
/*
 *  cache total size is 32 bytes
 * */

#include<stdio.h>
#include<math.h>
#include<assert.h>
#include<stdlib.h>

#include "cache.h"
#include "memory.h"

/* 
 * configure following parameters of the cache in question 2
 *
 * */

#define S 4
#define B 8
#define E 1

/* define the struct of the cache 
 * the Line struct below is given as the building brick
 *
 * */

struct Line {
    int valid;
    unsigned short tag;
    unsigned char block[B];
};

struct Set {
    struct Line *lines;
};

struct cache {
    struct Set *sets;
};

/* global variable C to denote the cache */
static int is_init = 0;
static struct cache C;

void init_cache() {
    /* to initialize the cache */
    assert(B * E * S == 32);
    struct Set tempSet;
    struct Line tempLine;
    int i, j;

    C.sets = (struct Set *) malloc(S * sizeof(struct Set));

    for (j = 0; j < S; j++) {
        tempSet.lines = (struct Line *) malloc(E * sizeof(struct Line));

        C.sets[j] = tempSet;

        for (i = 0; i < E; i++) {
            tempLine.tag = 0;
            tempLine.valid = 0;
            tempSet.lines[i] = tempLine;
        }
    }
} // init_cache


void print_line(struct Line l) {
    printf("valid:%d; tag:", l.valid);
    printf("%x", l.tag);

    printf("; block:");
    for (int j = 0; j < B; j++) {
        printf("%02x", l.block[j]);
    }
    printf("\n");
} // print_line


int search_cache(unsigned short addr, unsigned char *ptr) {
    /* search the cache to find a cache hit or end up with miss */

    int result = 0;
    int indexWidth = log2(S);
    int blockOffset = log2(B);
    int addrWidth = 1;
    int tempAddr = addr;

    while (tempAddr > 9) {
        addrWidth++;
        tempAddr /= 10;
    }

    // Get tge set index and tag.
    int setIndex = (addr >> blockOffset) & (S - 1);
    int tag = addr >> (indexWidth + blockOffset);
    int block_bit = ((1 << blockOffset) - 1) & (addr >> 1);

    // Go to the set index and tag
    struct Set temp;
    temp = C.sets[setIndex];

    // Iterate over the tags in a set to look for a match
    for (int i = 0; i < E; i++){
        if ((temp.lines[i].tag == tag) && (temp.lines[i].valid)) {
            result = 1;
            *ptr = temp.lines[i].block[block_bit];
        }
    }
    return result;
} // search_cache

void fill_cache(unsigned short addr) {
    /* read a block from memory and store it in the cache */

    int indexWidth = log2(S);
    int blockOffset = log2(B);
    int addrWidth = 1;
    int tempAddr = addr;

    while (tempAddr > 9) {
        addrWidth++;
        tempAddr /= 10;
    }

    long castedAddress = addr;
    long *ptr = (long *) castedAddress;

    unsigned char blockTemp[B];
    int to_evict = rand() % E;

    assert(to_evict < E);

    // Get the memory block an address.
    for (int i = 0; i < B; i++) {
        blockTemp[i] = ptr;
        ptr++;
    }

    int setIndex = (addr >> blockOffset) & (S - 1);
    int tag = addr >> (indexWidth + blockOffset);

    /*
     * Go set and line using the value from to_evict and fill in a new memory block
     * struct Set temp;
     * temp = C.sets[set_index]
     */
    for (int i = 0; i < B; i++) {
        C.sets[setIndex].lines[to_evict].block[i] = blockTemp[i];
    }

    C.sets[setIndex].lines[to_evict].valid = 1;
    C.sets[setIndex].lines[to_evict].tag = tag;
} // fill_cache

unsigned char read_cache(unsigned short addr) {
    int is_hit;
    unsigned char a;

    if (is_init == 0) {
        init_cache();
        is_init = 1;
    }
    is_hit = search_cache(addr, &a);

    if (is_hit == 0) {
        fill_cache(addr);
        search_cache(addr, &a);
    }
    return a;
} // read_cache



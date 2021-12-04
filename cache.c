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

    /* to initialize the cache */

}


void print_line(struct Line l) {
    printf("valid:%d; tag:", l.valid);
    printf("%x", l.tag);

    printf("; block:");
    for (int j = 0; j < B; j++) {
        printf("%02x", l.block[j]);
    }
    printf("\n");
}


int search_cache(unsigned short addr, unsigned char *ptr) {
    /* search the cache to find a cache hit or end up with miss */

    int result = 0;
    int index_width = log2(S);
    int block_offset = log2(B);
    int addr_width = 1;
    int temp_addr = addr;

    while (temp_addr > 9) {
        addr_width++;
        temp_addr /= 10;
    }

    int set_index = (addr >> block_offset) & (S - 1);
    int tag = addr >> (index_width + block_offset);
    int block_bit = ((1 << block_offset) - 1) & (addr >> 1);

    struct Set temp;
    temp = C.sets[set_index];

    for (int i = 0; i < E; i++) {
        if ((temp.lines[i].tag == tag) && (temp.lines[i].valid)) {
            result = 1;
            *ptr = temp.lines[i].block[block_bit];
        }
    }
    return result;
} // search_cache


void fill_cache(unsigned short addr) {
    /* read a block from memory and store it in the cache */

    int index_width = log2(S);
    int block_offset = log2(B);
    int addr_width = 1;
    int temp_addr = addr;

    while (temp_addr > 9) {
        addr_width++;
        temp_addr /= 10;
    }

    long casted_addr = addr;
    long *ptr = (long *) casted_addr;

    unsigned char block_temp[B];
    int to_evict = rand() % E;

    assert(to_evict < E);

    // Get the memory block at address
    for (int i = 0; i < B; i++) {
        block_temp[i] = ptr;
        ptr++;
    }

    int set_index = (addr >> block_offset) & (S - 1);
    int tag = addr >> (index_width + block_offset);


    for (int i = 0; i < B; i++) {
        C.sets[set_index].lines[to_evict].block[i] = block_temp[i];
    }

    C.sets[set_index].lines[to_evict].valid = 1;
    C.sets[set_index].lines[to_evict].tag = tag;
}


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
}



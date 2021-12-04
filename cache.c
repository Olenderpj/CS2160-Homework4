/*
 *  cache total size is 32 bytes
 * */


#include<stdio.h>
#include<math.h>
#include<assert.h>
#include<stdlib.h>
#include<time.h>


#include "cache.h"
#include "memory.h"

/* 
 * configure following parameters of the cache in question 2
 *
 * */

#define S 4
#define B 4
#define E 2


/* define the struct of the cache 
 * the Line struct below is given as the building brick
 * */

struct Line {
    int valid;
    unsigned short tag;
    unsigned char block[B];
};



struct Set {
    struct Line lines[E];
};

struct cache {
    struct Set sets[S];
};



/* global variable C to denote the cache */
static int is_init = 0;
// static struct Cache C;


void init_cache() {
    assert(B*E*S == 32);
    
    /* to initialize the cache */

}


void print_line(struct Line l) {
    printf("valid:%d; tag:", l.valid);
    printf("%x", l.tag);

    printf("; block:");
    for(int j = 0; j < B; j++) {
        printf("%02x", l.block[j]);
    }
    printf("\n");
}







int search_cache(unsigned short addr, unsigned char *ptr) {
    /* search the cache to find a cache hit or end up with miss */
}


void fill_cache(unsigned short addr) {
    /* read a block from memory and store it in the cache */

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



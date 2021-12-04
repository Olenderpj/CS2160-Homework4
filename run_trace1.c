/*
 *  run_trace1.c simulates a CPU getting data from the memory system.
 *  the simulated trace for accessing data: iterate through a array of SIZE with 1-byte element
 *  
 *  you may turn off cache (by setting cache_switch to be zero) to see why it is indispensable 
 *
 *
 * */



#include<stdio.h>
#include "memory.h"
#include "cache.h"
#include <time.h>



#define SIZE 100

int main() {

    unsigned short addr = 0x22;
    unsigned char mm[SIZE];

    int cache_switch = 1;

    for(int i = 0; i < SIZE; i++) {
        if (cache_switch == 1) {
            mm[i] = read_cache(addr+i);
        }
        else {
            read_memory(addr+i, &mm[i], 1);
        }
    }

    // print out fetched data

    printf("data fetched:\n");
     for(int i = 0; i < SIZE; i++) {
           printf("%02x ", mm[i]);
     }
    printf("\n");
    return 0;
}

/*
 *  run_trace2.c simulates a CPU getting data from the memory system.
 *  the simulated trace for accessing data: two arrays (mm1, mm2) of SIZE with 1-byte element; conduct mm1[i] xor mm2[i]
 *  
 *  you may turn off cache (by setting cache_switch to be zero) to see why it is indispensable 
 *
 *
 * */



#include<stdio.h>
#include "memory.h"
#include "cache.h"
#include <time.h>



#define SIZE 50

int main() {

    unsigned short addr1 = 0x18;
    unsigned char mm1[SIZE];

    unsigned short addr2 = 0x68;
    unsigned char mm2[SIZE];

    unsigned char sum[SIZE];

    int cache_switch = 1;

    for(int i = 0; i < SIZE; i++) {
        if (cache_switch == 1) {
            mm1[i] = read_cache(addr1+i);
            mm2[i] = read_cache(addr2+i);
            sum[i] = mm1[i] ^ mm2[i];
        }
        else {
            read_memory(addr1+i, &mm1[i], 1);
            read_memory(addr2+i, &mm2[i], 1);
            sum[i] = mm1[i] ^ mm2[i];
        }
    }

    // print out fetched data

    printf("data fetched #1:\n");
     for(int i = 0; i < SIZE; i++) {
           printf("%02x ", mm1[i]);
     }
    printf("\n");
    printf("data fetched #2:\n");
     for(int i = 0; i < SIZE; i++) {
           printf("%02x ", mm2[i]);
     }
    printf("\n");
    return 0;
}

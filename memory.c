#include<stdio.h>
#include<assert.h>
#include<unistd.h>
#include"memory.h"




unsigned char num_generator(unsigned short addr) {
    return (unsigned char) (addr%10 + addr%5 + addr%15);
}



/* addr: unsigned to denote 16-bit address
 *
 * */
void read_memory(unsigned short addr, unsigned char *dst, int num_bytes) {

    assert(num_bytes <= 32);

    int i;
    for (i = 0; i < num_bytes; i++)  {
        dst[i] = num_generator(addr+i);
    }

    /* insert 0.2 seconds delay */
    usleep(200000);

}

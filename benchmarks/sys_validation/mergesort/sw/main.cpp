#include <cstdio>
#include <cstdlib>
#include <cstring>
#include "bench.h"
#include "../../../common/m5ops.h"

volatile uint8_t  * top   = (uint8_t  *)0x2f000000;
volatile uint32_t * unsorted = (uint32_t *)0x2f000001;
volatile uint32_t * sorted = (uint32_t *)0x2f000009;

int __attribute__ ((optimize("0"))) main(void) {
	m5_reset_stats();
    uint32_t base = 0x80c00000;
	TYPE *originalArr = (TYPE *)base;
    TYPE *sortedArr = (TYPE *) (base+SIZE*sizeof(TYPE));
    int j = 0;
    for(int i = SIZE-1; i >= 0; i--) {
        originalArr[j] = i;
        j++;
    }
    // Prints out generated array
    // for(int i = 0; i < SIZE; i++){
    //     printf("Original Array: Index: %d Value: %d\n", i, originalArr[i]);
    // }
    // Prints out what should be the empty sorted array
    for(int i = 0; i < SIZE; i++){
        printf("Empty Array: Index: %d Value: %d\n", i, sortedArr[i]);
    }
    volatile int count = 0;
	stage = 0;

    // Set top address MMRs
    *unsorted = (uint32_t)(void *) originalArr;
    *sorted = (uint32_t)(void *) sortedArr;
    // Run the top level function
    *top = 0x01;
    while (stage < 1) count++;
    #ifdef CHECK
    for(int i = 0; i < SIZE; i++){
        printf("Sorted Array: Index: %d Value: %d \n", i, sortedArr[i]);
    }
    #endif
    #
    printf("Job complete\n");
	m5_dump_stats();
	m5_exit();
}
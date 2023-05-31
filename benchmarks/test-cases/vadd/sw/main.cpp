#include <cstdio>
#include <cstdlib>
#include <cstring>
#include "bench.h"
#include "../../../common/m5ops.h"

volatile uint8_t * top  = (uint8_t *)TOP;
volatile uint32_t *arg1 = (uint32_t *)(TOP + 0x01);// character 1 byte
// volatile uint32_t *arg2 = (uint32_t *)(TOP + 0x09);//int 8 bytes
// volatile uint32_t *arg3 = (uint32_t *)(TOP + 0x11);//int 8 bytes

int main(void) {


   TYPE arr[N];
   printf("Hello from main\n");


   for(int i=0;i<N;i++)
   {
    arr[i]=0;
   }
   printf("Initialised Array to 0.");

    // int stage=0;
    m5_reset_stats();

    // printf("Starting Simulation.\n");
    uint32_t base = 0x80c00000;
    TYPE *m1 = (TYPE *)base;
    // TYPE *m2 = (TYPE *)(base + sizeof(TYPE));
    // TYPE *m3 = (TYPE *)(base + 2 * sizeof(TYPE));
    
    printf("Declared m.\n");
    // m1=arr;
    for(int i=0;i<N;i++)
    {
        m1[i]=arr[i];
    }
    *arg1 = (TYPE)(void *)m1;
    // *arg2 = (TYPE)(void *)m2;
    // *arg3 = (TYPE)(void *)m3;

    // printf("Declared args.\n");

     int count;
    // printf("Starting TOP.\n");
     *top = 0x01;
    while (*top != 0)
    {count++;}
    *top=0x0;
    printf("Job complete\n");
    printf("Done\n");

    printf("The variables is:%d\n",m1[0]);

    printf("Completed Accelerator Job.\n");

    m5_dump_stats();
    m5_exit();
}


/*
Summary:
The code works when I do not change m before assigning to arg.
It executes when i copy arr onto m1, but does not print anything after Initialised array to 0.
*/
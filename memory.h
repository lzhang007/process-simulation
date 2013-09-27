#ifndef MEMORY_H
#define MEMORY_H

#include <stdbool.h>
#define MEMSIZE 1<<10

typedef struct 
{
        int addr;//memory id
        bool isUsed;
}Memory;

void InitializeMem(Memory mem[],int size);

#endif

#include <stdio.h>
#include "memory.h"

void InitilizeMem(Memory mem[],int size)
{
        int i = 0;
        for(;i < size;i++)
        {
                mem[i].addr = i;
                mem[i].isUsed = 0;
        }
}

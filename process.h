#ifndef PROCESS_H_
#define PROCESS_H_

#include<stdio.h>

#define MEMSIZE 1000 //memory size
#define PAGESIZE 10 //one page size
#define TIMEPIECE 100 //time piece
#define SLEEPTIEM 5 //sleep time

typedef struct memory
{
    unsigned int mid; //memory id
    bool isUsed;//is used?
}Memory; //memory type

typedef struct page
{
    
}Page;//page type




typedef struct pitem
{
    unsigned int pid;
    int procSize;
    Page *pageList;
}Pitem;//process type

Proc *runList = NULL;
Proc *blockList = NULL;
Proc *waitList = NULL;

#endif

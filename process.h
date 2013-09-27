#ifndef PROCESS_H_
#define PROCESS_H_

#include<stdbool.h>

struct procid //proc identifier
{
    int pid;
    int uid;
    int ppid;
};

struct cpuStatus//cpu status,not use here
{
    int pc;//program counter
    int proRegister[8];//general register
    int psw[10];//program status word
    int *sp;//user stack pointer
};

struct scheduledInfo//process scheduled information
{
    int procStatus;//process status
    int procPriority;//process priority
    int waitTime;
    int cpuTime;//other scheduled information
    int event;//blocked reason
};

struct contralInfo//process contral information
{
    int textAddr;
    int dataAddr;
    int resourceList[5];
};


typedef struct 
{   
    
}ProcItem;



#endif

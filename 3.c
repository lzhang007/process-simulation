#include<stdio.h>
#include<stdlib.h>
#define timepiece 200
#define pagesize 10
#define memorysize 50
#define sleeptime 5

struct page
{
	int pagetime;
	int virtunume;
	int memonume;
	int fage;
};

struct proc
{
	int procsize;//进程大小
	struct page *pagelist;//页的队列
	int pagefram;
	int pagenume;
	int procpid;
	int procrunnumber;
	int *sequence;
	int cs;
	int ip;
	struct quickpage *quicklist;
	struct proc *next;
};

struct mem
{
	int mempid;
	int flag;
};

struct quickpage
{
	int virtunume;
	int memonume;
};

struct proc *runlist = NULL;
struct proc *waitmlist = NULL;
struct proc *blocklist = NULL;
int pid = 0;
int waketime = 0;
struct mem memory[memorysize];


struct proc* creatproc()
{
	struct proc* newproc;
	int suiji,i;
	newproc = (struct proc*)malloc(sizeof(struct proc));


	suiji = rand();
	while(!(suiji >= 1000 && suiji <= 10000))
    {
        suiji=rand();
    }
	suiji = suiji / 100;
	newproc->procsize = suiji;


	if(newproc->procsize % pagesize == 0)
    {
        newproc->pagenume = newproc->procsize / pagesize;
    }
	else 
    {
        newproc->pagenume = newproc->procsize / pagesize + 1;
    }

	newproc->pagefram = newproc->pagenume / 2;

	newproc->pagelist = (struct page*) malloc( newproc->pagenume * sizeof(struct page));

	for(i = 0; i < newproc->pagenume; i++)
	{
		suiji = rand();
		while(!( suiji >= 1000 && suiji <= 10000))
		{
			suiji = rand();
		}
		suiji = suiji / 100;
		(newproc->pagelist)[i].pagetime = suiji;
		(newproc->pagelist)[i].virtunume = i;
		(newproc->pagelist)[i].fage = 0;
	}

	newproc->procpid = pid;
	pid++;
	newproc->next=NULL;

	while(!( (newproc->procrunnumber = rand() % 20) >= 10) );

	newproc->sequence = (int *)malloc( newproc->procrunnumber * (sizeof(int)));
	for(i = 0;i < newproc->pagenume; i++)
	{
		(newproc->sequence)[i]=i;
	}
	for(i = newproc->pagenume; i < newproc->procrunnumber; i++)
	{
		(newproc->sequence)[i] = rand() % (newproc->pagenume);
	}
	newproc->cs = 0;
	newproc->ip = (newproc->pagelist)[ (newproc->sequence)[newproc->cs] ].pagetime;
	return newproc;
}

void installm(struct proc* process)
{
	int i,j = 0;
	struct proc*p;
	int remain[memorysize];
	for(i = 0; i < memorysize; i++)
    {
		if(memory[i].flag == 0)
        { 
            remain[j] = i;
            j++;
        }
	}

	if(j >= process->pagefram)
    {
		for(i = 0; i < process->pagefram; i++)
        {
			process->quicklist=(struct quickpage*)malloc(process->pagefram*(sizeof(struct quickpage)));
			memory[remain[i]].flag = 1;
			(process->pagelist)[(process->sequence)[i]].fage = 1;
			(process->pagelist)[(process->sequence)[i]].memonume = remain[i];
			(process->quicklist)[i].virtunume = (process->sequence)[i];
			(process->quicklist)[i].memonume = remain[i];
		}
		
        p = runlist;
		if(p == NULL)
        {
            runlist = process;
        }
		else
        {
			while(p->next != NULL)
            {
                p = p->next;
            }
			p->next = process;
		}
	}
	else
    {
		p = waitmlist;
		if(p == NULL)
        {
            waitmlist = process;
        }
		else
        {
			while(p->next != NULL)
            {
                p = p->next;
            }
			p->next=process;
		}
	}
}



void block(struct proc* process)
{
	struct proc*p;
	p = blocklist;
	if(p == NULL)
    {
        blocklist=process;
    }
	else
    {
		while(p->next != NULL)
        {
            p = p->next;
        }
		p->next = process;                 
	}
}


int inmemory(struct proc*process,int cs)
{
	if((process->pagelist)[(process->sequence)[cs]].fage == 1)
    {
        return 1;
    }
	else 
    {
        return 0;
    }
}


void wakeup()
{
	struct proc*p1,*p2;
	if(blocklist != NULL)
    {
		p1 = blocklist;
		blocklist = blocklist->next;
		p1->next = NULL;
		p2 = runlist;
		if(p2 == NULL)
        {
            runlist = p1;
        }
		else
        {
			while(p2->next != NULL)
            {
                p2 = p2->next;
            }
			p2->next = p1;
		}
	}
}

void delproc(struct proc* process)
{
	int i;
	struct proc*p;
	for(i = 0; i < process->pagefram; i++)
    {
        memory[(process->quicklist)[i].memonume].flag = 0;
    }
	free(process->pagelist);
	free(process->quicklist);
	free(process->sequence);
	free(process);
	p = waitmlist;
	if(p != NULL)
    {
		waitmlist = waitmlist->next;
		p->next = NULL;
		installm(p);
	}
}



void FIFO(struct proc* process,int cs)
{
	int i,t;
	t = (process->quicklist)[0].memonume;
	for(i = 1; i < process->pagefram; i++)
    {
		(process->quicklist)[i-1].memonume = (process->quicklist)[i].memonume;
		(process->quicklist)[i-1].virtunume = (process->quicklist)[i].virtunume;
	}

	(process->quicklist)[(process->pagefram)-1].memonume = t;
	(process->quicklist)[(process->pagefram)-1].virtunume = (process->sequence)[cs];
}



int run(struct proc* process)
{
	int restime;
	struct proc*p;
	restime = timepiece;
	while(restime--)
    {
		process->ip--;
		if(process->ip == 0)
        {
			if(process->cs + 1 < process->procrunnumber)
            {
				process->cs++;
                process->ip = (process->pagelist)[(process->sequence)[process->cs]].pagetime;
			}
			else
            {
				delproc(process);
                return restime;
			}
			if(!inmemory(process,process->cs))
            {
                block(process);
                FIFO(process,process->cs);
                return restime;
            }
		}
	}
	p = runlist;
	if(p == NULL)
    {
        runlist = process;
    }
	else
    {
		while(p->next != NULL)
        {
            p=p->next;
        }
		p->next = process;
	}
	return restime;
}


int main()
{
	int i,restime;
	char key;
	struct proc* process;
	for(i = 0; i < memorysize; i++)
    {
		memory[i].mempid = i;
		memory[i].flag = 0;
	}
	key = 'w';
	while(key != 'q')
    {
		if(key == 'w')
        {
			process = creatproc();
			installm(process);
		}

		process = runlist;
		if(process == NULL)
        {
			wakeup();
            waketime = 0;
		}
		else
        {
			runlist = runlist->next;
			process->next = NULL;
			restime = run(process);
			waketime = waketime + timepiece - restime;
			if(waketime >= sleeptime)
            {
				wakeup();
                waketime = 0;
			}
		}
		process = runlist;
		i = 0;
		while( process != NULL)
        {
            process = process->next;
            i++;
        }
		printf("runing process is:%d\n",i);
		process = runlist;
		while(process != NULL)
        {
			printf("pid:%d procsize:%d\n",process->procpid,process->procsize);
			process = process->next;  
		}
		process = blocklist;
		i = 0;
		while(process != NULL)
        {
            process = process->next;
            i++;
        }
		printf("blocking process is:%d\n",i);
		process = blocklist;
		while(process != NULL)
        {
			printf("pid:%d procsize:%d\n",process->procpid,process->procsize);
			process = process->next;  
		}

		key = getchar();
	}
}

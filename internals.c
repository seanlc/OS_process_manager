#include "stdio.h"
#include "stdlib.h"
#include "string.h"

typedef enum {RUNNING = 1, READY = 2, BLOCKED = 3} status;

// forward declaration of structs
struct PCB;
struct PCB_node;
struct RCB_node;

// ready list
struct PCB_node * readyList[3];


typedef struct 
{
    int rid;					// resource number 1-4
    int k;					// number of initial resources
    int u;					// currently available resources
    struct PCB_node * waitList;  		// blocked processes waitinig on resource
} RCB;

typedef struct
{ 
    char * pid;					// process id
    struct RCB_node * other_resources;		// resources currenly held by process    
    status status_type;  			// ready, running, or blocked
    struct PCB_node * list;			// ready list or resource wait list
    struct PCB * parent;			// points to parent process
    struct PCB_node * children;			// llist pointers to child PCBs
    int priority;				// 0-init 1-user 2-system
} PCB;

typedef struct
PCB_node
{
    PCB * process;
    struct PCB_node * next;
    
} PCB_node;

typedef struct
RCB_node
{
    RCB * resource;
    struct RCB_node * next;
} RCB_node;

//list of all active processes
PCB * procList[50];
int numProc = 0;

// resources
RCB res1;
 
RCB res2;

RCB res3;

RCB res4;

void add_to_RL(struct PCB_node * process, int priority)
{ 
    struct PCB_node * trav = readyList[priority];
    if(trav == NULL)
        readyList[priority] = process;
    else
    {
        while(trav->next != NULL)
            trav = trav->next;
        trav->next = process;
    }
}


void Create(char * name, int priority)
{
    // allocate PCB_node
    PCB_node * pNode = (PCB_node *) malloc(sizeof(PCB_node));

    // populate PCB_node
    // allocate PCB
    pNode->process = (PCB *) malloc(sizeof(PCB));
    pNode->next = NULL;

    PCB * curProc = pNode->process;

    // populate PCB
    curProc->pid = (char *) malloc(strlen(name));
    strcpy(curProc->pid, name);
    curProc->other_resources = NULL;
    curProc->status_type = READY;
    curProc->list = readyList[priority];
    // TODO pNode->parent = ??
    curProc->children = NULL;
    curProc->priority = priority;
    
    // add to ready list
    add_to_RL(pNode, priority);
    
    //add to proclist
    procList[numProc++] = curProc;
    // TODO call scheduler
}

void print_RL()
{
    struct PCB_node * trav;
    printf("READY LIST: \n");
    for(int i = 0; i < 3; ++i)
    {
        printf("priority %d: ", i);
        trav = readyList[i];
        while(trav != NULL)
        {
            printf("process: %s ", trav->process->pid);
            trav = trav->next;
        }
        printf("\n");
        
    }
}
void print_PL()
{
    printf("ALL PROCESSES: \n");
    for(int i = 0; i < numProc; ++i)
    {
        printf("process: %s ", procList[i]->pid);
    }
    printf("\n");
}

PCB * get_ptr_of_pid(char * name)
{
    for(int i = 0; i < numProc; ++i)
    {
        if(strcmp(procList[i]->pid, name) == 0)
            return procList[i];
    }
    return NULL;    
}

void init_resources()
{
    res1.rid = 1;
    res1.k = 1;
    res1.u = 1;
    res1.waitList = NULL;

    res2.rid = 2;
    res2.k = 2;
    res2.u = 2;
    res2.waitList = NULL;

    res3.rid = 3;
    res3.k = 3;
    res3.u = 3;
    res3.waitList = NULL;

    res4.rid = 4;
    res4.k = 4;
    res4.u = 4;
    res4.waitList = NULL;    
}

int main()
{
    init_resources();
    Create("p1",1);
    Create("p2",1);
//    print_RL();
    print_PL();

    return 0;
}

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

//list of all active processes
struct PCB_node * procList;

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
    struct PCB_node * list;		// ready list or resource wait list
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

// resources
RCB res1[1];
RCB res2[2];
RCB res3[3];
RCB res4[4];


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

void add_to_PL(struct PCB_node * process)
{    
    struct PCB_node * trav = procList;
    if(trav == NULL)
        procList = process;
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

    // TODO call scheduler
}

void print_RL()
{
    struct PCB_node * trav;
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
/*
PCB_node * get_ptr_of_pid(char * name)
{
    
}
*/
int main()
{
    Create("p1",1);
    Create("p2",1);
    print_RL();
//    print_PL();
    return 0;
}

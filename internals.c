#include "stdio.h"
#include "stdlib.h"
#include "string.h"

typedef enum {RUNNING = 1, READY = 2, BLOCKED = 3} status;

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
    int num_req;
    struct PCB_node * next;
    
} PCB_node;

typedef struct
RCB_node
{
    RCB * resource;
    int num;
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


PCB Create(char * name, int priority, struct PCB * activeProc)
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
    curProc->parent = activeProc;
    curProc->children = NULL;
    curProc->priority = priority;
    
    // add to ready list
    add_to_RL(pNode, priority);
    
    //add to proclist
    procList[numProc++] = curProc;
    // TODO call scheduler
    // take out return statement and change type
    return *(pNode->process);
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

void insert_res(struct RCB_node ** list, struct RCB_node * res)
{
    RCB_node * trav = *list;
    if(trav == NULL)
        *list = res;
    else
    {
        while(trav->next != NULL)
            trav = trav->next;
        trav->next = res;
    }
}

void remove_PCB_from_RL(PCB_node * proc)
{
    PCB * curProc = proc->process;
    PCB_node * trav = readyList[curProc->priority];
    if(trav != NULL && proc != NULL)
    {
        if(strcmp(proc->process->pid, trav->process->pid) == 0)
        {
	    // remove from head
            readyList[curProc->priority] = trav->next;
        }
        else
        {
	    // attempt to remove from tail
            while(trav->next != NULL && strcmp(proc->process->pid, trav->next->process->pid) != 0)
		trav = trav->next;
            if(strcmp(proc->process->pid, trav->next->process->pid) == 0)
	        trav->next = trav->next->next;
        }
    }
}

void remove_PCB_from_waitList(PCB_node ** waitList, char * pid)
{
    PCB_node * trav = *waitList;
    if(trav != NULL)
    {
        if(strcmp(trav->process->pid, pid) == 0)
	{
	    // head removal
	    *waitList = trav->next;
	}
	else
	{
	    while(trav->next != NULL && strcmp(pid, trav->next->process->pid) != 0)
	        trav = trav->next;
	    if(strcmp(pid, trav->next->process->pid) == 0)
	        trav->next = trav->next->next;
	}
    }
}

void add_PCB_to_waitList(PCB_node * proc, RCB * res)
{
    PCB_node * trav = res->waitList;

    printf("number of resources requrested by process: %d\n", proc->num_req);

    if(trav == NULL)
        res->waitList = proc;
    else
    {
        while(trav->next != NULL)
            trav = trav->next;
        trav->next = proc;        
    }
}

RCB_node * find_res_node(int rid, PCB_node * activeProc)
{
    RCB_node * trav = activeProc->process->other_resources;
    while(trav != NULL && trav->resource->rid != rid)
        trav = trav->next; 
    return trav;
}

int request(int rid, int n, PCB_node * activeProc)
{
    RCB * res;
    struct RCB_node * res_alloc;
    struct RCB_node * res_node;
    switch(rid)
    {
        case 1:
            res = &res1;
	    break;
        case 2:
            res = &res2;
            break;
        case 3:
            res = &res3;
            break;
        case 4:
            res = &res4;
            break;
        default:
            printf("request made for nonexistant resource");
            return -1;
    }
    
    activeProc->num_req = n;



    if(n <= res->u)
    {
	if((res_node = find_res_node(rid, activeProc)) != NULL)
	    res_node->num += n;
	else
	{
            res_alloc = (RCB_node *) malloc(sizeof(RCB_node));
            res_alloc->resource = res;
            res_alloc->num = n;
            res_alloc->next = NULL;
            insert_res(&activeProc->process->other_resources, res_alloc);
	}
        res->u -= n;
    } 
    else
    {
        activeProc->process->status_type = BLOCKED;
        activeProc->process->list = res->waitList; 
        // remove self from readyList
        remove_PCB_from_RL(activeProc);
        // add self to res waitList
        add_PCB_to_waitList(activeProc, res);
    }   
 

    // call Scheduler()
    return 1;
}

void print_PCB_res_list(RCB_node * lst)
{
    printf("process holds: \n");
    RCB_node * trav = lst;
    while(trav != NULL)
    {
        printf("resource %d count %d\n", trav->resource->rid, trav->num);
        trav = trav->next;
    }
}

void print_RCB_waitList(PCB_node * lst)
{
    PCB_node * trav = lst;
    printf("waiting list for resource: ");
    while(trav != NULL)
    {
        printf("process: %s\n", trav->process->pid);
        trav = trav->next;
    }
    printf("\n");
}

void remove_RCB_from_PCB(int rid, RCB_node ** lst)
{
    RCB_node * trav = *lst;
    RCB_node * trail = NULL;
    while(trav != NULL && trav->resource->rid != rid)
    {
	trail = trav;
        trav = trav->next;
    }
    if(trav == *lst)
    {
        // head deletion
	*lst = (*lst)->next;
    }
    else
    {
	//tail deletion
        trail->next = trav->next; 
    } 
    free(trav);
    trav = NULL;
}

PCB_node * find_ready_PCB_on_waitList(PCB_node * waitList, int avail_res)
{
    PCB_node * trav = waitList;
    while(trav != NULL && trav->num_req > avail_res)
        trav = trav->next;
    return trav;
}

int release(int rid, int n, PCB_node * activeProc)
{
    RCB * res = NULL;
    RCB_node * res_node = NULL;
    PCB_node * launchProc = NULL;
    switch(rid)
    {
        case 1:
	    res = &res1;
	    break;
	
	case 2:
	    res = &res2;
	    break;

	case 3:
	    res = &res3;
	    break;

	case 4:
	    res = &res4;
	    break;
        
	default:
            printf("request made for nonexistant resource");
            return -1;
    }

    res_node = find_res_node(rid, activeProc);
//    printf("foudn RCB_node with rid %d and count %d\n", res_node->resource->rid, res_node->num);

    if(res_node->num >= n)
    {
        res_node->num -= n;
	if(res_node->num == 0)
	    remove_RCB_from_PCB(rid, &activeProc->process->other_resources);
	res->u += n;
	if((launchProc = find_ready_PCB_on_waitList(res->waitList, res->u)) != NULL)
        {
	    printf("found ready process %s\n", launchProc->process->pid);
	    remove_PCB_from_waitList( &res->waitList, launchProc->process->pid); 
	}
    }  

    // call Scheduler()
    return 1;    
}

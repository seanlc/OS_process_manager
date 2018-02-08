#include "stdio.h"
#include "stdlib.h"
#include "string.h"

#define MAX_CHILDREN 20

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
    struct PCB_node * parent;			// points to parent process
    struct PCB_node * children[MAX_CHILDREN];	// array pointers to child PCBs
    int numChildren;				// number of children
    int priority;				// 0-init 1-user 2-system
} PCB;

typedef struct
PCB_node
{
    PCB * process;
    int num_req;
    struct PCB_node * next;
    struct PCB_node * next_sib;
    
} PCB_node;

typedef struct
RCB_node
{
    RCB * resource;
    int num;
    struct RCB_node * next;
} RCB_node;

// resources
RCB res1;
 
RCB res2;

RCB res3;

RCB res4;

RCB * get_RCB_ptr_by_pid(int n)
{
    RCB * rNode = NULL;
    switch(n)
    {
        case 1:
	    rNode = &res1;
	    break;
	case 2:
	    rNode = &res2;
	    break;
	case 3:
	    rNode = &res3;
	    break;
	case 4:
	    rNode = &res4;
	    break;
    }
    return rNode;
}

PCB_node * get_PCB_node_by_pid(char * pid)
{
    PCB_node * trav = NULL;

    // look on readyList
    for(int i = 0; i < 3; ++i)
    {
        trav = readyList[i];
        while(trav != NULL)
        {
	    if(strcmp(trav->process->pid, pid) == 0)
                return trav;
	    trav = trav->next;
	}
    }

    // look on resource waitList
    
    RCB * res = NULL;
    for(int i = 1; i < 5; ++i)
    {
        res = get_RCB_ptr_by_pid(i);
	trav = res->waitList;
	while(trav != NULL)
	{
	    if(strcmp(trav->process->pid, pid) == 0)
	        return trav;
	    trav = trav->next;
	}
    }

    return NULL;
}
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

void PCB_info(PCB_node * nd)
{
    printf("pid: %s\n", nd->process->pid);
    print_PCB_res_list(nd->process->other_resources);
    printf("status: %d\n", nd->process->status_type);
    if(nd->process->parent != NULL)
	printf("parent pid: %s\n", nd->process->parent->process->pid);
}

PCB_node *  get_highest_pri_proc()
{
    PCB_node * trav;
    for(int i = 2; i > -1; --i )
    {
        trav = readyList[i];
	if(trav == NULL)
	    continue;
	else
	    return trav;
    }
    return NULL;
}

PCB_node * get_running_proc()
{
    PCB_node * trav = NULL;
    for(int i = 2; i > -1; --i)
    {
        trav = readyList[i];
	while(trav != NULL)
	{
	    if(trav->process->status_type == RUNNING)
	        return trav;
	    trav = trav->next;
	}
    }
    return trav;
}

void preempt(PCB_node * new, PCB_node * old)
{
    new->process->status_type = RUNNING;
    if(old != NULL && old->process->status_type == RUNNING && old != new)
        old->process->status_type = READY;
}

void Scheduler(PCB_node * activeProc)
{
    PCB_node * pNode = get_highest_pri_proc();
    if(pNode == NULL)
        printf("There is no process to run\n");
    else if(activeProc == NULL || activeProc->process->priority < pNode->process->priority || activeProc->process->status_type != RUNNING)
            preempt(pNode, activeProc);
}

PCB Create(char * name, int priority, struct PCB_node * activeProc)
{
    // allocate PCB_node
    PCB_node * pNode = (PCB_node *) malloc(sizeof(PCB_node));

    // populate PCB_node
    // allocate PCB
    pNode->process = (PCB *) malloc(sizeof(PCB));
    pNode->next_sib = NULL;
    pNode->next = NULL;

    PCB * curProc = pNode->process;

    // populate PCB
    curProc->pid = (char *) malloc(strlen(name));
    strcpy(curProc->pid, name);
    curProc->other_resources = NULL;
    curProc->status_type = READY;
    curProc->list = readyList[priority];
    curProc->parent = activeProc;
    curProc->numChildren = 0;
    for(int i =0; i < MAX_CHILDREN; ++i)
        curProc->children[i] = NULL;
    curProc->priority = priority;
   
    // add to child link of parent
    if(activeProc != NULL)
        activeProc->process->children[activeProc->process->numChildren++] = pNode;

    // add to ready list
    add_to_RL(pNode, priority);
    
    Scheduler(activeProc);
    // take out return statement and change type
    return *(pNode->process);
}

void print_RCB_waitList(PCB_node * lst)
{
    PCB_node * trav = lst;
    while(trav != NULL)
    {
        printf("process: %s\t", trav->process->pid);
        trav = trav->next;
    }
}

void print_blocked_procs()
{
    RCB * res = NULL;
    printf("blocked processes \n");
    for(int i = 1; i < 5; ++i)
    {
        res = get_RCB_ptr_by_pid(i);
	printf("resource %d:\t", i);
	print_RCB_waitList(res->waitList);
        printf("\n");
    }
}

void print_RL()
{
    struct PCB_node * trav;
    printf("READY LIST: \n");
    for(int i = 0; i < 3; ++i)
    {
        printf("priority %d:\t", i);
        trav = readyList[i];
        while(trav != NULL)
        {
            printf("process: %s \t", trav->process->pid);
            trav = trav->next;
        }
        printf("\n");
        
    }
}
void print_PL()
{
    printf("ALL PROCESSES: \n");
    print_RL();
    print_blocked_procs();
    printf("\n");
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
    PCB_node * tmp;
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
            {
		tmp = trav->next;
	        trav->next = trav->next->next;
		tmp->next = NULL;
	    }
	    
	}
    }
}

void add_PCB_to_waitList(PCB_node * proc, RCB * res)
{
    PCB_node * trav = res->waitList;

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

RCB_node * alc_RCB_node(RCB * res, int num_req)
{
    RCB_node * res_alloc = NULL;
    res_alloc = (RCB_node *) malloc(sizeof(RCB_node));
    res_alloc->resource = res;
    res_alloc->num = num_req ;
    res_alloc->next = NULL;
    return res_alloc;
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
	    res_alloc = alc_RCB_node(res, n);
            insert_res(&activeProc->process->other_resources, res_alloc);
	}
        res->u -= n;
    } 
    else
    {
        activeProc->process->status_type = BLOCKED;
        // remove self from readyList
        remove_PCB_from_RL(activeProc);
	activeProc->next = NULL;
        // add self to res waitList
        add_PCB_to_waitList(activeProc, res);
        activeProc->process->list = res->waitList; 
    }   
 

    Scheduler(activeProc);

    return 1;
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
    struct RCB_node * res_alloc;
 //   printf("rid: %d\n", rid);
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
            printf("request made for nonexistant resource\n");
            return -1;
    }
    
    // find RCB_node held by activeProc with rid matching argument
    res_node = find_res_node(rid, activeProc);
//    printf("foudn RCB_node with rid %d and count %d\n", res_node->resource->rid, res_node->num);

    if(res_node != NULL && res_node->num >= n)
    {
	// decrease count of resource in RCB_node held in other_resources
        res_node->num -= n;

	// if count of resource has fallen to 0, remove the RCB_node from other_resources
	if(res_node->num == 0)
	    remove_RCB_from_PCB(rid, &activeProc->process->other_resources);

	// increase count of resource by newly free n
	res->u += n;

	// check for for processes waiting on resources that can become ready with newly available resources
	// if found, have proc claim resources and move to ready list
	while((launchProc = find_ready_PCB_on_waitList(res->waitList, res->u)) != NULL)
        {
//	    printf("found ready process %s\n", launchProc->process->pid);
	    // remove found PCB from res waitList
	    remove_PCB_from_waitList( &res->waitList, launchProc->process->pid);

	    // change parameters of launchProc to reflect removal from list
            launchProc->process->status_type = READY;
	    launchProc->process->list = readyList[launchProc->process->priority];
	    launchProc->next = NULL;

	    // update available resource number
	    res->u -= launchProc->num_req;
            
	    // make new RCB_node and insert into launchProc->other_resources
	    res_alloc = alc_RCB_node(res, launchProc->num_req);
            insert_res(&launchProc->process->other_resources, res_alloc);

	    //insert into RL
	    add_to_RL(launchProc ,launchProc->process->priority);
	}
    }  

    Scheduler(activeProc);
    return 1;    
}

void free_res_held_by_PCB(PCB_node * proc)
{
    RCB_node * res = proc->process->other_resources;
    while(res != NULL)
    {
//        printf("resource found with rid %d and count %d\n", res->resource->rid, res->num);
	release(res->resource->rid, res->num, proc);
	res = proc->process->other_resources;
    }
}

void remove_child_from_parent(PCB_node * par, PCB_node * child)
{
    int numChildren = par->process->numChildren;
    int found = 0;
    for(int i = 0; i < numChildren; ++i)
    {
        if(! found && strcmp(par->process->children[i]->process->pid, child->process->pid) == 0)
	    found = 1;
	if(found)
	    par->process->children[i] = par->process->children[i + 1];
    }
    --par->process->numChildren;
}

void remove_from_res_waitList(PCB_node * nd)
{
    if(nd->process->list == res1.waitList)
	remove_PCB_from_waitList(&res1.waitList,  nd->process->pid);
    else if(nd->process->list == res2.waitList)
	remove_PCB_from_waitList(&res2.waitList,  nd->process->pid);
    else if(nd->process->list == res3.waitList)
	remove_PCB_from_waitList(&res3.waitList,  nd->process->pid);
    else if(nd->process->list == res4.waitList)
	remove_PCB_from_waitList(&res4.waitList,  nd->process->pid);
}


void delete_node(PCB_node * nd)
{
    free_res_held_by_PCB(nd);
    if(nd->process->status_type == READY || nd->process->status_type == RUNNING)
	remove_PCB_from_RL(nd);
    else
        remove_from_res_waitList(nd);
    
    // remove from parent child array
    if(nd->process->parent != NULL)
        remove_child_from_parent(nd->process->parent, nd);

    free(nd->process->pid);
    free(nd->process);
    free(nd);
}

void destroy_process(PCB_node * nd)
{
    PCB_node * proc = nd;
    for(int i = nd->process->numChildren-1; i > -1; --i)
        destroy_process(nd->process->children[i]);
    delete_node(proc);
    Scheduler(NULL);
}

void destroy_children(PCB_node * nd)
{
    for(int i = nd->process->numChildren-1; i > -1; --i)
        destroy_process(nd->process->children[i]);
    Scheduler(NULL);
}


void print_children(PCB_node * nd)
{
    int numChild = nd->process->numChildren;
    printf("children of process %s\n", nd->process->pid);
    for(int i = 0; i < numChild; ++i)
    {
        printf("process %s\n", nd->process->children[i]->process->pid);
    }
}

void timeout()
{
    PCB_node * runningProc = get_running_proc();
    remove_PCB_from_RL(runningProc);
    runningProc->next = NULL;
    runningProc->process->status_type = READY;
    add_to_RL(runningProc, runningProc->process->priority);
    Scheduler(runningProc);
}

void print_res_list()
{
    RCB * res = NULL;
    printf("resources:\n");
    for(int i = 1; i < 5; ++i)
    {
        res = get_RCB_ptr_by_pid(i);
	printf("resource %d \tintial: %d \tcurrent: %d\n", res->rid, res->k, res->u);
    }
    printf("\n");
}

void delete_RL()
{
    PCB_node * trav = NULL;
    for(int i = 2; i > 0; --i)
    {
	while((trav = readyList[i]) != NULL)
	{
	//    readyList[i] = readyList[i]->next;
	    destroy_process(trav);
	}

    }
}

void delete_waitList()
{
    RCB * res = NULL;
    PCB_node * trav = NULL;

    for(int i = 1; i < 5; ++i)
    {
        res = get_RCB_ptr_by_pid(i);
	trav = res->waitList;
	while((trav = res->waitList) != NULL)
	{
	    res->waitList = res->waitList->next;
	    destroy_process(trav);
	}
    }
}

void destroy_all_proc()
{
    delete_waitList();
    delete_RL();
}

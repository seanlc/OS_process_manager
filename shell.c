#include "internals.c"
#include "ctype.h"

PCB_node * curProc;
PCB_node * init;
void strip_newline(char * str)
{
    for(int i = 0; str[i]; ++i)
    {
        if(str[i] == '\n')
	{
	    str[i] = '\0';
	    break;
        }	    
    }
}

int get_rNumber_from_rName(char * rName)
{
    int rNumber = 0;
    if(strcmp(rName, "R1") == 0)
	rNumber = 1;
    else if(strcmp(rName, "R2") == 0)
	rNumber = 2;
    else if(strcmp(rName, "R3") == 0)
	rNumber = 3;
    else if(strcmp(rName, "R4") == 0)
	rNumber = 4;

    return rNumber;
}
void tokenize_input(char * tok, const char * delim, FILE * fp)
{
    if(strcmp(tok, "init") == 0)
    {
	destroy_all_proc();
    }
    else if(strcmp(tok, "cr") == 0)
    {
        char * pName = strtok(NULL, delim);
	char * pString = strtok(NULL, delim);
	int priority = atoi(pString);
	if(get_PCB_node_by_pid(pName) == NULL && priority > 0 && priority < 3 && isdigit(pString[0]))
	    Create(pName, priority, curProc);
    }
    else if(strcmp(tok, "de") == 0)
    {
        char * pName = strtok(NULL, delim);
	strip_newline(pName);
	if(get_PCB_node_by_pid(pName) != NULL && strcmp(pName, "init") != 0)
	    destroy_process(get_PCB_node_by_pid(pName));
    }
    else if(strcmp(tok, "rel") == 0)
    {
	char * rName = strtok(NULL, delim);
        int rNumber = get_rNumber_from_rName(rName);

	int count = atoi(strtok(NULL, delim));
        if(rNumber != 0 && count > -3)
	    release(rNumber, count, curProc);
    }
    else if(strcmp(tok, "to") == 0)
    {
        timeout();
    }
    else if(strcmp(tok, "req") == 0)
    {
	char * rName = strtok(NULL, delim);
	int rNumber = get_rNumber_from_rName(rName);
	
	int count = atoi(strtok(NULL, delim));
	if(rNumber != 0  && count > 0)
	    request(rNumber, count, curProc);
    }
    else if(strcmp(tok, "plist") == 0)
    {
        print_PL();
    }
    else if(strcmp(tok, "rlist") == 0)
    {
        print_res_list();
    }
    else
    {
	fprintf(fp, "error: %s \n", tok);
    }
    curProc = get_running_proc();
    fprintf(fp, "%s ", curProc->process->pid);
} 


int main(int argc, char * argv[])
{
    init = (PCB_node * ) malloc(sizeof(PCB_node));
    init->next = NULL;
    PCB initial = Create("init", 0, NULL);
    init->process = &initial;
    curProc = init;
    init_resources();

    char s[256] = "";
    const char delim[2] = " ";
    char * tok = NULL;

    // setup file input
    FILE * file = NULL;
    FILE * fp = NULL;
    if(argc >= 2)
    {
        if((file = fopen(argv[1], "r")) == NULL)
	{
	    printf("file at %s could not be found\n", argv[1]);
	}
	
    // setup file output
        if( (fp = fopen(argv[2], "w+")) == NULL )
	{
            fprintf(stderr, "could not create output file\n");
	}
    }

    if(file == NULL)
        file = stdin;
    if(fp == NULL)
	fp = fopen("out.txt", "w+");

    fprintf(fp,"%s ", curProc->process->pid);
    
    while(1)
    {
        if(fgets(s, 256 , file) == NULL)
	    return 0;
    //    printf("line read: %s\n", s);
    
        while(strcmp("\n", s) == 0)
	{
	    fprintf(fp,"\n");
	    if (fgets(s, 256, file) == NULL)
	        return 0;
	}

        tok = strtok(s, delim);
	strip_newline(tok);
	tokenize_input(tok, delim, fp);
    }
    return 0;
}

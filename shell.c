#include "internals.c"

void tokenize_input(char * tok)
{
    if(strcmp(tok, "init") == 0)
    {
        printf("found_init\n");
    }
    else if(strcmp(tok, "cr") == 0)
    {
        printf("found cr\n");
    }
    else if(strcmp(tok, "de") == 0)
    {
        printf("found de\n");
    }
    else if(strcmp(tok, "rel") == 0)
    {
        printf("found rel\n");
    }
    else if(strcmp(tok, "to") == 0)
    {
        printf("found to\n");
    }
    else if(strcmp(tok, "req") == 0)
    {
        printf("found req\n");
    }
    else
	printf("command not recognized\n");
} 

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

int main()
{
    char s[256] = "";
    const char delim[2] = " ";
    char * tok = NULL;
    
    while(1)
    {
        fgets(s, 256 , stdin);

        tok = strtok(s, delim);
	strip_newline(tok);
	tokenize_input(tok);
    }
    return 0;
}

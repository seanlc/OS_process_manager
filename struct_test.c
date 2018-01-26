#include "internals.c"

int main()
{
    PCB_node * nd = (PCB_node *) (malloc(sizeof(PCB_node)));
    PCB_node * trav = nd;
    printf("nd == trav: %d\n", nd == trav);
    free(nd);
    return 0;
}

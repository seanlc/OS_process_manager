#include "stdio.h"
#include "internals.c"

void test_request()
{
    PCB_node * nd = (PCB_node *) malloc(sizeof(PCB_node));
    nd->next = NULL;
    PCB  p1 = Create("p1",1, NULL);
    nd->process = &p1;
    request(1,1,nd);
    print_PCB_res_list(p1.other_resources);
    printf("number of r1 remaining: %d\n", res1.u);

    request(2,2,nd);
    print_PCB_res_list(p1.other_resources);
    printf("number of r2 remaining: %d\n", res2.u);

    request(1,1,nd);
    print_PCB_res_list(p1.other_resources);
    printf("number of r1 remaining: %d\n", res1.u);
    
    free(nd);
}

int main()
{
    init_resources();
    test_request();
    return 0;
}

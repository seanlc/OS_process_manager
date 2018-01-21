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

void test_remove_from_RL()
{
    PCB_node * nd = (PCB_node *) malloc(sizeof(PCB_node));
    nd->next = NULL;
    PCB  p1 = Create("p1",1, NULL);
    nd->process = &p1;
    
    PCB_node * nd2 = (PCB_node *) malloc(sizeof(PCB_node));
    nd2->next = NULL;
    PCB  p2 = Create("p2",1, NULL);
    nd2->process = &p2;
    
    PCB_node * nd3 = (PCB_node *) malloc(sizeof(PCB_node));
    nd3->next = NULL;
    PCB  p3 = Create("p3",1, NULL);
    nd3->process = &p3;

    print_RL();

    remove_PCB_from_RL(nd);
    print_RL();

    remove_PCB_from_RL(nd3);
    print_RL();
    
}

int main()
{
    init_resources();
//    test_request();
    test_remove_from_RL();
    return 0;
}

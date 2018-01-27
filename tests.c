#include "stdio.h"
#include "internals.c"

void test_find_ready_PCB()
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

    PCB_node * nd4 = (PCB_node *) malloc(sizeof(PCB_node));
    nd4->next = NULL;
    PCB  p4 = Create("p4",1, NULL);
    nd4->process = &p4;

    request(1,1,nd);
    print_PCB_res_list(p1.other_resources);

    request(1,3,nd2);
    request(1,3,nd3);
    request(1,1,nd4);

    print_RL();   

    release(1,1,nd);    

//    print_PCB_res_list(p1.other_resources);
    print_RCB_waitList(res1.waitList);

    print_RL();    
}

void test_remove_RCB_from_PCB()
{
    
    PCB_node * nd = (PCB_node *) malloc(sizeof(PCB_node));
    nd->next = NULL;
    PCB  p1 = Create("p1",1, NULL);
    nd->process = &p1;
    
    request(1,1,nd);
    print_PCB_res_list(p1.other_resources);

    request(2,2,nd);
    print_PCB_res_list(p1.other_resources);

    request(3,3,nd);
    print_PCB_res_list(p1.other_resources);
    
    printf("after removal of res2 from p1->other_resources\n");
    remove_RCB_from_PCB(2,&(nd->process->other_resources));
    print_PCB_res_list(p1.other_resources);

}

void test_request()
{
    PCB_node * nd = (PCB_node *) malloc(sizeof(PCB_node));
    nd->next = NULL;
    PCB  p1 = Create("p1",1, NULL);
    nd->process = &p1;
    
    request(1,1,nd);
    print_PCB_res_list(p1.other_resources);
    printf("number of r1 remaining: %d\n", res1.u);

    request(2,1,nd);
    print_PCB_res_list(p1.other_resources);
    printf("number of r2 remaining: %d\n", res2.u);

    request(2,1,nd);
    print_PCB_res_list(p1.other_resources);
    printf("number of r2 remaining: %d\n", res2.u);

    print_RL();   

    request(1,1,nd);
    print_PCB_res_list(p1.other_resources);
    printf("number of r1 remaining: %d\n", res1.u);

    print_RCB_waitList(res1.waitList);
    print_RL();
    
    free(nd);
}

void test_release()
{
    PCB_node * nd = (PCB_node *) malloc(sizeof(PCB_node));
    nd->next = NULL;
    PCB p1 = Create("p1", 1, NULL);
    nd->process = &p1;

    request(4, 2, nd);

    print_PCB_res_list(p1.other_resources);
    
    release(4,2, nd);

    print_PCB_res_list(p1.other_resources);
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

void test_remove_RL_add_WL()
{
    PCB_node * nd = (PCB_node *) malloc(sizeof(PCB_node));
    nd->next = NULL;
    PCB  p1 = Create("p1",1, NULL);
    nd->process = &p1;

    PCB_node * nd2 = (PCB_node *) malloc(sizeof(PCB_node));
    nd2->next = NULL;
    PCB  p2 = Create("p2",1, NULL);
    nd2->process = &p2;

    print_RL();

    remove_PCB_from_RL(nd);
    print_RL();

    add_PCB_to_waitList(nd, &res1);
    print_RCB_waitList(res1.waitList);

    print_RL();
}


int main()
{
    init_resources();
    test_find_ready_PCB();
//    test_remove_RCB_from_PCB();
//    test_release();
//    test_request();
//    test_remove_RL_add_WL();
    return 0;
}

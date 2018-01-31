#include "stdio.h"
#include "internals.c"

void test_scheduler()
{
    Create("p1", 1, NULL);
}
void test_get_highest_pri_proc()
{
    Create("p1", 2, NULL);
    Create("p2", 1, NULL);
    Create("p3", 0, NULL);

    PCB_node * nd = get_highest_pri_proc();
    printf("found process with pid: %s\n", nd->process->pid);
}
void test_print_PL()
{
    PCB_node * init = (PCB_node * ) malloc(sizeof(PCB_node));
    init->next = NULL;
    PCB initial = Create("init", 0, NULL);
    init->process = &initial;

    Create("p1",1,init);
    Create("p2", 2, init);
    Create("p3",2,init);
    Create("p4",1,init);

    request(1,1,get_PCB_node_by_pid("p1"));
    request(1,1,get_PCB_node_by_pid("p2"));

    print_PL();

}
void test_delete_node()
{

    PCB_node * init = (PCB_node * ) malloc(sizeof(PCB_node));
    init->next = NULL;
    PCB initial = Create("init", 0, NULL);
    init->process = &initial;

    Create("p1",1, init);
    Create("p2",1, init);
    Create("p3",1, init);
    Create("p4",1,init);
    Create("p5",1,init);
    Create("p6",1,init);
    Create("p7",1,init);
    Create("p8",1,init);
   
    request(1,1, get_PCB_node_by_pid("p1"));
    request(1,1, get_PCB_node_by_pid("p5"));
    request(1,1, get_PCB_node_by_pid("p6"));
    request(1,1, get_PCB_node_by_pid("p7"));
    request(1,1, get_PCB_node_by_pid("p8"));

    print_PL();
    
 //   destroy_process(get_PCB_node_by_pid("p4"));

    print_PL();

    free(init);
}

void test_remove_first_process_on_blocked_list()
{
    PCB_node * nd2 = (PCB_node *) malloc(sizeof(PCB_node));
    nd2->next = NULL;
    PCB  p2 = Create("p2",1, NULL);
    nd2->process = &p2;

    PCB_node * nd = (PCB_node *) malloc(sizeof(PCB_node));
    nd->next = NULL;
    PCB  p1 = Create("p1",1, NULL);
    nd->process = &p1;

    PCB_node * nd3 = (PCB_node *) malloc(sizeof(PCB_node));
    nd3->next = NULL;
    PCB  p3 = Create("p3",1, NULL);
    nd3->process = &p3;

    request(1,1,nd);
    request(1,1,nd2);
    request(1,1,nd3);

    release(1,1,nd);
    print_PL(); 
}

void test_free_all_res_held_by_PCB()
{
    PCB_node * nd2 = (PCB_node *) malloc(sizeof(PCB_node));
    nd2->next = NULL;
    PCB  p2 = Create("p2",1, NULL);
    nd2->process = &p2;

    PCB_node * nd = (PCB_node *) malloc(sizeof(PCB_node));
    nd->next = NULL;
    PCB  p1 = Create("p1",1, NULL);
    nd->process = &p1;

    PCB_node * nd3 = (PCB_node *) malloc(sizeof(PCB_node));
    nd3->next = NULL;
    PCB  p3 = Create("p3",1, NULL);
    nd3->process = &p3;
   
    request(1,1,nd2);
    request(1,1,nd);
    request(1,1,nd3);

    print_PCB_res_list(p1.other_resources);
    
   // free_res_held_by_PCB(nd);
    delete_node(nd);
    print_PL();

    printf("after first call to free_res_held_by_PCB()\n");

//    print_PCB_res_list(p1.other_resources);
}

void test_child_linking()
{

    PCB_node * init = (PCB_node * ) malloc(sizeof(PCB_node));
    init->next = NULL;
    PCB initial = Create("init", 0, NULL);
    init->process = &initial;

    Create("p1",1,init);
    Create("p2", 2, init);
    Create("p3",2,init);
    Create("p4",1,init);

    print_children(init);
    
}

void test_find_ready_PCB()
{
    PCB_node * nd = (PCB_node *) malloc(sizeof(PCB_node));
    nd->next = NULL;
    PCB  p1 = Create("p1",1, NULL);
    nd->process = &p1;

    PCB_node * nd2 = (PCB_node *) malloc(sizeof(PCB_node));
    nd2->next = NULL;
    PCB  p2 = Create("p2",2, NULL);
    nd2->process = &p2;

    PCB_node * nd3 = (PCB_node *) malloc(sizeof(PCB_node));
    nd3->next = NULL;
    PCB  p3 = Create("p3",1, NULL);
    nd3->process = &p3;

    PCB_node * nd4 = (PCB_node *) malloc(sizeof(PCB_node));
    nd4->next = NULL;
    PCB  p4 = Create("p4",1, NULL);
    nd4->process = &p4;

    request(4,4,nd);
    print_PCB_res_list(p1.other_resources);

    request(4,1,nd2);
    request(4,1,nd3);
    request(4,1,nd4);

    print_PL();

    release(4,4,nd);    

//    print_PCB_res_list(p1.other_resources);
    print_PL();

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

    Create("p2", 1, NULL);

    request(1,1,nd);
    print_PCB_res_list(p1.other_resources);
    printf("number of r1 remaining: %d\n", res1.u);

    request(2,1,nd);
    print_PCB_res_list(p1.other_resources);
    printf("number of r2 remaining: %d\n", res2.u);

    request(2,1,nd);
    print_PCB_res_list(p1.other_resources);
    printf("number of r2 remaining: %d\n", res2.u);

    print_PL();

    request(1,1,nd);
    print_PCB_res_list(p1.other_resources);
    printf("number of r1 remaining: %d\n", res1.u);

    print_PL();

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

    print_PL();

    remove_PCB_from_RL(nd);
    print_PL();

    remove_PCB_from_RL(nd3);
    print_PL();
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

    print_PL();

    remove_PCB_from_RL(nd);
    print_PL();

    add_PCB_to_waitList(nd, &res1);
    print_PL();
}

void test_get_running_proc()
{
    PCB_node * init = (PCB_node * ) malloc(sizeof(PCB_node));
    init->next = NULL;
    PCB initial = Create("init", 0, NULL);
    init->process = &initial;

    Create("p1",1,init);
    Create("p2", 2, init);
    Create("p3",2,init);
    Create("p4",1,init);

    PCB_node * cur = get_running_proc();
    printf("The running process is %s\n", cur->process->pid);
}

void test_delete_first_child_of_mult_sibs()
{
    PCB_node * init = (PCB_node * ) malloc(sizeof(PCB_node));
    init->next = NULL;
    PCB initial = Create("init", 0, NULL);
    init->process = &initial;

    Create("p1",1,init);
    Create("p2", 2, init);
    Create("p3",2,init);
    Create("p4",1,init);

    print_PL();

    printf("before call to destroy_process()\n");
    destroy_process(get_PCB_node_by_pid("p3"));
    printf("after call\n");

    print_PL();

}

void test_get_PCB_node_by_pid()
{
    PCB_node * init = (PCB_node * ) malloc(sizeof(PCB_node));
    init->next = NULL;
    PCB initial = Create("init", 0, NULL);
    init->process = &initial;

    Create("p1",1,init);
    Create("p2", 2, init);
    Create("p3",2,init);
    Create("p4",1,init);

    printf("p3 requests 1 of res1\n");   
    request(1,1,get_PCB_node_by_pid("p3"));
    
    print_PL();

    printf("resources held by p3\n");
    print_PCB_res_list(get_PCB_node_by_pid("p3")->process->other_resources);

    printf("p2 requests 1 of res1\n");
    request(1,1,get_PCB_node_by_pid("p2"));    

    print_PL();

}

int main()
{
    init_resources();
//    test_print_PL();
//    test_get_PCB_node_by_pid();
//    test_delete_first_child_of_mult_sibs();
//    test_get_running_proc();
//    test_scheduler();
//    test_get_highest_pri_proc();
    test_delete_node();
//    test_remove_first_process_on_blocked_list();
//    test_free_all_res_held_by_PCB();
//    test_child_linking();
//    test_find_ready_PCB();
//    test_remove_RCB_from_PCB();
//    test_release();
//    test_request();
//    test_remove_RL_add_WL();
    return 0;
}

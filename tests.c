#include "stdio.h"
#include "internals.c"

void test_request()
{

    PCB  p1 = Create("p1",1, NULL);
    request(1,1,&p1);
    print_PCB_res_list(p1.other_resources);
    printf("number of r1 remaining: %d\n", res1.u);

    request(2,2,&p1);
    print_PCB_res_list(p1.other_resources);
    printf("number of r2 remaining: %d\n", res2.u);

    request(1,1,&p1);
    print_PCB_res_list(p1.other_resources);
    printf("number of r1 remaining: %d\n", res1.u);
}

int main()
{
    init_resources();
    test_request();
    return 0;
}

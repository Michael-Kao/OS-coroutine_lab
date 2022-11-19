#include <stdio.h>
#include "../src/coroutine_int.h"

int main(){
    
    struct task_struct task[20];
    struct stk stk;
    struct task_struct *tmp;

    stack_init(&stk);
    for (int i = 0; i < 16; i++) {
        task[i].tfd = i;
        printf("push %d, return %d\n", i, stack_push(&stk, &task[i]));
    }

    for (int i = 0; i < 16; i++) {
        tmp = stack_pop(&stk);
        if (tmp)
            printf("pop %d\n", tmp->tfd);
        else
            printf("pop failed\n");
    }

    return 0;
}
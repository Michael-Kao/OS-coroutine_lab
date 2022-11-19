#include <errno.h>
#include <stdlib.h>
#include <stdio.h>
#include "coroutine_int.h"

//ringbuffer_size = 16

void rq_init(struct rq *rq)
{
    // setup the ring buffer
    rq->out = 0;
    rq->in = 0;
    rq->mask = RINGBUFFER_SIZE - 1;
}

static inline unsigned int __ringbuffer_unused(struct rq *rq)
{
    return rq->mask + 1 - (rq->in - rq->out);
}

// enqueue at in
int rq_enqueue(struct rq *rq, struct task_struct *task)
{
    if (!__ringbuffer_unused(rq))
        return -EAGAIN;

    printf("enqueuing\n");
    rq->r[rq->in & rq->mask] = task;
    rq->in++;

    return 0;
}

// dequeue at out
struct task_struct *rq_dequeue(struct rq *rq)
{
    struct task_struct *rev;

    if (rq->in == rq->out)
        return NULL;

    printf("dequeuing\n");
    rev = rq->r[rq->out & rq->mask];
    rq->out++;

    return rev;
}

// stack_size = 16
void stack_init(struct stk *stk)
{
    stk->flag = 0;
    stk->top = 0;
    stk->s_top = 0;
    stk->mask = STACKBUF_SIZE - 1;
}

static inline unsigned int __stack_unused(struct stk *stk)
{
    return stk->mask + 1 - stk->top;
}

int stack_push(struct stk *stk, struct task_struct *task)
{
    if (!__stack_unused(stk))
        return -EAGAIN;

    if(stk->flag){
    // printf("Pushing\n");
        stk->saved[stk->s_top] = task;
        stk->s_top++;
    // printf("Pushed\n");
    }
    else{
        stk->s[stk->top] = task;
        stk->top++;
    }
    

    return 0;
}

struct task_struct *stack_pop(struct stk *stk)
{
    struct task_struct *rev;

    if (!stk->top && !stk->s_top)
        return NULL;

    if(!stk->flag){
        stk->flag = 1;
    }

    if(stk->top > 0){
    // printf("Popping\n");
        stk->top--;
        rev = stk->s[stk->top];
    }
    else{
        stk->s_top--;
        rev = stk->saved[stk->s_top];
    }

    return rev;
}
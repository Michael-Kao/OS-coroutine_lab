#ifndef __COROUTINE_INT_H__
#define __COROUTINE_INT_H__

#include "rbtree.h"
#include "context.h"

typedef int (*job_t)(struct context *__context, void *args);

/*
 * task_struct maintain the coroutine or task object.
 */
struct task_struct {
    /* job information */
    struct cr *cr;
    int tfd; /* task fd */
    job_t job;
    void *args;
    struct context context; /* defined at context.h */

    /* default info */
    struct {
        struct rb_node node;
        long sum_exec_runtime;
        long exec_start;
    };
};

#ifndef container_of
#define container_of(ptr, type, member)                        \
    __extension__({                                            \
        const __typeof__(((type *)0)->member) *__mptr = (ptr); \
        (type *)((char *)__mptr - offsetof(type, member));     \
    })
#endif

#define task_of(__context) container_of(__context, struct task_struct, context)

/* runqueue */

// Need to be power of two
#define RINGBUFFER_SIZE 16

struct rq {
    unsigned int out, in; /* dequeue at out, enqueue  at in*/
    unsigned int mask; /* the size is power of two, so mask will be size - 1 */
    struct task_struct *r[RINGBUFFER_SIZE];
};

void rq_init(struct rq *rq);
int rq_enqueue(struct rq *rq, struct task_struct *task);
struct task_struct *rq_dequeue(struct rq *rq);

/* stack */

#define STACKBUF_SIZE 16

struct stk{
    unsigned int top;
    unsigned int s_top;
    unsigned int mask;
    unsigned int flag;
    struct task_struct *s[STACKBUF_SIZE];
    struct task_struct *saved[STACKBUF_SIZE];
};

void stack_init(struct stk *stk);
int stack_push(struct stk *stk, struct task_struct *task);
struct task_struct *stack_pop(struct stk *stk);



/* main data structure */

#define MAX_CR_TABLE_SIZE 10

struct cr {
    unsigned long size; /* number of the task in this scheduler */
    int crfd; /* coroutine fd number */
    int flags; /* Which type of scheduler, FIFO or CFS */
    struct task_struct *current; /* the job currently working */

    /* scheduler - chose by the flags */
    struct rq rq; /* FIFO */
    struct rb_root root; /* Default */
    struct stk stk; /* stack */

    /* sched operations */
    int (*schedule)(struct cr *cr, job_t func, void *args);
    struct task_struct *(*pick_next_task)(struct cr *cr);
    int (*put_prev_task)(struct cr *cr, struct task_struct *prev);
};

struct cr_struct {
    int size;
    struct cr *table[MAX_CR_TABLE_SIZE];
};

void sched_init(struct cr *cr);

#endif /* __COROUTINE_INT_H__ */

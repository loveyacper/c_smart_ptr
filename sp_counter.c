#include <string.h>
#include "sp_counter.h"

#ifdef BERT_DEBUG
#include <stdio.h>
#endif

sp_counter_t* sp_counter_new()
{
    sp_counter_t* c = (sp_counter_t* )malloc(sizeof(sp_counter_t));
#ifdef BERT_DEBUG
    printf("-- malloc sp_counter: %p\n", c);
#endif
    memset(c, 0, sizeof(sp_counter_t));
    return c;
}

void sp_counter_init(sp_counter_t* c)
{
    c->cnt_share = c->cnt_weak = 1;
    c->resourse = NULL;
    c->deleter = free; /* default is free */
}

void sp_counter_delete(sp_counter_t* c)
{
#ifdef BERT_DEBUG
    printf("-- free sp_counter: %p\n", c);
#endif
    free(c);
}

void sp_counter_incweak(sp_counter_t* c)
{
    __sync_fetch_and_add(&c->cnt_weak, 1);
}

void sp_counter_decweak(sp_counter_t* c)
{
    int weak = __sync_add_and_fetch(&c->cnt_weak, -1);
    if (weak == 0)
        sp_counter_delete(c);
}

void sp_counter_incshare(sp_counter_t* c)
{
    __sync_fetch_and_add(&c->cnt_share, 1);
}

void sp_counter_decshare(sp_counter_t* c)
{
    if (__sync_add_and_fetch(&c->cnt_share, -1) == 0) {
        if (c->resourse && c->deleter) {
#ifdef BERT_DEBUG
            printf("-- delete resource: %p\n", c->resourse);
#endif
            c->deleter(c->resourse);
        }

        sp_counter_decweak(c);
    }
}

int sp_counter_try_incshare(sp_counter_t* c)
{
    /*
     * When a shared_ptr is construct from weakptr
     * The weak ptr may be expired, so here is a infinite loop
     * to check whether the weak ptr is valid
     */
    for (;;) {
        const int oldCnt = c->cnt_share;
        if (oldCnt == 0)
            return 0;

        if (oldCnt == __sync_val_compare_and_swap(&c->cnt_share, oldCnt, oldCnt + 1))
            return 1;
    }

    return 0;
}


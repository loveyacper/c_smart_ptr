#include "sp_weak_ptr.h"
#include "sp_counter.h"
#include "sp_shared_ptr.h"

#ifdef BERT_DEBUG
#include <stdio.h>
#endif

static
void weak_ptr_init(sp_weak_ptr_t* wp)
{
    wp->counter = NULL;
}

sp_weak_ptr_t* weak_ptr_new()
{
    sp_weak_ptr_t* wp = (sp_weak_ptr_t* )malloc(sizeof(sp_weak_ptr_t));
    weak_ptr_init(wp);
#ifdef BERT_DEBUG
    printf("-- malloc weak_ptr: %p\n", wp);
#endif

    return wp;
}

sp_weak_ptr_t* weak_ptr_copy(sp_weak_ptr_t* wp)
{
    if (!wp) return NULL;

    sp_weak_ptr_t* p = weak_ptr_new();
    if (wp->counter) {
        p->counter = wp->counter;
        sp_counter_incweak(p->counter);
    } else {
        weak_ptr_init(p);
    }

    return p;
}

sp_weak_ptr_t* weak_ptr_from_shared(sp_shared_ptr_t* sp)
{
    if (!sp) return NULL;

    sp_weak_ptr_t* wp = weak_ptr_new();
    if (sp->counter) {
        wp->counter = sp->counter;
        sp_counter_incweak(sp->counter);
    } else {
        weak_ptr_init(wp);
    }

    return wp;
}

sp_shared_ptr_t* weak_ptr_lock(sp_weak_ptr_t* wp)
{
    if (!wp) return NULL;

    return shared_ptr_from_weak(wp);
}

int weak_ptr_expired(sp_weak_ptr_t* wp)
{
    if (!wp || !wp->counter) return 1;

    int share_cnt = __sync_fetch_and_add(&wp->counter->cnt_share, 0);
    return share_cnt == 0 ? 1 : 0;
}

void weak_ptr_destructor(sp_weak_ptr_t** wpp)
{
    if (!wpp || !*wpp)
        return;

    sp_weak_ptr_t* wp = *wpp;
    if (wp && wp->counter)
        sp_counter_decweak(wp->counter);

#ifdef BERT_DEBUG
    printf("-- free weak_ptr: %p\n", wp);
#endif
    free(wp);
}


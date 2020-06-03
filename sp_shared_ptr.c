#include "sp_shared_ptr.h"
#include "sp_counter.h"
#include "sp_weak_ptr.h"

static
void shared_ptr_init(sp_shared_ptr_t* p)
{
    p->counter = NULL;
}

static
void shared_ptr_init_with_resourse(sp_shared_ptr_t* p, void* res)
{
    if (!res) {
        shared_ptr_init(p);
    } else {
        p->counter = sp_counter_new();
        sp_counter_init(p->counter);
        p->counter->resourse = res;
    }
}

sp_shared_ptr_t* shared_ptr_new(void* res)
{
    sp_shared_ptr_t* p = (sp_shared_ptr_t* )malloc(sizeof(sp_shared_ptr_t));
    if (res)
        shared_ptr_init_with_resourse(p, res);
    else
        shared_ptr_init(p);

    return p;
}


sp_shared_ptr_t* shared_ptr_new_with_deleter(void* res, sp_deleter_fn_t del)
{
    sp_shared_ptr_t* p = (sp_shared_ptr_t* )malloc(sizeof(sp_shared_ptr_t));
    if (res) {
        shared_ptr_init_with_resourse(p, res);
        p->counter->deleter = del;
    } else {
        shared_ptr_init(p);
    }

    return p;
}

sp_shared_ptr_t* shared_ptr_copy(sp_shared_ptr_t* p)
{
    if (!p) return NULL;

    sp_shared_ptr_t* p2 = shared_ptr_new(NULL);
    p2->counter = p->counter;
    if (p2->counter)
        sp_counter_incshare(p2->counter);

    return p2;
}

sp_shared_ptr_t* shared_ptr_from_weak(sp_weak_ptr_t* wk)
{
    if (!wk) return NULL;

    sp_counter_t* tmp = wk->counter;
    if (tmp && sp_counter_try_incshare(tmp)) {
        sp_shared_ptr_t* sp = shared_ptr_new(NULL);
        sp->counter = tmp;
        return sp;
    } else {
        return NULL;
    }
}

void* shared_ptr_get(sp_shared_ptr_t* p)
{
    return p->counter->resourse;
}

void shared_ptr_destructor(sp_shared_ptr_t** pp)
{
    if (!pp || !*pp)
        return;

    sp_shared_ptr_t* p = *pp;

    if (p && p->counter)
        sp_counter_decshare(p->counter);

    free(p);
    *pp = NULL;
}


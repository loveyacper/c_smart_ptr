#ifndef BERT_SP_WEAK_PTR_H
#define BERT_SP_WEAK_PTR_H

#ifdef  __cplusplus
extern "C" {
#endif

#include "cleanup.h"

struct sp_shared_ptr;
struct sp_counter;

typedef struct sp_weak_ptr {
    struct sp_counter* counter;
} sp_weak_ptr_t;

sp_weak_ptr_t* weak_ptr_new();

//void weak_ptr_delete(sp_weak_ptr_t* wp);

sp_weak_ptr_t* weak_ptr_copy(sp_weak_ptr_t* wp) ;

sp_weak_ptr_t* weak_ptr_from_shared(struct sp_shared_ptr* sp) ;

struct sp_shared_ptr* weak_ptr_lock(sp_weak_ptr_t* wp);

int weak_ptr_expired(sp_weak_ptr_t* wp) ;

/* for gcc/clang cleanup */
void weak_ptr_destructor(sp_weak_ptr_t** p);

#define MAKE_WEAKPTR(var, shared_ptr) on_destroy(sp_weak_ptr_t*, weak_ptr_destructor) var = weak_ptr_from_shared(shared_ptr)
#define MAKE_WEAKPTR_COPY(to, from) on_destroy(sp_weak_ptr_t*, weak_ptr_destructor) to = weak_ptr_copy(from)

#ifdef  __cplusplus
}
#endif

#endif

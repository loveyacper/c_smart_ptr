#ifndef BERT_SP_SHARED_PTR_H
#define BERT_SP_SHARED_PTR_H

#include <stdlib.h>
#include "cleanup.h"

#ifdef  __cplusplus
extern "C" {
#endif

struct sp_counter;
struct sp_weak_ptr;

typedef void (*sp_deleter_fn_t)(void* );

typedef struct sp_shared_ptr {
    struct sp_counter* counter;
} sp_shared_ptr_t;

sp_shared_ptr_t* shared_ptr_new(void* res);

sp_shared_ptr_t* shared_ptr_new_with_deleter(void* res, sp_deleter_fn_t del);

sp_shared_ptr_t* shared_ptr_copy(sp_shared_ptr_t* p);

sp_shared_ptr_t* shared_ptr_from_weak(struct sp_weak_ptr * wk);

void* shared_ptr_get(sp_shared_ptr_t* p);

/* for gcc cleanup */
void shared_ptr_destructor(sp_shared_ptr_t** p);

#define MAKE_SHARED(var, T) on_destroy(sp_shared_ptr_t*, shared_ptr_destructor) var = shared_ptr_new(malloc(sizeof(T)))

//#define MAKE_SHARED_DELETER(var, T, del) on_destroy(sp_shared_ptr_t*, shared_ptr_destructor) var = shared_ptr_new_with_deleter(malloc(sizeof(T)), del)

#define MAKE_SHARED_COPY(to, from) on_destroy(sp_shared_ptr_t*, shared_ptr_destructor) to = shared_ptr_copy(from)

#define WEAK_PTR_LOCK(var, weak) on_destroy(sp_shared_ptr_t*, shared_ptr_destructor) var = weak_ptr_lock(weak)

#ifdef  __cplusplus
}
#endif

#endif


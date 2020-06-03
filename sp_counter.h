#ifndef BERT_SP_COUNTER_H
#define BERT_SP_COUNTER_H

#include <stdlib.h>
#include <stdio.h>

#ifdef  __cplusplus
extern "C" {
#endif

/* called when shared ptr expired */
typedef void (*sp_deleter_fn_t)(void* );

typedef struct sp_counter {
    int cnt_share;
    int cnt_weak;

    void* resourse;
    sp_deleter_fn_t deleter;
} sp_counter_t;

sp_counter_t* sp_counter_new();

void sp_counter_init(sp_counter_t* c);
void sp_counter_delete(sp_counter_t* c);
void sp_counter_incweak(sp_counter_t* c);
void sp_counter_decweak(sp_counter_t* c);
void sp_counter_incshare(sp_counter_t* c);
void sp_counter_decshare(sp_counter_t* c);
int sp_counter_try_incshare(sp_counter_t* c);

#ifdef  __cplusplus
}
#endif

#endif

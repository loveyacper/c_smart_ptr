#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <assert.h>

#include "sp_shared_ptr.h"
#include "sp_weak_ptr.h"

/* For test */
typedef struct Data {
    char a[32];
} Data_t;

/* The corresponding cpp code is on right side */

void test_shared_ptr_copy() {
    MAKE_SHARED(p1, Data_t);            /* auto p1 = std::make_shared<Data_t>(); */
    MAKE_SHARED_COPY(p2, p1);   /* auto p2(p1);*/

    Data_t* d1 = (Data_t*)shared_ptr_get(p1); /* auto d1 = p1.get(); */
    Data_t* d2 = (Data_t*)shared_ptr_get(p2); /* auto d2 = p2.get(); */

    strncpy(d1->a, "abcdefghijklmnopqrstuvwxyz", 27);
    printf("data: %s\n", d2->a);

    assert (d1 == d2);

    /* p1, p2 and d1 is freed automatically */
}

void test_weak_ptr() {

    MAKE_WEAKPTR(w0, NULL);     /* std::weak_ptr<Data_t> w0; */
    {
        MAKE_SHARED(p1, Data_t);        /* auto p1 = std::make_shared<Data_t>(); */
        Data_t* d1 = (Data_t*)shared_ptr_get(p1); /* auto d1 = p1.get(); */
        strncpy(d1->a, "abcdefghijklmnopqrstuvwxyz", 27);

        MAKE_WEAKPTR(w1, p1);   /* auto w1(p1);  make weak ptr from shared ptr */

        WEAK_PTR_LOCK(sw, w1);  /* auto sw = w1.lock(); */
        assert(sw);             /* 一定有效 */
        Data_t* d2 = (Data_t*)shared_ptr_get(sw); /* auto d2 = sw.get(); */
        printf("data2: %s\n", d2->a);

        assert(!w0);
        w0 = weak_ptr_from_shared(p1);  /* w0 = p1;  weak ptr比share ptr活得更久 */

    }

    sp_shared_ptr_t* nullsp = weak_ptr_lock(w0);    /* auto nullsp = w0.lock(); */
    assert(!nullsp);    /* 一定为空，因为p1无效 */
}

int main() {
    test_shared_ptr_copy();
    test_weak_ptr();

    printf("passed\n");
    return 0;
}

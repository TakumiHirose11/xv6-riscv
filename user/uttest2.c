#include "kernel/types.h" 
#include "kernel/stat.h" 
#include "user/user.h" 
#include "user/uthreads.h"

void foo() {
    int c = 0;
    while (c < 5) {
        printf("foo (tid=%d): %d\n", mytid(), c);
        c += 1;
        yield();
    }
    uthread_exit();
}
void bar() {
    int c = 0;
    while (c < 5) {
        printf("bar (tid=%d): %d\n", mytid(), c);
        c += 2;
        yield();
    }
    uthread_exit();
}
void baz_sub(int *cp) { 
    printf("baz (tid=%d): %d\n", mytid(), *cp); 
    yield();
    *cp += 3;
}
void baz() {
    int c = 0;
    while (c < 5) {
        baz_sub(&c);
        baz_sub(&c);
    }
    uthread_exit();
}
int main() { 
    make_uthread(foo); 
    make_uthread(bar); 
    make_uthread(baz); 
    start_uthreads(); 
    exit(0);
}
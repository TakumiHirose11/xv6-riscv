#include "kernel/types.h" 
#include "kernel/stat.h" 
#include "user/user.h" 
#include "user/uthreads.h"

void foo() { 
    int c = 0; 
    for (;;) {
        printf("foo (tid=%d): %d\n", mytid(), c); 
        c += 1;
        yield();
    } 
}

int make_foo_thread() {
    return make_uthread(foo);
}

int main() { 
    for (int i = 0; i < 17; i++) {
        int a = make_foo_thread();
        if (a == -1) {
            printf("make_uthread failure. i=%d\n", i);
            exit(1);
        }
    }
    start_uthreads(); 
    exit(0);
}
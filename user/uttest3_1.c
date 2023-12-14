#include "kernel/types.h"
#include "user/user.h"
#include "user/uthreads.h"

// 条件変数
int condition_variable = 1;
int zero = 0;

void thread_waiter() {
    printf("Thread Waiter: Waiting for condition_variable to become 1\n");
    uthread_wait(&condition_variable);
    printf("Thread Waiter: Received notification, condition_variable is now 1\n");
    uthread_exit();
}

void thread_notifier() {
    for (int i = 0; i < 3; i++) {
        printf("Thread Notifier: Sleeping for a bit...\n");
        yield();
    }
    printf("Thread Notifier: Sending wrong notification\n");
    uthread_notify_all(&zero);
    for (int i = 0; i < 3; i++) {
        printf("Thread Notifier: Sleeping for a bit...\n");
        yield();
    }
    printf("Thread Notifier: Sending correct notification\n");
    uthread_notify_all(&condition_variable);
    uthread_exit();
}

int main() {
    make_uthread(thread_waiter);
    make_uthread(thread_notifier);
    start_uthreads();
    exit(0);
}

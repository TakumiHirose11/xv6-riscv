#include "kernel/types.h"
#include "user/user.h"
#include "user/uthreads.h"

// 条件変数
int condition_variable = 0;

void thread_waiter1() {
    printf("Thread Waiter 1: Waiting for notification\n");
    uthread_wait(&condition_variable);
    printf("Thread Waiter 1: Received notification\n");
    uthread_exit();
}

void thread_waiter2() {
    printf("Thread Waiter 2: Waiting for notification\n");
    uthread_wait(&condition_variable);
    printf("Thread Waiter 2: Received notification\n");
    uthread_exit();
    printf("Thread Waiter 2: This should not be printed!!!!!!\n");
}

void thread_notifier() {
    for (int i = 0; i < 3; i++) {
        printf("Thread Notifier: Sleeping for a bit...\n");
        yield();
    }
    printf("Thread Notifier: Notifying thread_waiter1 (tid=0)\n");
    uthread_notify(0, &condition_variable); // スレッドID 0に通知を送る
    for (int i = 0; i < 3; i++) {
        printf("Thread Notifier: Sleeping for a bit...\n");
        yield();
    }
    printf("Thread Notifier: Notifying thread_waiter2 (tid=1)\n");
    uthread_notify(1, &condition_variable); // スレッドID 1に通知を送る
    uthread_exit();
}

int main() {
    int a = make_uthread(thread_waiter1); // tid 0
    int b = make_uthread(thread_waiter2); // tid 1
    int c = make_uthread(thread_notifier); // 通知を送るスレッド
    // check thread id
    printf("Thread Waiter 1: tid=%d\n", a);
    printf("Thread Waiter 2: tid=%d\n", b);
    printf("Thread Notifier: tid=%d\n", c);
    start_uthreads();
    exit(0);
}

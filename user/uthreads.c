#include "kernel/types.h"
#include "user/user.h"
#include "user/uthreads.h"

uthread_t uthreads[MAX_THREADS];

int current_tid = -1;

// 他のスレッドに実行を譲る
void yield(void)
{
  int tid = mytid();
  for (int i = 0; i < MAX_THREADS; i++)
  {
    int next_tid = (tid + i + 1) % MAX_THREADS;
    if (uthreads[next_tid].used)
    {
      struct context *current_context = &(uthreads[tid].context);
      struct context *next_context = &(uthreads[next_tid].context);
      current_tid = next_tid;
      swtch(current_context, next_context);
      return;
    }
  }
}

int make_uthread(void (*fun)())
{
  for (int tid = 0; tid < MAX_THREADS; tid++)
  {
    if (!uthreads[tid].used)
    {
      uthreads[tid].fun = fun;
      uthreads[tid].used = 1;
      uthreads[tid].context.ra = (uint64)fun;
      uthreads[tid].context.sp = (uint64)(uthreads[tid].stack + STACK_DEPTH);
      return tid;
    }
  }
  return -1; // スレッドの作成ができない場合
}

void start_uthreads(void) {
  while (1) {
    int active_threads = 0;
    for(int tid = 0; tid < MAX_THREADS; tid++) {
      if (uthreads[tid].used) {
        active_threads = 1;
        current_tid = tid;
        uthreads[tid].fun();
        // スレッドの関数が戻った場合は、通常はuthread_exitによるものと考えられる
        uthreads[tid].used = 0;
        current_tid = -1;
      }
    }
    if (!active_threads) {
      break; // 全てのスレッドが終了した
    }
  }
}


int mytid(void)
{
  return current_tid;
}

void uthread_exit(void) {
  int tid = mytid();
  uthreads[tid].used = 0; // スレッドを未使用状態に設定
  yield(); // 次のスレッドに実行を移す
}

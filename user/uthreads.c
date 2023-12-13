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

void start_uthreads(void)
{
  for(int tid = 0; tid < MAX_THREADS; tid++)
  {
    if (uthreads[tid].used)
    {
      current_tid = tid;
      uthreads[tid].fun();
      // これ以降は実行されない
      uthreads[tid].used = 0;
      current_tid = -1;
    }
  }
}

int mytid(void)
{
  return current_tid;
}